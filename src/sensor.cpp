#include "sensor.h"
#include <cstdlib>
#include <cstdint>

using namespace std;

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str)
{
	Sensor *ret = (Sensor*) malloc(sizeof(Sensor));
	ret->x = _x;
	ret->y = _y;
	ret->h = _h;
	ret->w = _w;
	ret->no_str = _no_str;

	ret->str_state = (bool*) malloc(ret->no_str * sizeof(bool));
	for (size_t i = 0; i < ret->no_str; ++i) {
		ret->str_state[i] = 0;
	}

	ret->cell_state = (bool**) malloc(ret->no_str * sizeof(bool*));
	ret->cell_pre_state = (bool**) malloc(ret->no_str * sizeof(bool*));
	for (size_t i = 0; i < ret->no_str; ++i) {
		ret->cell_state[i] = (bool*) malloc(2 * sizeof(bool));
		for (size_t j = 0; j < 2; ++j) {
			ret->cell_state[i][j] = 0;
		}
		ret->cell_pre_state[i] = (bool*) malloc(2 * sizeof(bool));
		for (size_t j = 0; j < 2; ++j) {
			ret->cell_pre_state[i][j] = 0;
		}
	}

	ret->_divx = ret->w / ret->no_str;
	ret->_cell_area = ret->_divx * ret->h;
	ret->_xstart = ret->x - ret->w;
	ret->_xend = ret->x + ret->w;
	ret->_ystart = ret->y - ret->h;
	ret->_yend = ret->y + ret->h;

	return ret;
}

void sensor_Free(Sensor *_s)
{
	for (size_t i = 0; i < _s->no_str; ++i) {
		free(_s->cell_state[i]);
		free(_s->cell_pre_state[i]);
	}
	free(_s->cell_state);
	free(_s->cell_pre_state);
	free(_s->str_state);
	free(_s);
}

void sensor_GetRect(Sensor* _s, int _x, int _y, int* ret)
{
	int divx = _s->w / _s->no_str;
	int x = _s->x - _s->w + _x * divx;
	int y = _s->y - _s->h + _y * _s->h;

	ret[0] = x;
	ret[1] = y;
	ret[2] = x + divx;
	ret[3] = y + _s->h;
}

void sensor_Feed(Sensor* _s, const Mat& data)
{
	int cell_count[2][_s->no_str];
	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = 0; j <_s->no_str; ++j) {
			cell_count[i][j] = 0;
		}
	}

	for (int i = _s->_ystart; i < _s->_yend; ++i) {
		for (int j = _s->_xstart; j < _s->_xend; ++j) {
			int cellx = (j - _s->_xstart) / _s->_divx;
			int celly = (i - _s->_ystart) / _s->h;
			cell_count[celly][cellx] += 1;
		}
	}

	for (size_t i = 0; i < 2; ++i) {
		for (size_t j = 0; j <_s->no_str; ++j) {

		}
	}
}

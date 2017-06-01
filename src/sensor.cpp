#include "sensor.h"
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <vector>

using namespace std;

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str, double _thr)
{
	Sensor *ret = (Sensor*) malloc(sizeof(Sensor));
	ret->x = _x;
	ret->y = _y;
	ret->h = _h;
	ret->w = _w;
	ret->no_str = _no_str;

	ret->str_state = (bool*) malloc(ret->no_str * sizeof(bool));
	for (int i = 0; i < ret->no_str; ++i) {
		ret->str_state[i] = 0;
	}

	ret->cell_state = (bool**) malloc(ret->no_str * sizeof(bool*));
	ret->cell_pre_state = (bool**) malloc(ret->no_str * sizeof(bool*));
	for (int i = 0; i < ret->no_str; ++i) {
		ret->cell_state[i] = (bool*) malloc(2 * sizeof(bool));
		for (int j = 0; j < 2; ++j) {
			ret->cell_state[i][j] = 0;
		}
		ret->cell_pre_state[i] = (bool*) malloc(2 * sizeof(bool));
		for (int j = 0; j < 2; ++j) {
			ret->cell_pre_state[i][j] = 0;
		}
	}

	ret->divx = ret->w / ret->no_str;
	ret->cell_area = ret->divx * ret->h;
	ret->xstart = ret->x - ret->w;
	ret->xend = ret->x + ret->w;
	ret->ystart = ret->y - ret->h;
	ret->yend = ret->y + ret->h;
	
	//double tmp_thr = _thr * (double) (ret->h * ret->divx);
	ret->cell_thr = 610;

	return ret;
}

void sensor_Free(Sensor *_s)
{
	for (int i = 0; i < _s->no_str; ++i) {
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
	vector<vector<int> > cell_count(_s->no_str * 2, vector<int>(2));
	for (int i = 0; i < data.cols; ++i) {
		for (int j = 0; j < data.rows; ++j) {
			if (data.at<uint8_t>(j, i)) {
				int x = i / _s->divx;
				int y = j / _s->h;
				cell_count[x][y] += 1;
				//_s->cell_state[x][y] =
					//cell_count[x][y] >= _s->cell_thr;
			}
		}
	}
}

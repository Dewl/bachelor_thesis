#include "sensor.h"
#include <stdlib.h>

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str)
{
	Sensor *ret = malloc(sizeof(Sensor));
	ret->x = _x;
	ret->y = _y;
	ret->h = _h;
	ret->w = _w;
	ret->no_str = _no_str;

	ret->str_state = malloc(ret->no_str * sizeof(uint8));
	for (size_t i = 0; i < ret->no_str; ++i) {
		ret->str_state[i] = 0;
	}

	ret->cell_state = malloc(ret->no_str * sizeof(uint8*));
	ret->cell_pre_state = malloc(ret->no_str * sizeof(uint8*));
	for (size_t i = 0; i < ret->no_str; ++i) {
		ret->cell_state[i] = malloc(2 * sizeof(uint8));
		for (size_t j = 0; j < 2; ++j) {
			ret->cell_state[i][j] = 0;
		}
		ret->cell_pre_state[i] = malloc(2 * sizeof(uint8));
		for (size_t j = 0; j < 2; ++j) {
			ret->cell_pre_state[i][j] = 0;
		}
	}

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

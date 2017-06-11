#include "sensor.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str,
		double _thr, double _human_ratio)
{
	Sensor *ret = malloc(sizeof(Sensor));
	ret->up = 0;
	ret->down = 0;
	ret->human_ratio = _human_ratio;

	ret->x = _x;
	ret->y = _y;
	ret->h = _h;
	ret->w = _w;
	ret->no_str = _no_str;

	int total_str = _no_str * 2;

	ret->str_state = malloc(total_str * sizeof(uint8_t ));
	for (int i = 0; i < total_str; ++i) {
		ret->str_state[i] = 0;
	}

	ret->cell_state = malloc(total_str * sizeof(uint8_t *));
	ret->cell_counter = malloc(total_str * sizeof(uint16_t*));
	ret->cell_pre_state = malloc(total_str * sizeof(uint8_t *));
	for (int i = 0; i < total_str; ++i) {
		ret->cell_state[i] = malloc(2 * sizeof(uint8_t ));
		for (int j = 0; j < 2; ++j) {
			ret->cell_state[i][j] = 0;
		}
		ret->cell_pre_state[i] = malloc(2 * sizeof(uint8_t ));
		for (int j = 0; j < 2; ++j) {
			ret->cell_pre_state[i][j] = 0;
		}
		ret->cell_counter[i] = malloc(2 * sizeof(uint16_t));
	}

	ret->divx = ret->w / ret->no_str;
	ret->cell_area = ret->divx * ret->h;
	ret->xstart = ret->x - ret->w;
	ret->xend = ret->x + ret->w;
	ret->ystart = ret->y - ret->h;
	ret->yend = ret->y + ret->h;

	ret->cell_thr_ratio = _thr;
	ret->cell_thr = ret->cell_area * _thr;

	return ret;
}

void sensor_Free(Sensor *_s)
{
	for (int i = 0; i < _s->no_str * 2; ++i) {
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


void sensor_Feed(Sensor* _s, const uint8_t *_data, int _cols, int _rows)
{
	for (int i = 0; i < _s->no_str * 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			_s->cell_counter[i][j] = 0;
		}
	}

	for (int i = 0; i < _cols; ++i) {
		for (int j = 0; j < _rows; ++j) {
			/*if (_data.at<uint8_t>(j, i)) {*/
			if (_data[_cols * j + i]) {
				int x = i / _s->divx;
				int y = j / _s->h;

				if (x >= _s->no_str * 2) {
					x = _s->no_str * 2 - 1;
				}
				_s->cell_counter[x][y] += 1;
			}
		}
	}

	for (int i = 0; i < _s->no_str * 2; ++i) {
		for (size_t j = 0; j < 2; ++j) {
			_s->cell_pre_state[i][j] = _s->cell_state[i][j];
			_s->cell_state[i][j] = _s->cell_counter[i][j] >= _s->cell_thr;
		}
	}

	for (int i = 0; i < _s->no_str * 2; ++i) {
		if (_s->cell_state[i][1] && !_s->cell_pre_state[i][1] &&
				_s->cell_pre_state[i][0]) {
			// Going down
			_s->str_state[i] = 2;
			
		} else if (_s->cell_state[i][0] && !_s->cell_pre_state[i][0]
				&& _s->cell_pre_state[i][1]) {
			// Going up
			_s->str_state[i] = 1;
		}
	}

	// Using two pointer technique
	int p0, p1;

	// Going down
	p0 = 0;
	p1 = 0;
	while (p0 < _s->no_str * 2) {
		if (_s->str_state[p0] == 2) {
			p1 = p0 + 1;
			while (p1 < _s->no_str * 2 && _s->str_state[p1] == 2) {
				p1 += 1;
			}

			int len = p1 - p0;
			if (len >= _s->human_ratio) {
				for (int i = p0; i < p1; ++i) {
					_s->str_state[i] = 0;
				}
				_s->down += floor(len / _s->human_ratio);
			}
			p0 = p1;
		} else {
			p0 += 1;
		}
	}

	// Going up
	p0 = 0;
	p1 = 0;
	while (p0 < _s->no_str * 2) {
		if (_s->str_state[p0] == 1) {
			p1 = p0 + 1;
			while (p1 < _s->no_str * 2 && _s->str_state[p1] == 1) {
				p1 += 1;
			}

			int len = p1 - p0;
			if (len >= _s->human_ratio) {
				for (int i = p0; i < p1; ++i) {
					_s->str_state[i] = 0;
				}
				_s->up += floor(len / _s->human_ratio);
			}
			p0 = p1;
		} else {
			p0 += 1;
		}
	}
}


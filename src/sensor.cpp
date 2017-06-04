#include "sensor.h"
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <iostream>	// for debug
#include <cstdio>	// for debug
#include <cmath>

using namespace std;

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str,
		double _thr, double _ratio)
{
	Sensor *ret = (Sensor*) malloc(sizeof(Sensor));
	ret->up = 0;
	ret->down = 0;
	ret->ratio = _ratio;

	ret->x = _x;
	ret->y = _y;
	ret->h = _h;
	ret->w = _w;
	ret->no_str = _no_str;

	int total_str = _no_str * 2;

	ret->str_state = (bool*) malloc(total_str * sizeof(bool));
	for (int i = 0; i < total_str; ++i) {
		ret->str_state[i] = 0;
	}

	ret->cell_state = (bool**) malloc(total_str * sizeof(bool*));
	ret->cell_pre_state = (bool**) malloc(total_str * sizeof(bool*));
	for (int i = 0; i < total_str; ++i) {
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

	ret->cell_thr_ratio = _thr;
	ret->cell_thr = ret->cell_area * _thr;

	printf("Sensor Info:\n");
	printf("Width = %d\n", ret->w);
	printf("Height = %d\n", ret->h);
	printf("Number of stripes = %d\n", ret->no_str);
	printf("Cell's threshold area= %d\n", ret->cell_thr);
	printf("Cell's threshold ratio= %f\n", ret->cell_thr_ratio);
	printf("Ratio = %f\n", ret->ratio);

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


void sensor_Feed(Sensor* _s, const Mat& data)
{
	vector<vector<int> > cell_count(_s->no_str * 2, vector<int>(2));
	for (int i = 0; i < data.cols; ++i) {
		for (int j = 0; j < data.rows; ++j) {
			if (data.at<uint8_t>(j, i)) {
				int x = i / _s->divx;
				int y = j / _s->h;

				if (x >= _s->no_str * 2) {
					x = _s->no_str * 2 - 1;
				}

				cell_count[x][y] += 1;
			}
		}
	}

	for (size_t i = 0; i < cell_count.size(); ++i) {
		for (size_t j = 0; j < cell_count[0].size(); ++j) {
			_s->cell_pre_state[i][j] = _s->cell_state[i][j];
			_s->cell_state[i][j] = cell_count[i][j] >= _s->cell_thr;
		}
	}

	for (int i = 0; i < _s->no_str * 2; ++i) {
		if (_s->cell_state[i][1] && !_s->cell_pre_state[i][1] && _s->cell_pre_state[i][0]) {
			_s->down += 1;
		} else if (_s->cell_state[i][0] && !_s->cell_pre_state[i][0] && _s->cell_pre_state[i][1]) {
			_s->up += 1;
		}
	}
	//printf("\rUp = %d, Down = %d", _s->up, _s->down);
}

void sensor_Export(Sensor* _s, const char *fname)
{
	FILE *p_file = fopen(fname, "w");

	fprintf(p_file, "Sensor Info:\n");
	fprintf(p_file, "Width = %d\n", _s->w);
	fprintf(p_file, "Height = %d\n", _s->h);
	fprintf(p_file, "Number of stripes = %d\n", _s->no_str);
	fprintf(p_file, "Cell's threshold ratio= %f\n", _s->cell_thr_ratio);
	fprintf(p_file, "Cell's threshold area = %d\n", _s->cell_thr);
	fprintf(p_file, "Ratio = %f\n", _s->ratio);

	fprintf(p_file, "Up = %d, Down = %d\n", _s->up, _s->down);
	fprintf(p_file, "Appr. People: Up = %f, Down = %f\n", _s->up / _s->ratio,
			_s->down / _s->ratio);

	fclose(p_file);
}

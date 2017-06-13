#ifndef SENSOR_H
#define SENSOR_H

/**
 * This virtual sensor is applied for horizontal people flow
 */
#include <opencv/cv.h>

using namespace std;
using namespace cv;

typedef struct _Sensor {
	int x;
	int y;
	int h;
	int w;
	int no_str;
	bool *str_state;
	bool **cell_state;
	bool **cell_pre_state;
	int divx;
	int cell_area;
	int xstart;
	int xend;
	int ystart;
	int yend;
	int cell_thr;
	double cell_thr_ratio;
	int down;
	int up;
	double ratio;
} Sensor;

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str, double _thr,
		double ratio);
void sensor_Free(Sensor *_s);
void sensor_GetRect(Sensor* _s, int _x, int _y, int* ret);
void sensor_Feed(Sensor* _s, const Mat& data);
void sensor_Export(Sensor* _s, const char *fname);

#endif /* !SENSOR_H */

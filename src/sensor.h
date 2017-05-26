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
	size_t no_str;
	bool *str_state;
	bool **cell_state;
	bool **cell_pre_state;
	int _divx;
	int _cell_area;
	int _xstart;
	int _xend;
	int _ystart;
	int _yend;

} Sensor;

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str);
void sensor_Free(Sensor *_s);
void sensor_GetRect(Sensor* _s, int _x, int _y, int* ret);
void sensor_Feed(Sensor* _s, const Mat& data);

#endif /* !SENSOR_H */

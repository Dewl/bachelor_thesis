#ifdef __cplusplus
extern "C"
{
#endif 

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

/**
 * This virtual sensor is applied for horizontal people flow
 */
typedef struct _Sensor {
	int x;
	int y;
	int h;
	int w;
	int no_str;
	uint8_t  *str_state;
	uint8_t  **cell_state;	// 0: inactivated, 1: up, 2: down
	uint16_t **cell_counter;
	uint8_t  **cell_pre_state;
	
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
	double human_ratio;
} Sensor;

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str, double _thr,
		double human_ratio);
void sensor_Free(Sensor *_s);
void sensor_GetRect(Sensor* _s, int _x, int _y, int* ret);
void sensor_Feed(Sensor* _s, const uint8_t *_data, int _cols, int _rows);

#endif /* !SENSOR_H */

#ifdef __cplusplus
}
#endif

#ifndef SENSOR_H
#define SENSOR_H

#ifdef __cplusplus
extern "C"
{
#endif 

/**
 * This virtual sensor is applied for horizontal people flow
 */

typedef unsigned char uint8;

typedef struct _Sensor {
	int x;
	int y;
	int h;
	int w;
	int no_str;
	uint8 *str_state;
	uint8 **cell_state;
	uint8 **cell_pre_state;
} Sensor;

Sensor *sensor_Alloc(int _x, int _y, int _h, int _w, int _no_str);
void sensor_Free(Sensor *_s);

void sensor_GetRect(Sensor* _s, int _x, int _y, int* ret);

#ifdef __cplusplus
}
#endif

#endif /* !SENSOR_H */

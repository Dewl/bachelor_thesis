/*
 * sensor.h
 * Copyright (C) 2017 hkhoi <hkhoi@hkpc>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SENSOR_H
#define SENSOR_H

class sensor {
	int center_x;
	int center_y;
	int width;
	int height;
	int no_stripe;
	double thres_area;
	int thres_people;

	int _ul;	// upper left
	int _ur;	// upper right
	int _ll;	// lower left
	int _lr;	// lower right
};

#endif /* !SENSOR_H */

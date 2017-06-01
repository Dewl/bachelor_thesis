/* Copyright (C) 
 * 2017 - Khoi Hoang, Kiet Chuong
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 n * 
 */

/**
 * @file main.cpp
 * @brief The main entry point for the application
 * @author Khoi Hoang & Kiet Chuong
 * @version 1.0
 * @date 2017-03-03
 */


#include <iostream>
#include <vector>
#include <string>
#include <list>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "vibe.h"
#include "cvutil.h"
#include "config.h"
#include "sensor.h"
#include "drawer.h"

using namespace std;
using namespace cv;

// Global config
unordered_map<string, string> config;
Sensor *sensor;
Mat origin, foreground, croppedOrigin;
char gui_flag = '0';

// Prototypes
void processVideo(char *src, unordered_map<string, string> config);
void onMouse(int event, int x, int y, int flags, void* userdata);
void onMouse2(int event, int x, int y, int flags, void* userdata);

int main(int args, char **argv)
{
	config = config_Read(argv[2]);
	namedWindow("Origin", WINDOW_AUTOSIZE);
	namedWindow("Foreground", WINDOW_AUTOSIZE);
	setMouseCallback("Origin", onMouse, NULL);
	setMouseCallback("Foreground", onMouse2, NULL);
	processVideo(argv[1], config);
	destroyAllWindows();
	config_Write(config, argv[2]);
	return (EXIT_SUCCESS);
}

void processVideo(char *src, unordered_map<string, string> config)
{
	VideoCapture cap(src);
	if (!cap.isOpened()) {
		cerr << ("ERROR: Unable to open video source.") << endl;
		exit(EXIT_FAILURE);
	}

	Mat frame;

	vibeModel_Sequential_t *model = NULL;
	bool init = false;

	sensor = sensor_Alloc(
			config_GetInt(config, "sensor_x", 100),
			config_GetInt(config, "sensor_y", 100),
			config_GetInt(config, "sensor_h", 40),
			config_GetInt(config, "sensor_w", 100),
			config_GetInt(config, "sensor_str", 8),
			config_GetDouble(config, "sensor_thr", 0.3)
			);

	bool play = true;

	while (true) {
		if (play) {
			if (!cap.read(origin)) {
				break;
			}

			Rect sensorRect(
					Point(sensor->xstart, sensor->ystart),
					Point(sensor->xend, sensor->yend)
					);
			Mat tmp = origin(sensorRect);
			tmp.copyTo(croppedOrigin);

			if (!init) {
				init = true;
				foreground = Mat(croppedOrigin.rows, croppedOrigin.cols, CV_8UC1);
				model = (vibeModel_Sequential_t*)
					libvibeModel_Sequential_New();

				libvibeModel_Sequential_SetMatchingThreshold(model,
						config_GetInt(config, "threshold", 32));

				libvibeModel_Sequential_AllocInit_8u_C3R(model,
						croppedOrigin.data,
						croppedOrigin.cols,
						croppedOrigin.rows);
			}

			libvibeModel_Sequential_Segmentation_8u_C3R(model,
					croppedOrigin.data,
					foreground.data);
			libvibeModel_Sequential_Update_8u_C3R(model,
					croppedOrigin.data,
					foreground.data);

			refineBinaryImage(foreground,
					config_GetInt(config, "median", 5),
					config_GetInt(config, "erode", 3),
					config_GetInt(config, "dilate", 3));

			sensor_Feed(sensor, foreground);
			drawer_DrawSensor(origin, sensor);

			imshow("Origin", origin);
			imshow("Foreground", foreground);

		}
		char wkey = waitKey(config_GetInt(config, "speed", 33));
		if (wkey == 'p') {
			play = !play;
		} else if (wkey == 'q') {
			break;
		} else if (wkey == 'w' || wkey == 'h'){
			gui_flag = wkey;
		}
	}

	cap.release();
	sensor_Free(sensor);
	libvibeModel_Sequential_Free(model);
}

void onMouse(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN) {
		sensor->x = x;
		sensor->y = y;
		config_SetInt(config, "sensor_x", x);
		config_SetInt(config, "sensor_y", y);
	} else if (event == EVENT_RBUTTONDOWN) {
		if (gui_flag == 'w') {
			int d = abs(sensor->x - x);
			config_SetInt(config, "sensor_w", d);
			sensor->w = d;
		} else if (gui_flag == 'h') {
			int d = abs(sensor->y - y);
			config_SetInt(config, "sensor_h", d);
			sensor->h = d;
		}
	}
}

void onMouse2(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN) {
		printf("debug:main: foreground[%d][%d] = %d\n", y, x, foreground.at<uint8_t>(y, x));
	} 
}

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
#include "const.h"

using namespace std;
using namespace cv;

// Global config
unordered_map<string, string> config;
Sensor *sensor;
Mat origin, foreground, croppedOrigin;
char gui_flag = '0';
uint32_t no_frame = 0;
int speed = 5;

// Prototypes
void processVideo(char *src, unordered_map<string, string> config, const char *fname);
void onMouse(int event, int x, int y, int flags, void* userdata);
void onMouse2(int event, int x, int y, int flags, void* userdata);

int main(int args, char **argv)
{
	config = config_Read(argv[2]);
	namedWindow("Origin", WINDOW_AUTOSIZE);
	namedWindow("Foreground", WINDOW_AUTOSIZE);
	setMouseCallback("Origin", onMouse, NULL);
	setMouseCallback("Foreground", onMouse2, NULL);
	processVideo(argv[1], config, argv[3]);
	destroyAllWindows();
	config_Write(config, argv[2]);
	return (EXIT_SUCCESS);
}

void processVideo(char *src, unordered_map<string, string> config, const char *fname)
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
			config_GetDouble(config, "sensor_cell_ratio", 0.3),
			config_GetDouble(config, "sensor_ratio", 5)
			);

	int config_median = config_GetInt(config, "median", 5);
	int config_erode = config_GetInt(config, "erode", 3);
	int config_dilate = config_GetInt(config, "dilate", 3);

	bool play = true;

	while (true) {
		if (play) {
			if (!cap.read(origin)) {
				break;
			}


			//printf("\rProgress: %d/%d", no_frame , 202518);
			no_frame += 1;

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
						config_GetInt(config, "vibe_thres", 32));

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
					config_median,
					config_erode,
					config_dilate);

			sensor_Feed(sensor, foreground);
			drawer_DrawSensor(origin, sensor);


			putText(origin, to_string(no_frame).c_str(), Point(50,50), FONT_HERSHEY_PLAIN, 2,  Scalar(0,255,0,255));

			imshow("Origin", origin);
			imshow("Foreground", foreground);
			printf("\rUp: %f (%d), Down: %f (%d)",
					sensor->up / sensor->ratio,
					sensor->up,
					sensor->down/sensor->ratio,
					sensor->down);

		}
		char wkey = waitKey(speed);
		if (wkey == 'p') {
			play = !play;
		} else if (wkey == 'q') {
			break;
		} else if (wkey == 'w' || wkey == 'h'){
			gui_flag = wkey;
		} else if (wkey == '1') {
			speed = 1;
		} else if (wkey == '2') {
			speed = 200000;
		}
	}


	cap.release();
	sensor_Export(sensor, fname);
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

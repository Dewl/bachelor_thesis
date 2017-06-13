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


#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>

#include "vibe.h"
#include "config.h"
#include "sensor.h"

using namespace std;
using namespace cv;

// Global config
unordered_map<string, string> config;
Sensor *sensor;
Mat origin, foreground, croppedOrigin;

// Prototypes
void processVideo(char *src, unordered_map<string, string> config, const char *fname);

int main(int args, char **argv)
{
	printf("> Reading config: %s\n", argv[1]);
	config = config_Read(argv[2]);
	printf("> Processing...\n");
	processVideo(argv[1], config, argv[3]);
	printf("> Writing config: %s\n", argv[1]);
	config_Write(config, argv[2]);
	printf("Exitting...\n");
	return (EXIT_SUCCESS);
}

void processVideo(char *src, unordered_map<string, string> config, const char *fname)
{
	int no_frame = 0;
	//double ratio = -1;
	vector<int> total_up;
	vector<int> total_down;
	vector<int> truth;

	total_up.push_back(0);
	total_down.push_back(0);
	VideoCapture cap(src);
	if (!cap.isOpened()) {
		cerr << ("ERROR: Unable to open video source.") << endl;
		exit(EXIT_FAILURE);
	}

	printf("> Creating VIBE model\n");
	vibeModel_Sequential_t *model = NULL;
	bool init = false;

	printf("> Creating Virtual Sensor\n");
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

	Rect sensorRect(
			Point(sensor->xstart, sensor->ystart),
			Point(sensor->xend, sensor->yend)
		       );
	Mat tmp;

	printf("> Processing frames\n");
	while (true) {
		if (!cap.read(origin)) {
			break;
		}

		tmp = origin(sensorRect);
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

		medianBlur(foreground,foreground,config_median);
		erode(foreground,foreground, Mat(), Point(-1, -1), config_erode);
		dilate(foreground, foreground, Mat(), Point(-1, -1), config_dilate);

		sensor_Feed(sensor, foreground);
		no_frame += 1;

		if (no_frame % 10000 == 0) {
			printf("\n");
			int batch = (no_frame / 10000);
			total_up.push_back(sensor->up);
			total_down.push_back(sensor->down);
			int size = total_up.size();
			int up_dif = total_up[size - 1] - total_up[size - 2];
			int down_dif = total_down[size - 1] - total_down[size - 2];

			printf("Batch %d: Dif Up = %d, Dif Down = %d\n", batch, up_dif, down_dif);
			printf("Up = %d, Down = %d\n", sensor->up, sensor->down);
		} else {
			printf("\rProgress: %f%%", (no_frame % 10000) / 100.0);
		}
	}

	printf("Releasing resources\n");
	cap.release();
	sensor_Export(sensor, fname);
	sensor_Free(sensor);
	libvibeModel_Sequential_Free(model);
}


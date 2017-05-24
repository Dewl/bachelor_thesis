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

using namespace std;
using namespace cv;

void processVideo(char *src, unordered_map<string, string> config);

int main(int args, char **argv)
{
	unordered_map<string, string> config = config_Read(argv[2]);
	processVideo(argv[1], config);
	destroyAllWindows();
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
	Mat origin, foreground;

	vibeModel_Sequential_t *model = NULL;	
	bool init = false;

	while (true) {
		if (!cap.read(origin)) {
			break;
		}

		if (!init) {
			init = true;
			foreground = Mat(origin.rows, origin.cols, CV_8UC1);
			model = (vibeModel_Sequential_t*)
				libvibeModel_Sequential_New();

			libvibeModel_Sequential_SetMatchingThreshold(model,
					config_GetInt(config, "threshold"));

			libvibeModel_Sequential_AllocInit_8u_C3R(model,
					origin.data,
					origin.cols,
					origin.rows);
		}

		libvibeModel_Sequential_Segmentation_8u_C3R(model,
				origin.data,
				foreground.data);
		libvibeModel_Sequential_Update_8u_C3R(model,
				origin.data,
				foreground.data);

		refineBinaryImage(foreground,
				config_GetInt(config, "median"),
				config_GetInt(config, "erode"),
				config_GetInt(config, "dilate"));

		imshow("Origin", origin);
		imshow("Foreground", foreground);

		if (waitKey(22) == 'q') {
			break;
		}
	}

	cap.release();
	libvibeModel_Sequential_Free(model);
}


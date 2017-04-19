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

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "vibe.h"
#include "matutil.h"
#include "devutil.h"
#include "const.h"
#include "tracker.h"
#include "extractor.h"

using namespace std;
using namespace cv;

#define INPUT "input.avi"

#define Y_UP 100
#define Y_DOWN 400

#define BLOB_SIZE 4000
#define TTL 30
#define RATIO 0.6

/* Prototypes */
void processVideo(char *src);

/**
 * This is for testing purposes only. The input is fixed with input.mp4
 */
int main(int args, char **argv)
{
	processVideo(argv[1]);
	destroyAllWindows();
	return (EXIT_SUCCESS);
}

void processVideo(char *src)
{
	VideoCapture cap(src);

	if (!cap.isOpened()) {
		error("ERROR: Unable to open video source.");
	}

	Mat frame;
	Mat origin, foreground;

	vibeModel_Sequential_t *model = NULL;	
	bool init = false;

	Tracker tracker(Y_UP, Y_DOWN, TTL);
	Extractor extractor;
	extractor.setMinArea(BLOB_SIZE);
	extractor.setRatio(RATIO);

	while (true) {
		if (!cap.read(origin)) {
			break;
		}

		if (!init) {
			init = true;
			foreground = Mat(origin.rows, origin.cols, CV_8UC1);
			model = (vibeModel_Sequential_t*)
				libvibeModel_Sequential_New();
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

		refineBlob(foreground);

		vector<vector<Point> > contours;
		findContours(foreground.clone(), contours, CV_RETR_EXTERNAL,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<vector<Point> > filteredBlobs = extractor.extractPoints(contours);

		tracker.receive(filteredBlobs);
		tracker.display(origin);

		imshow("Tracking", origin);
		imshow("Blob", foreground);

		if (waitKey(33) == 'q') {
			break;
		}
	}

	cap.release();
	libvibeModel_Sequential_Free(model);
}

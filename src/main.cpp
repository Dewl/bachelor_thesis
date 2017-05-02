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
#include "extractor.h"
#include "debug.h"
#include "const.h"
#include "contour.h"
#include "tracker.h"
#include "counter.h"

using namespace std;
using namespace cv;

void processVideo(char *src);

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
	
	Tracker tracker;
	Extractor extractor(750, 0.2, true, 220, 420);
	Counter counter(true, 240, 400);
	list<Blob> blobs;

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

		refineBinaryImage(foreground);

		blobs = extractor.extractBOI(foreground);
		tracker.receive(blobs);
		counter.receive(tracker.lostBlobs);

		blobDebug(origin, tracker.blobs, true, 220, 420);
		counterDebug(origin, counter);

		imshow("Origin", origin);
		imshow("Foreground", foreground);

		if (waitKey(22) == 'q') {
			break;
		}
	}

	cap.release();
	libvibeModel_Sequential_Free(model);
}


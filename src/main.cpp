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
 * 
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

//#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "vibe.h"
#include "matutil.h"
#include "devutil.h"
#include "const.h"

using namespace std;
using namespace cv;

#define INPUT "input.mp4"

/* Prototypes */
void processVideo();

/**
 * This is for testing purposes only. The input is fixed with input.mp4
 */
int main()
{
	processVideo();
	destroyAllWindows();
	return (EXIT_SUCCESS);
}

/* This routine takes the video, does the frame segmentation, then displays
 * on a window */
void processVideo()
{
	VideoCapture cap(INPUT);
	if (!cap.isOpened()) {
		error("ERROR: Unable to open video source.");
	}

	Mat curFrame, segFrame;
	vibeModel_Sequential_t *model = NULL;
	bool init = false;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	while (true) {
		if (!cap.read(curFrame)) {
			error("ERROR: Failed to grab the next video frame.");
		}

		if (!init) {
			init = true;
			segFrame = Mat(curFrame.rows, curFrame.cols, CV_8UC1);
			model = (vibeModel_Sequential_t*)
				libvibeModel_Sequential_New();
			libvibeModel_Sequential_AllocInit_8u_C3R(model,
					curFrame.data,
					curFrame.cols,
					curFrame.rows);
		}

		libvibeModel_Sequential_Segmentation_8u_C3R(model,
				curFrame.data, segFrame.data);
		libvibeModel_Sequential_Update_8u_C3R(model,
				curFrame.data, segFrame.data);
		refineBlob(segFrame);

		findContours(segFrame.clone(), contours, hierarchy, CV_RETR_EXTERNAL,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		for (unsigned int i = 0; i < contours.size(); ++i) {
			int contArea = contourArea(contours[i]);
			if (contArea < 3000)
				continue;
			Rect rect = boundingRect(contours[i]);
			//Point center = rectCenter(rect);
			drawRect(curFrame, rect);
		}

		imshow("Result", segFrame);
		imshow("Original", curFrame);

		if (waitKey(33) == 'q') {
			break;
		}
	}
	
	cap.release();
	libvibeModel_Sequential_Free(model);
}

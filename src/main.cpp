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

#include <opencv/cv.h>
#include <opencv/highgui.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/video/video.hpp>
//#include <opencv2/video/tracking.hpp>

#include "vibe.h"
#include "matutil.h"
#include "devutil.h"
#include "const.h"
#include "tracker.h"

using namespace std;
using namespace cv;

#define INPUT "input.avi"

#define Y_UP 150
#define Y_DOWN 350

#define BLOB_SIZE 4000
#define TTL 30

/* Prototypes */
void processVideo();
void start(KalmanFilter, Mat, Mat);

/**
 * This is for testing purposes only. The input is fixed with input.mp4
 */
int main()
{
	processVideo();
	destroyAllWindows();
	return (EXIT_SUCCESS);
}

void processVideo()
{
	VideoCapture cap(INPUT);

	if (!cap.isOpened()) {
		error("ERROR: Unable to open video source.");
	}

	Mat frame;
	Mat origin, foreground;

	vibeModel_Sequential_t *model = NULL;	
	bool init = false;

	Tracker tracker(Y_UP, Y_DOWN, TTL);

	while (true) {
		if (!cap.read(origin)) {
			break;
		}

		//drawBoundary(origin, Y_UP, Y_DOWN);
		//drawInfo(origin, in, out);

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

		//vector<vector<Point> > blobs;
		//vector<Rect> blobRect;
		vector<Point> centerPoints;

		for (size_t i = 0; i < contours.size(); i++) {
			Rect bBox = boundingRect(contours[i]);
			float ratio = (float) bBox.width / (float) bBox.height;

			if (ratio > 1.0f)
				ratio = 1.0f / ratio;

			if (ratio > 0.6 && bBox.area() > BLOB_SIZE) {
				//blobs.push_back(contours[i]);
				//blobRect.push_back(bBox);
				centerPoints.push_back(rectCenter(bBox));
			}
		}

		tracker.receive(centerPoints);
		tracker.display(origin);

		//for (size_t i = 0; i < blobs.size(); i++)
		//{
			//drawRect(origin, blobRect[i], i);
			//Point center = rectCenter(blobRect[i]);
			//cout << "debug:center: " << center.y << endl;
			//drawPoint(origin, center);
		//}
		
		

		// Final result
		imshow("Tracking", origin);
		//imshow("Result", foreground);

		if (waitKey(33) == 'q') {
			break;
		}
	}

	cap.release();
	libvibeModel_Sequential_Free(model);
}

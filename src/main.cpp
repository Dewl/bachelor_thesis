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

#include "vibe.h"

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
		cerr << "Can not open video" << endl;
		exit(EXIT_FAILURE);
	}

	Mat curFrame, segFrame;
	vibeModel_Sequential_t *model = NULL;
	bool init = false;

	namedWindow("Original", WINDOW_NORMAL);
	namedWindow("Result", WINDOW_NORMAL);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	while (true) {
		if (!cap.read(curFrame)) {
			cerr << "Can not grab the next frame" << endl;
			exit(EXIT_FAILURE);
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
	
		medianBlur(segFrame, segFrame, 5);
		erode(segFrame, segFrame, Mat(), Point(-1, -1), 2);
		dilate(segFrame, segFrame, Mat(), Point(-1, -1), 2);

		contours.clear();
		hierarchy.clear();
		findContours(segFrame.clone(), contours, hierarchy, CV_RETR_EXTERNAL,
				CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		for (int i = 0; i < contours.size(); ++i) {
			int contArea = contourArea(contours[i]);
			if (contArea < 3000)
				continue;
			Rect rect = boundingRect(contours[i]);
			rectangle(curFrame, rect.tl(),
					rect.br(), Scalar(0, 255, 0), 2);
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

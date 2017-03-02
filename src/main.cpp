#include <iostream>

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

	while (true) {
		if (!cap.read(curFrame)) {
			cerr << "Can not grab the next frame" << endl;
			exit(EXIT_FAILURE);
		}

		if (!init) {
			cout << "debug: first run, initializing..." << endl;
			init = true;
			segFrame = Mat(curFrame.rows, curFrame.cols, CV_8UC1);
			model = (vibeModel_Sequential_t*) libvibeModel_Sequential_New();
			libvibeModel_Sequential_AllocInit_8u_C3R(model,
					curFrame.data,
					curFrame.cols,
					curFrame.rows);
		}

		libvibeModel_Sequential_Segmentation_8u_C3R(model, curFrame.data, segFrame.data);
		libvibeModel_Sequential_Update_8u_C3R(model, curFrame.data, segFrame.data);
		
		medianBlur(segFrame, segFrame, 5);

		imshow("Original", curFrame);
		imshow("Result", segFrame);

		if (waitKey(33) == 'q') {
			break;
		}
	}
	
	cap.release();
	libvibeModel_Sequential_Free(model);
}
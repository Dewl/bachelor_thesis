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


// Module core
#include <opencv2/core/core.hpp>
// Module image process
#include <opencv2/imgproc/imgproc.hpp>
// Module video
#include <opencv2/video/video.hpp>
// Module Kalman Filter
#include <opencv2/video/tracking.hpp>

#include "vibe.h"
#include "matutil.h"
#include "devutil.h"
#include "const.h"

using namespace std;
using namespace cv;

#define INPUT "input.mp4"
#define drawCross(img, center, color, d )                                 \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

/* Prototypes */
void processVideo_initial();
void processVideo_running(KalmanFilter, Mat, Mat);

/**
 * This is for testing purposes only. The input is fixed with input.mp4
 */
int main()
{
	processVideo_initial();
	destroyAllWindows();
	return (EXIT_SUCCESS);
}

/* This routine takes the video, initialize the necessary requirement for
 * Kalman Filter tracking */
void processVideo_initial()
{	
    	// >>>> Kalman Filter
    	int stateSize = 6;
    	int measSize = 4;
    	int contrSize = 0;

    	unsigned int type = CV_32F;
    	KalmanFilter kf(stateSize, measSize, contrSize, type);

    	Mat state(stateSize, 1, type);  // [x,y,v_x,v_y,w,h]
    	Mat meas(measSize, 1, type);    // [z_x,z_y,z_w,z_h]
    	//cv::Mat procNoise(stateSize, 1, type)
    	// [E_x,E_y,E_v_x,E_v_y,E_w,E_h]

    	// Transition State Matrix A
    	// Note: set dT at each processing step!
    	// [ 1 0 dT 0  0 0 ]
    	// [ 0 1 0  dT 0 0 ]
    	// [ 0 0 1  0  0 0 ]
    	// [ 0 0 0  1  0 0 ]
    	// [ 0 0 0  0  1 0 ]
    	// [ 0 0 0  0  0 1 ]
    	setIdentity(kf.transitionMatrix);

    	// Measure Matrix H
    	// [ 1 0 0 0 0 0 ]
    	// [ 0 1 0 0 0 0 ]
    	// [ 0 0 0 0 1 0 ]
    	// [ 0 0 0 0 0 1 ]
    	kf.measurementMatrix = Mat::zeros(measSize, stateSize, type);
    	kf.measurementMatrix.at<float>(0) = 1.0f;
    	kf.measurementMatrix.at<float>(7) = 1.0f;
    	kf.measurementMatrix.at<float>(16) = 1.0f;
    	kf.measurementMatrix.at<float>(23) = 1.0f;

    	// Process Noise Covariance Matrix Q
    	// [ Ex   0   0     0     0    0  ]
    	// [ 0    Ey  0     0     0    0  ]
    	// [ 0    0   Ev_x  0     0    0  ]
    	// [ 0    0   0     Ev_y  0    0  ]
    	// [ 0    0   0     0     Ew   0  ]
    	// [ 0    0   0     0     0    Eh ]
    	//cv::setIdentity(kf.processNoiseCov, cv::Scalar(1e-2));
    	kf.processNoiseCov.at<float>(0) = 1e-2;
    	kf.processNoiseCov.at<float>(7) = 1e-2;
    	kf.processNoiseCov.at<float>(14) = 5.0f;
    	kf.processNoiseCov.at<float>(21) = 5.0f;
    	kf.processNoiseCov.at<float>(28) = 1e-2;
    	kf.processNoiseCov.at<float>(35) = 1e-2;

    	// Measures Noise Covariance Matrix R
    	setIdentity(kf.measurementNoiseCov, Scalar(1e-1));
    	// <<<< Kalman Filter

	processVideo_running(kf, state, meas);
	
}

void processVideo_running(KalmanFilter kf, Mat state, Mat meas)
{
	VideoCapture cap(INPUT);
	
	if (!cap.isOpened()) {
		cerr << "Can not open video" << endl;
		exit(EXIT_FAILURE);
	}

	Mat frame;
	Mat curFrame, segFrame;
    	vibeModel_Sequential_t *model = NULL;	
	
	bool init = false;
	namedWindow("Result", WINDOW_NORMAL);
	double ticks = 0;
	
	bool found = false;
	int notFoundCount = 0;
        // Frame acquisition
        	
        	
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
			libvibeModel_Sequential_AllocInit_8u_C3R(model,	curFrame.data, curFrame.cols, curFrame.rows);
		}

		libvibeModel_Sequential_Segmentation_8u_C3R(model, curFrame.data, segFrame.data);
		libvibeModel_Sequential_Update_8u_C3R(model, curFrame.data, segFrame.data);

		medianBlur(segFrame, segFrame, 5);
		erode(segFrame, segFrame, Mat(), Point(-1, -1), 2);
		dilate(segFrame, segFrame, Mat(), Point(-1, -1), 2);
	
		cap >> frame;
		Mat res;
        	frame.copyTo( res );

		double precTick = ticks;
        	ticks = (double) getTickCount();
        	double dT = (ticks - precTick) / getTickFrequency(); //seconds
        	if (found)
        	{
            		// >>>> Matrix A
            		kf.transitionMatrix.at<float>(2) = dT;
            		kf.transitionMatrix.at<float>(9) = dT;
            		// <<<< Matrix A

            		cout << "dT:" << endl << dT << endl;

            		state = kf.predict();
            		cout << "State post:" << endl << state << endl;

            		Rect predRect;
            		predRect.width = state.at<float>(4);
            		predRect.height = state.at<float>(5);
            		predRect.x = state.at<float>(0) - predRect.width / 2;
            		predRect.y = state.at<float>(1) - predRect.height / 2;

		    	Point center;
		    	center.x = state.at<float>(0);
		    	center.y = state.at<float>(1);
		    	circle(res, center, 2, CV_RGB(255,0,0), -1);

		    	rectangle(res, predRect, CV_RGB(255,0,0), 2);
        	}

		// Noise smoothing
		Mat blur;
		GaussianBlur(curFrame, blur, Size(5, 5), 3.0, 3.0);


		// HSV conversion
		Mat frmHsv;
		cvtColor(blur, frmHsv, CV_BGR2HSV);

		
		// Contours detection
		vector<vector<Point> > contours;
		findContours(segFrame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));		


		// Filtering
		vector<vector<Point> > balls;
		vector<Rect> ballsBox;
		for (size_t i = 0; i < contours.size(); i++)
		{

			Rect bBox;
		    	bBox = boundingRect(contours[i]);

		    	float ratio = (float) bBox.width / (float) bBox.height;
		    	if (ratio > 1.0f)
		        	ratio = 1.0f / ratio;

		    	// Searching for a bBox almost square
		    	if (ratio > 0.75 && bBox.area() >= 400)
		    	{
		        	balls.push_back(contours[i]);
		        	ballsBox.push_back(bBox);
		    	}

			int contArea = contourArea(contours[i]);
			if (contArea < 3000)
				continue;
			Rect rect = boundingRect(contours[i]);
			rectangle(segFrame, rect.tl(),
					rect.br(), COLOR_GREEN, 2);
			
			
  		}
		cout << "Objects found:" << ballsBox.size() << endl;


		// Detection result
		for (size_t i = 0; i < balls.size(); i++)
        	{
			drawContours(res, balls, i, CV_RGB(20,150,20), 1);
		    	rectangle(res, ballsBox[i], CV_RGB(0,255,0), 2);

		    	Point center;
		    	center.x = ballsBox[i].x + ballsBox[i].width / 2;
		    	center.y = ballsBox[i].y + ballsBox[i].height / 2;
		    	circle(res, center, 2, CV_RGB(20,150,20), -1);

		    	stringstream sstr;
		    	sstr << "(" << center.x << "," << center.y << ")";
		    	putText(res, sstr.str(),
                     	   	Point(center.x + 3, center.y - 3),
                     	   	FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(20,150,20), 2);
        	}


		// Kalman Update
		if (balls.size() == 0)
		{
		    	notFoundCount++;
		    	cout << "notFoundCount:" << notFoundCount << endl;
		    	if( notFoundCount >= 100 ){
                		found = false;
            		}
            	    	/*else
                    	kf.statePost = state;*/
        	}
        	else
        	{
            		notFoundCount = 0;

			meas.at<float>(0) = ballsBox[0].x + ballsBox[0].width / 2;
		    	meas.at<float>(1) = ballsBox[0].y + ballsBox[0].height / 2;
		    	meas.at<float>(2) = (float)ballsBox[0].width;
		    	meas.at<float>(3) = (float)ballsBox[0].height;

            		if (!found) // First detection!
            		{
				// >>>> Initialization
				kf.errorCovPre.at<float>(0) = 1; // px
				kf.errorCovPre.at<float>(7) = 1; // px
				kf.errorCovPre.at<float>(14) = 1;
				kf.errorCovPre.at<float>(21) = 1;
				kf.errorCovPre.at<float>(28) = 1; // px
				kf.errorCovPre.at<float>(35) = 1; // px

				state.at<float>(0) = meas.at<float>(0);
				state.at<float>(1) = meas.at<float>(1);
				state.at<float>(2) = 0;
				state.at<float>(3) = 0;
				state.at<float>(4) = meas.at<float>(2);
				state.at<float>(5) = meas.at<float>(3);
				// <<<< Initialization

                		kf.statePost = state;
                
                		found = true;
            		}
            		else
                		kf.correct(meas); // Kalman Correction

            		cout << "Measure matrix:" << endl << meas << endl;
        	}

        	// Final result
        	imshow("Tracking", res);
		imshow("Result", segFrame);

		if (waitKey(33) == 'q') {
			break;
		}

	}
}

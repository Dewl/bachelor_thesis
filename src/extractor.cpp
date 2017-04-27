#include "extractor.h"
#include "cvutil.h"

static vector<Contour> filter(const vector<Contour>& contours);

vector<Contour> extractBOI(const Mat& frame)
{
	vector<Contour> contours;
	
	findContours(frame.clone(), contours, CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<Contour> ret = filter(contours);

	return ret;
}

static vector<Contour> filter(const vector<Contour>& contours)
{
	vector<Contour> ret;

	for (unsigned int i = 0; i < contours.size(); ++i) {
		if (contourArea(contours[i]) > 500 &&
				contourBoundingRatio(contours[i]) > 0.6) {
			ret.push_back(contours[i]);
		}
	}

	return ret;
}

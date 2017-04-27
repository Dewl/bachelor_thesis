#include "extractor.h"
#include "cvutil.h"

static list<Blob> filterBlob(const vector<Contour>& contours);

list<Blob> extractBOI(const Mat& frame)
{
	vector<Contour> contours;
	
	findContours(frame.clone(), contours, CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	list<Blob> ret = filterBlob(contours);

	return ret;
}

static list<Blob> filterBlob(const vector<Contour>& contours)
{
	list<Blob> ret;

	for (unsigned int i = 0; i < contours.size(); ++i) {
		if (contourArea(contours[i]) > 500 &&
				contourBoundingRatio(contours[i]) > 0.6) {
			Contour cur = contours[i];
			ret.push_back(Blob(cur));
		}
	}

	return ret;
}

#include "extractor.h"
#include "cvutil.h"

Extractor::Extractor(int _area,
		    double _ratio,
		    bool _hor,
		    int _bound1,
		    int _bound2)
{
	area = _area;
	ratio = _ratio;
	bound1 = _bound1;
	bound2 = _bound2;
	hor = _hor;
}

list<Blob> Extractor::extractBOI(const Mat& frame)
{
	vector<Contour> contours;
	
	findContours(frame.clone(), contours, CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	list<Blob> ret = filterBlob(contours);

	return ret;
}

list<Blob> Extractor::filterBlob(const vector<Contour>& contours)
{
	list<Blob> ret;

	for (unsigned int i = 0; i < contours.size(); ++i) {
		int conArea = contourArea(contours[i]);
		double conRatio = contourBoundingRatio(contours[i]);
		double conX = contourCentroid(contours[i]).x;
		double conY = contourCentroid(contours[i]).x;
		double conBound1 = false, conBound2 = false;

		if (hor) {
			conBound1 = conX >= bound1;
			conBound2 = conX <= bound2;
		} else {
			conBound1 = conY >= bound1;
			conBound2 = conY <= bound2;
		}

		if (conArea >= area &&
		    conRatio >= ratio &&
		    conBound1 && conBound2) {
			Contour cur = contours[i];
			ret.push_back(Blob(cur));
		}
	}

	return ret;
}

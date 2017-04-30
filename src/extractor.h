#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <opencv/cv.h>
#include <list>

#include "contour.h"
#include "blob.h"

using namespace cv;
using namespace std;

class Extractor {
	
	private:
		list<Blob> filterBlob(const vector<Contour>& contours);

	public:
		int area;
		double ratio;
		int bound1;
		int bound2;
		bool hor;

		Extractor(int area,
			  double ratio,
			  bool hor,
			  int bound1,
			  int bound2
			  );
		list<Blob> extractBOI(const Mat& frame);
};

#endif /* !EXTRACTOR_H */

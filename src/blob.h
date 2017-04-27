#ifndef BLOB_H
#define BLOB_H

#include <list>
#include <opencv/cv.h>

#include "contour.h"

using namespace std;
using namespace cv;

class Blob {
	public:
		list<Point> path;
		int area;
		Rect bBox;

		Blob(const Contour& _contour);
		void update(const Contour& _contour);
		double distance(const Blob& _blob);
};

#endif /* !BLOB_H */

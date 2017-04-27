#ifndef BLOB_H
#define BLOB_H

#include <list>
#include <opencv/cv.h>

#include "contour.h"

using namespace std;
using namespace cv;

class blob {
	public:
		list<Point> path;
		int area;
		Rect bBox;

		blob(const contour& _contour);
		void update(const contour& _contour);
}

#endif /* !BLOB_H */

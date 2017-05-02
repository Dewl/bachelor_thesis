#ifndef BLOB_H
#define BLOB_H

#include <list>
#include <opencv/cv.h>

#include "contour.h"

using namespace std;
using namespace cv;

class Blob {
	private:
		double distance(const Blob& _blob) const;
	public:
		list<Point> path;
		int area;
		Rect bBox;
		bool associated;
		int estimate;

		Blob(const Contour& _contour);
		void update(const Blob& _blob);
		double dif(const Blob& _blob) const;
};

#endif /* !BLOB_H */

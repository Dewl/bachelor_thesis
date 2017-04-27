#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <opencv/cv.h>
#include <list>

#include "contour.h"
#include "blob.h"

using namespace cv;
using namespace std;

list<Blob> extractBOI(const Mat& frame);

#endif /* !EXTRACTOR_H */

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <opencv/cv.h>

#include "contour.h"

using namespace cv;


vector<contour> extractBOI(const Mat& frame);

#endif /* !EXTRACTOR_H */

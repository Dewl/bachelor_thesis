#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <opencv/cv.h>

#include "contour.h"
#include "blob.h"

using namespace cv;


vector<Contour> extractBOI(const Mat& frame);

#endif /* !EXTRACTOR_H */

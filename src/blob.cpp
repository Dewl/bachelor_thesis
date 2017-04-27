#include "blob.h"
#include "cvutil.h"

blob::blob(const contour& _contour)
{
	this.update(_contour);
}

void blob::update(const contour& _contour)
{
	this.path.push_back(this.centroid);
	this.area = contourArea(_contour, false);
	this.bBox = boundingRect(_contour);
}

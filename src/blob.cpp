#include "blob.h"
#include "cvutil.h"

Blob::Blob(const Contour& _contour)
{
	this.update(_contour);
}

void Blob::update(const Contour& _contour)
{
	this.path.push_back(this.centroid);
	this.area = contourArea(_contour, false);
	this.bBox = boundingRect(_contour);
}

double Blob::distance(const Blob& _blob)
{
	return norm(this.path.back() - _blob.back());
}

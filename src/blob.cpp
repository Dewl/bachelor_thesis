#include "blob.h"
#include "cvutil.h"
#include "const.h"

Blob::Blob(const Contour& _contour)
{
	Point centroid = contourCentroid(_contour);
	path.push_back(centroid);
	area = contourArea(_contour, false);
	bBox = boundingRect(_contour);
	associated = false;
	estimate = 1;
}

void Blob::update(const Blob& _blob)
{
	path.push_back(_blob.path.back());
	area = _blob.area;
	bBox = _blob.bBox;
}

double Blob::distance(const Blob& _blob) const
{
	return norm(path.back() - _blob.path.back());
}

double Blob::dif(const Blob& _blob) const
{
	return distance(_blob);
}

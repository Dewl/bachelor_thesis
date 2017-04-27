#include "blob.h"
#include "cvutil.h"

Blob::Blob(const Contour& _contour)
{
	update(_contour);
}

void Blob::update(const Contour& _contour)
{
	Point centroid = contourCentroid(_contour);
	path.push_back(centroid);
	area = contourArea(_contour, false);
	bBox = boundingRect(_contour);
}

double Blob::distance(const Blob& _blob)
{
	return norm(path.back() - _blob.path.back());
}

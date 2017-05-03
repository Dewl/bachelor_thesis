/* Copyright (C) 
 * 2017 - Khoi Hoang
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

/**
 * @file blob.cpp
 * @brief The implementation for blob.h
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-02
 */

#include "blob.h"
#include "cvutil.h"
#include "const.h"

Blob::Blob(const Contour& _contour)
{
	Point centroid = contourCentroid(_contour);
	path.push_back(centroid);
	int curArea = contourArea(_contour, false);
	area.push_back(curArea);
	bBox = boundingRect(_contour);
	ratio.push_back(rectRatio(bBox));
	associated = false;
	counted = false;
	estimation = 1;
}

void Blob::associate(const Blob& _blob)
{
	double dist = distance(_blob);
	speed.push_back(dist);
	path.push_back(_blob.path.back());
	area.push_back(_blob.area.back());
	bBox = _blob.bBox;
	ratio.push_back(_blob.ratio.back());
}

double Blob::distance(const Blob& _blob) const
{
	return euclideanDistance(path.back(), _blob.path.back());
}

double Blob::dif(const Blob& _blob) const
{
	return distance(_blob);
}

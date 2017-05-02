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
 * @file extractor.cpp
 * @brief The implementation of extractor.h
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-02
 */

#include "extractor.h"
#include "cvutil.h"

Extractor::Extractor(int _area,
		    double _ratio,
		    bool _hor,
		    int _bound1,
		    int _bound2)
{
	area = _area;
	ratio = _ratio;
	bound1 = _bound1;
	bound2 = _bound2;
	hor = _hor;
}

list<Blob> Extractor::extractBOI(const Mat& frame)
{
	vector<Contour> contours;
	
	findContours(frame.clone(), contours, CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	list<Blob> ret = filterBlob(contours);

	return ret;
}

list<Blob> Extractor::filterBlob(const vector<Contour>& contours)
{
	list<Blob> ret;

	for (unsigned int i = 0; i < contours.size(); ++i) {
		int conArea = contourArea(contours[i]);
		double conRatio = contourBoundingRatio(contours[i]);
		double conX = contourCentroid(contours[i]).x;
		double conY = contourCentroid(contours[i]).x;
		double conBound1 = false, conBound2 = false;

		if (hor) {
			conBound1 = conX >= bound1;
			conBound2 = conX <= bound2;
		} else {
			conBound1 = conY >= bound1;
			conBound2 = conY <= bound2;
		}

		if (conArea >= area &&
		    conRatio >= ratio &&
		    conBound1 && conBound2) {
			Contour cur = contours[i];
			ret.push_back(Blob(cur));
		}
	}

	return ret;
}

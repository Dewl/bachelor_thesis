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
 * @file extractor.h
 * @brief The implementation for extractor.h
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-04-01
 */
#include "extractor.h"
#include "matutil.h"

void Extractor::setMinArea(int a)
{
	minArea = a;
}

void Extractor::setRatio(double r)
{
	ratio = r;
}

vector<Point> Extractor::extractPoints(vector<vector<Point> >& c)
{
	vector<Point> centerPoints;
	for (size_t i = 0; i < c.size(); i++) {
		Rect bBox = boundingRect(c[i]);
		float r = (float) bBox.width / (float) bBox.height;

		if (r > 1.0f)
			r = 1.0f / r;

		if (r > ratio && bBox.area() > minArea) {
			centerPoints.push_back(rectCenter(bBox));
		}
	}

	return centerPoints;
}

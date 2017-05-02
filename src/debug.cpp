/* Copyright (C) 
 * 2017 - Khoi Hoang, Kiet Chuong
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
 * @file devutil.cpp
 * @brief Implementation for devutil.h
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-16
 */

#include <iostream>
#include <cstdlib>
#include <string>

#include "debug.h"
#include "cvutil.h"

void debug(string tag, string msg)
{
	if (tag == TAG)
		cerr << msg << endl;
}

void debug(string tag, string msg, int val)
{
	if (tag == TAG)
		cerr << msg << ": " << val << endl;
}

void debug(string tag, string msg, double val)
{
	if (tag == TAG)
		cerr << msg << ": " << val << endl;
}

void debug(string tag, string msg, string val)
{
	if (tag == TAG)
		cerr << msg << ": " << val << endl;
}

void error(string msg)
{
	cerr << msg << endl;
	exit(EXIT_FAILURE);
}

void contourDebug(Mat& canvas, const vector<Contour>& contours)
{
	for (unsigned int i = 0; i < contours.size(); ++i) {
		Contour cur = contours[i];

		Rect bBox = boundingRect(cur);
		drawRect(canvas, bBox);

		Point centroid = rectCentroid(bBox);
		drawPoint(canvas, centroid);

		double area = contourArea(cur, false);
		double ratio = rectRatio(bBox);
		string info = "A=" + to_string(area) + ", R="
			+ to_string(ratio);
		drawTextRect(canvas, info, bBox);
	}
}

void blobDebug(Mat& canvas, const list<Blob>& blobs,
	       bool hor, int val1, int val2)
{
	if (hor) {
		drawVerticalLine(canvas, val1);
		drawVerticalLine(canvas, val2);
	}

	list<Blob>::const_iterator it = blobs.cbegin();
	while (it != blobs.cend()) {
		list<Point>::const_iterator pathIt = it->path.cbegin();

		while (pathIt != it->path.cend()) {
			drawPoint(canvas, *pathIt);
			++pathIt;
		}
		
		string info = "a:" + to_string(rectRatio(it->bBox));
		drawTextRect(canvas, info, it->bBox);
		drawRect(canvas, it->bBox);
		++it;
	}
}

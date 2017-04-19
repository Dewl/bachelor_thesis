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
 * @file tracker.cpp
 * @brief The implementation for Tracker.
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-31
 */

#include <climits>
#include <iostream>

#include "tracker.h"
#include "matutil.h"

using namespace std;

Tracker::Tracker(int yu, int yd, int timetl)
{
	countUp = 0;
	countDown = 0;

	y_upper = yu;
	y_lower = yd;

	ttl = timetl;
}

int Tracker::requestNewId()
{
	for (int i = 0; i < TRACKER_CAP; ++i) {
		if (!blobPool[i].isActivated()) {
			return i;
		}
	}
	
	return -1;
}

int Tracker::requestBestId(const vector<Point>& blob)
{
	double minDistance = DBL_MAX;
	int bestId = -1;

	for (int i = 0; i < TRACKER_CAP; ++i) {
		if (blobPool[i].isActivated()) {
			double distance = blobPool[i].distance(blob);
			if (distance < minDistance) {
				minDistance = distance;
				bestId = i;
			}
		}
	}
	return bestId;
}

void Tracker::receive(const vector<vector<Point> >& blobs)
{
	for (int i = 0; i < TRACKER_CAP; ++i) {
		if (blobPool[i].isActivated()) {
			blobPool[i].increaseDeadTime();
			int deadTime = 0;
			if (blobPool[i].isUpper() || blobPool[i].isLower()) {
				deadTime = ttl;
			} else {
				deadTime = 2 * ttl;
			}
			
			if (blobPool[i].getTtl() > deadTime) {
				blobPool[i].deactivate();
			}
		}
	}

	for (unsigned int i = 0; i < blobs.size(); ++i) {
		vector<Point> curContour = blobs[i];
		int id = requestBestId(curContour);

		if (id < 0) {
			id = requestNewId();
			blobPool[id].init();
		}

		blobPool[id].update(curContour);

		if (blobPool[i].getY() < y_upper) {
			if (blobPool[id].getFlag()
					== Blob::STATE_MIDDLE_PART) {
				countUp += 1;
			}
			blobPool[id].setFlagToUpper();
		} else if (blobPool[i].getY() >= y_upper && blobPool[i].getY() <= y_lower) {
			blobPool[id].setFlagToMiddle();
		} else {
			if (blobPool[id].getFlag()
					== Blob::STATE_MIDDLE_PART) {
				countDown += 1;
			}
			blobPool[id].setFlagToLower();
		}
	}
}

void Tracker::display(Mat& canvas)
{
	drawBoundary(canvas, y_upper, y_lower);
	drawInfo(canvas, countUp, countDown);

	for (int i = 0; i < TRACKER_CAP; ++i) {
		if (blobPool[i].isActivated()) {
			Point center = blobPool[i].getCenter();
			ostringstream convert;
			convert << i;
			drawPoint(canvas, center);
			drawText(canvas, convert.str(), center);

		}
	}
}

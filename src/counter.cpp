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
 * @file counter.cpp
 * @brief The implementation for counter.h
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-03
 */

#include "counter.h"

Counter::Counter(bool _hor, int _bound1, int _bound2)
{
	hor = _hor;
	bound1 = _bound1;
	bound2 = _bound2;
	lowerCount = 0;
	upperCount = 0;
}

void Counter::receive(list<Blob>& blobs)
{
	for (lBi it = blobs.begin(); it != blobs.end(); ++it) {
		processBlob(*it);
	}
}

void Counter::processBlob(Blob& blob)
{
	if (blob.counted) {
		return;
	}

	lPcri it = blob.path.crbegin();
	int initial = getPathValue(blob.path.back());
	if (initial >= bound2) {
		while (it != blob.path.crend()) {
			if (getPathValue(*it) < bound2) {
				upperCount += blob.estimation;
				// TODO: someone passing the line
				cout << "debug:counter:upper line" << endl;
				blob.counted = true;
				break;
			}
			++it;
		}
	} else if (initial <= bound1) {
		while (it != blob.path.crend()) {
			if (getPathValue(*it) > bound1) {
				// TODO: someone passing the line
				lowerCount += blob.estimation;
				cout << "debug:counter:lower line" << endl;
				blob.counted = true;
				break;
			}
			++it;
		}
	}
}

int Counter::getPathValue(const Point& point)
{
	if (hor) {
		return point.x;
	}

	return point.y;
}

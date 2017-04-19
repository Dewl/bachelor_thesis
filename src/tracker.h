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
 * You should have d a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 */

/**
 * @file tracker.h
 * @brief Tracker
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-31
 */

#include <opencv/cv.h>

#include "blob.h"

using namespace cv;

#ifndef TRACKER_H
#define TRACKER_H

class Tracker
{
	private:
		/* This tracker can only hold at most 25 people at once */
		const static int TRACKER_CAP = 25;

		int countUp;
		int countDown;

		int y_upper;
		int y_lower;

		int ttl;

		Blob blobPool[TRACKER_CAP];
		
		/**
		 * @brief Returns the most appropriate ID for the blob.
		 *
		 * @param blob
		 *
		 * @return blob's id
		 */
		int requestBestId(const vector<Point>& point);

		/**
		 * @brief Returns an id, which can be new or reused one.
		 * NOTE: The case of out of id is not handled yet.
		 *
		 * @return id
		 */
		int requestNewId();
	public:
		/**
		 * @brief Constructor
		 */
		Tracker(int y_upper, int y_lower, int ttl);

		/**
		 * @brief Receives a point and adds it to blob pool or track
		 * it if the point was already existed.
		 *
		 * @param point
		 */
		void receive(const vector<vector<Point> >& blobs);

		/**
		 * @brief Return the number of blobs from lower to upper bar.
		 *
		 * @return 
		 */
		int getUp();

		/**
		 * @brief Return the number of blobs from upper to lower bar.
		 *
		 * @return 
		 */
		int getDown();

		/**
		 * @brief Takes the frame and add tracking information on it.
		 * 
		 * @param canvas
		 */
		void display(Mat& canvas);
};

#endif /* !TRACKER_H */

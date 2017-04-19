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
 * @file blob.h
 * @brief Blob model
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-31
 */

#ifndef BLOB_H
#define BLOB_H

#include <opencv/cv.h>
#include <vector>

using namespace cv;
using namespace std;

/**
 * @brief Blob model. Note about flag:
 *	=0: blob just appeared
 *	>0: blob just crossed the upper bar
 *	<0: blob just crossed the lower bar
 */
class Blob
{
	private:
		vector<Point> contour;
		int flag;
		bool activated;
		int ttl;
	public:
		const static int STATE_UPPER_PART = 1;
		const static int STATE_MIDDLE_PART = 0;
		const static int STATE_LOWER_PART = -1;
		const static int STATE_NULL = -2;

		/**
		 * @brief Constructor for blobs.
		 */
		Blob();

		/**
		 * @brief Set blob's position.and activate it.
		 *
		 * @param point
		 */
		void init();

		/**
		 * @brief 
		 *
		 * @param point
		 */
		void update(const vector<Point>& contour);

		/**
		 * @brief Calculates Euclidean distance to another Blob.
		 *
		 * @param b - Blob
		 *
		 * @return Distance.
		 */
		double distance(const vector<Point>& b);

		/**
		 * @brief Deactivate this blob.
		 */
		void deactivate();

		/**
		 * @brief Check if the current block is activated.
		 *
		 * @return 
		 */
		bool isActivated();

		/**
		 * @brief Increases the deadTime for blob.
		 */
		void increaseDeadTime();

		/**
		 * @brief Sets deadTime back to zero.
		 */
		void resetDeadTime();

		/**
		 * @brief Gets center point of the blob.
		 *
		 * @return Center point.
		 */
		Point getCenter();

		/**
		 * @brief Gets flag (status of the blob).
		 */
		int getFlag();

		/**
		 * @brief Sets flag to STATE_UPPER_PART
		 *
		 * @param flag
		 */
		void setFlagToUpper();

		/**
		 * @brief Sets flag to STATE_MIDDLE_PART
		 *
		 * @param flag
		 */
		void setFlagToMiddle();

		/**
		 * @brief Sets flag to STATE_LOWER_PART
		 *
		 * @param flag
		 */
		void setFlagToLower();

		/**
		 * @brief Gets Time To Live.
		 *
		 * @return 
		 */
		int getTtl();

		/**
		 * @brief Returns true if blob is at upper part.
		 *
		 * @return 
		 */
		bool isUpper();

		/**
		 * @brief Returns true if blob is at middle part.
		 *
		 * @return 
		 */
		bool isMiddle();

		/**
		 * @brief Returns true if blob is at lower part.
		 *
		 * @return 
		 */
		bool isLower();


		/**
		 * @brief Get the contour of blob
		 *
		 * @return contour (vector<Point>)
		 */
		vector<Point> getContour();

		/**
		 * @brief Get the Y coordinate
		 *
		 * @return 
		 */
		int getY();
};

#endif

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
 * @brief Model to hold binary blobs
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-02
 */
#ifndef BLOB_H
#define BLOB_H

#include <list>
#include <opencv/cv.h>

#include "contour.h"

using namespace std;
using namespace cv;

class Blob {
	private:
		/**
		 * @brief Calculate 2D distance between 2 blobs.
		 *
		 * @param _blob
		 *
		 * @return 
		 */
		double distance(const Blob& _blob) const;
	public:
		/**
		 * @brief A linked list contains previous centroid position
		 * of the current blob.
		 */
		list<Point> path;

		/**
		 * @brief Current area in pixels of the current blob.
		 */
		int area;

		/**
		 * @brief Bounding rectangle of the current blob.
		 */
		Rect bBox;

		/**
		 * @brief A flag to determine if the current blob is associated
		 * with one of the blob from Tracker Module.
		 */
		bool associated;

		/**
		 * @brief The estimation of number of people in the current
		 * blob.
		 */
		int estimation;

		/**
		 * @brief Constructs Blob from Contour.
		 *
		 * @param _contour
		 */
		Blob(const Contour& _contour);

		/**
		 * @brief Associates this current blob to the best match blob
		 * in Tracker Module.
		 *
		 * @param _blob
		 */
		void associate(const Blob& _blob);

		/**
		 * @brief Calculate the difference between two blobs.
		 * The less, the better.
		 *
		 * @param _blob
		 *
		 * @return 
		 */
		double dif(const Blob& _blob) const;
};

#endif /* !BLOB_H */

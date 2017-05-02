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
 * @brief Extracts the Blobs of Interest (BOIs).
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-02
 */

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <opencv/cv.h>
#include <list>

#include "contour.h"
#include "blob.h"

using namespace cv;
using namespace std;

class Extractor {
	
	private:
		/**
		 * @brief The core logic for filtering out noises.
		 *
		 * @param contours
		 *
		 * @return a linked list of BOIs
		 */
		list<Blob> filterBlob(const vector<Contour>& contours);

	public:
		/**
		 * @brief the minimum area of pixel, if a blob doesn't meet
		 * this condition, it would be filtered out.
		 */
		int area;

		/**
		 * @brief the minimum bounding rectangle ratio for a blob.
		 */
		double ratio;

		/**
		 * @brief Lower boundary of Region of Interest.
		 */
		int bound1;

		/**
		 * @brief Upper boundary of Region of Interest.
		 */
		int bound2;

		/**
		 * @brief Flag to check if the flow of passengers is
		 * horizontal.
		 * True: The flow of people is horizontal.
		 * False: The flow of people is vertical.
		 */
		bool hor;

		/**
		 * @brief Construct the extractor from various parameters.
		 *
		 * @param area
		 * @param ratio
		 * @param hor
		 * @param bound1
		 * @param 
		 */
		Extractor(int area,
			  double ratio,
			  bool hor,
			  int bound1,
			  int bound2
			  );

		/**
		 * @brief The main functionality. Receives the current
		 * foreground segmented frame, outputs a linkedlist
		 * of BOIs.
		 *
		 * @param frame
		 *
		 * @return 
		 */
		list<Blob> extractBOI(const Mat& frame);
};

#endif /* !EXTRACTOR_H */

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
 * @brief Takes responsibility to extract appropriate blobs.
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-04-01
 */

#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <vector>

#include <opencv/cv.h>

using namespace std;
using namespace cv;

class Extractor
{
	private:
		int minArea;
		int ratio;
	public:
		/**
		 * @brief Set minimum area for blobs.
		 *
		 * @param a - integer - area
		 */
		void setMinArea(int a);

		/**
		 * @brief Set ratio for bounding rectangle
		 * 
		 * @param r - double - ration
		 */
		void setRatio(double r);

		/**
		 * @brief 
		 *
		 * @param c - contours.
		 */
		vector<Point> extractPoints(vector<vector<Point> >& c);
};

#endif /* !EXTRACTOR_H */

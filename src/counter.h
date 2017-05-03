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
 * @file counter.h
 * @brief Takes responsibility to count the passing blobs.
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-03
 */

#ifndef COUNTER_H
#define COUNTER_H

#include <list>
#include "blob.h"

using namespace std;

class Counter {
	private:
		/**
		 * @brief Get the x-coordinate of a pointif the passenger
		 * flow is horizontal (hor = true).
		 * And y-coordinate for another case.
		 *
		 * @param point
		 *
		 * @return 
		 */
		int getPathValue(const Point& point);

		/**
		 * @brief Takes one expired blob and decides whether it crossed
		 * the upper or lower line.
		 *
		 * @param blob
		 */
		void processBlob(Blob& blob);
	public:
		/**
		 * @brief The number of estimated people passing the lower
		 * line.
		 */
		int lowerCount;

		/**
		 * @brief The number of estimated people passing the uppper
		 * line.
		 */
		int upperCount;

		/**
		 * @brief Flag to check if the passenger flow is horizontal.
		 * hor = false in the case of vertical flow.
		 */
		bool hor;

		/**
		 * @brief The value of lower boundary.
		 */
		int bound1;

		/**
		 * @brief The value of upper boundary.
		 */
		int bound2;

		/**
		 * @brief Constructor that takes parameters.
		 *
		 * @param _hor
		 * @param _bound1
		 * @param _bound2
		 */
		Counter(bool _hor, int _bound1, int _bound2);

		/**
		 * @brief Main functionality. Receives a list of expired
		 * blobs, and calls processBlob for each of the blob.
		 *
		 * @param blobs
		 */
		void receive(list<Blob>& blobs);
};

#endif /* !COUNTER_H */

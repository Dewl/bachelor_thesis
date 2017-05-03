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
 * @file stat.h
 * @brief Retrieves data from blobs for the learning purposes.
 *        This is still a prototype.
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-03
 */


#ifndef STAT_H
#define STAT_H

#include "blob.h"

using namespace std;

class Stat {
	private:
		/**
		 * @brief Retrieves area data of the current expired blob.
		 *
		 * @param blob
		 */
		void processArea(Blob& blob);

		/**
		 * @brief Retrieves various data types of the
		 * current expired blob.
		 *
		 * @param blob
		 */
		void processBlob(Blob& blob);
	public:
		/**
		 * @brief Main functionality. Receives a list of expired blob
		 * and retreives data from each of it.
		 *
		 * @param blobs
		 */
		void receive(list<Blob> blobs);
};

#endif /* !STAT_H */

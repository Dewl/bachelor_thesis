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
 * @file stat.cpp
 * @brief The prototype implementation of Stat Module
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-05-03
 */

#include <fstream>
#include "stat.h"

using namespace std;

void Stat::processBlob(Blob& blob)
{
	processArea(blob);
}

void Stat::receive(list<Blob> blobs)
{
	for (lBi it = blobs.begin(); it != blobs.end(); ++it) {
		processBlob(*it);
	}
}

void Stat::processArea(Blob& blob)
{
	ofstream out;
	out.open("area.log", ios_base::app);
	for (list<int>::iterator it = blob.area.begin();
			it != blob.area.end(); ++it) {
		out << *it << endl;
	}
}

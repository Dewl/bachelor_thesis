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
 * @file blob.cpp
 * @brief Impementation for Blob model
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-31
 */

#include "blob.h"
#include "matutil.h"

Blob::Blob()
{
	init();
	activated = false;
}

void Blob::init()
{
	flag = STATE_NULL;
	activated = true;
}

void Blob::update(const vector<Point>& _contour)
{
	contour = _contour;
	ttl = 0;
}

double Blob::distance(const vector<Point>& b)
{
	Point p1 = contourCenter(contour);
	Point p2 = contourCenter(b);

	return norm(Mat(p1) - Mat(p2));
}

void Blob::increaseDeadTime()
{
	ttl += 1;
}

bool Blob::isActivated()
{
	return activated;
}

Point Blob::getCenter()
{
	return contourCenter(contour);
}

int Blob::getFlag()
{
	return flag;
}

void Blob::setFlagToUpper()
{
	flag = STATE_UPPER_PART;
}

void Blob::setFlagToMiddle()
{
	flag = STATE_MIDDLE_PART;
}

void Blob::setFlagToLower()
{
	flag = STATE_LOWER_PART;
}

void Blob::deactivate()
{
	activated = false;
}

int Blob::getTtl()
{
	return ttl;
}

bool Blob::isUpper()
{
	return flag == STATE_UPPER_PART;
}

bool Blob::isMiddle()
{
	return flag == STATE_MIDDLE_PART;
}

bool Blob::isLower()
{
	return flag == STATE_LOWER_PART;
}

vector<Point> Blob::getContour() const
{
	return contour;
}

int Blob::getY()
{
	//return getCenter().y;
	return 0;
}

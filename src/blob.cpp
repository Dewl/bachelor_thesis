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

Blob::Blob()
{
	set(Point(0, 0));
	activated = false;
}

void Blob::set(const Point& point)
{
	update(point);
	flag = STATE_NULL;
	activated = true;
	ttl = 0;
}

void Blob::update(const Point& point)
{
	pos.x = point.x;
	pos.y = point.y;
	ttl = 0;
}

double Blob::distance(const Point& b)
{
	return norm(Mat(pos) - Mat(b));
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
	return pos;
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

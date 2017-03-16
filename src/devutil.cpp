/* Copyright (C) 
 * 2017 - Khoi Hoang, Kiet Chuong
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
 * @file devutil.h
 * @brief Utilities for project development
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-16
 */

#include <iostream>
#include <cstdlib>

#include "devutil.h"

using namespace std;

void debug(string msg)
{
	cerr << msg << endl;
}

void debug(string msg, int val)
{
	cerr << msg << ": " << val << endl;
}

void debug(string msg, double val)
{
	cerr << msg << ": " << val << endl;
}

void debug(string msg, string val)
{
	cerr << msg << ": " << val << endl;
}

void error(string msg)
{
	cerr << msg << endl;
	exit(EXIT_FAILURE);
}

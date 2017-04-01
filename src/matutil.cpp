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
 * @file matutil.cpp
 * @brief The implementation for matutil.h
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-16
 */

#include <iostream>
#include <string>

#include "matutil.h"
#include "const.h"

using namespace std;

void refineBlob(Mat& binImage)
{
	medianBlur(binImage, binImage, 5);
	erode(binImage, binImage, Mat(), Point(-1, -1), 2);
	dilate(binImage, binImage, Mat(), Point(-1, -1), 2);
}

Point rectCenter(const Rect& rect)
{
	Point ret = Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
	return ret;
}

void drawRect(Mat& canvas, const Rect& rect, int id)
{
	ostringstream convert;
	convert << id;
	rectangle(canvas, rect.tl(), rect.br(), COLOR_GREEN, 2);
	drawTextRect(canvas, convert.str(), rect);
}

void drawPoint(Mat& canvas, const Point& point)
{
	circle(canvas, point, 0,CV_RGB(255,0,0),3);
}

void drawTextRect(Mat& canvas, const string& str, const Rect& rect)
{
	putText(canvas, str, Point(0, 0),
			FONT_HERSHEY_COMPLEX_SMALL, 0.8,
			COLOR_RED, 1, CV_AA);
}

void drawText(Mat& canvas, const string& str, const Point& point)
{
	putText(canvas, str, point,
			FONT_HERSHEY_COMPLEX_SMALL, 1.0,
			COLOR_YELLOW, 1, CV_AA);
}

void drawBoundary(Mat& canvas, int y_up, int y_down)
{
	int x1 = 0;
	int x2 = canvas.cols;

	Point up1 = Point(x1, y_up);
	Point up2 = Point(x2, y_up);

	Point down1 = Point(x1, y_down);
	Point down2 = Point(x2, y_down);

	line(canvas, up1, up2, COLOR_GREEN);
	line(canvas, down1, down2, COLOR_GREEN);
}

void drawInfo(Mat& canvas, int upper, int lower)
{
	ostringstream convert;
	convert << "UPPER= " << upper << ", LOWER= " << lower;
	putText(canvas, convert.str(), Point(20, 20),
			FONT_HERSHEY_COMPLEX_SMALL, 0.8,
			COLOR_YELLOW, 1, CV_AA);
}


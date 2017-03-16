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
 * @file matutil.h
 * @brief Utilities for processing images
 * @author Khoi Hoang
 * @version 1.0
 * @date 2017-03-16
 */

#ifndef MATUTIL_H
#define MATUTIL_H

#include <opencv/cv.h>

using namespace cv;

/**
 * @brief Significantly reduces the noise and correctly splits the blobs
 *
 * @param binImage
 */
void refineBlob(Mat& binImage);


/**
 * @brief Draws a rectangle
 *
 * @param canvas
 * @param rect
 */
void drawRect(Mat& canvas, const Rect& rect);


/**
 * @brief  Returns the center point of a rectangle
 *
 * @param rect
 *
 * @return Point
 */
Point rectCenter(const Rect& rect);

#endif /* !MATUTIL_H */

/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#ifndef IMAGETEST_COLORSPACES_H
#define IMAGETEST_COLORSPACES_H

#include <colorspaces/image.h>
#include <colorspaces/color.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Create a rgb888 test image with a horizontal line
 * \param lineWidth
 * \param startRow
 * \param bg color used in the background. Default black
 * \param bgColor color used in the line. Default white
 */
Image* Image_createTestHline(const int width,
			     const int height,
			     const int lineWidth,
			     const int startRow,
			     const RGBColor *bgColor,
			     const RGBColor *fgColor);

/**
 * Create a rgb888 test image with a vertical line
 * \param lineWidth
 * \param startCol
 * \param bg color used in the background. Default black
 * \param fgColor color used in the line. Default white
 */
Image* Image_createTestVline(const int width,
			     const int height,
			     const int lineWidth,
			     const int startCol,
			     const RGBColor *bgColor,
			     const RGBColor *fgColor);

/**
 * Create a rgb888 test image with a square
 * \param sideLenght
 * \param xStartCorner x coordinate of left-top corner
 * \param yStartCorner y coordinate of left-top corner
 * \param bg color used in the background. Default black
 * \param fgColor color used in the line. Default white
 */
Image* Image_createTestSquare(const int width,
			      const int height,
			      const int sideLength,
			      const int xStartCorner,
			      const int yStartCorner,
			      const RGBColor *bgColor,
			      const RGBColor *fgColor);
  

#ifdef __cplusplus
}//extern
#endif

#endif //IMAGETEST_COLORSPACES_H

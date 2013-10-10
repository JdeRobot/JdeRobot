/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#ifndef POINT_H
#define POINT_H

#include <string>
#include <iostream>
#include <math.h>

#include "common.h"

// Definition of this class
class Point {
public:
	// Constructor
	Point ( float x, float y );

	// Destructor
	~Point ();

	// Setters
	void setX ( float x );
	void setY ( float y );

	// Getters
	float getX ();
	float getY ();

	// Another functions
	void move ( float dx, float dy );

	Point calculateGoodArrowPosition ( Point p );
	Point midpoint ( Point p );

	Point copy ();
	Point* copyAsPointer ();

private:
	float x, y;
};

#endif // POINT_H
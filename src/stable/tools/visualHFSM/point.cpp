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
 *  Authors : Borja Menéndez Moreno <b.menendez.moreno@gmail.com>
 *            José María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "point.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
Point::Point ( float x, float y ) {
	this->x = x;
	this->y = y;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
Point::~Point () {}

/*************************************************************
 * SETTERS
 *************************************************************/
void Point::setX ( float x ) {
	this->x = x;
}

void Point::setY ( float y ) {
	this->y = y;
}

/*************************************************************
 * GETTERS
 *************************************************************/
float Point::getX () {
	return this->x;
}

float Point::getY () {
	return this->y;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void Point::move ( float dx, float dy ) {
	this->x += dx;
	this->y += dy;
}

bool Point::equals ( Point p ) {
	return ((this->x == p.getX()) && (this->y == p.getY()));
}

Point Point::calculateGoodArrowPosition ( Point p ) {
	float distance_op = this->y - p.getY();
	float distance_ad = this->x - p.getX();

	float alpha = atan(distance_op / distance_ad);

	float possible1_final_x = this->x + RADIUS_NORMAL * cos(alpha);
    float possible1_final_y = this->y + RADIUS_NORMAL * sin(alpha);

    float distance1 = sqrt(pow(possible1_final_x - p.getX(), 2.0) +
    						pow(possible1_final_y - p.getY(), 2.0));

    float possible2_final_x = this->x - RADIUS_NORMAL * cos(alpha);
    float possible2_final_y = this->y - RADIUS_NORMAL * sin(alpha);

    float distance2 = sqrt(pow(possible2_final_x - p.getX(), 2.0) +
    						pow(possible2_final_y - p.getY(), 2.0));

    if (distance1 < distance2) {
    	Point point(possible1_final_x, possible1_final_y);
    	return point;
    } else {
    	Point point(possible2_final_x, possible2_final_y);
    	return point;
    }
}

Point Point::midpoint ( Point p ) {
	Point point((this->x + p.getX()) / 2.0, (this->y + p.getY()) / 2.0);
	return point;
}

Point Point::copy () {
	Point p(this->x, this->y);
	return p;
}

Point* Point::copyAsPointer () {
	return (new Point(this->x, this->y));
}
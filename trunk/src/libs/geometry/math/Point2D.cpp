/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  Authors : Alejandro Hernández <ahcorde [at] gmail [dot] com>
 *            Roberto Calvo <rocapal [at] gsyc [dot] urjc [dot] es>
 *            Eduardo Perdices <eperdices [at] gsyc [dot] es>
 *
 */

#include "Point2D.h"

Point2D::Point2D() {
  this->point.setZero();
}

Point2D::Point2D(double x, double y, double h) {
  this->point << x, y, h;
}

Point2D::Point2D(Eigen::Vector2d p, double h) {
  this->point << p(0), p(1), h;
}

Point2D::Point2D(Eigen::Vector3d p) {
  this->point = p;
}

Eigen::Vector3d
Point2D::getPoint() {
  return this->point;
}

double
Point2D::distanceTo(Point2D p) {
  return sqrt(G_SQUARE(this->point(0)-p.point(0)) + G_SQUARE(this->point(1)-p.point(1)));
}




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
 *  Authors : Eduardo Perdices <eperdices [at] gsyc [dot] es>
 *
 */

#include "Point3D.h"
#include "Line3D.h"
#include "Plane3D.h"

Point3D::Point3D() {
  this->point.setZero();
}

Point3D::Point3D(double x, double y, double z, double h) {
  this->point << x, y, z, h;
}

Point3D::Point3D(Eigen::Vector3d &p, double h) {
  this->point << p(0), p(1), p(2), h;
}

Point3D::Point3D(Eigen::Vector4d &p) {
  this->point = p;
}

void
Point3D::set(double x, double y, double z, double h) {
  this->point << x, y, z, h;
}

void
Point3D::set(Eigen::Vector3d &p, double h) {
  this->point << p(0), p(1), p(2), h;
}

void
Point3D::set(Eigen::Vector4d &p) {
  this->point = p;
}

Eigen::Vector4d&
Point3D::getPoint() {
  return this->point;
}

bool
Point3D::isInfinite() {
  return this->point(3) == 0.0;
}

bool
Point3D::normalize() {
  if(this->point(3) == 0.0)
    return false;

  this->point = this->point/this->point(3);
  return true;
}

double
Point3D::distanceTo(Point3D &p) {
  return sqrt(GEOMETRY_SQUARE(this->point(0)-p.point(0)) + GEOMETRY_SQUARE(this->point(1)-p.point(1)) + + GEOMETRY_SQUARE(this->point(2)-p.point(2)));
}

bool
Point3D::belongsToLine(Line3D &l) {
  return l.hasPoint(*this);
}

bool
Point3D::belongsToPlane(Plane3D &p) {
  return p.hasPoint(*this);
}

Point3D&
Point3D::operator =(const Point3D &pt) {
  this->point = pt.point;

  return *this;
}

std::ostream&
operator <<(std::ostream &o,const Point3D &p) {
  o << "(" << p.point(0) << "," << p.point(1) << "," << p.point(2) << "," << p.point(3) << ")";
  return o;
}

std::istream&
operator >>(std::istream &in, Point3D &p) {
  // Skip white spaces
  in.setf(std::ios_base::skipws);

  double x, y, z;

  in >> x >> y >> z;
  p.point(0) = x;
  p.point(1) = y;
  p.point(2) = z;
  return in;
}


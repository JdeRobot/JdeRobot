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

#include "Plane3D.h"
#include "Point3D.h"
#include "Line3D.h"

Plane3D::Plane3D() {
  this->plane.setZero();
}

Plane3D::Plane3D(Point3D &p1, Point3D &p2, Point3D &p3) {
  this->plane = this->getPlane(p1, p2, p3);
}


Plane3D::Plane3D(Line3D &l, Point3D &p) {
  this->plane = l.toPlane(p).plane;
}

Plane3D::Plane3D(Eigen::Vector4d p) {
  this->plane = p;
}

Eigen::Vector4d&
Plane3D::getPlane() {
  return this->plane;
}

Eigen::Vector4d
Plane3D::getPlane(Point3D &p1, Point3D &p2, Point3D &p3) {
  Eigen::Vector4d p;

  p(0) = p1.getPoint()(1)*(p2.getPoint()(2) - p3.getPoint()(2)) + p2.getPoint()(1)*(p3.getPoint()(2) - p1.getPoint()(2)) + p3.getPoint()(1)*(p1.getPoint()(2) - p2.getPoint()(2));

  p(1) = p1.getPoint()(2)*(p2.getPoint()(0) - p3.getPoint()(0)) + p2.getPoint()(2)*(p3.getPoint()(0) - p1.getPoint()(0)) + p3.getPoint()(2)*(p1.getPoint()(0) - p2.getPoint()(0));

  p(2) = p1.getPoint()(0)*(p2.getPoint()(1) - p3.getPoint()(1)) + p2.getPoint()(0)*(p3.getPoint()(1) - p1.getPoint()(1)) + p3.getPoint()(0)*(p1.getPoint()(1) - p2.getPoint()(1));

  p(3) = -(p1.getPoint()(0)*(p2.getPoint()(1)*p3.getPoint()(2) - p3.getPoint()(1)*p2.getPoint()(2)) + p2.getPoint()(0)*(p3.getPoint()(1)*p1.getPoint()(2) - p1.getPoint()(1)*p3.getPoint()(2)) + p3.getPoint()(0)*(p1.getPoint()(1)*p2.getPoint()(2) - p2.getPoint()(1)*p1.getPoint()(2)));

  return p;
}

Line3D
Plane3D::intersectPlane(Plane3D &p) {
  return Line3D(*this, p);
}

bool
Plane3D::hasPoint(Point3D &p) {
  return this->plane(0) * p.getPoint()(0) + this->plane(1) * p.getPoint()(1) + this->plane(2) * p.getPoint()(2) + this->plane(3) == 0;
}

Plane3D &
Plane3D::operator =(const Plane3D &p) {
  this->plane = p.plane;

  return *this;
}

std::ostream&
operator <<(std::ostream &o,const Plane3D &p) {
  o << p.plane;
  return o;
}


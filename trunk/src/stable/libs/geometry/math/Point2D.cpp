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

#include "Point2D.h"
#include "Segment2D.h"
#include "Line2D.h"

Point2D::Point2D() {
  this->point.setZero();
}

Point2D::Point2D(double x, double y, double h) {
  this->point << x, y, h;
}

Point2D::Point2D(Eigen::Vector2d &p, double h) {
  this->point << p(0), p(1), h;
}

Point2D::Point2D(Eigen::Vector3d &p) {
  this->point = p;
}

Eigen::Vector3d&
Point2D::getPoint() {
  return this->point;
}

bool
Point2D::isInfinite() {
  return this->point(2) == 0.0;
}

bool
Point2D::normalize() {
  if(this->point(2) == 0.0)
    return false;

  this->point = this->point/this->point(2);
  return true;
}

double
Point2D::distanceTo(Point2D &p) {
  return sqrt(G_SQUARE(this->point(0)-p.point(0)) + G_SQUARE(this->point(1)-p.point(1)));
}

double
Point2D::distanceTo(Line2D &l) {
  return l.distanceTo(*this);
}

double
Point2D::distanceTo(Segment2D &s) {
  return s.distanceTo(*this);
}

bool
Point2D::isInsideSegment(Segment2D &s) {
  double tmp;

  if(s.isPoint())
    return false;

  tmp = this->getPositionInSegment(s);

  if(tmp >=0 && tmp <=1)
    return true;

  return false;
}

double
Point2D::getPositionInSegment(Segment2D &s) {
  double tmp;
  Point2D ps, pe;

  /*  (Px-Ax)(Bx-Ax) + (Py-Ay)(By-Ay)
  u = -------------------------------
      (Bx-Ax)^2 + (By-Ay)^2*/
  
  /*Check if the line is a point*/
  if(s.isPoint())
    return 0;

  ps = s.getPointStart();
  pe = s.getPointEnd();

  tmp = (this->point(0)-ps.point(0))*(pe.point(0)-ps.point(0)) + (this->point(1)-ps.point(1))*(pe.point(1)-ps.point(1));
  tmp = tmp /(G_SQUARE(pe.point(0)-ps.point(0)) + G_SQUARE(pe.point(1)-ps.point(1)));

  return tmp;
}

bool
Point2D::belongsToLine(Line2D &l) {
  return l.hasPoint(*this);
}

bool
Point2D::belongsToSegment(Segment2D &s) {
  return s.hasPoint(*this);
}

Point2D&
Point2D::operator =(const Point2D &pt) {
  this->point = pt.point;

  return *this;
}

std::ostream&
operator <<(std::ostream &o,const Point2D &p) {
  o << "(" << p.point(0) << "," << p.point(1) << "," << p.point(2) << ")";
  return o;
}

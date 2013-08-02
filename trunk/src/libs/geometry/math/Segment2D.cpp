/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published this->pend->getPoint()(1)
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

#include "Segment2D.h"
#include "Point2D.h"

Segment2D::Segment2D() {
}

Segment2D::Segment2D(Point2D &p1, Point2D &p2) {
  this->pstart = new Point2D(p1.getPoint());
  this->pend = new Point2D(p2.getPoint());
}

Point2D&
Segment2D::getPointStart() {
  return *(this->pstart);
}

Point2D&
Segment2D::getPointEnd() {
  return *(this->pend);
}

double
Segment2D::getLength() {
  return this->pstart->distanceTo(*(this->pend));
}

bool
Segment2D::isPoint() {
  return this->pstart->getPoint() == this->pend->getPoint();
}

Line2D
Segment2D::toLine() {
  return Line2D(*(this->pstart),*(this->pend));
}

Point2D
Segment2D::getPointInPosition(double u) {
  double px, py;

	/*Get P from the equation P = A+u(B-A)*/
	px = this->pstart->getPoint()(0) + u*(this->pend->getPoint()(0)-this->pstart->getPoint()(0));
	py = this->pstart->getPoint()(1) + u*(this->pend->getPoint()(1)-this->pstart->getPoint()(1));  

  return Point2D(px, py, 1.0);
}

bool
Segment2D::hasPoint(Point2D &p) {
  return this->toLine().hasPoint(p) && p.isInsideSegment(*this);
}

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

double
Segment2D::distanceTo(Point2D &p) {
  double pos;

  /*Position in segment*/
  pos = p.getPositionInSegment(*this);

  if(pos >= 0 && pos <= 1)
    return this->toLine().distanceTo(p);

  if(pos < 0)
    return pstart->distanceTo(p);
  else
    return pend->distanceTo(p);    
}

double
Segment2D::getAngle() {
	double alpha;
	double diffx, diffy;

	diffx = this->pend->getPoint()(0) - this->pstart->getPoint()(0);
	diffy = this->pend->getPoint()(1) - this->pstart->getPoint()(1);

	if(diffy == 0.0)
		return G_PI_2;

	alpha = atan(-diffx/diffy);

	/*Normalize*/
	if(alpha < 0)
		alpha += G_PI;
	if(alpha > G_PI)
		alpha -= G_PI;	

	return alpha;
}

double
Segment2D::getGradient() {
	double diffx, diffy;

	diffx = this->pend->getPoint()(0) - this->pstart->getPoint()(0);
	diffy = this->pend->getPoint()(1) - this->pstart->getPoint()(1);

	if(diffy == 0.0)
		return G_INFINITE;

	return -diffx/diffy;
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

Point2D
Segment2D::intersectSegment(Segment2D &s) {
  Point2D p2d;
  Line2D l1, l2;

  /*Calc line intersection*/
  l1 = this->toLine();
  l2 = s.toLine();
  p2d = l1.intersectLine(l2);

  /*Check if p2d is valid*/
	if(p2d.isInfinite())
		return Point2D(0.0,0.0,0.0);

	/*Check extremes*/
	if(p2d.isInsideSegment(*this) && p2d.isInsideSegment(s))
		return p2d;

	return Point2D(0.0,0.0,0.0);
}

bool
Segment2D::parallelTo(Segment2D &s, double threshold) {
	double diff;

	diff = this->getAngle() - s.getAngle();

	/*Normalize*/
	while(diff < -G_PI_2)
		diff += G_PI;
	while(diff > G_PI_2)
		diff -= G_PI;

	return fabs(diff) < threshold;
}

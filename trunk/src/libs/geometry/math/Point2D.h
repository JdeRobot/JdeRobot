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
 *
 *
 *  Point2D: Represents a 2D Point in homogeneous coordinates
 */

#ifndef POINT2D_H
#define POINT2D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>
#include "geoconst.h"

class Segment2D;
class Line2D;

class Point2D {
public:
  Point2D();
  Point2D(double x, double y, double h=1.0);
  Point2D(Eigen::Vector2d &p, double h=1.0);
  Point2D(Eigen::Vector3d &p);

  Eigen::Vector3d& getPoint();

  /*Return true if the point is at the infinite*/
  bool isInfinite();

  /*Distance between 2D objects*/
  double distanceTo(Point2D &p);
  double distanceTo(Line2D &l);
  double distanceTo(Segment2D &s);

  /*Check if the point is inside a 2D segment*/
  bool isInsideSegment(Segment2D &s);

	/*Get the position of the point according to a segment: Solve u in P = A+u(B-A)*/
  double getPositionInSegment(Segment2D &s);

  /*Return true if the point belongs to a 2D line*/
  bool belongsToLine(Line2D &l);

  /*Return true if the point belongs to a 2D segment*/
  bool belongsToSegment(Segment2D &s);

  /*Operators*/
  Point2D &operator =(const Point2D &pt);
  
private:

  Eigen::Vector3d point;
    
};

#endif

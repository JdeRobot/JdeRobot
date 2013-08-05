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
 *  Segment2D: Represents a 2D Segment represented by 2 2D Points
 */

#ifndef SEGMENT2D_H
#define SEGMENT2D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>
#include "geoconst.h"
#include "Line2D.h"

class Point2D;

class Segment2D {
public:
  Segment2D();
  Segment2D(Point2D &p1, Point2D &p2);

  Point2D& getPointStart();
  Point2D& getPointEnd();

  /*Get segment length*/
  double getLength();

  /*Return true if the segment is a point*/
  bool isPoint();

  /*Convert 2D segment into a 2D line*/
  Line2D toLine();

  /*Distance between segment and point*/
  double distanceTo(Point2D &p);

  /*Calc positive angle*/
  double getAngle();
  double getGradient();

	/*Return a 2D Point belonging to the segment: Solve P in P = A+u(B-A)*/
  Point2D getPointInPosition(double u);

  /*Return true if the 2D segment has a concrete 2D Point*/
  bool hasPoint(Point2D &p);

  /*Intersect two segments into a 2D Point. If intersection is not valid Point2D is set at the infinite*/
  Point2D intersectSegment(Segment2D &s);

  /*Compare parallel segments with a threshold*/
  bool parallelTo(Segment2D &s, double threshold);
  
private:

  Point2D *pstart;
  Point2D *pend;
    
};

#endif

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
 *  Line2D: Represents a 2D line with the general equation of the line: Ax+By+C = 0
 */

#ifndef LINE2D_H
#define LINE2D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>
#include "geoconst.h"

class Point2D;

class Line2D {
public:
  Line2D();
  Line2D(double p1x, double p1y, double p2x, double p2y);
  Line2D(Eigen::Vector2d &p1, Eigen::Vector2d &p2);
  Line2D(Point2D &p1, Point2D &p2);
  Line2D(double va, double vb, double vc);
  Line2D(Eigen::Vector3d &v);

  Eigen::Vector3d& getVector();

  /*Calculate line from 2 2D points*/
  Eigen::Vector3d getLine(double p1x, double p1y, double p2x, double p2y);
  Eigen::Vector3d getLine(Eigen::Vector2d &p1, Eigen::Vector2d &p2);
  Eigen::Vector3d getLine(Point2D &p1, Point2D &p2);

  /*Calculate a 2D normal line from current 2D line and a 2D point*/
  Line2D getNormalLine(double px, double py);
  Line2D getNormalLine(Point2D &p);

  /*Distance between line and point*/
  double distanceTo(Point2D &p);

  /*Distance between line and the coordinates origin (0,0)*/
  double distanceToOrigin();

  /*Calc positive angle*/
  double getAngle();
  double getGradient();

  /*Intersect two lines into a 2D Point. If intersection is not valid Point2D is set at the infinite*/
  Point2D intersectLine(Line2D &l);

  /*Return true if the 2D line has a concrete 2D Point*/
  bool hasPoint(Point2D &p);

  /*Compare parallel lines with a threshold*/
  bool parallelTo(Line2D &l, double threshold);
  
private:

  Eigen::Vector3d v;
    
};

#endif

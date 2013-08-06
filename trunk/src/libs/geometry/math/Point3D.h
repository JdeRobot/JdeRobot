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
 *
 *
 *  Point3D: Represents a 3D Point in homogeneous coordinates
 */

#ifndef POINT3D_H
#define POINT3D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>
#include "geoconst.h"

class Line3D;
class Plane3D;

class Point3D {
public:
  Point3D();
  Point3D(double x, double y, double z, double h=1.0);
  Point3D(Eigen::Vector3d &p, double h=1.0);
  Point3D(Eigen::Vector4d &p);

  Eigen::Vector4d& getPoint();

  /*Return true if the point is at the infinite*/
  bool isInfinite();

  /*Normaliza point. Return true the point is not the infinite*/
  bool normalize();

  /*Distance between 3D points*/
  double distanceTo(Point3D &p);

  /*Return true if the point belongs to a 3D line*/
  bool belongsToLine(Line3D &l);

  /*Return true if the point belongs to a 3D plane*/
  bool belongsToPlane(Plane3D &p);

  /*Operators*/
  friend std::ostream& operator <<(std::ostream &o,const Point3D &p);
  
private:

  Eigen::Vector4d point;
    
};

#endif

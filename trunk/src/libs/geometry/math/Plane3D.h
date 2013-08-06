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
 *  Plane3D: Represents a 3D plane with the general equation of the plane: Ax+By+Cz+D = 0
 */

#ifndef PLANE3D_H
#define PLANE3D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>
#include "geoconst.h"

class Point3D;
class Line3D;

class Plane3D {
public:
  Plane3D();
  Plane3D(Point3D &p1, Point3D &p2, Point3D &p3);
  Plane3D(Line3D &l, Point3D &p);
  Plane3D(Eigen::Vector4d p);

  Eigen::Vector4d& getPlane();

  /*Calculate plane from 3 3D points*/
  Eigen::Vector4d getPlane(Point3D &p1, Point3D &p2, Point3D &p3);

  /*Intersect two planes into a 3D line*/
  Line3D intersectPlane(Plane3D &p);

  /*Return true if the 3D plane has a concrete 3D Point*/
  bool hasPoint(Point3D &p);

  /*Operators*/
  Plane3D &operator =(const Plane3D &p);
  friend std::ostream& operator <<(std::ostream &o,const Plane3D &p);
 
private:

  Eigen::Vector4d plane;
    
};

#endif

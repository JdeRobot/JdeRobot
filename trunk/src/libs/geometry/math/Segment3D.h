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
 *  Segment3D: Represents a 3D Segment represented by 2 3D Points
 */

#ifndef SEGMENT3D_H
#define SEGMENT3D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>
#include "geoconst.h"
#include "Line3D.h"

class Point3D;

class Segment3D {
public:
  Segment3D();
  Segment3D(Point3D &p1, Point3D &p2);

  Point3D& getPointStart();
  Point3D& getPointEnd();

  /*Get segment length*/
  double getLength();

  /*Return true if the segment is a point*/
  bool isPoint();

  /*Convert 3D segment into a 3D line*/
  Line3D toLine();

  /*Operators*/
  friend std::ostream& operator <<(std::ostream &o,const Segment3D &s);
  
private:

  Point3D *pstart;
  Point3D *pend;
    
};

#endif

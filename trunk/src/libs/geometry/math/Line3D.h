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
 *  Line3D: Represents a 3D line in Plücker coordinates
 */

#ifndef LINE3D_H
#define LINE3D_H
#define EIGEN_DONT_ALIGN_STATICALLY True

#include <math.h>
#include <eigen3/Eigen/Dense>
#include "geoconst.h"

class Point3D;
class Plane3D;

class Line3D {
public:
  Line3D();
  Line3D(Point3D &p1, Point3D &p2);
  Line3D(Plane3D &p1, Plane3D &p2);

  Eigen::VectorXd& getVector();

  /*Calculate line from 2 3D points or 2 3D planes*/
  Eigen::VectorXd getLine(Point3D &p1, Point3D &p2);
  Eigen::VectorXd getLine(Plane3D &p1, Plane3D &p2);

  /*Create a plane from a 3D point and current line*/
  Plane3D toPlane(Point3D &p);
 
private:

  void plucker_matrix2vector(Eigen::MatrixXd &m, Eigen::VectorXd &v);
  void plucker_vector2matrix(Eigen::MatrixXd &m, Eigen::VectorXd &v);
  void plucker_vector_swap(Eigen::VectorXd &v);

  Eigen::VectorXd v;
  Eigen::MatrixXd m;
    
};

#endif

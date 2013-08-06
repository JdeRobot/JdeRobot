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
 */

#include "Line3D.h"
#include "Point3D.h"
#include "Plane3D.h"

Line3D::Line3D() {
  this->v.resize(6);
  this->m.resize(4,4);
  this->v.setZero();
}

Line3D::Line3D(Point3D &p1, Point3D &p2) {
  this->v.resize(6);
  this->m.resize(4,4);
  this->v = this->getLine(p1, p2);
}

Line3D::Line3D(Plane3D &p1, Plane3D &p2) {
  this->v.resize(6);
  this->m.resize(4,4);
  this->v = this->getLine(p1, p2);
}

Eigen::VectorXd&
Line3D::getVector() {
  return this->v;
}

Eigen::VectorXd
Line3D::getLine(Point3D &p1, Point3D &p2) {
  Eigen::VectorXd v(6);

  this->m = p1.getPoint() * p2.getPoint().transpose() - p2.getPoint() * p1.getPoint().transpose();
  this->plucker_matrix2vector(this->m, v);
  return v;
}

Eigen::VectorXd
Line3D::getLine(Plane3D &p1, Plane3D &p2) {
  Eigen::VectorXd v(6);

  this->m = p1.getPlane() * p2.getPlane().transpose() - p2.getPlane() * p1.getPlane().transpose();
  this->plucker_matrix2vector(this->m, v);
  this->plucker_vector_swap(v);
  return v;
}

Plane3D
Line3D::toPlane(Point3D &p) {
  Eigen::Vector4d plane;

  this->plucker_vector2matrix(this->m, this->v);
  plane = this->m * p.getPoint();
  return Plane3D(plane);
}

Point3D
Line3D::intersectPlane(Plane3D &p) {
   Eigen::Vector4d point;

  this->plucker_vector2matrix(this->m, this->v);
  point = this->m * p.getPlane();  
  return Point3D(point); 
}

void
Line3D::plucker_matrix2vector(Eigen::MatrixXd &m, Eigen::VectorXd &v) {
  v << m(0,1), m(0,2), m(0,3), m(1,2), m(3,1), m(2,3);
}

void
Line3D::plucker_vector2matrix(Eigen::MatrixXd &m, Eigen::VectorXd &v) {
  Eigen::MatrixXd mtmp;

  m = Eigen::MatrixXd::Zero(4, 4);    
  m(0,1) = v(0);
  m(0,2) = v(1);
  m(0,3) = v(2);
  m(1,2) = v(3);
  m(3,1) = v(4);
  m(2,3) = v(5);
  mtmp = m.transpose();
  m = m-mtmp;
}

void
Line3D::plucker_vector_swap(Eigen::VectorXd &v) {
  Eigen::VectorXd vaux(6);

  vaux = v;
  v(0) = vaux(5);
  v(1) = vaux(4);
  v(2) = vaux(3);
  v(3) = vaux(2);
  v(4) = vaux(1);
  v(5) = vaux(0);
}

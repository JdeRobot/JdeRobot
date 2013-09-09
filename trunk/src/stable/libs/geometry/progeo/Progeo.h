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

#ifndef PROGEOMM_H
#define PROGEOMM_H

#define EIGEN_DONT_ALIGN_STATICALLY True

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*xml*/
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#define PI 3.141592654
#define BIGNUM 1.0e4

/* geometric distances */
#define DIST2D(p1,p2) sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))
#define DIST3D(p1,p2) sqrt((p1.X-p2.X)*(p1.X-p2.X)+(p1.Y-p2.Y)*(p1.Y-p2.Y)+(p1.Z-p2.Z)*(p1.Z-p2.Z))

namespace Progeo {

class Progeo
{

public:

    Progeo();
    Progeo(std::string filename);

    Progeo(Eigen::Vector4d posCamera,
           Eigen::Matrix3d KMatrix,
           Eigen::Matrix4d RTMatrix,
           int width, int height);

    void setPosition (Eigen::Vector4d pos);
    void setKMatrix (Eigen::Matrix3d KMatrix);
    void setRTMatrix (Eigen::Matrix4d RTMatrix);
    void setImageSize (int width, int height);
    
    void display_camerainfo();
    void backproject(Eigen::Vector3d point, Eigen::Vector4d& pro);
    int project(Eigen::Vector4d in, Eigen::Vector3d &out);
    void update_camera_matrix();

    void pixel2optical (Eigen::Vector3d &point);
    void optical2pixel (Eigen::Vector3d &point);

private:

    /* camera 3d position in mm */
    Eigen::Vector4d position;

    /* camera 3d focus of attention in mm */
    Eigen::Vector4d foa;

    /* top right and bottom left points */
    Eigen::Vector3d tr, bl;

    /* camera roll position angle in rads */
    float roll;

    /* focus x distance in mm*/
    float fdistx;
    float fdisty;

    /* pixels */
    float u0,v0;

    /*angle between the x and y pixel axes in rads*/
    float skew

    /* image height in pixels */;
    int rows;

    /* image width in pixels */
    int columns;

    /* camera K matrix */
    float k11, k12, k13, k14, k21, k22, k23, k24, k31, k32, k33, k34;
    Eigen::Matrix3d K;

    /* camera rotation + translation matrix */
    float rt11, rt12, rt13, rt14, rt21, rt22, rt23, rt24, rt31, rt32, rt33, rt34, rt41, rt42, rt43, rt44;
    Eigen::Matrix4d RT;

    /* distortion parameters */
    float d1,d2,d3,d4,d5,d6;
    float dx,dy;

    /* name */
    std::string name;
};
}

#endif

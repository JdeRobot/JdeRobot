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
 *
 */

#ifndef PROGEO2_H
#define PROGEO2_H

#define EIGEN_DONT_ALIGN_STATICALLY True

// standard C
#include <iostream>

/*xml*/
#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#include "../math/vector3H.h"
#include "../math/vector2H.h"
#include "../math/matriz3x3.h"
#include "../math/matriz4x4.h"

#include <stdio.h>

#include <stdlib.h>
#include <errno.h>

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

    Progeo(math::Vector3H posCamera,
           math::Matriz3x3 KMatrix,
           math::Matriz4x4 RTMatrix,
           int width, int height);

    void setPosition (math::Vector3H pos);
    void setKMatrix (math::Matriz3x3 KMatrix);
    void setRTMatrix (math::Matriz4x4 RTMatrix);
    void setImageSize (int width, int height);
    
    void display_camerainfo();
    void backproject(math::Vector2H, math::Vector3H& pro);
    int project(math::Vector3H in, math::Vector2H &out);
    void update_camera_matrix();



private:

    /* camera 3d position in mm */
    math::Vector3H position;

    /* camera 3d focus of attention in mm */
    math::Vector3H foa;

    /* top right and bottom left points */
    math::Vector3 tr, bl;

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
    math::Matriz3x3 K;

    /* camera rotation + translation matrix */
    float rt11, rt12, rt13, rt14, rt21, rt22, rt23, rt24, rt31, rt32, rt33, rt34, rt41, rt42, rt43, rt44;
    math::Matriz4x4 RT;

    /* distortion parameters */
    float d1,d2,d3,d4,d5,d6;
    float dx,dy;

    /* name */
    std::string name;
};
}

#endif

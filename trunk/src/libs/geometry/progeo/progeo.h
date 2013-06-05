#ifndef PROGEO_H
#define PROGEO_H

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

        public:Progeo();

        public: void xmlReader(std::string filename);

        public: void display_camerainfo();

        public: void backproject(math::Vector2H, math::Vector3H& pro);
        public: int project(math::Vector3H in, math::Vector2H &out);
        public: void update_camera_matrix();

        /* camera 3d position in mm */
        private:    math::Vector3H position;

        /* camera 3d focus of attention in mm */
        private:    math::Vector3H foa;

        /* top right and bottom left points */
        private:    math::Vector3 tr, bl;

        /* camera roll position angle in rads */
        private:    float roll;

        /* focus x distance in mm*/
        private:    float fdistx;
        private:    float fdisty;

        /* pixels */
        private:    float u0,v0;

        /*angle between the x and y pixel axes in rads*/
        private:    float skew

        /* image height in pixels */;
        private:    int rows;

        /* image width in pixels */
        private:    int columns;

        /* camera K matrix */
        private:    float k11, k12, k13, k14, k21, k22, k23, k24, k31, k32, k33, k34;
        private:    math::Matriz3x3 K;

        /* camera rotation + translation matrix */
        private:    float rt11, rt12, rt13, rt14, rt21, rt22, rt23, rt24, rt31, rt32, rt33, rt34, rt41, rt42, rt43, rt44;
        private:    math::Matriz4x4 RT;

          /* distortion parameters */
        private:     float d1,d2,d3,d4,d5,d6;
        private:     float dx,dy;

        /* name */
        std::string name;
    };
}

#endif 

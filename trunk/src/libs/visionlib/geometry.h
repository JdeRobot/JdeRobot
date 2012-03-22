/*
 *  Copyright (C) 2011 Julio Vega Pérez
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * 	Author : Julio Vega Pérez (julio [dot] vega [at] urjc [dot] es)
 *           Eduardo Perdices (eperdices [at] gsyc [dot] es)
 *
 *  This library was programed for RobotVision Project http://jderobot.org/index.php/robotvision
 *
 */

#ifndef VISIONLIBRARY_GEOMETRY_H
#define VISIONLIBRARY_GEOMETRY_H

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "structs.h"
/*GSL*/
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_eigen.h>

#ifndef G_SQUARE
#define G_SQUARE(a) ( (a) * (a) )
#endif

namespace visionLibrary {
  class geometry {

		public:
			geometry ();
		  virtual ~geometry ();

			/*Intersections*/
			static void pixel2Optical(TPinHoleCamera * camera, HPoint2D * p);
			static void optical2Pixel(TPinHoleCamera * camera, HPoint2D * p);
			static void get2DPosition(TPinHoleCamera * camera, HPoint3D in, HPoint2D &res);
			static void get3DPosition(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in);
			static void get3DPositionX(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float X = 0.0);
			static void get3DPositionY(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float Y = 0.0);
			static void get3DPositionZ(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float Z = 0.0);

			/*Distance between two points in 2D*/
			static double distanceBetweenPoints2D(int x1, int y1, int x2, int y2);
			static double distanceBetweenPoints2D(double x1, double y1, double x2, double y2);
			static double distanceBetweenPoints2D(HPoint2D p1, HPoint2D p2);

			/*Distance between two points in 3D*/
			static double distanceBetweenPoints3D(HPoint3D p1, HPoint3D p2);

			/*Distance between two points in 2D in a concrete axis*/
			static double calcDistanceAxis(double x1, double y1, double x2, double y2, double alpha);
			static double calcDistanceAxis(HPoint2D p1, HPoint2D p2, double alpha);
			static double calcDistanceAxis(HPoint2D p1, HPoint2D p2, double cosa, double sina);
			static double calcDistanceAxis(HPoint3D p1, HPoint3D p2, double alpha);
			static double calcDistanceAxis(HPoint3D p1, HPoint3D p2, double cosa, double sina);

			/*Calc the positive angle of a vector*/
			static double calcVectorAngle(double x1, double y1, double x2, double y2);
			static double calcVectorAngle(HPoint3D line);
			/*Calc gradient of a vector*/
			static double calcVectorGradient(double x1, double y1, double x2, double y2);			
			static double calcVectorGradient(HPoint3D line);	

			/*Calc a 2D vector from 2 2D points*/
			static void calcVector2D(HPoint2D p1, HPoint2D p2, HPoint3D &v);
			static void calcVector2D(HPoint3D p1, HPoint3D p2, HPoint3D &v);

			/*Calc a 2D normal vector from 3 2D points (normal vector of p1-p2 in p3)*/
			static void calcNormalVector2D(HPoint2D p1, HPoint2D p2, HPoint2D p3, HPoint3D &v);
			static void calcNormalVector2D(HPoint3D p1, HPoint3D p2, HPoint3D p3, HPoint3D &v);

			/*Calc intersection between 2 2D vectors in a 2D point*/
			static void calcIntersection2D(HPoint3D v1, HPoint3D v2, HPoint2D &p);
			static void calcIntersection2D(HPoint3D v1, HPoint3D v2, HPoint3D &p);

			/*Return true if the angles of two vectors can be considerated parallel*/
			static bool areVectorsParallel(double alpha1, double alpha2, double threshold);

			/*Return true if the projection of the point P is "inside" the segment A-B*/
			static bool isPointInsideLine(HPoint2D p, HPoint2D a, HPoint2D b);
			static bool isPointInsideLine(double px, double py, double ax, double ay, double bx, double by);

			/*Get the u parameter in the equation P = A+u(B-A)*/
			static double getPositionInLine(HPoint2D p, HPoint2D a, HPoint2D b);
			static double getPositionInLine(double px, double py, double ax, double ay, double bx, double by);

			/*Return a point belonging to the vector A-B*/
			static void getPointFromVector(int &px, int &py, int ax, int ay, int bx, int by, double u);
			static void getPointFromVector(double &px, double &py, double ax, double ay, double bx, double by, double u);

			/*Return the intersection between a circle (radius and P_central) and a vector*/
			static void calIntersectionCircleVector(HPoint3D v, HPoint3D p_c, double r, HPoint3D &int1, HPoint3D &int2);

			/*Return the intersection between two points (A and B) on 3D, and the ground*/
			static void lineGroundIntersection (HPoint3D A, HPoint3D B, HPoint3D &intersectionPoint);

			/*Return true if two segments P1-P2 and P3-P4 intersect each other*/
			static bool intersectSegments(double p1x, double p1y, double p2x, double p2y, double p3x, double p3y, double p4x, double p4y, HPoint2D& pres);

			/*Try to merge two segments, return true if merged and update segment1.
			Params: max parallel angle (rads), max horizontal distance (pixs), max perpendicular distance (pixs)*/
			static bool mergeSegments(Segment2D &segment1, Segment2D segment2, double max_parallel = 0.3, double max_distance = 10.0, double max_normal_distance = 10.0);

			/*Merge two segments to obtain the longest one*/
			static void getMaximizedSegment(HPoint3D seg1Start, HPoint3D seg1End, HPoint3D seg2Start, HPoint3D seg2End, HPoint3D &startPoint, HPoint3D &endPoint);

			/*Length of a segment*/
			static double segmentLength(Segment3D segment);

			/*Distance between a point and a segment. Check if the points is inside the line in "isInside" parameter*/
			static double distancePointLine(HPoint3D point, Segment3D segment, HPoint3D &intersection, int &isInside); /*Deprecated*/
			static double distancePointSegment(HPoint3D point, Segment3D segment, HPoint3D &intersection, int &isInside);

			/*Distance between a point and a line*/
			static double distancePointLine(HPoint2D point, HPoint3D line);

			/*Distance between the origin (0,0) and a line*/
			static double distanceOriginLine(HPoint3D line);			

			/*Check if two segments are equal*/
			static bool areTheSameSegment (Segment3D s1, Segment3D s2);

			/*Check if two parallelograms are equal*/
			static bool areTheSameParallelogram (Parallelogram3D par1, Parallelogram3D par2);

			/*Calculate centroid of a polygon*/
			static void getPolygonCentroid (Parallelogram3D &parallelogram);

			/*Print matrix*/
			static void printMatrix(gsl_matrix * m);

		private:
			static const double GEOMETRY_PI;
			static const double GEOMETRY_PI_2;
			static const double GEOMETRY_SQRT_2;
			static const double GEOMETRY_INFINITE;
	};
}

#endif

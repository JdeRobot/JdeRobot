 /*
 *  Copyright (C) 2010 Julio Vega Pérez
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

#include "geometry.h"

namespace visionLibrary {

	const double geometry::GEOMETRY_PI = 3.1415926535897932384626433832795;
	const double geometry::GEOMETRY_PI_2 = 1.570796327;
	const double geometry::GEOMETRY_SQRT_2 = 1.414213562;
	const double geometry::GEOMETRY_INFINITE = 123456789.123456789;

	geometry::geometry () {}

	geometry::~geometry () {}

	void geometry::pixel2Optical(TPinHoleCamera * camera, HPoint2D * p)
	{
		  float aux;
		  int height;

		  height = camera->rows;

		  aux = p->x;
		  p->x = height-1-p->y;
		  p->y = aux;
	}

	void geometry::optical2Pixel(TPinHoleCamera * camera, HPoint2D * p)
	{
		  float aux;
		  int height;

		  height = camera->rows;

		  aux = p->y;
		  p->y = height-1-p->x;
		  p->x = aux;
	}

	void geometry::get2DPosition(HPoint3D in, HPoint2D &res)
	{
		  this->get2DPosition(this->getCamera(), in, res);
	}

	void geometry::get2DPosition(TPinHoleCamera * camera, HPoint3D in, HPoint2D &res)
	{
		  HPoint2D p2d;

		  project(in, &p2d, *camera);
		  this->optical2Pixel(camera, &p2d);

		  res.x=p2d.x;
		  res.y=p2d.y;
		  res.h=p2d.h;
	}

	void geometry::get3DPosition(HPoint3D &res, HPoint2D in)
	{
		  this->get3DPositionZ(res, in, 0.0);
	}

	void geometry::get3DPosition(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in)
	{
		  this->get3DPositionZ(camera, res, in, 0.0);
	}

	void geometry::get3DPositionX(HPoint3D &res, HPoint2D in, float X = 0.0)
	{
		  this->get3DPositionX(this->getCamera(), res, in, X);
	}

	void geometry::get3DPositionX(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float X = 0.0)
	{
		  HPoint2D p2d;
		  HPoint3D p3d;
		  float x, y, z;
		  float xfinal, yfinal, zfinal;

		  x = camera->position.X;
		  y = camera->position.Y;
		  z = camera->position.Z;

		  p2d.x = in.x;
		  p2d.y = in.y;
		  p2d.h = in.h;

		  this->pixel2Optical(camera, &p2d);
		  backproject(&p3d, p2d, *camera);

		  /*Check division by zero*/
		  if((p3d.X-x) == 0.0) {
		      res.H = 0.0;
		      return;
		  }

		  xfinal = X;

		  /*Linear equation (X-x)/(p3d.X-x) = (Y-y)/(p3d.Y-y) = (Z-z)/(p3d.Z-z)*/
		  yfinal = y + (p3d.Y - y)*(xfinal - x)/(p3d.X-x);
		  zfinal = z + (p3d.Z - z)*(xfinal - x)/(p3d.X-x);

		  res.X = xfinal;
		  res.Y = yfinal;
		  res.Z = zfinal;
		  res.H = 1.0;
	}

	void geometry::get3DPositionY(HPoint3D &res, HPoint2D in, float Y = 0.0)
	{
		  this->get3DPositionY(this->getCamera(), res, in, Y);
	}

	void geometry::get3DPositionY(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float Y = 0.0)
	{
		  HPoint2D p2d;
		  HPoint3D p3d;
		  float x, y, z;
		  float xfinal, yfinal, zfinal;

		  x = camera->position.X;
		  y = camera->position.Y;
		  z = camera->position.Z;

		  p2d.x = in.x;
		  p2d.y = in.y;
		  p2d.h = in.h;

		  this->pixel2Optical(camera, &p2d);
		  backproject(&p3d, p2d, *camera);

		  /*Check division by zero*/
		  if((p3d.Y-y) == 0.0) {
		      res.H = 0.0;
		      return;
		  }

		  yfinal = Y;

		  /*Linear equation (X-x)/(p3d.X-x) = (Y-y)/(p3d.Y-y) = (Z-z)/(p3d.Z-z)*/
		  xfinal = x + (p3d.X - x)*(yfinal - y)/(p3d.Y-y);
		  zfinal = z + (p3d.Z - z)*(yfinal - y)/(p3d.Y-y);

		  res.X = xfinal;
		  res.Y = yfinal;
		  res.Z = zfinal;
		  res.H = 1.0;
	}

	void geometry::get3DPositionZ(HPoint3D &res, HPoint2D in, float Z = 0.0)
	{
		  this->get3DPositionZ(this->getCamera(), res, in, Z);
	}

	void geometry::get3DPositionZ(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float Z = 0.0)
	{
		  HPoint2D p2d;
		  HPoint3D p3d;
		  float x, y, z;
		  float xfinal, yfinal, zfinal;


		  x = camera->position.X;
		  y = camera->position.Y;
		  z = camera->position.Z;

		  //cerr<<"camera: "<<x<<", "<<y<<", "<<z<<endl;

		  p2d.x = in.x;
		  p2d.y = in.y;
		  p2d.h = in.h;
		  //cerr<<"p2d: "<<in.x<<", "<<in.y<<", "<<in.h<<endl;

		  this->pixel2Optical(camera, &p2d);
		  backproject(&p3d, p2d, *camera);
		  //cerr<<"p3d: "<<p3d.X<<", "<<p3d.Y<<", "<<p3d.Z<<endl;

		  /*Check division by zero*/
		  if((p3d.Z-z) == 0.0) {
		      res.H = 0.0;
		      return;
		  }

		  zfinal = Z;

		  /*Linear equation (X-x)/(p3d.X-x) = (Y-y)/(p3d.Y-y) = (Z-z)/(p3d.Z-z)*/
		  xfinal = x + (p3d.X - x)*(zfinal - z)/(p3d.Z-z);
		  yfinal = y + (p3d.Y - y)*(zfinal - z)/(p3d.Z-z);

		  res.X = xfinal;
		  res.Y = yfinal;
		  res.Z = zfinal;
		  res.H = 1.0;
	} 

	double geometry::distanceBetweenPoints2D(int x1, int y1, int x2, int y2)
	{
		return sqrt(G_SQUARE(x2-x1) + G_SQUARE(y2-y1));
	}

	double geometry::distanceBetweenPoints2D(double x1, double y1, double x2, double y2)
	{
		return sqrt(G_SQUARE(x2-x1) + G_SQUARE(y2-y1));
	}

	double geometry::distanceBetweenPoints2D(HPoint2D p1, HPoint2D p2)
	{
		return sqrt(G_SQUARE(p2.x-p1.x) + G_SQUARE(p2.y-p1.y));
	}

	double geometry::distanceBetweenPoints3D(HPoint3D p1, HPoint3D p2)
	{
		return sqrt(G_SQUARE(p2.X-p1.X) + G_SQUARE(p2.Y-p1.Y) + G_SQUARE(p2.Z-p1.Z));
	}

	double geometry::calcDistanceAxis(double x1, double y1, double x2, double y2, double alpha)
	{
		double dist;

		dist = (x2 - x1)*cos(alpha) + (y2 - y1)*sin(alpha);

		return fabs(dist);
	}

	double geometry::calcDistanceAxis(HPoint3D p1, HPoint3D p2, double alpha)
	{
		double dist;

		dist = (p2.X - p1.X)*cos(alpha) + (p2.Y - p1.Y)*sin(alpha);

		return fabs(dist);
	}

	double geometry::calcDistanceAxis(HPoint3D p1, HPoint3D p2, double cosa, double sina)
	{
		double dist;

		dist = (p2.X - p1.X)*cosa + (p2.Y - p1.Y)*sina;

		return fabs(dist);
	}

	double geometry::calcVectorAngle(double x1, double y1, double x2, double y2)
	{
		double diffx, diffy;
		double alpha;

		diffx = x2 - x1;
		diffy = y2 - y1;

		if(diffx == 0.0)
			return GEOMETRY_PI_2;

		alpha = atan(diffy/diffx);

		/*Normalize*/
		if(alpha < 0)
			alpha += GEOMETRY_PI;
		if(alpha > GEOMETRY_PI)
			alpha -= GEOMETRY_PI;	

		return alpha;		
	}

	double geometry::calcVectorAngle(HPoint3D line)
	{
		double alpha;

		if(line.Y == 0.0)
			return GEOMETRY_PI_2;

		alpha = atan(-line.X/line.Y);

		/*Normalize*/
		if(alpha < 0)
			alpha += GEOMETRY_PI;
		if(alpha > GEOMETRY_PI)
			alpha -= GEOMETRY_PI;	

		return alpha;		
	}

	double geometry::calcVectorGradient(double x1, double y1, double x2, double y2)
	{
		double diffx, diffy;

		diffx = x2 - x1;
		diffy = y2 - y1;

		if(diffx == 0.0)
			return GEOMETRY_INFINITE;

		return diffy/diffx;
	}

	double geometry::calcVectorGradient(HPoint3D line)
	{
		if(line.Y == 0.0)
			return GEOMETRY_INFINITE;

		return -line.X/line.Y;
	}

	void geometry::calcVector2D(HPoint2D p1, HPoint2D p2, HPoint3D &v)
	{
		/*Get the Ax + By + C = 0 parameters*/
		v.X = p1.y - p2.y; 				//y1*z2 - z1*y2
		v.Y = p2.x - p1.x; 				//z1*x2 - x1*z2
		v.Z = p1.x*p2.y - p1.y*p2.x; 	//x1*y2 - y1*x2
		v.H = 1.0;
	}

	void geometry::calcVector2D(HPoint3D p1, HPoint3D p2, HPoint3D &v)
	{
		/*Get the Ax + By + C = 0 parameters*/
		v.X = p1.Y - p2.Y; 				//y1*z2 - z1*y2
		v.Y = p2.X - p1.X; 				//z1*x2 - x1*z2
		v.Z = p1.X*p2.Y - p1.Y*p2.X; 	//x1*y2 - y1*x2
		v.H = 1.0;
	}

	void geometry::calcNormalVector2D(HPoint2D p1, HPoint2D p2, HPoint2D p3, HPoint3D &v)
	{
		HPoint3D vtmp;
		double nA, nB, nC;

		/*Get the vector p1-p2*/
		calcVector2D(p1, p2, vtmp);

		/*Calc the normal*/
		nA = vtmp.Y;
		nB = -vtmp.X;
		nC = -(p3.x*nA + p3.y*nB); //Solve equation Ax+By+C with central point

		v.X = nA;
		v.Y = nB;
		v.Z = nC;	
		v.H = 1.0;
	}

	void geometry::calcNormalVector2D(HPoint3D p1, HPoint3D p2, HPoint3D p3, HPoint3D &v)
	{
		HPoint3D vtmp;
		double nA, nB, nC;

		/*Get the vector p1-p2*/
		calcVector2D(p1, p2, vtmp);

		/*Calc the normal*/
		nA = vtmp.Y;
		nB = -vtmp.X;
		nC = -(p3.X*nA + p3.Y*nB); //Solve equation Ax+By+C with central point

		v.X = nA;
		v.Y = nB;
		v.Z = nC;	
		v.H = 1.0;
	}

	void geometry::calcIntersection2D(HPoint3D v1, HPoint3D v2, HPoint2D &p)
	{
		double h;

		h = v1.X*v2.Y - v1.Y*v2.X; 		/*x1*y2 - y1*x2*/

		/*Are parallel*/	
		if(h==0)
			p.h = 0.0;

		p.x = (v1.Y*v2.Z - v2.Y*v1.Z)/h; /*y1*z2 - z1*y2*/
		p.y = (v1.Z*v2.X - v1.X*v2.Z)/h; /*z1*x2 - x1*z2*/
		p.h = 1.0;
	}

	void geometry::calcIntersection2D(HPoint3D v1, HPoint3D v2, HPoint3D &p)
	{
		double h;

		h = v1.X*v2.Y - v1.Y*v2.X; 		/*x1*y2 - y1*x2*/

		/*Are parallel*/	
		if(h==0)
			p.H = 0.0;

		p.X = (v1.Y*v2.Z - v2.Y*v1.Z)/h; /*y1*z2 - z1*y2*/
		p.Y = (v1.Z*v2.X - v1.X*v2.Z)/h; /*z1*x2 - x1*z2*/
		p.Z = 0.0;
		p.H = 1.0;
	}

	bool geometry::areVectorsParallel(double alpha1, double alpha2, double threshold)
	{
		double diff;

		diff = alpha2 - alpha1;

		/*Normalize*/
		while(diff < -GEOMETRY_PI_2)
			diff += GEOMETRY_PI;
		while(diff > GEOMETRY_PI_2)
			diff -= GEOMETRY_PI;

		return fabs(diff) < threshold;
	}

	bool geometry::isPointInsideLine(HPoint2D p, HPoint2D a, HPoint2D b)
	{
		return isPointInsideLine(p.x, p.y, a.x, a.y, b.x, b.y);
	}

	bool geometry::isPointInsideLine(double px, double py, double ax, double ay, double bx, double by)
	{

		double tmp;

		/*Get the u parameter in the equation P = A+u(B-A)*/
		/*	(Px-Ax)(Bx-Ax) + (Py-Ay)(By-Ay)
		u = -------------------------------
				(Bx-Ax)^2 + (By-Ay)^2*/

		/*Check if the line is a point*/
		if(ax == bx && ay == by)
			return false;

		tmp = (px-ax)*(bx-ax) + (py-ay)*(by-ay);
		tmp = tmp /(pow(bx-ax,2) + pow(by-ay,2));

		if(tmp >=0 && tmp <=1)
			return true;

		return false;	
	}

	double geometry::getPositionInLine(HPoint2D p, HPoint2D a, HPoint2D b)
	{
		return getPositionInLine(p.x, p.y, a.x, a.y, b.x, b.y);
	}

	double geometry::getPositionInLine(double px, double py, double ax, double ay, double bx, double by)
	{
		/*	(Px-Ax)(Bx-Ax) + (Py-Ay)(By-Ay)
		u = -------------------------------
				(Bx-Ax)^2 + (By-Ay)^2*/

		double tmp;
	
		/*Check if the line is a point*/
		if(ax == bx && ay == by)
			return 0;

		tmp = (px-ax)*(bx-ax) + (py-ay)*(by-ay);
		tmp = tmp /(pow(bx-ax,2) + pow(by-ay,2));

		return tmp;
	}

	void geometry::getPointFromVector(int &px, int &py, int ax, int ay, int bx, int by, double u)
	{
		/*Get P from the equation P = A+u(B-A)*/
		px = (int)((double)ax + u*(double)(bx-ax));
		py = (int)((double)ay + u*(double)(by-ay));
	}

	void geometry::getPointFromVector(double &px, double &py, double ax, double ay, double bx, double by, double u)
	{
		/*Get P from the equation P = A+u(B-A)*/
		px = ax + u*(bx-ax);
		py = ay + u*(by-ay);
	}

	void geometry::calIntersectionCircleVector(HPoint3D v, HPoint3D p_c, double r, HPoint3D &int1, HPoint3D &int2)
	{
		/*Solve equations:
		(x-px)^2 + (y - py)^2 = r^2
		Ax + By + C = 0*/

		double i,j,k, A_2;
		double a,b,c;
		double tmp;

		if(v.X == 0.0) {
			/*Avoid div by 0*/
			v.X = 0.000001;
		}

		i = -2*p_c.X;
		j = -2*p_c.Y;
		k = G_SQUARE(p_c.X) + G_SQUARE(p_c.Y) - G_SQUARE(r);

		A_2 = G_SQUARE(v.X);
		a = G_SQUARE(-v.Y)/A_2 + 1;
		b = -2*v.Z*-v.Y/A_2  - v.Y*i/v.X + j;
		c = G_SQUARE(v.Z)/A_2 - v.Z*i/v.X + k;	

		/*Solve a*Y^2 + b+Y + c = 0*/
		tmp = G_SQUARE(b) - 4*a*c;
		if(tmp<0) {
			/*No intersection*/
			int1.H = 0.0;
			int2.H = 0.0;	
			return;	
		}

		tmp = sqrt(tmp);
		int1.Y = (-b + tmp)/(2*a);
		int2.Y = (-b - tmp)/(2*a);

		/*Get X Coordinate*/
		int1.X = (-v.Y*int1.Y - v.Z)/v.X;
		int2.X = (-v.Y*int2.Y - v.Z)/v.X;	

		int1.H = 1.0;
		int2.H = 1.0;
	}

	void geometry::lineGroundIntersection (HPoint3D A, HPoint3D B, HPoint3D &intersectionPoint) {
		HPoint3D v;	// Line director vector: it the same to take A or B as origin or destination extrem...
		double t;

		A.X = A.X;
		A.Y = A.Y;
		A.Z = A.Z;

		B.X = B.X;
		B.Y = B.Y;
		B.Z = B.Z;

		v.X = (B.X - A.X);
		v.Y = (B.Y - A.Y);
		v.Z = (B.Z - A.Z);

		// We'll calculate the ground intersection (Z = 0) on our robot system. Parametric equations:
		intersectionPoint.Z = 0.; // intersectionPoint.Z = A.Z + t*v.Z => t = (-A.Z / v.Z)
		t = (-A.Z) / (v.Z);

		intersectionPoint.X = A.X + (t*v.X);
		intersectionPoint.Y = A.Y + (t*v.Y);
		intersectionPoint.H = 1.;
	}

	bool geometry::mergeSegments(Segment2D &segment1, Segment2D segment2, double max_parallel, double max_distance, double max_normal_distance) {
		double dist1, dist2, dist3, dist4, l1dist, l2dist;
		double cent_x1, cent_y1, cent_x2, cent_y2;
		double disty1, disty2;
		double max_dist;
		bool mix1, mix2;
		double angle1, angle2;

		/*Get angles*/
		angle1 = calcVectorAngle(segment1.start.x, segment1.start.y, segment1.end.x, segment1.end.y);
		angle2 = calcVectorAngle(segment2.start.x, segment2.start.y, segment2.end.x, segment2.end.y);

		/*They have to be almost parallel*/
		if(!areVectorsParallel(angle1, angle2, max_parallel))
			return false;

		/*Check parallel distance*/
		cent_x1 = (segment1.start.x + segment1.end.x)/2.0;
		cent_y1 = (segment1.start.y + segment1.end.y)/2.0;
		cent_x2 = (segment2.start.x + segment2.end.x)/2.0;
		cent_y2 = (segment2.start.y + segment2.end.y)/2.0;

		disty1 = calcDistanceAxis(cent_x1, cent_y1, cent_x2, cent_y2, angle1+(GEOMETRY_PI_2));
		disty2 = calcDistanceAxis(cent_x1, cent_y1, cent_x2, cent_y2, angle2+(GEOMETRY_PI_2));
		if(disty1 > max_normal_distance && disty2 > max_normal_distance)
			return false;

		/*Distance between segments*/
		dist1 = distanceBetweenPoints2D(segment1.start, segment2.start);
		dist2 = distanceBetweenPoints2D(segment1.start, segment2.end);
		dist3 = distanceBetweenPoints2D(segment1.end, segment2.start);
		dist4 = distanceBetweenPoints2D(segment1.end, segment2.end);
		l1dist = distanceBetweenPoints2D(segment1.start, segment1.end);
		l2dist = distanceBetweenPoints2D(segment2.start, segment2.end);

		/*Line 2 in the middle of line 1*/
		if(dist1 <= l1dist && dist2 <= l1dist && dist3 <= l1dist && dist4 <= l1dist)
			return true;

		/*Line 1 in the middle of line 2*/
		if(dist1 <= l2dist && dist2 <= l2dist && dist3 <= l2dist && dist4 <= l2dist) {	
			segment1.start = segment2.start;
			segment1.end = segment2.end;
			return true;
		}

		/*Check if lines are not mixed*/
		mix1 = isPointInsideLine(segment2.start.x, segment2.start.y, segment1.start.x, segment1.start.y, segment1.end.x, segment1.end.y);
		mix2 = isPointInsideLine(segment2.end.x, segment2.end.y, segment1.start.x, segment1.start.y, segment1.end.x, segment1.end.y);

		if(!mix1 && !mix2) {
			/*if not mixed, check distance*/
			if(dist1 > max_distance && dist2 > max_distance && dist3 > max_distance && dist4 > max_distance)
				return false;
		}

		max_dist = dist1;	
		if(dist2 > max_dist) max_dist = dist2;
		if(dist3 > max_dist) max_dist = dist3;
		if(dist4 > max_dist) max_dist = dist4;
		
		/*Lines mixed, change one extreme*/
		if(max_dist == dist1)
			segment1.end = segment2.start;
		if(max_dist == dist2)
			segment1.end = segment2.end;
		if(max_dist == dist3)
			segment1.start = segment2.start;
		if(max_dist == dist4)
			segment1.start = segment2.end;

		return true;
	}

	void geometry::getMaximizedSegment (HPoint3D seg1Start, HPoint3D seg1End, HPoint3D seg2Start, HPoint3D seg2End, HPoint3D &startPoint, HPoint3D &endPoint) {

		HPoint3D myArray[4] = {seg1Start, seg1End, seg2Start, seg2End};
		double max = 0;
		int i, j;
		double value;

		for (i = 0; i < 4; i ++) {
			for (j = i+1; j < 4; j ++) {
				value = abs(distanceBetweenPoints3D (myArray[i], myArray[j]));
				if (value > max) {
					max = value;
					startPoint = myArray[i];
					endPoint = myArray[j];
				}
			}
		}
		// Finally, we'll get two points in order to maximize distance between them
	}

	double geometry::segmentLength(Segment3D segment) {
		return distanceBetweenPoints3D(segment.start, segment.end);
	}

	double geometry::distancePointLine(HPoint3D point, Segment3D segment, HPoint3D &intersection, int &isInside) {
		return distancePointSegment(point, segment, intersection, isInside);
	}

	double geometry::distancePointSegment(HPoint3D point, Segment3D segment, HPoint3D &intersection, int &isInside) {
		double LineMag;
		double U;

		LineMag = segmentLength(segment);

		U = ( ( ( point.X - segment.start.X ) * ( segment.end.X - segment.start.X ) ) +
				  ( ( point.Y - segment.start.Y ) * ( segment.end.Y - segment.start.Y ) ) +
				  ( ( point.Z - segment.start.Z ) * ( segment.end.Z - segment.start.Z ) ) ) /
					( LineMag * LineMag );

		intersection.X = segment.start.X + U * ( segment.end.X - segment.start.X );
		intersection.Y = segment.start.Y + U * ( segment.end.Y - segment.start.Y );
		intersection.Z = segment.start.Z + U * ( segment.end.Z - segment.start.Z );

		if( U >= 0.0f || U <= 1.0f ) {
			isInside = 0;
		} else {
			if (U < 0.) { // Intersection will be after segment
				isInside = -1;
			} else { // Intersection will be before segment
				isInside = +1;
			}
		}

		return distanceBetweenPoints3D(point, intersection);
	}

	double geometry::distancePointLine(HPoint2D point, HPoint3D line) {
		/*dist = |A*x + B*y + C|
						---------------
						sqrt(A^2 + B^2)*/

		if(line.X == 0.0 && line.Y == 0.0)
			return GEOMETRY_INFINITE;
		
		return abs(line.X*point.x + line.Y*point.y + line.Z)/(sqrt(G_SQUARE(line.X) + G_SQUARE(line.Y)));
	}

	double geometry::distanceOriginLine(HPoint3D line) {
		/*dist = 			|C|
						---------------
						sqrt(A^2 + B^2)*/
		if(line.X == 0.0 && line.Y == 0.0)
			return GEOMETRY_INFINITE;

		return abs(line.Z)/(sqrt(G_SQUARE(line.X) + G_SQUARE(line.Y)));
	}

	bool geometry::areTheSameSegment (Segment3D s1, Segment3D s2) {
		bool areTheSame = false;
		if ((((double)s1.start.X==(double)s2.start.X) && 
				 ((double)s1.start.Y==(double)s2.start.Y) && 
				 ((double)s1.start.Z==(double)s2.start.Z) && 
				 ((double)s1.end.X==(double)s2.end.X) &&
				 ((double)s1.end.Y==(double)s2.end.Y) && 
				 ((double)s1.end.Z==(double)s2.end.Z))	|| 
				(((double)s1.start.X==(double)s2.end.X) && 
				 ((double)s1.start.Y==(double)s2.end.Y) && 
				 ((double)s1.start.Z==(double)s2.end.Z) && 
				 ((double)s1.end.X==(double)s2.start.X) &&
				 ((double)s1.end.Y==(double)s2.start.Y) && 
				 ((double)s1.end.Z==(double)s2.start.Z))) { // they're the same segment
			areTheSame = true;
		}

		return areTheSame;
	}

	bool geometry::areTheSameParallelogram (Parallelogram3D par1, Parallelogram3D par2) {
		HPoint3D* myArray1[4] = {&(par1.p1), &(par1.p2), &(par1.p3), &(par1.p4)};
		HPoint3D* myArray2[4] = {&(par2.p1), &(par2.p2), &(par2.p3), &(par2.p4)};
		int i, j, found;
		double value;
		int equalPointsCounter = 0;

		for (i = 0; i < 4; i ++) {
			j = 0;
			found = false;
			while ((j < 4) && (!found)) {
				value = abs(distanceBetweenPoints3D (*myArray1[i], *myArray2[j]));
				if (value < 20.) {
					equalPointsCounter ++;
					found = true;
				}
				j ++;
			}
		}

		return (equalPointsCounter == 4);
	}

	void geometry::getPolygonCentroid (Parallelogram3D &parallelogram) {
		HPoint3D vertexList[5]; // Remember: Counter Clockwise Winding in order to get the centroid
		int i, j;
		double sum = 0.;
		double area = 0.;
		HPoint3D centroid;

		vertexList[0] = parallelogram.p2;
		vertexList[1] = parallelogram.p1;
		vertexList[2] = parallelogram.p3;
		vertexList[3] = parallelogram.p4;
		vertexList[4] = vertexList[0];

		centroid.X = 0.;
		centroid.Y = 0.;
		centroid.Z = 0.;
		centroid.H = 1.;

		for (i = 0; i < 5; i++) {
			j = (i+1)%5;
		  area = vertexList[i].X * vertexList[j].Y - vertexList[i].Y * vertexList[j].X;
		  sum += area;
		  centroid.X += (vertexList[i].X + vertexList[j].X) * area;
		  centroid.Y += (vertexList[i].Y + vertexList[j].Y) * area;
		}
		sum *= 3.;
		centroid.X /= sum;
		centroid.Y /= sum;

		parallelogram.centroid = centroid;
	}

	void geometry::printMatrix(gsl_matrix * m) {
		unsigned int i, j;

		std::cout << "-------------" << std::endl;

		for(i=0;i<m->size1;i++) {
			std::cout << "|\t";
			for(j=0;j<m->size2;j++) {
				std::cout << gsl_matrix_get(m,i,j) << "\t|\t";
			}
			std::cout << std::endl;
		} 
	}
}


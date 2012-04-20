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

#include "image.h"

namespace visionLibrary {

	const int image::IMAGE_WIDTH = 320;
	const int image::IMAGE_HEIGHT = 240;
	const double image::MIN_DISTANCE_CORNERS = 30.0;
	const double image::MIN_PERCENTAGE_VALID = 0.95;
	const double image::MIN_DISTANCE_UNIQUE_CORNERS = 15.0;

	cv::Mat *image::current_src = NULL;
	double image::sobel_threshold = 0.0;
	bool * image::calThreshold = new bool[IMAGE_WIDTH*IMAGE_HEIGHT];
	bool * image::thresholds = new bool[IMAGE_WIDTH*IMAGE_HEIGHT];
	bool * image::calPixel = new bool[IMAGE_WIDTH*IMAGE_HEIGHT];
	bool * image::pixels = new bool[IMAGE_WIDTH*IMAGE_HEIGHT];
	std::vector<HPoint2D> * image::unique_corners = new std::vector<HPoint2D>();

	image::image () {}

	image::~image () {}

	std::vector<HPoint2D> * image::getSegments(cv::Mat &src, std::vector<Segment2D> * segments, double threshold_fast, double threshold_sobel) {
		CvPoint * corners;
		Segment2D segment;
		HPoint2D new_corner;
		int * scores;
		int numCorners;

		image::current_src = &src;
		image::sobel_threshold = threshold_sobel;

		/*Reset parameters*/
		memset(image::calThreshold, 0, sizeof(bool)*IMAGE_WIDTH*IMAGE_HEIGHT);
		memset(image::thresholds, 0, sizeof(bool)*IMAGE_WIDTH*IMAGE_HEIGHT);
		memset(image::calPixel, 0, sizeof(bool)*IMAGE_WIDTH*IMAGE_HEIGHT);
		memset(image::pixels, 0, sizeof(bool)*IMAGE_WIDTH*IMAGE_HEIGHT);
		segments->clear();
		image::unique_corners->clear();

		/*Get corners fast*/
		cvCornerFast(src, threshold_fast, 9, false, &numCorners, &corners, &scores);

		/*Check unique corners*/
		for(int i=0;i<numCorners;i++) {
			/*Save unique corners*/
			if(is_unique_corner(corners[i].x, corners[i].y, scores[i])) {
				new_corner.x = (float) corners[i].x;
				new_corner.y = (float) corners[i].y;
				new_corner.h = (float) scores[i];				
				image::unique_corners->push_back(new_corner);
			}
		}

		/*Free memory*/
		if(corners != NULL)	free(corners);
		if(scores != NULL) free(scores);

		/*Select lines*/
		for(std::vector<HPoint2D>::iterator p1=image::unique_corners->begin(); p1 != image::unique_corners->end(); p1++)
			for(std::vector<HPoint2D>::iterator p2=p1+1; p2 != image::unique_corners->end(); p2++) {
				/*Check distance between corners*/
				if(!(geometry::distanceBetweenPoints2D((*p1).x, (*p1).y, (*p2).x, (*p2).y) > MIN_DISTANCE_CORNERS))
					continue;
			
				/*Check line (fast way)*/
				if(!isLineFast((*p1).x, (*p1).y, (*p2).x, (*p2).y))
					continue;

				/*Check line (slow way)*/
				if(!isLineSlow((*p1).x, (*p1).y, (*p2).x, (*p2).y))
					continue;

				/*Save line*/
				segment.start.x = (float) (*p1).x;
				segment.start.y = (float) (*p1).y;
				segment.end.x = (float) (*p2).x;
				segment.end.y = (float) (*p2).y;
				segment.isValid = true;
				segments->push_back(segment);
			}

		return image::unique_corners;
	}

/*	int image::multiplyFFT (const CvArr* srcAarr, const CvArr* srcBarr, CvArr* dstarr) {
		CvMat *srcA = (CvMat*)srcAarr;
		CvMat *srcB = (CvMat*)srcBarr;
		CvMat *dst = (CvMat*)dstarr;

		int i,j, rows, cols;
		rows = srcA->rows;
		cols = srcA->cols;
		double c_re,c_im;

		for( i=0; i<rows; i++ )	{
			for( j = 0; j < cols; j ++ ) {
				c_re = ((double*)(srcA->data.ptr + srcA->step*i))[j*2]*((double*)(srcB->data.ptr + srcB->step*i))[j*2] -
				((double*)(srcA->data.ptr + srcA->step*i))[j*2+1]*((double*)(srcB->data.ptr + srcB->step*i))[j*2+1];
				c_im = ((double*)(srcA->data.ptr + srcA->step*i))[j*2]*((double*)(srcB->data.ptr + srcB->step*i))[j*2+1] +
				((double*)(srcA->data.ptr + srcA->step*i))[j*2+1]*((double*)(srcB->data.ptr + srcB->step*i))[j*2];
				((double*)(dst->data.ptr + dst->step*i))[j*2]	= c_re;
				((double*)(dst->data.ptr + dst->step*i))[j*2+1]	= c_im;
			}
		}

		return 1;
	}*/

	bool image::isLineFast(int x1, int y1, int x2, int y2) {
		HPoint2D v;
		int cx, cy;

		/*Get vector*/
		v.x = x2-x1;
		v.y = y2-y1;
			
		/*Center*/
		cx = x1 + 0.5*v.x;
		cy = y1 + 0.5*v.y;

		if(!isEdge(cx, cy))
			return false;

		/*1st cuarter*/
		cx = x1 + 0.25*v.x;
		cy = y1 + 0.25*v.y;

		if(!isEdge(cx, cy))
			return false;

		/*3rd cuarter*/
		cx = x1 + 0.75*v.x;
		cy = y1 + 0.75*v.y;

		if(!isEdge(cx, cy))
			return false;

		return true;
	}

	bool image::isLineSlow(int x1, int y1, int x2, int y2) {
		HPoint2D v;
		int cx, cy;
		int dist, num_steps, max_nvalid, step = 2;
		double frac, frac_step;
		int n_nvalid = 0;

		/*Get vector*/
		v.x = x2-x1;
		v.y = y2-y1;

		/*Distance between points*/
		dist = geometry::distanceBetweenPoints2D(x1, y1, x2, y2);
		frac_step = (double)step/(double)dist;
		num_steps = dist/step;

		/*Calculate max non valid*/
		max_nvalid = (int) ((double)num_steps*(1.0-MIN_PERCENTAGE_VALID) + 1.0);

		/*Cover line*/
		frac = frac_step;
		while(frac <= 1.0) {
			cx = x1 + frac*v.x;
			cy = y1 + frac*v.y;

			if(!isEdge(cx, cy))
				n_nvalid++;

			/*Max non valid reached, it's not a valid line*/
			if(n_nvalid >= max_nvalid)
				return false;

			frac += frac_step;
		}
	
		return true;
	}

	bool image::isEdge(int x, int y) {
		int pos;

		pos = y*IMAGE_WIDTH + x;	

		/*Check if its already calculated*/
		if(image::calThreshold[pos])
			return image::thresholds[pos];

		/*Calculate new point*/
		if(image::getEdgeThresholed(x, y))
			image::thresholds[pos] = true;
		else
			image::thresholds[pos] = false;

		image::calThreshold[pos] = true;
		return image::thresholds[pos];
	}

	bool image::getEdgeThresholed(int x, int y) {
		int threshold = 2;
		int ti, tj;

		/*Check basic point*/
		if(getEdge(x, y))
			return true;

		/*Check neighbors*/
		for(int i=2;i<=threshold;i++) {
			/*Top*/
			ti = x;
			tj = y-i;
			if(tj>=0) {
				if(getEdge(ti, tj))
					return true;
			}

			/*Bottom*/
			ti = x;
			tj = y+i;
			if(tj<IMAGE_HEIGHT) {
				if(getEdge(ti, tj))
					return true;
			}

			/*Left*/
			ti = x-i;
			tj = y;
			if(ti>=0) {
				if(getEdge(ti, tj))
					return true;
			}

			/*Right*/
			ti = x+i;
			tj = y;
			if(ti<IMAGE_WIDTH) {
				if(getEdge(ti, tj))
					return true;
			}
		}

		return false;
	}

	bool image::getEdge(int x, int y) {
		int pos;
		int gx, gy, sum;
		int posc1, posc2, posc3;

		pos = y*IMAGE_WIDTH + x;

		/*Check if its already calculated*/
		if(image::calPixel[pos])
			return image::pixels[pos];

		/*Check borders*/
		if(x==0 || y==0 || x==IMAGE_WIDTH-1 || y==IMAGE_WIDTH-1) {
			image::pixels[pos] = false;
			image::calPixel[pos] = true;
			return false;
		}

		posc1 = (y-1)*IMAGE_WIDTH + x;
		posc2 = pos;
		posc3 = (y+1)*IMAGE_WIDTH + x;

		/*Sobel filter: Sum = abs(Gx) + abs(Gy):

					-1	0	+1					+1	+2	+1	
		Gx = 	-2	0	+2		Gy = 	0		0		0
					-1	0	+1					-1	-2	-1*/

		gx = 	-(unsigned int)image::current_src->data[posc1-1] + (unsigned int)image::current_src->data[posc1+1] +
					-2*(unsigned int)image::current_src->data[posc2-1] + 2*(unsigned int)image::current_src->data[posc2+1] +
					-(unsigned int)image::current_src->data[posc3-1] + (int)image::current_src->data[posc3+1];

		gy = 	1*(unsigned int)image::current_src->data[posc1-1] + 2*(unsigned int)image::current_src->data[posc1] + 1*(unsigned int)image::current_src->data[posc1+1] +
					-1*(unsigned int)image::current_src->data[posc3-1] + -2*(unsigned int)image::current_src->data[posc3] + -1*(unsigned int)image::current_src->data[posc3+1];

		sum = abs(gx) + abs(gy);
		
		/*Calculate new point*/
		if(sum > image::sobel_threshold)
			image::pixels[pos] = true;
		else
			image::pixels[pos] = false;

		image::calPixel[pos] = true;
		return image::pixels[pos];
			
	}

	void image::getSegmentsDebug(cv::Mat &src) {

		for(int ti=0;ti<IMAGE_WIDTH;ti++)
			for(int tj=0;tj<IMAGE_HEIGHT;tj++)
				isEdge(ti, tj);

		for(int ti=0;ti<IMAGE_WIDTH;ti++)
			for(int tj=0;tj<IMAGE_HEIGHT;tj++) {
				int pos = tj*IMAGE_WIDTH + ti;
				if(image::thresholds[pos])
					src.data[pos] = 255;
				else
					src.data[pos] = 0;
			}
	}

	bool image::is_unique_corner(int x, int y, int score) {
		double dist;

		for(std::vector<HPoint2D>::iterator p1=image::unique_corners->begin(); p1 != image::unique_corners->end(); p1++) {
			dist = geometry::distanceBetweenPoints2D((double)(*p1).x, (double)(*p1).y, (double)x, (double)y);

			/*Check distance in pixels*/
			if(dist <= MIN_DISTANCE_UNIQUE_CORNERS) {
				/*Check score*/
				if((float) score > (*p1).h) {
					(*p1).x = (float) x;
					(*p1).y = (float) y;
					(*p1).h = (float) score;
				}
				return false;
			}
		}

		return true;
	}
}


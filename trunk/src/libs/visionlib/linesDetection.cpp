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
 *
 *  This library was programed for RobotVision Project http://jde.gsyc.es/index.php/robotvision
 *
 */

#include "linesDetection.h"

namespace visionLibrary {

	const int linesDetection::CASES_OFFSET = 5;
	const int linesDetection::solis_cases[][11] = {
		{6,6,6,7,7,7,7,7,8,8,8},
		{6,6,6,7,7,7,7,7,8,8,8},
		{6,6,6,6,7,7,7,8,8,8,8},
		{5,5,6,6,7,7,7,8,8,1,1},
		{5,5,5,5,0,0,0,1,1,1,1},
		{5,5,5,5,0,0,0,1,1,1,1},
		{5,5,5,5,0,0,0,1,1,1,1},
		{5,5,4,4,3,3,3,2,2,1,1},
		{4,4,4,4,3,3,3,2,2,2,2},
		{4,4,4,3,3,3,3,3,2,2,2},
		{4,4,4,3,3,3,3,3,2,2,2}
	};

	void linesDetection::solisAlgorithm (cv::Mat &image, std::vector<Segment2D> *segments) {
		cv::Mat imgTmp1,imgTmp2,imgBlack,imgLaplace;
		CvPoint pstart, pend;
		int i, i_jump = 6;
		int ThressValue = 30;
		int diff_x, diff_y;
		const int min_size_contour = 30;
		CvScalar color;
		Segment2D mySegment;
		int type, current_type;
		int num_frag = 0, min_frags = 3, counter = 0;
		double max_distance;
		bool first, debug = false;

		imgTmp1 = cv::Mat(image.size(), CV_8UC1);
		imgTmp2 = cv::Mat(image.size(), CV_8UC1);
		imgLaplace = cv::Mat(image.size(), CV_16SC1);

		if(debug) {
			imgBlack = cv::Mat(image.size(), CV_8UC3);
			imgBlack = (const cv::Scalar&) 0;
		}

		/*Convert to Gray Image*/
		cv::cvtColor(image, imgTmp1, CV_RGB2GRAY);

		/*Normalize image*/
		cv::normalize(imgTmp1, imgTmp1, 0, 255, CV_MINMAX);

		// Make a average filtering
		cv::blur(imgTmp1,imgTmp2,cv::Size(3,3));

		//Laplace
		cv::Laplacian(imgTmp2, imgLaplace, CV_16S, 3);
		cv::convertScaleAbs(imgLaplace,imgTmp1);

		/*Perform a binary threshold*/
		cv::threshold(imgTmp1,imgTmp2,ThressValue,255,CV_THRESH_BINARY);

		/*Find contours*/
		std::vector< std::vector<cv::Point> > vecContours;
		cv::findContours(imgTmp2, vecContours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		/*Run through found coutours*/
		for(int contour=0; contour<vecContours.size(); contour++) {

			/*Check size*/
			if(vecContours[contour].size() < min_size_contour)
				continue;

			i = 0;
			first = true;

			while ((i < (vecContours[contour].size() - (i_jump-1)))) {

				counter++;

				pstart.x = vecContours[contour][(int)i].x;
				pstart.y = vecContours[contour][(int)i].y;
				pend.x = vecContours[contour][(int)(i+i_jump-1)].x;
				pend.y = vecContours[contour][(int)(i+i_jump-1)].y;

				/*Calculate type*/
				diff_x = pstart.x - pend.x;
				diff_y = pstart.y - pend.y;
				type = linesDetection::solis_cases[diff_x+linesDetection::CASES_OFFSET][diff_y+linesDetection::CASES_OFFSET];

				if (debug) {
					/*Visual portions*/
					if (type==0)
						color = CV_RGB(255,255,255);
					else if (type==1)
						color = CV_RGB(255,255,0);
					else if (type==2)
						color = CV_RGB(0,255,255);
					else if (type==3)
						color = CV_RGB(255,0,0);
					else if (type==4)
						color = CV_RGB(0,255,0);
					else if (type==5)
						color = CV_RGB(0,0,255);
					else if (type==6)
						color = CV_RGB(255,0,255);
					else if (type==7)
						color = CV_RGB(0,128,128);
					else if (type==8)
						color = CV_RGB(128,128,0);

					/*Draw line with orientation*/
					cv::line(imgBlack, pstart, pend, color, 2, CV_AA, 0);
				} else {
					if (first) {
						if(type != 0) {
							mySegment.start.x = pstart.x;
							mySegment.start.y = pstart.y;
							mySegment.start.h = (float) counter;
							mySegment.end.x = pend.x;
							mySegment.end.y = pend.y;
							mySegment.end.h = (float) counter;
							mySegment.type = type;
							first = false;
							current_type = type;
							num_frag = 1;
						}
					} else {
						/*Check type threshold*/
						if (current_type == type) { //current_type == type || type == 0
							/*Save current end*/
							mySegment.end.x = pend.x;
							mySegment.end.y = pend.y;
							mySegment.end.h = (float) counter;
							num_frag++;
						} else {
							/*Save current segment if length is enough*/
							if(num_frag >= min_frags)
								segments->push_back(mySegment);
							first = true;
						}
					}
				}

				i += i_jump;
			}

			/*Save the last one*/
			if(!first && num_frag >= min_frags)
				segments->push_back(mySegment);
		}

		if (debug) {
			//cv::cvtColor(imgTmp1, imgBlack, CV_GRAY2RGB);
			imgBlack.copyTo(image);
		} else {

			/*CvPoint p1, p2;*/

			/*Draw lines*/
			/*for(std::vector<Segment2D>::iterator it1 = segments->begin(); it1 != segments->end(); it1++) {
				p1.x = (*it1).start.x;
				p1.y = (*it1).start.y;
				p2.x = (*it1).end.x;
				p2.y = (*it1).end.y;
				cv::line (&image, p1, p2, CV_RGB(0,0,255), 2, 8);	
			}*/
		}

		max_distance = (double) (i_jump*min_frags);

		/*Merge consecutive fragments*/
		std::vector<Segment2D>::iterator it1 = segments->begin();
		while(it1 != segments->end()) {
				/*Compare with next one*/
				std::vector<Segment2D>::iterator it2 = it1;
				it2++;
				if(it2 != segments->end()) {	
					if((*it1).type == (*it2).type && (*it2).start.h - (*it1).end.h <= min_frags) {
						if(geometry::distanceBetweenPoints2D((*it2).start.x, (*it2).start.y, (*it1).end.x, (*it1).end.y) <= max_distance) { 
							(*it1).end.x = (*it2).end.x;
							(*it1).end.y = (*it2).end.y;
							(*it1).end.h = (*it2).end.h;
							segments->erase(it2);
							continue;
						}
					}
				}

				it1++;
		}

		/*Reset .h values*/
		for(std::vector<Segment2D>::iterator it_s = segments->begin(); it_s != segments->end(); it_s++) {
			(*it_s).start.h = 1.0;
			(*it_s).end.h = 1.0;
		}

		/*Clean up*/
		imgTmp1.release();
		imgTmp2.release();
		imgLaplace.release();
		if(debug)
			imgBlack.release();

		for(int t1 = 0; t1<vecContours.size(); t1++)
			vecContours[t1].clear();
		vecContours.clear();
	}
}


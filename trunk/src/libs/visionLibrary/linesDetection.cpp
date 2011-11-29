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

	void linesDetection::solisAlgorithm (IplImage &image, std::vector<Segment2D> *segments) {
		IplImage *IplTmp1,*IplTmp2,*IplBlack,*IplLaplace;
		CvPoint pstart, pend;
		CvMemStorage *storage;
		CvSeq* contour = NULL;
		int i, i_jump = 6;
		int ThressValue = 30;
		int diff_x, diff_y;
		CvPoint *WholePointArray;
		const int min_size_contour = 30;
		CvScalar color;
		Segment2D mySegment;
		int type, current_type;
		int num_frag, min_frags = 3, counter = 0;
		double max_distance;
		bool first, debug = false;

		IplTmp1 = cvCreateImage (cvSize(image.width,image.height), IPL_DEPTH_8U, 1);
		IplTmp2 = cvCreateImage (cvSize(image.width,image.height), IPL_DEPTH_8U, 1);
		IplLaplace = cvCreateImage (cvSize(image.width,image.height), IPL_DEPTH_16S, 1);
		storage = cvCreateMemStorage(0);

		if(debug) {
			IplBlack = cvCreateImage (cvSize(image.width,image.height), IPL_DEPTH_8U, 3);
			cvZero(IplBlack);
		}

		/*Convert to Gray Image*/
		cvCvtColor (&image, IplTmp1, CV_RGB2GRAY);

		/*Normalize image*/
		cvNormalize(IplTmp1, IplTmp1, 0, 255, CV_MINMAX);

		// Make a average filtering
		cvSmooth(IplTmp1,IplTmp2,CV_BLUR,3,3);

		//Laplace
		cvLaplace(IplTmp2, IplLaplace, 3);
		cvConvertScale(IplLaplace,IplTmp1);

		/*Perform a binary threshold*/
		cvThreshold(IplTmp1,IplTmp2,ThressValue,255,CV_THRESH_BINARY);

		/*Find contours*/
		cvFindContours(IplTmp2, storage, &contour,sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		/*Run through found coutours*/
		while (contour != NULL) {
			/*Check length*/
			if (contour->total >= min_size_contour) {
				/*Convert to array*/
				WholePointArray = (CvPoint *)malloc(contour->total * sizeof(CvPoint));
				cvCvtSeqToArray(contour, WholePointArray, CV_WHOLE_SEQ);

				i = 0;
				first = true;

				while ((i < (contour->total - (i_jump-1)))) {

					counter++;

					/*Get current segment*/
					pstart.x = WholePointArray[(int)i].x;
					pstart.y = WholePointArray[(int)i].y;
					pend.x = WholePointArray[(int)(i+i_jump-1)].x;
					pend.y = WholePointArray[(int)(i+i_jump-1)].y;

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
						cvLine(IplBlack, pstart, pend, color, 2, CV_AA, 0);
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
							/*Check angle threshold*/
							if (current_type == type || type == 0) {
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

				free(WholePointArray);
			}

			contour = contour->h_next;
		}

		if (debug) {
			//cvCvtColor (IplTmp1, IplBlack, CV_GRAY2RGB);
			cvCopy(IplBlack, &image, NULL);
		} else {

			/*CvPoint p1, p2;*/

			/*Draw lines*/
			/*for(std::vector<Segment2D>::iterator it1 = segments->begin(); it1 != segments->end(); it1++) {
				p1.x = (*it1).start.x;
				p1.y = (*it1).start.y;
				p2.x = (*it1).end.x;
				p2.y = (*it1).end.y;
				cvLine (&image, p1, p2, CV_RGB(0,0,255), 2, 8);	
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
		cvReleaseImage(&IplTmp1);
		cvReleaseImage(&IplTmp2);
		cvClearMemStorage(storage); //Free contour
		cvReleaseMemStorage (&storage);

		if(debug)
			cvReleaseImage(&IplBlack);
	}
}


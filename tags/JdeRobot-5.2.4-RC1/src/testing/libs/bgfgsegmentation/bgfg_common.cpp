/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *            Redouane Kachach <redo.robot at gmail.com>
 *
 */

#include "bgfgsegmentation.h"


// Function bgfgSegmentation returns number of foreground regions
// parameters:
//      model     - pointer to BGMeanStatModel structure
//      sg_params - pointer to segmentation parameters
CV_IMPL int
bgfgSegmentation(CvBGStatModel*  model, BGFGSegmentationParams* sg_params){
  CvSeq *first_seq = NULL, *prev_seq = NULL, *seq = NULL;
  int region_count = 0;

  //clears storage
  cvClearMemStorage(model->storage);
  // Foreground segmentation.
  // Smooth foreground map:
  if( sg_params->perform_morphing ){
    cvMorphologyEx( model->foreground, model->foreground, 0, 0, CV_MOP_OPEN,  sg_params->perform_morphing );
    cvMorphologyEx( model->foreground, model->foreground, 0, 0, CV_MOP_CLOSE, sg_params->perform_morphing );
  }

  if( sg_params->minArea > 0 || sg_params->is_obj_without_holes ){
    // Discard under-size foreground regions:
    //
    cvFindContours( model->foreground, model->storage, &first_seq, sizeof(CvContour), CV_RETR_LIST,
		    CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    for( seq = first_seq; seq; seq = seq->h_next ){
      CvContour* cnt = (CvContour*)seq;
      if( cnt->rect.width * cnt->rect.height < sg_params->minArea || 
	  (sg_params->is_obj_without_holes && CV_IS_SEQ_HOLE(seq)) ){
	// Delete under-size contour:
	prev_seq = seq->h_prev;
	if( prev_seq ){
	  prev_seq->h_next = seq->h_next;
	  if( seq->h_next ) seq->h_next->h_prev = prev_seq;
	}else{
	  first_seq = seq->h_next;
	  if( seq->h_next ) seq->h_next->h_prev = NULL;
	}
      }else{
	region_count++;
      }
    }        
    model->foreground_regions = first_seq;
    cvZero(model->foreground);
    cvDrawContours(model->foreground, first_seq, CV_RGB(0, 0, 255), CV_RGB(0, 0, 255), 10, -1, 8, cvPoint(0,0));
  }else{
    model->foreground_regions = NULL;
  }

  return region_count;
}

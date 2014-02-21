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
 *
 */

#include "bgfgsegmentation.h"
#include <iostream>

//fw declarations
static void releaseBGExpStatModel( BGExpStatModel** _model );
static int updateBGExpStatModel( IplImage* curr_frame,
				 BGExpStatModel*  model );

// Function createBGExpStatModel initializes foreground detection process
// parameters:
//      first_frame - frame from video sequence
//      parameters  - (optional) if NULL default parameters of the algorithm will be used
//      p_model     - pointer to BGExpStatModel structure
CV_IMPL CvBGStatModel*
createBGExpStatModel( IplImage* first_frame, BGExpStatModelParams* parameters ){
  BGExpStatModel* p_model = 0;
    
  CV_FUNCNAME( "createBGExpStatModel" );

  __BEGIN__;

  BGExpStatModelParams params;

  if( !CV_IS_IMAGE(first_frame) )
    CV_ERROR( CV_StsBadArg, "Invalid or NULL first_frame parameter" );
  
  if (first_frame->nChannels != 3)
    CV_ERROR( CV_StsBadArg, "first_frame must have 3 color channels" );


  // Initialize parameters:
  if( parameters == NULL ){
    params.alpha = BGFG_EXP_ALPHA;
    params.bg_update_rate = BGFG_EXP_BG_UPDATE_RATE;
    params.fg_update_rate = BGFG_EXP_FG_UPDATE_RATE;
    params.sg_params.is_obj_without_holes = BGFG_SEG_OBJ_WITHOUT_HOLES;
    params.sg_params.perform_morphing = BGFG_SEG_PERFORM_MORPH;
    params.sg_params.minArea = BGFG_SEG_MINAREA;
    params.perform_segmentation = 1;
  }else
    params = *parameters;

  std::cerr << "Params: alpha=" << params.alpha << 
    ", perform segmentation=" << params.perform_segmentation << std::endl;

  CV_CALL( p_model = (BGExpStatModel*)cvAlloc( sizeof(*p_model) ));
  memset( p_model, 0, sizeof(*p_model) );
  p_model->type = BG_MODEL_EXP;
  p_model->release = (CvReleaseBGStatModel)releaseBGExpStatModel;
  p_model->update = (CvUpdateBGStatModel)updateBGExpStatModel;
  p_model->params = params;
  
  //init frame counters. Max value so first call will inialize bg & fg: FIXME: fast initialization??
  p_model->bg_frame_count = params.bg_update_rate;
  p_model->fg_frame_count = params.fg_update_rate;

  // Init temporary images:
  CV_CALL( p_model->foreground = cvCreateImage(cvSize(first_frame->width, first_frame->height),
					       IPL_DEPTH_8U, 1));
  CV_CALL( p_model->background = cvCloneImage(first_frame));
  CV_CALL( p_model->storage = cvCreateMemStorage(0));


  __END__;

  if( cvGetErrStatus() < 0 ){
    CvBGStatModel* base_ptr = (CvBGStatModel*)p_model;
    
    if( p_model && p_model->release )
      p_model->release( &base_ptr );
    else
      cvFree( &p_model );
    p_model = 0;
  }
  
  return (CvBGStatModel*)p_model;
}

void
releaseBGExpStatModel( BGExpStatModel** _model )
{
    CV_FUNCNAME( "releaseBGExpStatModel" );

    __BEGIN__;
    
    if( !_model )
        CV_ERROR( CV_StsNullPtr, "" );

    if( *_model ){
      BGExpStatModel* model = *_model;

      cvReleaseImage( &model->foreground );
      cvReleaseImage( &model->background );
      cvReleaseMemStorage(&model->storage);
      cvFree( _model );
    }

    __END__;
}

// Function updateBGExpStatModel updates statistical model and returns number of foreground regions
// parameters:
//      curr_frame  - current frame from video sequence
//      p_model     - pointer to BGExpStatModel structure
int
updateBGExpStatModel( IplImage* curr_frame, BGExpStatModel*  model ){
  int region_count = 0;
  
  if (model->bg_frame_count >= model->params.bg_update_rate){
    model->bg_frame_count = 0;
    //update model
    cv::Mat curr(curr_frame);
    cv::Mat bg(model->background);
    cv::addWeighted(curr, model->params.alpha, 
		    bg, (1.0-model->params.alpha), 
		    0, bg);
  }

  if (model->fg_frame_count >= model->params.fg_update_rate){
    model->fg_frame_count = 0;
    //clear fg
    cvZero(model->foreground);

    //difference bg - curr_frame. Adaptative threshold
    cvChangeDetection( model->background, curr_frame, model->foreground );

    //segmentation if required
    if (model->params.perform_segmentation)
      region_count = bgfgSegmentation((CvBGStatModel*)model, &model->params.sg_params);
  }


  //update counters
  model->fg_frame_count++;
  model->bg_frame_count++;

  return region_count;
}



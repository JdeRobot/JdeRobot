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

static void releaseBGFIXEDStatModel( BGFIXEDStatModel** _model );
static int updateBGFIXEDStatModel( IplImage* curr_frame,
				   BGFIXEDStatModel*  model );

// Function createBGFIXEDStatModel initializes foreground detection process
// parameters:
//      first_frame - frame from video sequence
//      parameters  - (optional) if NULL default parameters of the algorithm will be used
//      p_model     - pointer to CvFGDStatModel structure
CV_IMPL CvBGStatModel*
createBGFIXEDStatModel( IplImage* first_frame, BGFIXEDStatModelParams* parameters ){
  BGFIXEDStatModel* p_model = 0;
  
  CV_FUNCNAME( "createBGFIXEDStatModel" );
  
  __BEGIN__;
  
  int i, j, k, pixel_count, buf_size;
  BGFIXEDStatModelParams params;
  
  if( !CV_IS_IMAGE(first_frame) )
    CV_ERROR( CV_StsBadArg, "Invalid or NULL first_frame parameter" );
  
  if (first_frame->nChannels != 3)
    CV_ERROR( CV_StsBadArg, "first_frame must have 3 color channels" );
  
  // Initialize parameters:
  if( parameters == NULL ){
    params.bg_update_rate = BGFG_FIXED_BG_UPDATE_RATE;
    params.fg_update_rate = BGFG_FIXED_FG_UPDATE_RATE;
    params.sg_params.is_obj_without_holes = BGFG_SEG_OBJ_WITHOUT_HOLES;
    params.sg_params.perform_morphing = BGFG_SEG_PERFORM_MORPH;
    params.sg_params.minArea = BGFG_SEG_MINAREA;
    params.perform_segmentation = 1;
  }else{
    params = *parameters;
  }

  CV_CALL( p_model = (BGFIXEDStatModel*)cvAlloc( sizeof(*p_model) ));
  memset( p_model, 0, sizeof(*p_model) );
  p_model->type = BG_MODEL_FIXED;
  p_model->release = (CvReleaseBGStatModel)releaseBGFIXEDStatModel;
  p_model->update = (CvUpdateBGStatModel)updateBGFIXEDStatModel;;
  p_model->params = params;

  //init frame counters
  //Max value so first call will inialize bg & fg: FIXME: fast initialization??
  p_model->bg_frame_count = params.bg_update_rate;
  p_model->fg_frame_count = params.fg_update_rate;

  // Initialize storage pools:
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
releaseBGFIXEDStatModel( BGFIXEDStatModel** _model ){
  CV_FUNCNAME( "releaseBGFIXEDStatModel" );
  
  __BEGIN__;
  
  if( !_model )
    CV_ERROR( CV_StsNullPtr, "" );
  
  if( *_model ){
    BGFIXEDStatModel* model = *_model;
    
    cvReleaseImage( &model->foreground );
    cvReleaseImage( &model->background );
    cvReleaseMemStorage(&model->storage);
    cvFree( _model );
  }
  
  __END__;
}


// Function updateBGFIXEDStatModel updates model and returns number of foreground regions
// parameters:
//      curr_frame  - current frame from video sequence
//      p_model     - pointer to BGFIXEDStatModel structure
int
updateBGFIXEDStatModel( IplImage* curr_frame, BGFIXEDStatModel*  model ){
  int region_count = 0;

  if (model->fg_frame_count >= model->params.fg_update_rate){
    model->fg_frame_count = 0;//reset counter
    //clear fg
    cvZero(model->foreground);

    //difference
    cvChangeDetection( model->background, curr_frame, model->foreground );

    //segmentation if required
    if (model->params.perform_segmentation)
      region_count = bgfgSegmentation((CvBGStatModel*)model, &model->params.sg_params);
  }

  //update counters
  model->fg_frame_count++;

  return region_count;
}


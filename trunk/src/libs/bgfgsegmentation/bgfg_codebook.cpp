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

static void releaseBGCBStatModel( BGCBStatModel** _model );
static int updateBGCBStatModel( IplImage* curr_frame,
				BGCBStatModel*  model );

// Function createBGCBStatModel initializes foreground detection process
// parameters:
//      first_frame - frame from video sequence
//      parameters  - (optional) if NULL default parameters of the algorithm will be used
//      p_model     - pointer to CvFGDStatModel structure
CV_IMPL CvBGStatModel*
createBGCBStatModel( IplImage* first_frame, BGCBStatModelParams* parameters ){
  BGCBStatModel* p_model = 0;
  
  CV_FUNCNAME( "createBGCBStatModel" );
  
  __BEGIN__;
  
  int i, j, k, pixel_count, buf_size;
  BGCBStatModelParams params;
  
  if( !CV_IS_IMAGE(first_frame) )
    CV_ERROR( CV_StsBadArg, "Invalid or NULL first_frame parameter" );
  
  if (first_frame->nChannels != 3)
    CV_ERROR( CV_StsBadArg, "first_frame must have 3 color channels" );
  
  // Initialize parameters:
  if( parameters == NULL ){
    params.cb_rotation_rate = BGFG_CB_ROTATION_RATE;
    params.bg_update_rate = BGFG_CB_BG_UPDATE_RATE;
    params.fg_update_rate = BGFG_CB_FG_UPDATE_RATE;
    params.sg_params.is_obj_without_holes = BGFG_SEG_OBJ_WITHOUT_HOLES;
    params.sg_params.perform_morphing = BGFG_SEG_PERFORM_MORPH;
    params.sg_params.minArea = BGFG_SEG_MINAREA;
    params.perform_segmentation = 1;
  }else{
    params = *parameters;
  }

  CV_CALL( p_model = (BGCBStatModel*)cvAlloc( sizeof(*p_model) ));
  memset( p_model, 0, sizeof(*p_model) );
  p_model->type = BG_MODEL_CB;
  p_model->release = (CvReleaseBGStatModel)releaseBGCBStatModel;
  p_model->update = (CvUpdateBGStatModel)updateBGCBStatModel;;
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
  CV_CALL( cvZero(p_model->background));/*cb doesn't have a bg image*/
  CV_CALL( p_model->storage = cvCreateMemStorage(0));

  //Init codebook models
  p_model->cb_rotation_count = params.cb_rotation_rate;
  p_model->fast_init_count = BGFG_CB_ROTATION_RATE;
  CV_CALL( p_model->active_cb = cvCreateBGCodeBookModel());
  //CV_CALL( cvBGCodeBookUpdate(p_model->active_cb,first_frame));
  CV_CALL( p_model->updating_cb = cvCreateBGCodeBookModel());
  

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
releaseBGCBStatModel( BGCBStatModel** _model ){
  CV_FUNCNAME( "releaseBGCBStatModel" );
  
  __BEGIN__;
  
  if( !_model )
    CV_ERROR( CV_StsNullPtr, "" );
  
  if( *_model ){
    BGCBStatModel* model = *_model;
    
    cvReleaseImage( &model->foreground );
    cvReleaseImage( &model->background );
    cvReleaseMemStorage(&model->storage);
    cvReleaseBGCodeBookModel(&model->active_cb);
    cvReleaseBGCodeBookModel(&model->updating_cb);
    cvFree( _model );
  }
  
  __END__;
}


// Function updateBGCBStatModel updates model and returns number of foreground regions
// parameters:
//      curr_frame  - current frame from video sequence
//      p_model     - pointer to BGCBStatModel structure
int
updateBGCBStatModel( IplImage* curr_frame, BGCBStatModel*  model ){
  int region_count = 0;

  /*fast initialization of codebook*/
  if (model->fast_init_count-- > 0){
    cvBGCodeBookUpdate(model->updating_cb, curr_frame);
    return 0;
  }

  if (model->cb_rotation_count >= model->params.cb_rotation_rate){
    model->cb_rotation_count=0;

    //clear stale from active
    cvBGCodeBookClearStale(model->active_cb,model->active_cb->t/2);

    //switch codebooks
    CvBGCodeBookModel* tmp_cb = model->active_cb;
    model->active_cb = model->updating_cb;
    model->updating_cb = tmp_cb;
  }

  if (model->bg_frame_count >= model->params.bg_update_rate){
    model->bg_frame_count = 0;//reset counter
    //update model
    cvBGCodeBookUpdate(model->updating_cb, curr_frame);
  }

  if (model->fg_frame_count >= model->params.fg_update_rate){
    model->fg_frame_count = 0;//reset counter
    //clear fg
    cvZero(model->foreground);

    //difference
    cvBGCodeBookDiff(model->active_cb, curr_frame, model->foreground);

    //segmentation if required
    if (model->params.perform_segmentation)
      region_count = bgfgSegmentation((CvBGStatModel*)model, &model->params.sg_params);
  }

  //update counters
  model->fg_frame_count++;
  model->bg_frame_count++;
  model->cb_rotation_count++;

  return region_count;
}


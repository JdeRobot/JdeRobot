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
#include <cassert>

static void releaseBGModeStatModel( BGModeStatModel** _model );
static int updateBGModeStatModel( IplImage* curr_frame,
				  BGModeStatModel*  model );

// Function createBGModeStatModel initializes foreground detection process
// parameters:
//      first_frame - frame from video sequence
//      parameters  - (optional) if NULL default parameters of the algorithm will be used
//      p_model     - pointer to CvFGDStatModel structure
CV_IMPL CvBGStatModel*
createBGModeStatModel( IplImage* first_frame, BGModeStatModelParams* parameters ){
  BGModeStatModel* p_model = 0;
  
  CV_FUNCNAME( "createBGModeStatModel" );
  
  __BEGIN__;
  
  int i, j, k, pixel_count, buf_size;
  BGModeStatModelParams params;
  
  if( !CV_IS_IMAGE(first_frame) )
    CV_ERROR( CV_StsBadArg, "Invalid or NULL first_frame parameter" );
  
  if (first_frame->nChannels != 3)
    CV_ERROR( CV_StsBadArg, "first_frame must have 1-3 color channels" );
  
  // Initialize parameters:
  if( parameters == NULL ){
    params.n_frames = BGFG_MODE_NFRAMES;
    params.levels = BGFG_MODE_LEVELS;
    params.bg_update_rate = BGFG_MODE_BG_UPDATE_RATE;
    params.fg_update_rate = BGFG_MODE_FG_UPDATE_RATE;
    params.sg_params.is_obj_without_holes = BGFG_SEG_OBJ_WITHOUT_HOLES;
    params.sg_params.perform_morphing = BGFG_SEG_PERFORM_MORPH;
    params.sg_params.minArea = BGFG_SEG_MINAREA;
    params.perform_segmentation = 1;
  }else{
    params = *parameters;
  }

  CV_CALL( p_model = (BGModeStatModel*)cvAlloc( sizeof(*p_model) ));
  memset( p_model, 0, sizeof(*p_model) );
  p_model->type = BG_MODEL_MODE;
  p_model->release = (CvReleaseBGStatModel)releaseBGModeStatModel;
  p_model->update = (CvUpdateBGStatModel)updateBGModeStatModel;;
  p_model->params = params;

  //init frame counters. Max value so first call will inialize bg & fg: FIXME: fast initialization??
  p_model->bg_frame_count = params.bg_update_rate;
  p_model->fg_frame_count = params.fg_update_rate;

  // Initialize storage pools:
  pixel_count = first_frame->width * first_frame->height;

  p_model->frame_cbuffer_elemsize = pixel_count * first_frame->nChannels * sizeof(uchar);
  p_model->frame_cbuffer_size = params.n_frames * p_model->frame_cbuffer_elemsize;
  p_model->frame_cbuffer_idx = 0;
  CV_CALL( p_model->frame_cbuffer = (uchar*)cvAlloc(p_model->frame_cbuffer_size) );
  memset( p_model->frame_cbuffer, 0, p_model->frame_cbuffer_size );
  p_model->frame_cbuffer_idx = 0;
  

  buf_size = pixel_count * first_frame->nChannels * params.levels * sizeof(uchar);
  p_model->mode_buffer_elemsize = params.levels * sizeof(uchar);/*each element is the history for a pixel channel*/
  p_model->mode_buffer_size = p_model->mode_buffer_elemsize * first_frame->nChannels * pixel_count;
  CV_CALL( p_model->mode_buffer = (uchar*)cvAlloc(p_model->mode_buffer_size) );
  memset( p_model->mode_buffer, 0, p_model->mode_buffer_size );
  
  p_model->to_levels_scale = (params.levels-1.0)/255.0;

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
releaseBGModeStatModel( BGModeStatModel** _model ){
  CV_FUNCNAME( "releaseBGModeStatModel" );
  
  __BEGIN__;
  
  if( !_model )
    CV_ERROR( CV_StsNullPtr, "" );
  
  if( *_model ){
    BGModeStatModel* model = *_model;
    
    cvFree( &model->frame_cbuffer );
    // cvFree( &model->mode );
    // cvFree( &model->std_dev );
    cvReleaseImage( &model->foreground );
    cvReleaseImage( &model->background );
    cvReleaseMemStorage(&model->storage);
    cvFree( _model );
  }
  
  __END__;
}


// Function updateBGModeStatModel updates model and returns number of foreground regions
// parameters:
//      curr_frame  - current frame from video sequence
//      p_model     - pointer to BGModeStatModel structure
int
updateBGModeStatModel( IplImage* curr_frame, BGModeStatModel*  model ){
  int region_count = 0;

  assert(curr_frame->imageSize == model->background->imageSize);  
  if (model->bg_frame_count >= model->params.bg_update_rate){
    model->bg_frame_count = 0;
    //update model
      
    //IplImage *curr_frame_scaled = cvCloneImage(curr_frame);
    //cvConvertScaleAbs(curr_frame,curr_frame_scaled,model->to_levels_scale,0);

    assert(model->frame_cbuffer_elemsize == curr_frame->imageSize);
    uchar* frameT0_p = model->frame_cbuffer + model->frame_cbuffer_idx*model->frame_cbuffer_elemsize;
    uchar* frameNew_p = (uchar*)curr_frame->imageData;
    uchar* bg = (uchar*)model->background->imageData;
      
    for (int i=0; i<model->frame_cbuffer_elemsize; i++){//through each pixel channel p0ch0,p0ch1,...,p0chN,..
      //delete count from frame t0
      assert(frameT0_p[i] < model->params.levels);
      uchar* mode_buffer_p = model->mode_buffer + (i*model->mode_buffer_elemsize);
      if (mode_buffer_p[frameT0_p[i]] > 0)//first n_frames will initialize mode_buffer
	mode_buffer_p[frameT0_p[i]]--;
	
      //update frame_cbuffer with new frame
      frameT0_p[i] = round((double)frameNew_p[i] * model->to_levels_scale);
      if (frameT0_p[i] < 0)
	frameT0_p[i] = 0;
      else if (frameT0_p[i] > (model->params.levels-1))
	frameT0_p[i] = model->params.levels-1;

      //update count for new frame
      assert(frameT0_p[i] < model->params.levels);
      assert(mode_buffer_p[frameT0_p[i]] < 255);
      mode_buffer_p[frameT0_p[i]]++;
	
      //get mode: position of max value
      cv::Mat mode_mat(1,model->mode_buffer_elemsize,CV_8UC1,mode_buffer_p);
      cv::Point mode_pos;
      cv::minMaxLoc(mode_mat,0,0,0,&mode_pos);//y:rows,x:cols -> x=value in range [0..levels-1]
	
      //update background
      double v = round((double)mode_pos.x / model->to_levels_scale);
      bg[i] = (uchar)v;
    }

    //update frame circular buffer idx
    model->frame_cbuffer_idx++;
    if (model->frame_cbuffer_idx >= model->params.n_frames)
      model->frame_cbuffer_idx = 0;

  }

  if (model->fg_frame_count >= model->params.fg_update_rate){
    model->fg_frame_count = 0;
    //clear fg
    cvZero(model->foreground);
    
    //difference bg - curr_frame. Adaptative threshold
    cvChangeDetection( model->background, curr_frame, model->foreground );//FIXME: just 3 channel support
    
    //segmentation if required
    if (model->params.perform_segmentation)
      region_count = bgfgSegmentation((CvBGStatModel*)model, &model->params.sg_params);
  }

  //update counters
  model->fg_frame_count++;
  model->bg_frame_count++;

  return region_count;
}


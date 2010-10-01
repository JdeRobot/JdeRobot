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
#include <algorithm>

static void releaseBGMeanStatModel( BGMeanStatModel** _model );
static int updateBGMeanStatModel( IplImage* curr_frame,
				  BGMeanStatModel*  model );

// Function createBGMeanStatModel initializes foreground detection process
// parameters:
//      first_frame - frame from video sequence
//      parameters  - (optional) if NULL default parameters of the algorithm will be used
//      p_model     - pointer to CvFGDStatModel structure
CV_IMPL CvBGStatModel*
createBGMeanStatModel( IplImage* first_frame, BGMeanStatModelParams* parameters ){
  BGMeanStatModel* p_model = 0;
  
  CV_FUNCNAME( "createBGMeanStatModel" );
  
  __BEGIN__;
  
  int i, j, k, pixel_count, buf_size;
  BGMeanStatModelParams params;
  
  if( !CV_IS_IMAGE(first_frame) )
    CV_ERROR( CV_StsBadArg, "Invalid or NULL first_frame parameter" );
  
  if (first_frame->nChannels != 3)
    CV_ERROR( CV_StsBadArg, "first_frame must have 1-3 color channels" );
  
  // Initialize parameters:
  if( parameters == NULL ){
    params.n_frames = BGFG_MEAN_NFRAMES;
    params.bg_update_rate = BGFG_MEAN_BG_UPDATE_RATE;
    params.fg_update_rate = BGFG_MEAN_FG_UPDATE_RATE;
    params.sg_params.is_obj_without_holes = BGFG_SEG_OBJ_WITHOUT_HOLES;
    params.sg_params.perform_morphing = BGFG_SEG_PERFORM_MORPH;
    params.sg_params.minArea = BGFG_SEG_MINAREA;
    params.perform_segmentation = 1;
  }else{
    params = *parameters;
  }

  CV_CALL( p_model = (BGMeanStatModel*)cvAlloc( sizeof(*p_model) ));
  memset( p_model, 0, sizeof(*p_model) );
  p_model->type = BG_MODEL_MEAN;
  p_model->release = (CvReleaseBGStatModel)releaseBGMeanStatModel;
  p_model->update = (CvUpdateBGStatModel)updateBGMeanStatModel;;
  p_model->params = params;

  //init frame counters. Max value so first call will inialize bg & fg: FIXME: fast initialization??
  p_model->bg_frame_count = params.bg_update_rate;
  p_model->fg_frame_count = params.fg_update_rate;

  // Initialize storage pools:
  pixel_count = first_frame->width * first_frame->height * sizeof(uchar);
  buf_size = pixel_count * first_frame->nChannels * params.n_frames;

  CV_CALL( p_model->frame_cbuffer = (uchar*)cvAlloc(buf_size) );
  memset( p_model->frame_cbuffer, 0, buf_size );
  p_model->cbuffer_idx = 0;
  p_model->cbuffer_nentries_init = 0;

  buf_size = pixel_count * first_frame->nChannels * sizeof(double);
  CV_CALL( p_model->mean = (double*)cvAlloc(buf_size) );
  CV_CALL( p_model->std_dev = (double*)cvAlloc(buf_size) );
  

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
releaseBGMeanStatModel( BGMeanStatModel** _model ){
  CV_FUNCNAME( "releaseBGMeanStatModel" );
  
  __BEGIN__;
  
  if( !_model )
    CV_ERROR( CV_StsNullPtr, "" );
  
  if( *_model ){
    BGMeanStatModel* model = *_model;
    
    cvFree( &model->frame_cbuffer );
    cvFree( &model->mean );
    cvFree( &model->std_dev );
    cvReleaseImage( &model->foreground );
    cvReleaseImage( &model->background );
    cvReleaseMemStorage(&model->storage);
    cvFree( _model );
  }
  
  __END__;
}

/**
 * Calc mean weight based on std_dev
 * FIXME: could be a math function with some param? (log?)
 */
double get_mean_weight(double std_dev){

  if (std_dev>30)
    return 0.1;
  else if(std_dev>25)
    return 0.2;
  else if(std_dev>20)
    return 0.5;
  else if(std_dev>10)
    return 0.7;
  else if(std_dev>5)
    return 0.8;
  else 
    return 0.98;
}


// Function updateBGMeanStatModel updates model and returns number of foreground regions
// parameters:
//      curr_frame  - current frame from video sequence
//      p_model     - pointer to BGMeanStatModel structure
int
updateBGMeanStatModel( IplImage* curr_frame, BGMeanStatModel*  model ){
  int region_count = 0;

  if (model->bg_frame_count >= model->params.bg_update_rate){
    model->bg_frame_count = 0;//reset counter
    //update model
    //insert curr_frame in circular buffer
    int i,j,k;
    int frame_cbuffer_pixelcluster_step = model->params.n_frames*model->background->nChannels;
    int frame_cbuffer_cbufferpixel_offset = model->cbuffer_idx*model->background->nChannels;
    int frame_cbuffer_width_step = model->background->width*frame_cbuffer_pixelcluster_step;
    
    cv::Scalar mean, std_dev;

    //idx_last is used to avoid calc mean over not initialized values
    if (model->cbuffer_nentries_init < model->params.n_frames)
      model->cbuffer_nentries_init++;

    //bg and curr_frame have same size
    for (i = 0; i < model->background->height; i++){//rows
      uchar* frame_cbuffer_row_p = model->frame_cbuffer + i*frame_cbuffer_width_step;
      uchar* curr_frame_row_p = (uchar*)curr_frame->imageData + i*curr_frame->widthStep;
      uchar* bg_frame_row_p = (uchar*)model->background->imageData + i*model->background->widthStep;
      for (j = 0; j < model->background->width; j++){//cols
	uchar* pixel_cluster_p = frame_cbuffer_row_p + j*frame_cbuffer_pixelcluster_step;
	uchar* pixel_to_update_p = pixel_cluster_p + frame_cbuffer_cbufferpixel_offset;//pointer to pixel in circular buffer to update
	uchar* curr_pixel_p = curr_frame_row_p + j*model->background->nChannels;//pixel from curr_frame
	//update circular buffer
	for (k=0; k<model->background->nChannels; k++)
	  pixel_to_update_p[k] = curr_pixel_p[k];
	
	//calc mean and std dev   
	cv::Mat pixel_cluster(1, std::min(model->params.n_frames,model->cbuffer_nentries_init), 
			      CV_MAKETYPE(CV_8U,model->background->nChannels),
			      pixel_cluster_p);/*cv::Mat with pixel cluster with last n frames*/
	cv::meanStdDev(pixel_cluster, mean, std_dev);
	
	int pixel_offset = j*model->background->nChannels + (i * model->background->width * model->background->nChannels);
	//double* mean_p = model->mean + pixel_offset;
	//double* std_dev_p = model->std_dev + pixel_offset;
	uchar* bg_p = bg_frame_row_p + j*model->background->nChannels;
	/*copy mean and std dev to each channel*/
	for (k=0; k<model->background->nChannels; k++){
	  //mean_p[k] = mean[k];
	  //std_dev_p[k] = std_dev[k];
	  double mean_weight = get_mean_weight(std_dev[k]);
	  double a = ((double)bg_p[k])*(1-mean_weight);
	  double b = mean[k]*mean_weight;
	  bg_p[k] = cv::saturate_cast<uchar>(a+b);
	}
      }
    }
    //update circular buffer idx
    model->cbuffer_idx++;
    if (model->cbuffer_idx >= model->params.n_frames)
      model->cbuffer_idx = 0;
  }
  
  if (model->fg_frame_count >= model->params.fg_update_rate){
    model->fg_frame_count = 0;//reset counter
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


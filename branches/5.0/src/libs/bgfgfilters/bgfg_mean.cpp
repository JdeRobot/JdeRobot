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

#include "bgfgfilters.h"

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
    CV_ERROR( CV_StsBadArg, "first_frame must have 3 color channels" );
  
  // Initialize parameters:
  if( parameters == NULL ){
  }else{
    params = *parameters;
  }

  CV_CALL( p_model = (BGMeanStatModel*)cvAlloc( sizeof(*p_model) ));
  memset( p_model, 0, sizeof(*p_model) );
  p_model->type = BG_MODEL_MEAN;
  p_model->release = (CvReleaseBGStatModel)releaseBGMeanStatModel;
  p_model->update = (CvUpdateBGStatModel)updateBGMeanStatModel;;
  p_model->params = params;

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
    
    cvReleaseImage( &model->foreground );
    cvReleaseImage( &model->background );
    cvReleaseMemStorage(&model->storage);
    cvFree( _model );
  }
  
  __END__;
}


// Function updateBGMeanStatModel updates model and returns number of foreground regions
// parameters:
//      curr_frame  - current frame from video sequence
//      p_model     - pointer to BGMeanStatModel structure
int
updateBGMeanStatModel( IplImage* curr_frame, BGMeanStatModel*  model ){
  int region_count = 0;

  //clear fg
  cvZero(model->foreground);


  //difference bg - curr_frame. Adaptative threshold
  cvChangeDetection( model->background, curr_frame, model->foreground );

  //segmentation if required
  if (model->params.perform_segmentation)
    region_count = bgfgSegmentation((CvBGStatModel*)model, &model->params.sg_params);

  //update model
  
  return region_count;
}


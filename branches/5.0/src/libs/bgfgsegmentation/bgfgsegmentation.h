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

#ifndef BGFGFILTER_H
#define BGFGFILTER_H

/* We are going to use OpenCV API to implement our background/foreground filters*/

#include <opencv/cv.h>
#include <opencv/cvaux.h>

/*Implemented models. See cvaux.h. Last declared there CV_BG_MODEL_FGD_SIMPLE 2*/
#define BG_MODEL_MEAN 3
#define BG_MODEL_MODE 4
#define BG_MODEL_EXP 5
#define BG_MODEL_CB 6
#define BG_MODEL_FIXED 7

/* BG/FG Segmentation API*/

#define BGFG_SEG_OBJ_WITHOUT_HOLES 1
#define BGFG_SEG_PERFORM_MORPH 1
#define BGFG_SEG_MINAREA 15.0f

typedef struct BGFGSegmentationParams
{
  int   is_obj_without_holes;/* If TRUE we ignore holes within foreground blobs. Defaults to TRUE.*/
  int   perform_morphing;    /* Number of erode-dilate-erode foreground-blob cleanup iterations.*/
  float minArea; /* Discard foreground blobs whose bounding box is smaller than this threshold.*/
}BGFGSegmentationParams;

CVAPI(int) bgfgSegmentation(CvBGStatModel*  model, BGFGSegmentationParams* sg_params);

/* BG mean based API*/
/* BG Exp dafault parameters*/
#define BGFG_MEAN_NFRAMES 30
#define BGFG_MEAN_BG_UPDATE_RATE 25
#define BGFG_MEAN_FG_UPDATE_RATE 1

typedef struct BGMeanStatModelParams
{
  int n_frames;/*number of frames to calculate mean*/
  int bg_update_rate;/*number of frames every bg is updated. [1..]*/
  int fg_update_rate;/*number of frames every fg mask is updated. if 0 fg mask is never updated [0..]*/
  BGFGSegmentationParams sg_params;
  int perform_segmentation;/*apply segmentation to calculated fg mask*/
}BGMeanStatModelParams;

typedef struct BGMeanStatModel
{
  CV_BG_STAT_MODEL_FIELDS();
  int pixel_count;/*number of pixels*/
  int bg_frame_count;/*count updated bg frames*/
  int fg_frame_count;/*count updated fg frames*/
  uchar* frame_cbuffer;/*circular frame buffer*/
  int cbuffer_idx;/*circular buffer idx*/
  int cbuffer_nentries_init;/*circular buffer idx*/
  double* mean;
  double* std_dev;
  BGMeanStatModelParams params;
}BGMeanStatModel;

CVAPI(CvBGStatModel*) createBGMeanStatModel( IplImage* first_frame,
					     BGMeanStatModelParams* parameters CV_DEFAULT(NULL));


/* BG mode based API*/
#define BGFG_MODE_NFRAMES 30
#define BGFG_MODE_LEVELS 64
#define BGFG_MODE_BG_UPDATE_RATE 25
#define BGFG_MODE_FG_UPDATE_RATE 1


typedef struct BGModeStatModelParams
{
  int n_frames;/*number of frames to calculate mode*/
  int levels;/*quantization per levels color channel*/
  int bg_update_rate;/*number of frames every bg is updated. [1..]*/
  int fg_update_rate;/*number of frames every fg mask is updated. if 0 fg mask is never updated [0..]*/
  BGFGSegmentationParams sg_params;
  int perform_segmentation;
}BGModeStatModelParams;

typedef struct BGModeStatModel
{
  CV_BG_STAT_MODEL_FIELDS();
  int pixel_count;/*number of pixels*/
  int bg_frame_count;/*count updated bg frames*/
  int fg_frame_count;/*count updated fg frames*/
  uchar* frame_cbuffer;/*circular frame buffer*/
  int frame_cbuffer_idx;/*circular buffer idx*/
  int frame_cbuffer_elemsize;
  int frame_cbuffer_size;
  uchar* mode_buffer;
  int mode_buffer_elemsize;
  int mode_buffer_size;
  double to_levels_scale;/*scale factor to adjust from 0..255 to 0..levels-1*/
  BGModeStatModelParams params;
}BGModeStatModel;

CVAPI(CvBGStatModel*) createBGModeStatModel( IplImage* first_frame,
					     BGModeStatModelParams* parameters CV_DEFAULT(NULL));

/*BG Exp dafault parameters*/
#define BGFG_EXP_ALPHA 0.2f
#define BGFG_EXP_BG_UPDATE_RATE 25
#define BGFG_EXP_FG_UPDATE_RATE 1


/* BG exponential based API*/
typedef struct BGExpStatModelParams
{
  float alpha; /*Input image weight. values in [0,1].*/
  int bg_update_rate;/*number of frames every bg is updated. [1..]*/
  int fg_update_rate;/*number of frames every fg mask is updated. if 0 fg mask is never updated [0..]*/
  BGFGSegmentationParams sg_params;
  int perform_segmentation;
}BGExpStatModelParams;

typedef struct BGExpStatModel
{
  CV_BG_STAT_MODEL_FIELDS();
  int bg_frame_count;/*count updated bg frames*/
  int fg_frame_count;/*count updated fg frames*/
  BGExpStatModelParams params;
}BGExpStatModel;

CVAPI(CvBGStatModel*) createBGExpStatModel( IplImage* first_frame,
					    BGExpStatModelParams* parameters CV_DEFAULT(NULL));



/* BG cb based API*/
#define BGFG_CB_ROTATION_RATE 300
#define BGFG_CB_BG_UPDATE_RATE 25
#define BGFG_CB_FG_UPDATE_RATE 1


typedef struct BGCBStatModelParams
{
  int cb_rotation_rate;/*number of frames between cb rotation*/
  int bg_update_rate;/*number of frames every bg is updated. [1..]*/
  int fg_update_rate;/*number of frames every fg mask is updated. if 0 fg mask is never updated [0..]*/
  BGFGSegmentationParams sg_params;
  int perform_segmentation;
}BGCBStatModelParams;

typedef struct BGCBStatModel
{
  CV_BG_STAT_MODEL_FIELDS();
  int pixel_count;/*number of pixels*/
  int cb_rotation_count;/*count frames using codebook*/
  int bg_frame_count;/*count updated bg frames*/
  int fg_frame_count;/*count updated fg frames*/
  int fast_init_count;/*update cb faster upon init until count*/
  CvBGCodeBookModel* active_cb;
  CvBGCodeBookModel* updating_cb;
  BGCBStatModelParams params;
}BGCBStatModel;

CVAPI(CvBGStatModel*) createBGCBStatModel( IplImage* first_frame,
					   BGCBStatModelParams* parameters CV_DEFAULT(NULL));


/* BG fixed based API*/
#define BGFG_FIXED_BG_UPDATE_RATE 0
#define BGFG_FIXED_FG_UPDATE_RATE 1


typedef struct BGFIXEDStatModelParams
{
  int bg_update_rate;/*number of frames every bg is updated. [1..]*/
  int fg_update_rate;/*number of frames every fg mask is updated. if 0 fg mask is never updated [0..]*/
  BGFGSegmentationParams sg_params;
  int perform_segmentation;
}BGFIXEDStatModelParams;

typedef struct BGFIXEDStatModel
{
  CV_BG_STAT_MODEL_FIELDS();
  int bg_frame_count;/*count updated bg frames*/
  int fg_frame_count;/*count updated fg frames*/
  BGFIXEDStatModelParams params;
}BGFIXEDStatModel;

CVAPI(CvBGStatModel*) createBGFIXEDStatModel( IplImage* first_frame,
					      BGFIXEDStatModelParams* parameters CV_DEFAULT(NULL));


#endif //BGFGFILTER_H

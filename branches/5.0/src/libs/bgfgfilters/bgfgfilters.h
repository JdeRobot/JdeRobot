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
  double* mean;
  double* std_dev;
  BGMeanStatModelParams params;
}BGMeanStatModel;

CVAPI(CvBGStatModel*) createBGMeanStatModel( IplImage* first_frame,
					     BGMeanStatModelParams* parameters CV_DEFAULT(NULL));


/* BG mode based API*/
#define BGFG_MODE_NFRAMES 30
#define BGFG_MODE_BG_UPDATE_RATE 25
#define BGFG_MODE_FG_UPDATE_RATE 1


typedef struct BGModeStatModelParams
{
  int n_frames;/*number of frames to calculate mode*/
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
  int cbuffer_idx;/*circular buffer idx*/
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

#endif //BGFGFILTER_H

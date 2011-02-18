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

#include "bgmodelfactory.h"

namespace bgfgsegmentation {
  BGModelFactory::FactoryDict registerBGFactories(){
    BGModelFactory::FactoryDict dict;

    BGModelFactory* bgmF = new BGModelCvFGDFactory();
    dict.insert(make_pair(bgmF->description,bgmF));
    bgmF = new BGModelCvMoGFactory();
    dict.insert(make_pair(bgmF->description,bgmF));
    bgmF = new BGModelExpFactory();
    dict.insert(make_pair(bgmF->description,bgmF));
    bgmF = new BGModelMeanFactory();
    dict.insert(make_pair(bgmF->description,bgmF));
    bgmF = new BGModelModeFactory();
    dict.insert(make_pair(bgmF->description,bgmF));
    bgmF = new BGModelCBFactory();
    dict.insert(make_pair(bgmF->description,bgmF));
    bgmF = new BGModelFIXEDFactory();
    dict.insert(make_pair(bgmF->description,bgmF));

    return dict;
  }
  
  const BGModelFactory::FactoryDict BGModelFactory::factories = registerBGFactories();

  BGModelFactory::BGModelFactory(const std::string desc)
    :description(desc) {}

  const CvFGDStatModelParams BGModelCvFGDFactory::defaultParams = {CV_BGFG_FGD_LC, //Lc
								   CV_BGFG_FGD_N1C,//N1c
								   CV_BGFG_FGD_N2C,//N2c
								   CV_BGFG_FGD_LCC,//Lcc
								   CV_BGFG_FGD_N1CC,//N1cc
								   CV_BGFG_FGD_N2CC,//N2cc
								   1,//is_obj_without_holes
								   1,//perform_morphing
								   CV_BGFG_FGD_ALPHA_1,//alpha1
								   CV_BGFG_FGD_ALPHA_2,//alpha2
								   CV_BGFG_FGD_ALPHA_3,//alpha3
								   CV_BGFG_FGD_DELTA,//delta
								   CV_BGFG_FGD_T,//T
								   CV_BGFG_FGD_MINAREA//minArea
  };
  
  BGModelCvFGDFactory::BGModelCvFGDFactory(const std::string desc)
    :BGModelFactory(desc) {}

  // BGModelCvFGDFactory* BGModelCvFGDFactory::clone() const{
  //   return new BGModelCvFGDFactory(*this);
  // }


  CvBGStatModel* BGModelCvFGDFactory::createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const{
    CvFGDStatModelParams tmpParams;
    //parse params
    tmpParams.Lc = params.getParamAsIntWithDefault("Lc",defaultParams.Lc);
    tmpParams.N1c = params.getParamAsIntWithDefault("N1c",defaultParams.N1c);
    tmpParams.N2c = params.getParamAsIntWithDefault("N2c",defaultParams.N2c);
    tmpParams.Lcc = params.getParamAsIntWithDefault("Lcc",defaultParams.Lcc);
    tmpParams.N1cc = params.getParamAsIntWithDefault("N1cc",defaultParams.N1cc);
    tmpParams.N2cc = params.getParamAsIntWithDefault("N2cc",defaultParams.N2cc);
    tmpParams.is_obj_without_holes = params.getParamAsIntWithDefault("is_obj_without_holes",
								     defaultParams.is_obj_without_holes);
    tmpParams.perform_morphing = params.getParamAsIntWithDefault("perform_morphing",
								 defaultParams.perform_morphing);
    tmpParams.alpha1 = params.getParamAsFloatWithDefault("alpha1",defaultParams.alpha1);
    tmpParams.alpha2 = params.getParamAsFloatWithDefault("alpha2",defaultParams.alpha2);
    tmpParams.alpha3 = params.getParamAsFloatWithDefault("alpha3",defaultParams.alpha3);
    tmpParams.delta = params.getParamAsFloatWithDefault("delta",defaultParams.delta);
    tmpParams.T = params.getParamAsFloatWithDefault("T",defaultParams.T);
    tmpParams.minArea = params.getParamAsFloatWithDefault("minArea",defaultParams.minArea);
    return cvCreateFGDStatModel(firstFrame,&tmpParams);
  }

  const CvGaussBGStatModelParams BGModelCvMoGFactory::defaultParams = {CV_BGFG_MOG_WINDOW_SIZE,//win_size
								       CV_BGFG_MOG_NGAUSSIANS,//n_gauss
								       CV_BGFG_MOG_BACKGROUND_THRESHOLD,//bg_threshold
								       CV_BGFG_MOG_STD_THRESHOLD,//std_threshold
								       CV_BGFG_MOG_MINAREA,//minArea
								       CV_BGFG_MOG_WEIGHT_INIT,//weight_init
								       CV_BGFG_MOG_SIGMA_INIT,//variance_init
  };

  BGModelCvMoGFactory::BGModelCvMoGFactory(const std::string desc)
    :BGModelFactory(desc) {}

  // BGModelCvMoGFactory* BGModelCvMoGFactory::clone() const{
  //   return new BGModelCvMoGFactory(*this);
  // }

  CvBGStatModel* BGModelCvMoGFactory::createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const{
    CvGaussBGStatModelParams tmpParams;
    tmpParams.win_size = params.getParamAsIntWithDefault("win_size",defaultParams.win_size);
    tmpParams.n_gauss = params.getParamAsIntWithDefault("n_gauss",defaultParams.n_gauss);
    tmpParams.bg_threshold = params.getParamAsFloatWithDefault("bg_threshold",defaultParams.bg_threshold);
    tmpParams.std_threshold = params.getParamAsFloatWithDefault("std_threshold",defaultParams.std_threshold);
    tmpParams.minArea  = params.getParamAsFloatWithDefault("minArea",defaultParams.minArea);
    tmpParams.weight_init = params.getParamAsFloatWithDefault("weight_init",defaultParams.weight_init);
    tmpParams.variance_init = params.getParamAsFloatWithDefault("variance_init",defaultParams.variance_init);
    return cvCreateGaussianBGModel(firstFrame,&tmpParams);
  }

  const BGExpStatModelParams BGModelExpFactory::defaultParams = {BGFG_EXP_ALPHA, //alpha
								 BGFG_EXP_BG_UPDATE_RATE,
								 BGFG_EXP_FG_UPDATE_RATE,
								 {BGFG_SEG_OBJ_WITHOUT_HOLES,
								  BGFG_SEG_PERFORM_MORPH,
								  BGFG_SEG_MINAREA},//segmentation params
								 1 //perform segmentation
  };

  BGModelExpFactory::BGModelExpFactory(const std::string desc)
    :BGModelFactory(desc) {}

  // BGModelExpFactory* BGModelExpFactory::clone() const{
  //   return new BGModelExpFactory(*this);
  // }

  CvBGStatModel* BGModelExpFactory::createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const{
    BGExpStatModelParams tmpParams;
    tmpParams.alpha  = params.getParamAsFloatWithDefault("alpha",defaultParams.alpha);
    tmpParams.bg_update_rate = params.getParamAsIntWithDefault("bg_update_rate",defaultParams.bg_update_rate);
    tmpParams.fg_update_rate = params.getParamAsIntWithDefault("fg_update_rate",defaultParams.fg_update_rate);
    tmpParams.sg_params.is_obj_without_holes  = params.getParamAsIntWithDefault("is_obj_without_holes",
										defaultParams.sg_params.is_obj_without_holes);
    tmpParams.sg_params.perform_morphing = params.getParamAsIntWithDefault("perform_morphing",defaultParams.sg_params.perform_morphing);
    tmpParams.sg_params.minArea = params.getParamAsFloatWithDefault("minArea",defaultParams.sg_params.minArea);
    tmpParams.perform_segmentation = params.getParamAsIntWithDefault("perform_segmentation",defaultParams.perform_segmentation);
    return createBGExpStatModel(firstFrame,&tmpParams);
  }

  const BGMeanStatModelParams BGModelMeanFactory::defaultParams = { BGFG_MEAN_NFRAMES,//n_frames
								    BGFG_MEAN_BG_UPDATE_RATE,
								    BGFG_MEAN_FG_UPDATE_RATE,
								    {BGFG_SEG_OBJ_WITHOUT_HOLES,
								     BGFG_SEG_PERFORM_MORPH,
								     BGFG_SEG_MINAREA},//segmentation params
								    1 //perform segmentation
  };

  BGModelMeanFactory::BGModelMeanFactory(const std::string desc)
    :BGModelFactory(desc) {}

  // BGModelMeanFactory* BGModelMeanFactory::clone() const{
  //   return new BGModelMeanFactory(*this);
  // }
  

  CvBGStatModel* BGModelMeanFactory::createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const{
    BGMeanStatModelParams tmpParams;
    tmpParams.n_frames  = params.getParamAsIntWithDefault("n_frames",defaultParams.n_frames);
    tmpParams.bg_update_rate = params.getParamAsIntWithDefault("bg_update_rate",defaultParams.bg_update_rate);
    tmpParams.fg_update_rate = params.getParamAsIntWithDefault("fg_update_rate",defaultParams.fg_update_rate);
    tmpParams.sg_params.is_obj_without_holes  = params.getParamAsIntWithDefault("is_obj_without_holes",
										defaultParams.sg_params.is_obj_without_holes);
    tmpParams.sg_params.perform_morphing = params.getParamAsIntWithDefault("perform_morphing",defaultParams.sg_params.perform_morphing);
    tmpParams.sg_params.minArea = params.getParamAsFloatWithDefault("minArea",defaultParams.sg_params.minArea);
    tmpParams.perform_segmentation = params.getParamAsIntWithDefault("perform_segmentation",defaultParams.perform_segmentation);
    return createBGMeanStatModel(firstFrame,&tmpParams);
  }

  const BGModeStatModelParams BGModelModeFactory::defaultParams = {BGFG_MODE_NFRAMES,//n_frames
								   BGFG_MODE_LEVELS,
								   BGFG_MODE_BG_UPDATE_RATE,
								   BGFG_MODE_FG_UPDATE_RATE,
								   {BGFG_SEG_OBJ_WITHOUT_HOLES,
								    BGFG_SEG_PERFORM_MORPH,
								    BGFG_SEG_MINAREA},//segmentation params
								   1 //perform segmentation
  };

  BGModelModeFactory::BGModelModeFactory(const std::string desc)
    :BGModelFactory(desc) {}

  // BGModelModeFactory* BGModelModeFactory::clone() const{
  //   return new BGModelModeFactory(*this);
  // }

  CvBGStatModel* BGModelModeFactory::createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const{
    BGModeStatModelParams tmpParams;
    tmpParams.n_frames  = params.getParamAsIntWithDefault("n_frames",defaultParams.n_frames);
    tmpParams.levels  = params.getParamAsIntWithDefault("levels",defaultParams.levels);
    tmpParams.bg_update_rate = params.getParamAsIntWithDefault("bg_update_rate",defaultParams.bg_update_rate);
    tmpParams.fg_update_rate = params.getParamAsIntWithDefault("fg_update_rate",defaultParams.fg_update_rate);
    tmpParams.sg_params.is_obj_without_holes  = params.getParamAsIntWithDefault("is_obj_without_holes",
										defaultParams.sg_params.is_obj_without_holes);
    tmpParams.sg_params.perform_morphing = params.getParamAsIntWithDefault("perform_morphing",defaultParams.sg_params.perform_morphing);
    tmpParams.sg_params.minArea = params.getParamAsFloatWithDefault("minArea",defaultParams.sg_params.minArea);
    tmpParams.perform_segmentation = params.getParamAsIntWithDefault("perform_segmentation",defaultParams.perform_segmentation);
    return createBGModeStatModel(firstFrame,&tmpParams);
  }

  const BGCBStatModelParams BGModelCBFactory::defaultParams = {BGFG_CB_ROTATION_RATE,
							       BGFG_CB_BG_UPDATE_RATE,
							       BGFG_CB_FG_UPDATE_RATE,
							       {BGFG_SEG_OBJ_WITHOUT_HOLES,
								BGFG_SEG_PERFORM_MORPH,
								BGFG_SEG_MINAREA},//segmentation params
							       1 //perform segmentation
  };
  
  BGModelCBFactory::BGModelCBFactory(const std::string desc)
    :BGModelFactory(desc) {}

  // BGModelCBFactory* BGModelCBFactory::clone() const{
  //   return new BGModelCBFactory(*this);
  // }

  CvBGStatModel* BGModelCBFactory::createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const{
    BGCBStatModelParams tmpParams;
    tmpParams.cb_rotation_rate  = params.getParamAsIntWithDefault("cb_rotation_rate",defaultParams.cb_rotation_rate);
    tmpParams.bg_update_rate = params.getParamAsIntWithDefault("bg_update_rate",defaultParams.bg_update_rate);
    tmpParams.fg_update_rate = params.getParamAsIntWithDefault("fg_update_rate",defaultParams.fg_update_rate);
    tmpParams.sg_params.is_obj_without_holes  = params.getParamAsIntWithDefault("is_obj_without_holes",
										defaultParams.sg_params.is_obj_without_holes);
    tmpParams.sg_params.perform_morphing = params.getParamAsIntWithDefault("perform_morphing",defaultParams.sg_params.perform_morphing);
    tmpParams.sg_params.minArea = params.getParamAsFloatWithDefault("minArea",defaultParams.sg_params.minArea);
    tmpParams.perform_segmentation = params.getParamAsIntWithDefault("perform_segmentation",defaultParams.perform_segmentation);
    return createBGCBStatModel(firstFrame,&tmpParams);
  }

  const BGFIXEDStatModelParams BGModelFIXEDFactory::defaultParams = {BGFG_FIXED_BG_UPDATE_RATE,
								     BGFG_FIXED_FG_UPDATE_RATE,
								     {BGFG_SEG_OBJ_WITHOUT_HOLES,
								      BGFG_SEG_PERFORM_MORPH,
								      BGFG_SEG_MINAREA},//segmentation params
								     1 //perform segmentation
  };
  
  BGModelFIXEDFactory::BGModelFIXEDFactory(const std::string desc)
    :BGModelFactory(desc) {}

  CvBGStatModel* BGModelFIXEDFactory::createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const{
    BGFIXEDStatModelParams tmpParams;
    tmpParams.bg_update_rate = params.getParamAsIntWithDefault("bg_update_rate",defaultParams.bg_update_rate);
    tmpParams.fg_update_rate = params.getParamAsIntWithDefault("fg_update_rate",defaultParams.fg_update_rate);
    tmpParams.sg_params.is_obj_without_holes  = params.getParamAsIntWithDefault("is_obj_without_holes",
										defaultParams.sg_params.is_obj_without_holes);
    tmpParams.sg_params.perform_morphing = params.getParamAsIntWithDefault("perform_morphing",defaultParams.sg_params.perform_morphing);
    tmpParams.sg_params.minArea = params.getParamAsFloatWithDefault("minArea",defaultParams.sg_params.minArea);
    tmpParams.perform_segmentation = params.getParamAsIntWithDefault("perform_segmentation",defaultParams.perform_segmentation);
    return createBGFIXEDStatModel(firstFrame,&tmpParams);
  }

}//namespace

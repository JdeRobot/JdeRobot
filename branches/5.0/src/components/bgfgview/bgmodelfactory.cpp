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

namespace bgfgview{
  BGModelFactory::BGModelFactory(const std::string desc)
    :description(desc) {}

  const CvFGDStatModelParams BGModelCvFGDFactory::defaultParams = {128, //Lc
								   15,//N1c
								   25,//N2c
								   64,//Lcc
								   25,//N1cc
								   40,//N2cc
								   1,//is_obj_without_holes
								   1,//perform_morphing
								   0.1f,//alpha1
								   0.005f,//alpha2
								   0.1f,//alpha3
								   2,//delta
								   0.9f,//T
								   15.0f//minArea
  };
  
  BGModelCvFGDFactory::BGModelCvFGDFactory(const std::string desc, const CvFGDStatModelParams& params)
    :BGModelFactory(desc), params(params) {}

  BGModelCvFGDFactory* BGModelCvFGDFactory::clone() const{
    return new BGModelCvFGDFactory(*this);
  }

  CvBGStatModel* BGModelCvFGDFactory::createModel(IplImage* firstFrame) const{
    CvFGDStatModelParams tmpParams(params);//params is const
    return cvCreateFGDStatModel(firstFrame,&tmpParams);
  }

  const CvGaussBGStatModelParams BGModelCvMoGFactory::defaultParams = {200,//win_size
								5,//n_gauss
								0.7,//bg_threshold
								2.5,//std_threshold
								15.0f,//minArea
								0.05f,//weight_init
								30,//variance_init
  };

  BGModelCvMoGFactory::BGModelCvMoGFactory(const std::string desc, const CvGaussBGStatModelParams& params)
    :BGModelFactory(desc), params(params) {}

  BGModelCvMoGFactory* BGModelCvMoGFactory::clone() const{
    return new BGModelCvMoGFactory(*this);
  }

  CvBGStatModel* BGModelCvMoGFactory::createModel(IplImage* firstFrame) const{
    CvGaussBGStatModelParams tmpParams(params);//params is const
    return cvCreateGaussianBGModel(firstFrame,&tmpParams);
  }

  const BGExpStatModelParams BGModelExpFactory::defaultParams = {BGFG_EXP_ALPHA, //alpha
								 {BGFG_SEG_OBJ_WITHOUT_HOLES,
								  BGFG_SEG_PERFORM_MORPH,
								  BGFG_SEG_MINAREA},//segmentation params
								 1 //perform segmentation
  };

  BGModelExpFactory::BGModelExpFactory(const std::string desc, const BGExpStatModelParams& params)
    :BGModelFactory(desc), params(params) {}

  BGModelExpFactory* BGModelExpFactory::clone() const{
    return new BGModelExpFactory(*this);
  }

  CvBGStatModel* BGModelExpFactory::createModel(IplImage* firstFrame) const{
    BGExpStatModelParams tmpParams(params);//params is const
    return createBGExpStatModel(firstFrame,&tmpParams);
  }

  const BGMeanStatModelParams BGModelMeanFactory::defaultParams = { BGFG_MEAN_NFRAMES,//n_frames
								    {BGFG_SEG_OBJ_WITHOUT_HOLES,
								     BGFG_SEG_PERFORM_MORPH,
								     BGFG_SEG_MINAREA},//segmentation params
								    1 //perform segmentation
  };

  BGModelMeanFactory::BGModelMeanFactory(const std::string desc, const BGMeanStatModelParams& params)
    :BGModelFactory(desc), params(params) {}

  BGModelMeanFactory* BGModelMeanFactory::clone() const{
    return new BGModelMeanFactory(*this);
  }  

  CvBGStatModel* BGModelMeanFactory::createModel(IplImage* firstFrame) const{
    BGMeanStatModelParams tmpParams(params);//params is const
    return createBGMeanStatModel(firstFrame,&tmpParams);
  }

  const BGModeStatModelParams BGModelModeFactory::defaultParams = {};

  BGModelModeFactory::BGModelModeFactory(const std::string desc, const BGModeStatModelParams& params)
    :BGModelFactory(desc), params(params) {}

  BGModelModeFactory* BGModelModeFactory::clone() const{
    return new BGModelModeFactory(*this);
  }

  CvBGStatModel* BGModelModeFactory::createModel(IplImage* firstFrame) const{
    BGModeStatModelParams tmpParams(params);//params is const
    return createBGModeStatModel(firstFrame,&tmpParams);
  }

}//namespace

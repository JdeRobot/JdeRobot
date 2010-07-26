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

#ifndef BGFGVIEW_BGMODEL_H
#define BGFGVIEW_BGMODEL_H

#include <string>
#include <cvaux.h>
#include <bgfgfilters.h>

namespace bgfgview {
  class BGModelFactory{
  public:
    BGModelFactory(const std::string desc);
    virtual ~BGModelFactory() {}
    
    //virtual copy constructor
    virtual BGModelFactory* clone() const = 0;

    virtual CvBGStatModel* createModel(IplImage* firstFrame) const = 0;
    

    const std::string description;
  };

  class BGModelCvFGDFactory: public BGModelFactory{
  public:
    BGModelCvFGDFactory(const std::string desc = std::string("CvFGD"), 
			const CvFGDStatModelParams& params = BGModelCvFGDFactory::defaultParams);
    virtual ~BGModelCvFGDFactory() {}

    virtual BGModelCvFGDFactory* clone() const;
    virtual CvBGStatModel* createModel(IplImage* firstFrame) const;

    const CvFGDStatModelParams params;
    static const CvFGDStatModelParams defaultParams;
  };

  class BGModelCvMoGFactory: public BGModelFactory{
  public:
    BGModelCvMoGFactory(const std::string desc = std::string("CvMoG"), 
			const CvGaussBGStatModelParams& params = BGModelCvMoGFactory::defaultParams);
    virtual ~BGModelCvMoGFactory(){}

    virtual BGModelCvMoGFactory* clone() const;
    virtual CvBGStatModel* createModel(IplImage* firstFrame) const;

    const CvGaussBGStatModelParams params;
    static const CvGaussBGStatModelParams defaultParams;
  };

  class BGModelExpFactory: public BGModelFactory{
  public:
    BGModelExpFactory(const std::string desc = std::string("Exp"), 
		      const BGExpStatModelParams& params = BGModelExpFactory::defaultParams);
    virtual ~BGModelExpFactory() {}
    
    virtual BGModelExpFactory* clone() const;
    virtual CvBGStatModel* createModel(IplImage* firstFrame) const;

    const BGExpStatModelParams params;
    static const BGExpStatModelParams defaultParams;
  };

  class BGModelMeanFactory: public BGModelFactory{
  public:
    BGModelMeanFactory(const std::string desc = std::string("Mean"), 
		       const BGMeanStatModelParams& params = BGModelMeanFactory::defaultParams);
    virtual ~BGModelMeanFactory() {}

    virtual BGModelMeanFactory* clone() const;
    virtual CvBGStatModel* createModel(IplImage* firstFrame) const;

    const BGMeanStatModelParams params;
    static const BGMeanStatModelParams defaultParams;
  };

  class BGModelModeFactory: public BGModelFactory{
  public:
    BGModelModeFactory(const std::string desc = std::string("Mode"), 
		       const BGModeStatModelParams& params = BGModelModeFactory::defaultParams);
    virtual ~BGModelModeFactory() {}

    virtual BGModelModeFactory* clone() const;
    virtual CvBGStatModel* createModel(IplImage* firstFrame) const;

    const BGModeStatModelParams params;
    static const BGModeStatModelParams defaultParams;
  };

}//namespace

#endif //BGFGVIEW_BGMODEL_H

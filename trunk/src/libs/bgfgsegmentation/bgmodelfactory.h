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

#ifndef BGFGSEGMENTATION_BGMODELFACTORY_H
#define BGFGSEGMENTATION_BGMODELFACTORY_H

#include <string>
#include <tr1/memory>
#include <map>
#include <jderobotutil/paramdict.h>
#include <gbxutilacfr/exceptions.h>
#include "bgfgsegmentation.h"

namespace bgfgsegmentation {
  /**
   * NoSuchBGModel exception is thrown when trying to instance an unknown model
   */
  class NoSuchBGModel: public gbxutilacfr::Exception {
  public:
    NoSuchBGModel(const char *file, const char *line, const std::string &message)
      :gbxutilacfr::Exception(file,line,message) {}
  };

  class BGModelFactory{
  public:
    BGModelFactory(const std::string desc);
    virtual ~BGModelFactory() {}
    
    static CvBGStatModel* instance(const std::string modelName, 
				   const jderobotutil::ParamDict params,
				   IplImage* firstFrame) throw (NoSuchBGModel);

    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const = 0;
    const std::string description;
    typedef std::map<std::string, std::tr1::shared_ptr<BGModelFactory> > FactoryDict;
    static const FactoryDict factories;
  };
  typedef std::tr1::shared_ptr<BGModelFactory> BGModelFactoryPtr;

  class BGModelCvFGDFactory: public BGModelFactory{
  public:
    BGModelCvFGDFactory(const std::string desc = std::string("CvFGD"));
    virtual ~BGModelCvFGDFactory() {}

    //virtual BGModelCvFGDFactory* clone() const;
    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const;

    //const CvFGDStatModelParams params;
    static const CvFGDStatModelParams defaultParams;
  };

  class BGModelCvMoGFactory: public BGModelFactory{
  public:
    BGModelCvMoGFactory(const std::string desc = std::string("CvMoG"));
    virtual ~BGModelCvMoGFactory(){}

    //virtual BGModelCvMoGFactory* clone() const;
    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const;

    //const CvGaussBGStatModelParams params;
    static const CvGaussBGStatModelParams defaultParams;
  };

  class BGModelExpFactory: public BGModelFactory{
  public:
    BGModelExpFactory(const std::string desc = std::string("Exp"));
    virtual ~BGModelExpFactory() {}
    
    //virtual BGModelExpFactory* clone() const;
    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const;

    //const BGExpStatModelParams params;
    static const BGExpStatModelParams defaultParams;
  };

  class BGModelMeanFactory: public BGModelFactory{
  public:
    BGModelMeanFactory(const std::string desc = std::string("Mean"));
    virtual ~BGModelMeanFactory() {}

    //virtual BGModelMeanFactory* clone() const;
    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const;

    //const BGMeanStatModelParams params;
    static const BGMeanStatModelParams defaultParams;
  };

  class BGModelModeFactory: public BGModelFactory{
  public:
    BGModelModeFactory(const std::string desc = std::string("Mode"));
    virtual ~BGModelModeFactory() {}

    //virtual BGModelModeFactory* clone() const;
    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const;

    //const BGModeStatModelParams params;
    static const BGModeStatModelParams defaultParams;
  };

  class BGModelCBFactory: public BGModelFactory{
  public:
    BGModelCBFactory(const std::string desc = std::string("CvCB"));
    virtual ~BGModelCBFactory() {}

    //virtual BGModelCBFactory* clone() const;
    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const;

    //const BGCBStatModelParams params;
    static const BGCBStatModelParams defaultParams;
  };

  class BGModelFIXEDFactory: public BGModelFactory{
  public:
    BGModelFIXEDFactory(const std::string desc = std::string("Fixed"));
    virtual ~BGModelFIXEDFactory() {}

    //virtual BGModelFIXEDFactory* clone() const;
    virtual CvBGStatModel* createModel(const jderobotutil::ParamDict params, IplImage* firstFrame) const;

    //const BGFIXEDStatModelParams params;
    static const BGFIXEDStatModelParams defaultParams;
  };

}//namespace

#endif //BGFGSEGMENTATION_BGMODELFACTORY_H

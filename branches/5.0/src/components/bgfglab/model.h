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

#ifndef BGFGLAB_MODEL_H
#define BGFGLAB_MODEL_H

#include <jderobotutil/observer.h>
#include <jderobotutil/time.h>
#include <jderobotutil/paramdict.h>
#include <colorspaces/colorspacesmm.h>
#include <gbxutilacfr/tracer.h>
#include <opencv/cvaux.h>
#include <fstream>
#include "bgmodelfactory.h"

namespace bgfglab {
  class Model : public jderobotutil::Subject{
  public:
    Model(gbxutilacfr::Tracer& tracer, 
	  const colorspaces::Image& initialImg,
	  const colorspaces::Image::FormatPtr internalFmt) throw();
    ~Model();

    /*model input data*/
    void updateBGModel(const colorspaces::Image& img) throw();

    bool isDumpingData(int* dumpedFrames = 0) const;
    bool startDumpData(std::string filename="modeldata.dump",
		       int maxFrames=-1,
		       int startDumpingAfterFrames=0,
		       bool dumpDataImg=true, 
		       bool dumpDataBg=true, 
		       bool dumpDataFgMask=false);
    void stopDumpData();

    const colorspaces::Image& getCurrentImage() const throw() { return currentImage; }

    /*Returned image is available until BG model updated*/
    const colorspaces::Image& getBGImage() const throw();

    /*Returned image is available until BG model updated*/
    const colorspaces::Image& getFGMaskImage() const throw();

    gbxutilacfr::Tracer& tracer() { return _tracer; };

    void setBGModel(const std::string modelName, const jderobotutil::ParamDict& param) throw();
    
    const jderobotutil::IpsCounter& bgModelIps() const { return bg_model_ips; }
    
    //return a null pointer until algorithm is set
    CvBGStatModel const* bgModel() const throw() { return bg_model; }
    const jderobotutil::ParamDict& bgModelParam() const throw() { return bg_modelParam; }
  private:
    gbxutilacfr::Tracer& _tracer;
    colorspaces::Image currentImage;
    colorspaces::Image bgImage;
    colorspaces::ImageGRAY8 fgMaskImage;
    jderobotutil::IpsCounter bg_model_ips;
    CvBGStatModel* bg_model;
    jderobotutil::ParamDict bg_modelParam;
    colorspaces::Image::FormatPtr internalFmt;
    bool dumpDataOn;
    bool dumpDataImg;
    bool dumpDataBg;
    bool dumpDataFgMask;
    std::string dumpDataFilename;
    int dumpDataFrameCounter;
    int maxDumpFrames;
    int maxDumpSeqDigits;
    int noDumpFrames;
    std::ofstream ofDumpData;
    std::ofstream ofDumpDataImg;
    std::ofstream ofDumpDataBg;
    std::ofstream ofDumpDataFgMask;
  };
}//namespace
#endif /*BGFGLAB_MODEL_H*/

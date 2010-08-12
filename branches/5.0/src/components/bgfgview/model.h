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

#ifndef BGFGVIEW_MODEL_H
#define BGFGVIEW_MODEL_H

#include <jderobotutil/observer.h>
#include <jderobotutil/time.h>
#include <colorspaces/colorspacesmm.h>
#include <gbxutilacfr/tracer.h>
#include <opencv/cvaux.h>
#include <fstream>

namespace bgfgview {
  class Model : public jderobotutil::Subject{
  public:
    Model(gbxutilacfr::Tracer& tracer, const colorspaces::Image& initialImg) throw();
    ~Model();

    /*model input data*/
    void updateBGModel(const colorspaces::Image& img) throw();


    bool startDumpData(std::string filename="modeldata.dump", 
		       bool dumpDataImg=true, 
		       bool dumpDataBg=true, 
		       bool dumpDataFgMask=false);
    void stopDumpData();

    const colorspaces::Image& getCurrentImage() const throw() { return currentImage; }

    /*Returned image is available until BG model updated*/
    const colorspaces::Image& getBGImage() const throw()  { return bgImage; }

    /*Returned image is available until BG model updated*/
    const colorspaces::Image& getFGMaskImage() const throw()  { return fgMaskImage; }

    gbxutilacfr::Tracer& tracer() { return _tracer; };

    void setBGModel(CvBGStatModel* newBGModel) throw();
    
    const jderobotutil::IpsCounter& bgModelIps() const { return bg_model_ips; }
    
    //return a null pointer until algorithm is set
    CvBGStatModel const* bgModel() const throw() { return bg_model; }
  private:
    gbxutilacfr::Tracer& _tracer;
    colorspaces::Image currentImage;
    colorspaces::Image bgImage;
    colorspaces::Image fgMaskImage;
    jderobotutil::IpsCounter bg_model_ips;
    CvBGStatModel* bg_model;
    bool dumpDataOn;
    bool dumpDataImg;
    bool dumpDataBg;
    bool dumpDataFgMask;
    std::string dumpDataFilename;
    int dumpDataFrameCounter;
    std::ofstream ofDumpData;
    std::ofstream ofDumpDataImg;
    std::ofstream ofDumpDataBg;
    std::ofstream ofDumpDataFgMask;
  };
}//namespace
#endif /*BGFGVIEW_MODEL_H*/

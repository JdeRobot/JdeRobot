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


#include "model.h"
#include <assert.h>
#include <iostream>
#include <unistd.h>

namespace bgfglab {
  const std::string imgdumpSuffix = ".imgdump";
  const std::string bgdumpSuffix = ".bgdump";
  const std::string fgmaskdumpSuffix = ".fgmaskdump";

  //dump image data per byte
  std::ostream &operator<<(std::ostream &stream, const colorspaces::Image& src){
    cv::Size sz = src.size();

    CV_Assert( src.elemSize1() == 1 );//each element must be 1byte 

    if (src.isContinuous()){
      sz.width *= sz.height;
      sz.height = 1;
    }
    sz.width *= src.channels();

    for (int i = 0; i < sz.height; i++){
      const uchar* row = src.ptr(i);
      for (int j = 0; j < sz.width; j++){
	//int tmp = (int)row[j];
	//stream << tmp << ' ';
	stream << row[j];
      }
    }
    return stream;
  }

  Model::Model(gbxutilacfr::Tracer& tracer, 
	       const colorspaces::Image& initialImg,
	       const colorspaces::Image::FormatPtr internalFmt) throw ()
    : _tracer(tracer), 
      currentImage(initialImg.clone()), 
      bgImage(currentImage), 
      fgMaskImage(initialImg.width, 
		  initialImg.height),
      bg_model_ips(),
      bg_model(0),
      internalFmt(internalFmt?internalFmt:initialImg.format()) {}

  Model::~Model(){
    if (bg_model != 0){
      stopDumpData();
      cvReleaseBGStatModel(&bg_model);
    }
  }
  
  void Model::updateBGModel(const colorspaces::Image& img) throw () {
    currentImage = img.clone();//FIXME: avoid copy
    if (bg_model != 0) {
      colorspaces::Image workingImg(img.width,img.height,internalFmt);
      img.convert(workingImg);//conversion if needed
      IplImage tmpImg(workingImg);
      cvUpdateBGStatModel(&tmpImg, bg_model);

      //update bg & fgmask
      bgImage = colorspaces::Image(cv::Mat(bg_model->background),internalFmt);
      fgMaskImage = colorspaces::Image(cv::Mat(bg_model->foreground), 
				       colorspaces::ImageGRAY8::FORMAT_GRAY8);

      if (noDumpFrames > 0)
	noDumpFrames--;
      else{
	if(ofDumpData.is_open()){//dump data
	  dumpDataFrameCounter++;
	  if (ofDumpDataImg.is_open())
	    ofDumpDataImg << workingImg;//operator<< defined above
	  if (ofDumpDataBg.is_open())
	    ofDumpDataBg << getBGImage();
	  if (ofDumpDataFgMask.is_open())
	    ofDumpDataFgMask << getFGMaskImage();
	  if ((maxDumpFrames > 0) && (dumpDataFrameCounter >= maxDumpFrames))
	    stopDumpData();
	}
      }
    }
    bg_model_ips.inc();
    notifyObservers();
  }

  bool Model::isDumpingData(int* dumpedFrames) const{
    if (dumpedFrames != 0)
      *dumpedFrames = dumpDataFrameCounter-noDumpFrames;
    return ofDumpData.is_open();
  }

  bool Model::startDumpData(std::string filename, 
			    int maxFrames,
			    int startDumpingAfterFrames,
			    bool dumpDataImg, 
			    bool dumpDataBg, 
			    bool dumpDataFgMask){
    //stop previously running dump
    stopDumpData();

    if (bg_model == 0)
      return false;

    if (maxFrames == 0)
      return false;
    
    //if filename relative prefix cwd to make paths absolute
    std::string pathprefix;
    if (filename[0] != '/'){
      char cwd[PATH_MAX];

      if (getcwd(cwd,PATH_MAX))
	pathprefix = cwd;
    }
      
    this->dumpDataFilename = pathprefix + '/' + filename;
    this->dumpDataFrameCounter = 0;
    this->maxDumpFrames = maxFrames;
    this->noDumpFrames = startDumpingAfterFrames;
    
    std::cerr << "Dumping to: " << this->dumpDataFilename << std::endl;

    ofDumpData.open(dumpDataFilename.c_str(),std::ios_base::out|std::ios_base::trunc);
    if (!ofDumpData.is_open())//error, stop dump and return false
      goto startDumpData_err;

    if (dumpDataImg){
      std::string fname(dumpDataFilename + imgdumpSuffix);
      ofDumpDataImg.open(fname.c_str(),std::ios_base::out|std::ios_base::trunc);
      if (!ofDumpDataImg.is_open())//error, stop dump and return false
	goto startDumpData_err;
      //operator<< from colorspaces
      //::operator<<(ofDumpData,currentImage) << std::endl;
    }

    if (dumpDataBg){
      std::string fname(dumpDataFilename + bgdumpSuffix);
      ofDumpDataBg.open(fname.c_str(),std::ios_base::out|std::ios_base::trunc);
      if (!ofDumpDataBg.is_open())//error, stop dump and return false
	goto startDumpData_err;
      //::operator<<(ofDumpData,bgImage) << std::endl;
    }

    if (dumpDataFgMask){
      std::string fname(dumpDataFilename + fgmaskdumpSuffix);
      ofDumpDataFgMask.open(fname.c_str(),std::ios_base::out|std::ios_base::trunc);
      if (!ofDumpDataFgMask.is_open())//error, stop dump and return false
	goto startDumpData_err;
      //::operator<<(ofDumpData,fgMaskImage) << std::endl;
    }

    return ofDumpData.is_open();

  startDumpData_err:
    stopDumpData();
    return ofDumpData.is_open();
  }
    
  void Model::stopDumpData(){
    if (ofDumpData.is_open()){
      int rows = 0,cols = 0,channels = 0,nframes=0;

      if (ofDumpDataImg.is_open()){
	ofDumpDataImg.close();
	rows = currentImage.rows;
	cols = currentImage.cols;
	channels = currentImage.channels();
	nframes = dumpDataFrameCounter;
      }
      ofDumpData << dumpDataFilename + imgdumpSuffix << ' ' << nframes << ' ' 
		 << rows << ' ' << cols << ' ' 
		 << channels << std::endl;

      rows = cols = channels = nframes = 0;
      if (ofDumpDataBg.is_open()){
	ofDumpDataBg.close();
	rows = bgImage.rows;
	cols = bgImage.cols;
	channels = bgImage.channels();
	nframes = dumpDataFrameCounter;
      }
      ofDumpData << nframes << ' ' << rows << ' ' << cols << ' ' 
		 << channels << ' ' << dumpDataFilename + bgdumpSuffix << std::endl;

      rows = cols = channels = nframes = 0;
      if (ofDumpDataFgMask.is_open()){
	ofDumpDataFgMask.close();
	rows = fgMaskImage.rows;
	cols = fgMaskImage.cols;
	channels = fgMaskImage.channels();
	nframes = dumpDataFrameCounter;
      }
      ofDumpData << nframes << ' ' << rows << ' ' << cols << ' ' 
		 << channels << ' ' << dumpDataFilename + fgmaskdumpSuffix << std::endl;
      ofDumpData.close();
    }
  }

  const colorspaces::Image& Model::getBGImage() const throw() {
    return bgImage;
  }

  const colorspaces::Image& Model::getFGMaskImage() const throw(){
    return fgMaskImage;
  }

  void Model::setBGModel(CvBGStatModel* newBGModel) throw()
  { 
    assert(newBGModel != 0);

    stopDumpData();

    CvBGStatModel* oldBGModel = bg_model;
    bg_model = newBGModel;
    if (oldBGModel != 0)
      cvReleaseBGStatModel(&oldBGModel);

    notifyObservers(); 
  }

}//namespace

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
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iomanip>
#include <bgfgsegmentation/bgmodelfactory.h>

namespace bgfglab {
  const std::string imgdumpSuffix = ".img";
  const std::string bgdumpSuffix = ".bg";
  const std::string fgmaskdumpSuffix = ".fgmask";
  const std::string dumpext = "pnm";//file extension

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
      bg_modelParam(),
      internalFmt(internalFmt?internalFmt:initialImg.format()),
      dumpDataFrameCounter(0),
      maxDumpFrames(0),
      noDumpFrames(0){}

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
    }

    if(isDumpingData()){//dump data
      if (noDumpFrames > 0)//dalay
	noDumpFrames--;
      else{
	std::stringstream ss;//sequence number string filled with 0s
	ss << std::setw(maxDumpSeqDigits) << std::setfill('0') << (dumpDataFrameCounter+1);//start sequences from 1

	char s[PATH_MAX];
	snprintf(s,PATH_MAX,dumpDataFilename.c_str(),(dumpDataFrameCounter+1));
	std::string dumpfname(s);
	if (dumpDataImg){
	  //std::string fname(dumpDataFilename + imgdumpSuffix + ss.str() + '.' + dumpext);
	  std::string fname(dumpfname + imgdumpSuffix + '.' + dumpext);
	  colorspaces::ImageRGB8 imgdump(img);
	  imgdump.write(fname);
	}
	
	if (dumpDataBg){
	  //std::string fname(dumpDataFilename + bgdumpSuffix + ss.str() + '.' + dumpext);
	  std::string fname(dumpfname + bgdumpSuffix + '.' + dumpext);
	  colorspaces::ImageRGB8 bgdump(bgImage);
	  bgdump.write(fname);
	}
	
	if (dumpDataFgMask){
	  //std::string fname(dumpDataFilename + fgmaskdumpSuffix + ss.str() + '.' + dumpext);
	  std::string fname(dumpfname + fgmaskdumpSuffix + '.' + dumpext);
	  colorspaces::ImageGRAY8 fgmaskdump(fgMaskImage);
	  fgmaskdump.write(fname);
	}

	//update frame counter
	dumpDataFrameCounter++;

	// if (ofDumpDataBg.is_open())
	//   ofDumpDataBg << getBGImage();
	// if (ofDumpDataFgMask.is_open())
	//   ofDumpDataFgMask << getFGMaskImage();
	if ((maxDumpFrames > 0) && (dumpDataFrameCounter >= maxDumpFrames))
	  stopDumpData();
      }
    }
    bg_model_ips.inc();
    notifyObservers();
  }

  bool Model::isDumpingData(int* dumpedFrames) const{
    if (dumpedFrames != 0)
      *dumpedFrames = dumpDataFrameCounter-noDumpFrames;
    return dumpDataOn;
  }

  bool Model::startDumpData(std::string filename, 
			    int maxFrames,
			    int startDumpingAfterFrames,
			    bool dumpDataImg, 
			    bool dumpDataBg, 
			    bool dumpDataFgMask){
    //stop previously running dump
    stopDumpData();

    if (maxFrames == 0)
      return false;
    
    char *filenameAbsPath;
    if ((filenameAbsPath=realpath(filename.c_str(),0))){
      this->dumpDataFilename = std::string(filenameAbsPath);
      free(filenameAbsPath);
    }else//can't resolve, continue with it
      this->dumpDataFilename = filename;

    this->dumpDataFrameCounter = 0;
    this->maxDumpFrames = maxFrames;
    if (this->maxDumpFrames > 0)
      this->maxDumpSeqDigits = (int)ceil(log10(this->maxDumpFrames));
    else
      this->maxDumpSeqDigits = 10;//if unlimited frames requested use 10 digits
    this->noDumpFrames = startDumpingAfterFrames;
    this->dumpDataImg = dumpDataImg;
    this->dumpDataBg = (dumpDataBg && bg_model);//dumped only if bg_model present
    this->dumpDataFgMask = (dumpDataFgMask && bg_model);//dumped only if bg_model present
    
    std::cerr << "Dumping to: " << this->dumpDataFilename << std::endl;

    //ofDumpData.open(dumpDataFilename.c_str(),std::ios_base::out|std::ios_base::trunc);
    //dumpDataOn = ofDumpData.is_open();
    dumpDataOn=true;
    //if (!dumpDataOn)//error, stop dump and return false
    //  stopDumpData();

    return dumpDataOn;
  }
    
  void Model::stopDumpData(){
    if (dumpDataOn){
      // int rows = 0,cols = 0,channels = 0,nframes=0;

      // //if (ofDumpDataImg.is_open()){
      // if (dumpDataImg){
      // 	//ofDumpDataImg.close();
      // 	rows = currentImage.rows;
      // 	cols = currentImage.cols;
      // 	channels = currentImage.channels();
      // 	nframes = dumpDataFrameCounter;
      // }
      // ofDumpData << dumpDataFilename + imgdumpSuffix + "%0" << maxDumpSeqDigits << "d." << dumpext
      // 		 << ' ' << nframes << ' ' << rows << ' ' << cols << ' ' << channels << std::endl;

      // rows = cols = channels = nframes = 0;
      // //if (ofDumpDataBg.is_open()){
      // if (dumpDataBg){
      // 	//ofDumpDataBg.close();
      // 	rows = bgImage.rows;
      // 	cols = bgImage.cols;
      // 	channels = bgImage.channels();
      // 	nframes = dumpDataFrameCounter;
      // }
      // ofDumpData << dumpDataFilename + bgdumpSuffix + "%0" << maxDumpSeqDigits << "d." << dumpext
      // 		 << ' ' << nframes << ' ' << rows << ' ' << cols << ' ' << channels << std::endl;

      // rows = cols = channels = nframes = 0;
      // //if (ofDumpDataFgMask.is_open()){
      // if (dumpDataFgMask){
      // 	//ofDumpDataFgMask.close();
      // 	rows = fgMaskImage.rows;
      // 	cols = fgMaskImage.cols;
      // 	channels = fgMaskImage.channels();
      // 	nframes = dumpDataFrameCounter;
      // }
      // ofDumpData << dumpDataFilename + fgmaskdumpSuffix + "%0" << maxDumpSeqDigits << "d." << dumpext
      // 		 << ' ' << nframes << ' ' << rows << ' ' << cols << ' ' << channels << std::endl;
      // ofDumpData.close();
      dumpDataOn = false;
    }
  }

  const colorspaces::Image& Model::getBGImage() const throw() {
    return bgImage;
  }

  const colorspaces::Image& Model::getFGMaskImage() const throw(){
    return fgMaskImage;
  }

  void Model::setBGModel(const std::string modelName, const jderobotutil::ParamDict& param) throw()
  { 
    CvBGStatModel* newBGModel;

    try{
      IplImage tmp(bgImage);
      newBGModel = bgfgsegmentation::BGModelFactory::instance(modelName,param,&tmp);
    }catch(bgfgsegmentation::NoSuchBGModel e){
      std::cerr << "No such model found: " << e.what() << std::endl;
      return;//ignored
    }
    CvBGStatModel* oldBGModel = bg_model;
    stopDumpData();
    bg_model = newBGModel;
    bg_modelParam = param;
    if (oldBGModel != 0)
      cvReleaseBGStatModel(&oldBGModel);
    notifyObservers(); 
  }

}//namespace

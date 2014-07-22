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

#include "motiondetection.h"
#include <gbxsickacfr/gbxiceutilacfr/timer.h>
#include <sstream>

namespace motiondetection {
  const MotionItem2DSeq& NullAlgorithm::calcMotion(const colorspaces::Image& currentImg) throw (){
    //update state
    _state.previousImage = _state.currentImage;
    _state.currentImage = currentImg;
    _state.ipsCounter.inc();

    //calc time used
    // gbxiceutilacfr::Timer t;

//     std::stringstream ss;
//     ss << "OpticalFlowAlgorithm::calcMotion: elapsed=" << t.elapsedMs() << "ms; ips=" << _state.ipsCounter.ips();
//     tracer().debug(ss.str());

    return _state.motionDetected;
  }

  const MotionItem2DSeq& OpticalFlowAlgorithm::calcMotion(const colorspaces::Image& currentImg)  throw (){
    //update state
    _state.previousImage = _state.currentImage;
    _state.previousImageGRAY8 = _state.currentImageGRAY8;
    _state.currentImage = currentImg;
    _state.currentImageGRAY8 = _state.currentImage;//implicit conversion
    _state.ipsCounter.inc();

    //calc time used
    //gbxiceutilacfr::Timer t;
    
    /*fill previousPoints with good points to track*/
    int pointsToFill = nPoints - _state.previousPoints.size();
    if (pointsToFill > 10){/*avoid to recalc if only one point has been lost*/
      std::vector<cv::Point2f> fill(pointsToFill);
      std::stringstream ss;
      ss << "filling with " << pointsToFill;
      tracer().info(ss.str());
          
      cv::goodFeaturesToTrack(_state.previousImageGRAY8, fill, pointsToFill, 0.05, 5.0);
      /*add points to previousPoints*/
      _state.previousPoints.reserve(_state.previousPoints.size() + pointsToFill);
      _state.previousPoints.insert(_state.previousPoints.end(),fill.begin(),fill.end());
    }
    
    //cv::TermCriteria termCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );
    /* Pyramidal Lucas Kanade Optical Flow algorithm, search feature points in img */
    cv::calcOpticalFlowPyrLK(_state.previousImageGRAY8, _state.currentImageGRAY8, 
			     _state.previousPoints, _state.currentPoints, _state.status, _state.error);
    //cvSize(5,5), 5, termCriteria, 0);
    
    _state.motionDetected.resize(_state.previousPoints.size());//at least
    _state.maxMotionDetected = MotionItem2D();//reset

    cv::Mat_<float> x(1,_state.previousPoints.size());
    cv::Mat_<float> y(1,_state.previousPoints.size());
    cv::Mat_<float> mag(1,_state.previousPoints.size());
    
    //fill vectors coordinates
    std::vector<cv::Point2f>::iterator pp_it(_state.previousPoints.begin());
    std::vector<cv::Point2f>::iterator cp_it(_state.currentPoints.begin());
    cv::MatIterator_<float> x_it(x.begin());
    cv::MatIterator_<float> y_it(y.begin());
    for(; pp_it != _state.previousPoints.end(); pp_it++,cp_it++,x_it++,y_it++){
      cv::Point2f diff = *pp_it - *cp_it;
      *x_it = diff.x;
      *y_it = diff.y;
    }
    
    //calc vectors modulus
    cv::magnitude(x,y,mag);
    
    //fill detectedMotion
    pp_it =  _state.previousPoints.begin();
    std::vector<cv::Point2f>::iterator ppnext_it(pp_it);//to keep previous points for next iteration
    cp_it = _state.currentPoints.begin();
    std::vector<uchar>::iterator s_it(_state.status.begin());
    cv::MatIterator_<float> mag_it(mag.begin());
    MotionItem2DSeq::iterator mSeq_it(_state.motionDetected.begin());
    for(; pp_it!=_state.previousPoints.end(); pp_it++,cp_it++,s_it++,mag_it++){
      if (*s_it == 0)//point not calculated
	continue;
      
      float magnitude = *mag_it;
      if (magnitude >= (float)opticalFlowThreshold){//point cross threshold
	//normalize magnitude to motion range 0..10
	float magnitude0 = magnitude - (float)opticalFlowThreshold;
	float scale = (float)(opticalFlowMaxMotion-opticalFlowThreshold);
	MotionItem2D item((int)((magnitude0/scale)*10.0),1,cv::Rect(*pp_it,*cp_it));
	_state.maxMotionDetected = std::max(_state.maxMotionDetected,item);//update max
	*mSeq_it = item;mSeq_it++;
      }
      *ppnext_it = *cp_it;ppnext_it++;
    }
    _state.previousPoints.erase(ppnext_it,_state.previousPoints.end());//remove those which haven't been calculated
    _state.motionDetected.resize(_state.previousPoints.size());//now we know exactly

    // std::stringstream ss;
//     ss << "OpticalFlowAlgorithm::calcMotion: elapsed=" << t.elapsedMs() << "ms; ips=" << _state.ipsCounter.ips();
//     tracer().info(ss.str());

    return _state.motionDetected;
  }


  const MotionItem2DSeq& PixelDifferenceAlgorithm::calcMotion(const colorspaces::Image& currentImg) throw (){
    //update state
    _state.previousImage = _state.currentImage;
    _state.currentImage = currentImg;
    _state.ipsCounter.inc();
    
    //calc time used
    //gbxiceutilacfr::Timer t;

    if (_state.backgroundCount > 10){//FIXME: use a parameter to allow this value to be changed from GUI
      cv::addWeighted(_state.background,0.6,_state.currentImage,0.4,0.0,_state.background);
      _state.backgroundCount = 0;
    }else
      _state.backgroundCount++;

    cv::absdiff(_state.background,_state.currentImage,_state.difference);
    cv::threshold(_state.difference, _state.difference, (double)pixelDiffThreshold, 255, cv::THRESH_BINARY);
    
    // std::stringstream ss;
//     ss << "OpticalFlowAlgorithm::calcMotion: elapsed=" << t.elapsedMs() << "ms; ips=" << _state.ipsCounter.ips();
//     tracer().info(ss.str());

    return _state.motionDetected;

    // void Model::calcMotionWithPixelDifference(){
//     if (!pImpl->pixelDiffState)//first iteration
//       pImpl->pixelDiffState.reset(new PixelDifferenceState(pImpl->previousImageL8));

//     CvMat *background,*difference,*aux1,*aux2,*aux3;
//     background = cvCreateMatHeader(pImpl->pixelDiffState->backgroundL8->description.width,
// 				   pImpl->pixelDiffState->backgroundL8->description.height,
// 				   CV_8UC1);
//     cvSetData(background,pImpl->pixelDiffState->backgroundL8->imageData,CV_AUTOSTEP);
//     difference = cvCreateMatHeader(pImpl->pixelDiffState->differenceL8->description.width,
// 				   pImpl->pixelDiffState->differenceL8->description.height,
// 				   CV_8UC1);
//     cvSetData(difference,pImpl->pixelDiffState->differenceL8->imageData,CV_AUTOSTEP);
//     aux1 = cvCreateMatHeader(pImpl->previousImageL8->description.width,
// 			     pImpl->previousImageL8->description.height,
// 			     CV_8UC1);
//     aux2 = cvCreateMat(pImpl->pixelDiffState->backgroundL8->description.width,
// 		       pImpl->pixelDiffState->backgroundL8->description.height,
// 		       CV_8UC1);//used to calc background and differences
//     aux3 = cvCreateMat(difference->width/pImpl->pixelDiffState->xStep,
// 		       difference->height/pImpl->pixelDiffState->yStep,
// 		       CV_8UC1);/*will be a resized version of difference
// 				  that will be used to fill the motion grid*/
    
//     //update background
//     if (pImpl->pixelDiffState->backgroundCount > 10){
//       cvSetData(aux1,pImpl->previousImageL8->imageData,CV_AUTOSTEP);//last frame
//       cvAddWeighted(background,0.5,aux1,0.5,0.0,aux2);
// //       cvCopy(aux2,background);
//       pImpl->pixelDiffState->backgroundCount = 0;
//     }else
//       pImpl->pixelDiffState->backgroundCount++;

//     //calculate differences
//     cvSetData(aux1,pImpl->imageL8->imageData,CV_AUTOSTEP);
//     cvAbsDiff(background,aux1,aux2);
//     cvCopy(aux2,difference);

//     //translate differences to motionGrid
//     cvResize(difference,aux3);

//     pImpl->motionGrid.reset(new MotionGrid2D(pImpl->motionGridRows,pImpl->motionGridCols));
//     pImpl->maxMotionDetected = MotionGridItem2D();/*reset*/

//     float xScale = (float)aux3->width/(float)pImpl->motionGridCols;
//     float yScale = (float)aux3->height/(float)pImpl->motionGridRows;

//     int i,j,k;
//     for (i = 0,k = 0; i<aux3->height; i++){
//       for (j = 0; j<aux3->width; j++,k++){
// 	if (aux3->imageData[k] > pImpl->pixelDiffState->threshold){
// 	  int x,y,t;
// 	  x = (int) truncf(j/xScale);x=std::min(x,pImpl->motionGridCols-1);
// 	  y = (int) truncf(i/yScale);y=std::min(y,pImpl->motionGridRows-1);
// 	  t = x+(y*pImpl->motionGridCols);/*offset inside the grid*/
	  
// 	  pImpl->motionGrid->grid[t].motion = pImpl->motionThreshold;/*with pixel diff we can't calculate motion value, just use the minimum value*/
// 	  pImpl->motionGrid->grid[t].count++;
// 	  if (pImpl->motionGrid->grid[t]>pImpl->maxMotionDetected){
// 	    pImpl->maxMotionDetectedArea = cvRect(x,y,xScale,yScale);
// 	    pImpl->maxMotionDetected = pImpl->motionGrid->grid[t];
// 	  }
// 	}
//       }
//     }
//     cvReleaseMat(&background);
//     cvReleaseMat(&difference);
//     cvReleaseMat(&aux1);
//     cvReleaseMat(&aux2);
//     cvReleaseMat(&aux3);
//   }
  }
}//namespace

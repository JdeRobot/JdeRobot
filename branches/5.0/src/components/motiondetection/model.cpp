/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
#include <colorspaces/formats.h>
#include <cmath>
#include <algorithm>
#include <tr1/memory>

namespace motiondetection {
  class Model::PImpl {
  public:
    PImpl()
      : algorithm(Model::PixelDifference),optFlowState(),
	pixelDiffState(),image(),imageL8(),previousImageL8(),
	motionThreshold(10),motionGridRows(3),motionGridCols(3),motionGrid(),
	maxMotionDetectedArea(cvRect(0,0,0,0)),maxMotionDetected(0,0) {}
    MotionDetectionAlgorithm algorithm;
    OpticalFlowStatePtr optFlowState;
    PixelDifferenceStatePtr pixelDiffState;
    colorspaces::ImageppPtr image;
    colorspaces::ImageppPtr imageL8;
    colorspaces::ImageppPtr previousImageL8;
    int motionThreshold;
    int motionGridRows;
    int motionGridCols;
    Model::MotionGrid2DPtr motionGrid;
    CvRect maxMotionDetectedArea;
    Model::MotionGridItem2D maxMotionDetected;
  };

  Model::Model() throw()
    : pImpl(new PImpl()) {}
  
  const colorspaces::ImageppPtr Model::getImage() const throw(){ 
    return pImpl->image;
  }

  void Model::setImage(const colorspaces::ImageppPtr img) throw(gbxutilacfr::Exception){
    if (img->description.fmt->format == FORMAT_UNKNOWN)
      throw gbxutilacfr::Exception(ERROR_INFO, "Unkwnown image format");


    /*keep img reference and get grayscale*/
    pImpl->image = img;
    pImpl->imageL8 = img->toL8();

    /*motion grid can only be computed if image can be converted to L8 and we have previousImageL8*/
    if (pImpl->imageL8 && pImpl->previousImageL8){
      if (pImpl->algorithm == OpticalFlow)
	calcMotionWithOpticalFlow();
      else
	calcMotionWithPixelDifference();
    }else
      pImpl->motionGrid.reset();//no motionGrid for this iteration

    /*save image for next iteration*/
    pImpl->previousImageL8 = pImpl->imageL8;
    notifyObservers();
  }

  void Model::setMotionDetectionAlgorithm(const MotionDetectionAlgorithm a) throw(){
    pImpl->algorithm = a;
    if (pImpl->algorithm == OpticalFlow)
      pImpl->pixelDiffState.reset();
    else
      pImpl->optFlowState.reset();
    notifyObservers();
  }

  Model::MotionDetectionAlgorithm Model::getMotionDetectionAlgorithm() const throw(){
    return pImpl->algorithm;
  }

  void Model::setMotionGridSize(const int rows, const int cols) throw() {
    pImpl->motionGridRows = rows;
    pImpl->motionGridCols = cols;
    pImpl->motionGrid.reset();
  }

  void Model::getMotionGridSize(int *rows, int *cols) const throw(){
    *rows = pImpl->motionGridRows;
    *cols = pImpl->motionGridCols;
  }

  void Model::setMotionThreshold(const int t) throw() { pImpl->motionThreshold = t; notifyObservers(); }
  int Model::getMotionThreshold() const throw() { return pImpl->motionThreshold; }

  void Model::setOpticalFlowNPoints(const int n) throw() {
    if (pImpl->optFlowState.get()){//algorithm is optFlow
      pImpl->optFlowState.reset(new OpticalFlowState(n));
      notifyObservers();
    }
  }

  int Model::getOpticalFlowNPoints() const throw() { 
    if (pImpl->optFlowState.get())
      return pImpl->optFlowState->nPoints; 
    return 0;
  }

  const Model::OpticalFlowStatePtr Model::getOpticalFlowState() const throw(){
    return pImpl->optFlowState;
  }

  void Model::setPixelDifferenceThreshold(const int threshold) throw(){
    if (pImpl->pixelDiffState.get()){
      pImpl->pixelDiffState->threshold = threshold;
      notifyObservers();
    }
  }
      
  int Model::getPixelDifferenceThreshold() const throw(){
    if (pImpl->pixelDiffState.get())
      return pImpl->pixelDiffState->threshold;
    return 0;
  }

  void Model::setPixelDifferenceXStep(const int xStep) throw(){
    if (pImpl->pixelDiffState.get()){
      pImpl->pixelDiffState->xStep = xStep;
      notifyObservers();
    }
  }

  int Model::getPixelDifferenceXStep() const throw(){
    if (pImpl->pixelDiffState.get())
      return pImpl->pixelDiffState->xStep;
    return 0;
  }

  void Model::setPixelDifferenceYStep(const int yStep) throw(){
    if (pImpl->pixelDiffState.get()){
      pImpl->pixelDiffState->yStep = yStep;
      notifyObservers();
    }
  }

  int Model::getPixelDifferenceYStep() const throw(){
    if (pImpl->pixelDiffState.get())
      return pImpl->pixelDiffState->yStep;
    return 0;
  }

  const Model::PixelDifferenceStatePtr Model::getPixelDifferenceState() const throw(){
    return pImpl->pixelDiffState;
  }

  const Model::MotionGrid2DPtr Model::getMotionGrid() const throw(){
    return pImpl->motionGrid;
  }
  
  bool Model::isMotionDetected(CvRect *area, MotionGridItem2D *value) const throw(){
    if (area != 0)
      *area = pImpl->maxMotionDetectedArea;
    if (value != 0)
      *value = pImpl->maxMotionDetected;
    return (pImpl->maxMotionDetected.count > 5) && (pImpl->maxMotionDetected.motion > pImpl->motionThreshold);
  }
  

  void Model::calcMotionWithOpticalFlow(){
    if (!pImpl->optFlowState)//first iteration
      pImpl->optFlowState.reset(new OpticalFlowState());

    CvPoint2D32f *currentPoints = &pImpl->optFlowState->currentPoints[0];
    CvPoint2D32f *previousPoints = &pImpl->optFlowState->previousPoints[0];
    char *status = &pImpl->optFlowState->status[0];
    colorspaces::ImageppPtr currentImageL8 = pImpl->imageL8;
    colorspaces::ImageppPtr previousImageL8 = pImpl->previousImageL8;
    IplImage *img_8u1,*prev_8u1,*aux1,*aux2;
    
    
    CvTermCriteria termCriteria;
    CvSize s = cvSize(currentImageL8->description.width,
		      currentImageL8->description.height);
    
    
    /* images used to calculate featured points */
    img_8u1 = cvCreateImageHeader(s,IPL_DEPTH_8U, 1);
    prev_8u1 = cvCreateImageHeader(s,IPL_DEPTH_8U, 1);
    img_8u1->imageData = img_8u1->imageDataOrigin = currentImageL8->imageData;
    prev_8u1->imageData = prev_8u1->imageDataOrigin = previousImageL8->imageData;
    
    termCriteria = cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );
    
    
    /*fill previousPoints with good points to track*/
    int pointsToFill = pImpl->optFlowState->nPoints - pImpl->optFlowState->nPreviousPoints;
    if (pointsToFill > 10){/*avoid to recalc if only one point has been lost*/
      printf("filling with %d points\n",pointsToFill);
      
      /*Temp images for feature points calcs*/
      aux1 = cvCreateImage(s, IPL_DEPTH_32F, 1);
      aux2 = cvCreateImage(s, IPL_DEPTH_32F, 1);
      
      /* Shi and Tomasi algorithm, get feature points from prev */       
      cvGoodFeaturesToTrack(prev_8u1, aux1, aux2,
			    previousPoints + pImpl->optFlowState->nPreviousPoints, 
			    &pointsToFill, 0.05, 5.0, NULL, 3, 0, 0.04);
      cvReleaseImage(&aux1);
      cvReleaseImage(&aux2);
      /*we add filled point count*/
      pImpl->optFlowState->nPreviousPoints += pointsToFill;
    }
    assert(pImpl->optFlowState->nPreviousPoints <= pImpl->optFlowState->nPoints && 
	   "nPreviousPoints can't be bigger than nPoints");
    
    /* Pyramidal Lucas Kanade Optical Flow algorithm, search feature points in img */
    cvCalcOpticalFlowPyrLK(prev_8u1, img_8u1, 0, 0, 
			   previousPoints, currentPoints, 
			   pImpl->optFlowState->nPreviousPoints, 
			   cvSize(5,5), 5, status, 0, termCriteria, 0);
    cvReleaseImageHeader(&img_8u1);
    cvReleaseImageHeader(&prev_8u1);
    
    int j = 0;/*keeps count of next previous points*/
    pImpl->motionGrid.reset(new MotionGrid2D(pImpl->motionGridRows,pImpl->motionGridCols));
    pImpl->maxMotionDetected = MotionGridItem2D();/*reset*/

    float xScale = (float)currentImageL8->description.width/(float)pImpl->motionGridCols;
    float yScale = (float)currentImageL8->description.height/(float)pImpl->motionGridRows;
    for(int i = 0; i < pImpl->optFlowState->nPreviousPoints; i++){
      if (status[i] == 0)
	continue;
      
      
      double norm = sqrt(pow(((double)previousPoints[i].y) - ((double)currentPoints[i].y),2) + 
			 pow(((double)previousPoints[i].x) - ((double)currentPoints[i].x),2));/*norm*/
      
      previousPoints[j++] = currentPoints[i];
      
      if (norm < (double)pImpl->motionThreshold)
	continue;
      
      int x,y,k;
      x = (int) truncf(previousPoints[i].x/xScale);x=std::min(x,pImpl->motionGridCols-1);
      y = (int) truncf(previousPoints[i].y/yScale);y=std::min(y,pImpl->motionGridRows-1);
      k = x+(y*pImpl->motionGridCols);/*offset inside the grid*/
      if (pImpl->motionGrid->grid[k].motion == 0)
	pImpl->motionGrid->grid[k].motion = (int)norm;
      else
	pImpl->motionGrid->grid[k].motion = 
	  ((int)norm + pImpl->motionGrid->grid[k].motion) >> 2;
      pImpl->motionGrid->grid[k].count++;
      if (pImpl->motionGrid->grid[k]>pImpl->maxMotionDetected){
	pImpl->maxMotionDetectedArea = cvRect(x,y,xScale,yScale);
	pImpl->maxMotionDetected = pImpl->motionGrid->grid[k];
      }
    }
    pImpl->optFlowState->nPreviousPoints = j;
  }

  void Model::calcMotionWithPixelDifference(){
    if (!pImpl->pixelDiffState)//first iteration
      pImpl->pixelDiffState.reset(new PixelDifferenceState(pImpl->previousImageL8));

    CvMat *background,*difference,*aux1,*aux2,*aux3;
    background = cvCreateMatHeader(pImpl->pixelDiffState->backgroundL8->description.width,
				   pImpl->pixelDiffState->backgroundL8->description.height,
				   CV_8UC1);
    cvSetData(background,pImpl->pixelDiffState->backgroundL8->imageData,CV_AUTOSTEP);
    difference = cvCreateMatHeader(pImpl->pixelDiffState->differenceL8->description.width,
				   pImpl->pixelDiffState->differenceL8->description.height,
				   CV_8UC1);
    cvSetData(difference,pImpl->pixelDiffState->differenceL8->imageData,CV_AUTOSTEP);
    aux1 = cvCreateMatHeader(pImpl->previousImageL8->description.width,
			     pImpl->previousImageL8->description.height,
			     CV_8UC1);
    aux2 = cvCreateMat(pImpl->pixelDiffState->backgroundL8->description.width,
		       pImpl->pixelDiffState->backgroundL8->description.height,
		       CV_8UC1);//used to calc background and differences
    aux3 = cvCreateMat(difference->width/pImpl->pixelDiffState->xStep,
		       difference->height/pImpl->pixelDiffState->yStep,
		       CV_8UC1);/*will be a resized version of difference
				  that will be used to fill the motion grid*/
    
    //update background
    if (pImpl->pixelDiffState->backgroundCount > 10){
      cvSetData(aux1,pImpl->previousImageL8->imageData,CV_AUTOSTEP);//last frame
      cvAddWeighted(background,0.5,aux1,0.5,0.0,aux2);
//       cvCopy(aux2,background);
      pImpl->pixelDiffState->backgroundCount = 0;
    }else
      pImpl->pixelDiffState->backgroundCount++;

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
    cvReleaseMat(&background);
    cvReleaseMat(&difference);
    cvReleaseMat(&aux1);
    cvReleaseMat(&aux2);
    cvReleaseMat(&aux3);
  }

}//namespace

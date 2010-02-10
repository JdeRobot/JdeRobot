#include "model.h"
#include <formats.h>
#include <cmath>

namespace opencvdemo {
  Model::Model()
    :previousImage(0),image(0)
  
  jderobot::ImageDataPtr Model::getImage() {
    jderobot::ImageDataPtr img; 
    return image;
  }

  void Model::setImage(jderobot::ImageDataPtr img){
    const Format *img_fmt = searchPixelFormat(img->description->format.c_str());
    
    if (fmt==0)
      throw BadData( ERROR_INFO, "Unkwnown image format");
    
    /*on first call image and previousImage are the same*/
    if (previousImage == 0)
      previousImage = img;
    else{
      const Format *previousImage_fmt = 
	searchPixelFormat(previousImage->description->format.c_str());

      /*If different formats, reset previous*/
      if (*img_fmt != *previousImage_fmt)
	previousImage = img;
    }
    image = img;
    notifyObservers();
  }

  Model::OptFlowSeq Model::getOptFlow(){
    jderobot::ImageDataPtr img,prev;
    IplImage *img_8u1, *prev_8u1, *aux1, *aux2;
    CvSize s;
    CvPoint2D32f img_points[nPoints];
    CvPoint2D32f prev_points[nPoints];
    char status[nPoints];                     
    float errors[nPoints];
    //CvSize sizeWindow = cvSize(5,5);
    CvTermCriteria termCriteria;

    if (image==0)
      return OptFlowSeq::vector(0);/*empty vector*/

    /*get the pointers from the store*/
    image.get(img);
    previousImage.get(prev);

    s = cvSize(img->description->width,img->description->height);

    /* images with feature points */
    img_8u1 = cvCreateImage(s,IPL_DEPTH_8U, 1);
    prev_8u1 = cvCreateImage(s,IPL_DEPTH_8U, 1);
    
    /*convert 3-byte depth to 1*/
    int npixels = s.width*s.height;
    for (int i = 0; i < npixels; i++)
      img_8u1->imageData[i] = img->pixelData[i*2];
    
    for (int i = 0; i < npixels; i++)
      prev_8u1->imageData[i] = prev->pixelData[i*2];
    
    /*Temp images for feature points calc*/
    aux1 = cvCreateImage(s, IPL_DEPTH_32F, 1);
    aux2 = cvCreateImage(s, IPL_DEPTH_32F, 1);

    termCriteria = cvTermCriteria( CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3 );

    /* Shi and Tomasi algorithm, get feature points from prev */       
    cvGoodFeaturesToTrack(prev_8u1, aux1, aux2, prev_points, 
			  &nPoints, 0.05, 0.1, NULL, 3, 0, 0.04);
    cvReleaseImage(&aux1);
    cvReleaseImage(&aux2);

    /* Pyramidal Lucas Kanade Optical Flow algorithm, search feature points in img */
    cvCalcOpticalFlowPyrLK(prev_8u1, img_8u1, 0, 0, 
			   prev_points, img_points, nPoints, 
			   cvSize(5,5), 5, status, errors, termCriteria, 0);
    
    OptFlowSeq fs(nPoints);
    int j = 0;/*number of points returned*/
    for(int i = 0; i < nPoints; i++){
      if (status[i] == 0)
	continue;
      CvPoint p = {(int)img_points[i].x,(int)img_points[i].y};
      CvPoint q = {(int)prev_points[i].x,(int)prev_points[i].y};
      double flow = sqrt(pow(p.y - q.y,2) + pow(p.x - q.x,2));/*modulo*/
      
      if (flow < threshold)
	continue;

      fs[j].p =  p;
      fs[j++].flow = flow;
    }
    fs.resize(j);
    return fs;
  }

}//namespace

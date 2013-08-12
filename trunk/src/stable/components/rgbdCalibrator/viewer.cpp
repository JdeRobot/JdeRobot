/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  Authors : Roberto Calvo <rocapal [at] gsyc [dot] urjc [dot] es>
 *
 */

#include "viewer.h" 
#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <boost/filesystem.hpp>
#include "calibration.h"

using namespace boost::filesystem; 
using namespace cv;

namespace rgbdCalibrator{
  
  
  const std::string gladepath = std::string(GLADE_DIR) + 
    std::string("/rgbdCalibrator.glade");

  const std::string pathImage = "./images/";


  Viewer::Viewer() 
    : gtkmain(0,0),frameCount(0),
      intrinsicsEnable(0),contPhoto(1) {

    std::cout << "Loading glade\n";

    // ref widgets
    refXml = Gnome::Glade::Xml::create(gladepath);
    refXml->get_widget("color_image", gtkimage_color);
    refXml->get_widget("depth_image", gtkimage_depth);
    refXml->get_widget("mainwindow",mainwindow);
    refXml->get_widget("fpslabel",fpsLabel);
    refXml->get_widget("bt_take_photo", btTakePhoto);
    refXml->get_widget("et_sleep_photo", etSleepPhoto);
    refXml->get_widget("et_num_photo", etNumPhoto);
    refXml->get_widget("tv_status", tvStatus);
    refXml->get_widget("bt_intrinsic_calib", btIntrinsic);

    // connect signals
    btTakePhoto->signal_clicked().connect(sigc::mem_fun(this,&Viewer::on_bt_take_photo_clicked));

    btIntrinsic->signal_clicked().connect(sigc::mem_fun(this,&Viewer::on_bt_intrinsic));
        

    // start the timer for calculating the number of frames per second
    // the images are being displayed at
    oldFrameTime = IceUtil::Time::now();

   
  }
    

  Viewer::~Viewer() {}

  bool Viewer::isVisible(){
    return mainwindow->is_visible();
  }

  void Viewer::display( const colorspaces::Image& imageColor, const colorspaces::Image& imageDepth )
  {
    colorspaces::ImageRGB8 img_rgb8(imageColor);//conversion will happen if needed
    Glib::RefPtr<Gdk::Pixbuf> imgBuffColor = 
      Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8.width,
				    img_rgb8.height,
				    img_rgb8.step);
    
    gtkimage_color->clear();
    gtkimage_color->set(imgBuffColor);

    if (intrinsicsEnable)
      saveImage(imageColor);
    

    colorspaces::ImageRGB8 img_rgb8D(imageDepth);
    Glib::RefPtr<Gdk::Pixbuf> imgBuffDepth = 
      Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8D.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8D.width,
				    img_rgb8D.height,
				    img_rgb8D.step);
    
    gtkimage_depth->clear();
    gtkimage_depth->set(imgBuffDepth);
    


    displayFrameRate();
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }
    
  void Viewer::setDepth(const jderobot::ImageDataPtr depth)
  {
    dataDepth = depth;
  }

  void
  Viewer::displayFrameRate()
  {
    double diff;
    IceUtil::Time diffT;

    currentFrameTime = IceUtil::Time::now();
    diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
    if (diff < 1000.0)
      frameCount++;
    else{
      oldFrameTime = currentFrameTime;
      fps = frameCount*1000.0/diff;
      frameCount=0;
      // Display the frame rate
      std::stringstream fpsString;
      fpsString << "fps = " << int(fps);
      fpsLabel->set_label(fpsString.str());
    }
  }

  void Viewer::on_bt_take_photo_clicked() 
  {
    intrinsicsEnable = 1;

    // Get num of photos
    std::string num (etNumPhoto->get_buffer()->get_text());
    numPhoto = atoi(num.c_str());
    std::cout << numPhoto << std::endl;

    // Get delay photo and Init time last photo
    std::string sleep (etSleepPhoto->get_buffer()->get_text());
    delayPhoto = atoi(sleep.c_str());
    lastTimePhoto = IceUtil::Time::now();

    tvStatus->get_buffer()->set_text("Init process, show the pattern!");
  }

  void Viewer::saveImage(const colorspaces::Image& imageColor)
  {
    

    IceUtil::Time currentTime = IceUtil::Time::now();    
    if ((currentTime - lastTimePhoto).toSeconds() == delayPhoto)
    {

      // Check the directory
      if ( !exists(pathImage)) 
      {
	path dir(pathImage);
	if (!boost::filesystem::create_directory(dir))
	  std::cout << "Error to create directory" << std::endl;
      }


      lastTimePhoto = IceUtil::Time::now();

      // Convert to gray
      Mat gray(imageColor.size(), CV_8UC1);
      cvtColor(imageColor, gray, CV_RGB2GRAY);

      // Save Image
      std::stringstream filename;
      filename << pathImage << "img" << contPhoto << ".jpg";      
      imwrite( filename.str().c_str(), gray );

      std::string msg = "Image saved: " + filename.str();
      tvStatus->get_buffer()->set_text(msg.c_str());
      
      beep();

      if (contPhoto < numPhoto)
	contPhoto ++;
      else
	{
	  intrinsicsEnable = 0;
	  contPhoto = 1;
	}	
    }

  }

  void Viewer::beep()
  {
    system("mplayer beep.wav &> /dev/null");
  }
  
  void Viewer::on_bt_intrinsic() 
  {


    tvStatus->get_buffer()->set_text("Getting intrinsics ...");

    Size boardSize, imageSize; 
    Mat cameraMatrix, distCoeffs;
    vector<vector<Point2f> > imagePoints;


    // TODO: get this data from user by GUI
    boardSize.width = 9;
    boardSize.height= 6;

    //flag
    int flag = 0;
    flag |= CV_CALIB_FIX_PRINCIPAL_POINT;
    flag |= CV_CALIB_ZERO_TANGENT_DIST;
    flag |= CV_CALIB_FIX_ASPECT_RATIO;

    // List of images
    directory_iterator end_itr; 
    for ( directory_iterator itr( pathImage ); itr != end_itr; ++itr )
    {

      Mat view;
      view = imread(itr->path().c_str(), CV_LOAD_IMAGE_COLOR);

      imageSize = view.size();

      vector<Point2f> pointBuf;
      bool found = findChessboardCorners( view, boardSize, pointBuf, 
					  CV_CALIB_CB_ADAPTIVE_THRESH | 
					  CV_CALIB_CB_FAST_CHECK | 
					  CV_CALIB_CB_NORMALIZE_IMAGE);

      if (found)
      {
	
	Mat viewGray;
	cvtColor(view, viewGray, CV_BGR2GRAY);
	cornerSubPix( viewGray, pointBuf, Size(11,11),
		      Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));

	imagePoints.push_back(pointBuf);

	// Draw the corners.
	drawChessboardCorners( view, boardSize, Mat(pointBuf), found );
	//imshow("Image View", view);
	
      }
    }

    Calibration* calib = new Calibration();
    calib->runCalibrationAndSave(boardSize, 20.0, flag, imageSize,  
				 cameraMatrix, distCoeffs, imagePoints);

    std::cout << std::endl << cameraMatrix << std::endl;

    std::stringstream matrixStr;

    matrixStr << "Intrinsic Matrix" << std::endl;
    matrixStr << "================" << std::endl << std::endl;

    for (int i=0; i<cameraMatrix.cols ; i++)
    {
      for (int j=0; j<cameraMatrix.rows; j++)
      {
	matrixStr << cameraMatrix.at<double>(i,j) << "\t\t";
	if (i==2 && j==0)
	  matrixStr << "\t";
      }
      matrixStr<<std::endl;

      tvStatus->get_buffer()->set_text(matrixStr.str().c_str());
    }

    calib->extrinsics(cameraMatrix, dataDepth);

  }


}//namespace

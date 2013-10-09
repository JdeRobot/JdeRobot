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
#include <visionlib/cvBlob/cvblob.h>

#define DEGTORAD     (3.14159264 / 180.0)
#define DEBUG TRUE

using namespace cv;
using namespace cvb;

namespace rgbdCalibrator{


const std::string gladepath = std::string(GLADE_DIR) +
		std::string("/rgbdCalibrator.glade");

const std::string pathImage = "./images2/";


Viewer::Viewer()
: gtkmain(0,0),frameCount(0),
  intrinsicsEnable(0),contPhoto(1),hsvFilter(NULL), mFrameBlob(NULL) {

	std::cout << "Loading glade\n";

	// ref widgets
	refXml = Gnome::Glade::Xml::create(gladepath);
	refXml->get_widget("color_image", gtkimage_color);
	refXml->get_widget("color_image2", gtkimage_color2);
	refXml->get_widget("depth_image", gtkimage_depth);
	refXml->get_widget("hsv_image", gtkimage_hsv);
	refXml->get_widget("blob_image", gtkimage_blob);
	refXml->get_widget("mainwindow",mainwindow);
	refXml->get_widget("fpslabel",fpsLabel);
	refXml->get_widget("bt_take_photo", btTakePhoto);
	refXml->get_widget("et_sleep_photo", etSleepPhoto);
	refXml->get_widget("et_num_photo", etNumPhoto);
	refXml->get_widget("tv_status", tvStatus);
	refXml->get_widget("bt_intrinsic_calib", btIntrinsic);
	refXml->get_widget("eventbox", ebImage);
	refXml->get_widget("eb_extrinsics", ebImageExtrinsics);

	// connect signals
	btTakePhoto->signal_clicked().connect(sigc::mem_fun(this,&Viewer::on_bt_take_photo_clicked));

	btIntrinsic->signal_clicked().connect(sigc::mem_fun(this,&Viewer::on_bt_intrinsic));
	ebImage->signal_button_press_event().connect(sigc::mem_fun(this, &Viewer::on_eventbox_clicked));

	ebImageExtrinsics->signal_button_press_event().connect(sigc::mem_fun(this, &Viewer::on_eventbox_extrinsics_clicked));

	// start the timer for calculating the number of frames per second
	// the images are being displayed at
	oldFrameTime = IceUtil::Time::now();

	RGB2HSV_init();
	RGB2HSV_createTable();

	mCalibration = new Calibration();

	pthread_mutex_init(&mutex, NULL);


}


Viewer::~Viewer()
{
	if(mCalibration)
		delete(mCalibration);

	RGB2HSV_destroyTable();
}

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

	gtkimage_color2->clear();
	gtkimage_color2->set(imgBuffColor);

	if (intrinsicsEnable)
		saveImage(imageColor);


	imgOrig.create(imageColor.size(), CV_8UC3);
	imageColor.copyTo(imgOrig);

	pthread_mutex_lock(&mutex);
	mImageDepth = imageDepth.clone();
	pthread_mutex_unlock(&mutex);

	// Show depth image in color
	// 3 RGB canals
	// 0: Image in gray scale
	// [1,2]: Real data of distance
	// 1: 8bit MSB
	// 2: 8bit LSB

	std::vector<cv::Mat> layers;
	cv::Mat colorDepth(imageDepth.size(),imageDepth.type());
	cv::split(imageDepth, layers);

	cv::cvtColor(layers[0],colorDepth,CV_GRAY2RGB);

	Glib::RefPtr<Gdk::Pixbuf> imgBuffDepth =
			Gdk::Pixbuf::create_from_data((const guint8*) colorDepth.data,
					Gdk::COLORSPACE_RGB,
					false,
					8,
					imageDepth.width,
					imageDepth.height,
					imageDepth.step);

	gtkimage_depth->clear();
	gtkimage_depth->set(imgBuffDepth);

	if (hsvFilter != NULL)
	{
		createImageHSV(imageDepth);

		// Show HSV image
		Glib::RefPtr<Gdk::Pixbuf> imgBuffHSV =
				Gdk::Pixbuf::create_from_data((const guint8*)imgHSV.data,
						Gdk::COLORSPACE_RGB,
						false,
						8,
						imgHSV.size().width,
						imgHSV.size().height,
						imgHSV.step);

		gtkimage_hsv->clear();
		gtkimage_hsv->set(imgBuffHSV);

		// Show Blob Image

		Glib::RefPtr<Gdk::Pixbuf> imgBuffBLOB =
				Gdk::Pixbuf::create_from_data(
						(guint8*)mFrameBlob->imageData,
						Gdk::COLORSPACE_RGB,
						false,
						mFrameBlob->depth,
						mFrameBlob->width,
						mFrameBlob->height,
						mFrameBlob->widthStep);

		gtkimage_blob->clear();
		gtkimage_blob->set(imgBuffBLOB);

		//cvReleaseImage(&mFrameBlob);

	}

	displayFrameRate();
	mainwindow->resize(1,1);
	while (gtkmain.events_pending())
		gtkmain.iteration();
}

void Viewer::createImageHSV(const colorspaces::Image& imageDepth)
{
	float r,g,b;

	imgHSV.create(imgOrig.size(), CV_8UC1);
	imgOrig.copyTo(imgHSV);

	IplImage *threshy=cvCreateImage(imgOrig.size(),8,1);

	for (int i=0;i< imgHSV.size().width*imgHSV.size().height; i++)
	{
		r = (float)(unsigned int)(unsigned char) imgOrig.data[i*3];
		g = (float)(unsigned int)(unsigned char) imgOrig.data[i*3+1];
		b = (float)(unsigned int)(unsigned char) imgOrig.data[i*3+2];

		const HSV* hsvData =  RGB2HSV_getHSV (r,g,b);

		if( hmax >= hsvData->H*DEGTORAD && hmin <= hsvData->H*DEGTORAD
				&& smax >= hsvData->S && smin <= hsvData->S
				&& vmax >= hsvData->V && vmin <=  hsvData->V )
		{
			threshy->imageData[i] = 1;
		}
		else
		{
			imgHSV.data[i*3] = imgHSV.data[i*3+1] = imgHSV.data[i*3+2] = 0;
			threshy->imageData[i] = 0;
		}


	}

	//Structure to hold blobs
	CvBlobs blobs;

	IplImage *iplOrig = new IplImage(imgOrig);

	if (mFrameBlob)
		cvReleaseImage(&mFrameBlob);
	mFrameBlob=cvCreateImage(imgOrig.size(),8,3);

	IplImage *labelImg=cvCreateImage(imgOrig.size(),IPL_DEPTH_LABEL,1);

	cvResize(iplOrig,mFrameBlob,CV_INTER_LINEAR );

	//Threshy is a binary image
	cvSmooth(threshy,threshy,CV_MEDIAN,7,7);

	//Finding the blobs
	unsigned int result=cvLabel(threshy,labelImg,blobs);

	//Rendering the blobs
	cvRenderBlobs(labelImg,blobs,mFrameBlob,mFrameBlob);

	//Filter Blobs
	cvFilterByArea(blobs,500,5000);

	double area = 0.0;
	int x=0;
	int y=0;

	for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
	{
		//std::cout << "BLOB found: " << it->second->area  <<std::endl;

		double moment10 = it->second->m10;
		double moment01 = it->second->m01;

		if (it->second->area >= area)
		{
			area = it->second->area;
			x = moment10/area;
			y = moment01/area;
		}

	}

	std::cout << "Max BLOB: " << area << ": " << x << " , " << y  <<std::endl;

	//cvShowImage("Live",mFrameBlob);

	if (area != 0)
	{
		Eigen::Vector3d pixel;
		pixel(0) = x;
		pixel(1) = y;
		pixel(2) = 1.0;

		Eigen::Vector4d target;

		mCalibration->BackProjectWithDepth(pixel, imageDepth, target);

	}

	// Release and free memory
	delete(iplOrig);
	cvReleaseImage(&threshy);
	cvReleaseImage(&labelImg);

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

bool Viewer::on_eventbox_extrinsics_clicked(GdkEventButton * event)
{

	if (mCalibration){

		pthread_mutex_lock(&mutex);
		bool res = mCalibration->addPatternPixel (Eigen::Vector3d ((int) event->x,
				(int) event->y,
				1.0),
				mImageDepth);

		if (res == false)
		{
			Eigen::Vector3d p2D ((int) event->x, (int) event->y, 1.0);
			Eigen::Vector4d p3D;
			mCalibration->BackProjectWithDepth (p2D, mImageDepth, p3D);
			mCalibration->test(p3D);
		}

		pthread_mutex_unlock(&mutex);
	}
}

bool Viewer::on_eventbox_clicked(GdkEventButton * event)
{
	int posX, posY;
	float r,g,b;
	posX = (int) event->x;
	posY = (int) event->y;

	pthread_mutex_lock(&mutex);

	int index = posY*imgOrig.step+posX*imgOrig.channels();
	r = (float)(unsigned int) (unsigned char)imgOrig.data[index];
	g = (float)(unsigned int) (unsigned char)imgOrig.data[index+1];
	b = (float)(unsigned int) (unsigned char)imgOrig.data[index+2];

	pthread_mutex_unlock(&mutex);


	if (DEBUG) std::cout << "[RGB] -> " << r << " " << g << " " << b << std::endl;
	hsvFilter = RGB2HSV_getHSV (r,g,b);
	if (DEBUG) std::cout << "[HSV] -> " << hsvFilter->H << " " << hsvFilter->S << " " << hsvFilter->V << std::endl;

	// Calculate HSV Min y Max
	hmax = hsvFilter->H*DEGTORAD + 0.2;
	hmin = hsvFilter->H*DEGTORAD - 0.2;
	if(hmax>6.28) hmax = 6.28;
	if(hmin<0.0)  hmin = 0.0;

	smax = hsvFilter->S + 0.1;
	smin = hsvFilter->S - 0.1;
	if(smax > 1.0)
		smax = 1.0;
	if(smin < 0.0)
		smin = 0.0;

	vmax = hsvFilter->V + 50.0;
	vmin = hsvFilter->V - 50.0;
	if(vmax > 255.0)
		vmax = 255.0;
	if(vmin < 0.0)
		vmin = 0.0;

	if (DEBUG)
		std::cout << "H[min,max] - S[min,max] - V[min,max]: " <<
		"[" << hmin << " " << hmax << "] " <<
		"[" << smin << " " << smax << "] " <<
		"[" << vmin << " " << vmax << "] " << std::endl;

	return true;
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
		if ( !boost::filesystem::exists(pathImage))
		{
			boost::filesystem::path dir(pathImage);
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


	if ( !boost::filesystem::exists(pathImage))
	{
		std::cerr << "[E] Images calibration directory doesn't exist: " << pathImage << std::endl;
		return;
	}

	// List of images
	boost::filesystem::directory_iterator end_itr;
	for ( boost::filesystem::directory_iterator itr( pathImage ); itr != end_itr; ++itr )
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


	mCalibration->runCalibrationAndSave(boardSize, 20.0, flag, imageSize,
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


}


}//namespace

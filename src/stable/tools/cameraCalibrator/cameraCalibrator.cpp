/*
 *  Copyright (C) 1997-2014 JDE Developers Team
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
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */





#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/pointcloud.h>
#include "pthread.h"
#include "parallelIce/cameraClient.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <boost/lexical_cast.hpp>
#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <geometry/progeo/Progeo.h>


enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };



static double computeReprojectionErrors(
        const std::vector<std::vector<cv::Point3f> >& objectPoints,
        const std::vector<std::vector<cv::Point2f> >& imagePoints,
        const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
        const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs,
        std::vector<float>& perViewErrors )
{
    std::vector<cv::Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); i++ )
    {
        projectPoints(cv::Mat(objectPoints[i]), rvecs[i], tvecs[i],
                      cameraMatrix, distCoeffs, imagePoints2);
        err = norm(cv::Mat(imagePoints[i]), cv::Mat(imagePoints2), CV_L2);
        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float)std::sqrt(err*err/n);
        totalErr += err*err;
        totalPoints += n;
    }

    return std::sqrt(totalErr/totalPoints);
}


static void calcChessboardCorners(cv::Size boardSize, float squareSize, std::vector<cv::Point3f>& corners, Pattern patternType = CHESSBOARD)
{
    corners.resize(0);

    switch(patternType)
    {
      case CHESSBOARD:
      case CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; i++ )
            for( int j = 0; j < boardSize.width; j++ )
                corners.push_back(cv::Point3f(float(j*squareSize),
                                          float(i*squareSize), 0));
        break;

      case ASYMMETRIC_CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; i++ )
            for( int j = 0; j < boardSize.width; j++ )
                corners.push_back(cv::Point3f(float((2*j + i % 2)*squareSize),
                                          float(i*squareSize), 0));
        break;

      default:
        CV_Error(CV_StsBadArg, "Unknown pattern type\n");
    }
}

static bool runCalibration( std::vector<std::vector<cv::Point2f> > imagePoints,
                    cv::Size imageSize, cv::Size boardSize, Pattern patternType,
                    float squareSize, float aspectRatio,
                    int flags, cv::Mat& cameraMatrix, cv::Mat& distCoeffs,
                    std::vector<cv::Mat>& rvecs, std::vector<cv::Mat>& tvecs,
                    std::vector<float>& reprojErrs,
                    double& totalAvgErr)
{
    cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
    if( flags & CV_CALIB_FIX_ASPECT_RATIO )
        cameraMatrix.at<double>(0,0) = aspectRatio;

    distCoeffs = cv::Mat::zeros(8, 1, CV_64F);

    std::vector<std::vector<cv::Point3f> > objectPoints(1);
    calcChessboardCorners(boardSize, squareSize, objectPoints[0], patternType);

    objectPoints.resize(imagePoints.size(),objectPoints[0]);

    double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
                    distCoeffs, rvecs, tvecs, flags|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
                    ///*|CV_CALIB_FIX_K3*/|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
    printf("RMS error reported by calibrateCamera: %g\n", rms);

    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

    totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
                rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

    return ok;
}


static void saveCameraParams( const std::string& filename,
                       cv::Size imageSize, cv::Size boardSize,
                       float squareSize, float aspectRatio, int flags,
                       const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs,
                       const std::vector<cv::Mat>& rvecs, const std::vector<cv::Mat>& tvecs,
                       const std::vector<float>& reprojErrs,
                       const std::vector<std::vector<cv::Point2f> >& imagePoints,
                       double totalAvgErr )
{
    cv::FileStorage fs( filename, cv::FileStorage::WRITE );

    time_t tt;
    time( &tt );
    struct tm *t2 = localtime( &tt );
    char buf[1024];
    strftime( buf, sizeof(buf)-1, "%c", t2 );

    fs << "calibration_time" << buf;

    if( !rvecs.empty() || !reprojErrs.empty() )
        fs << "nframes" << (int)std::max(rvecs.size(), reprojErrs.size());
    fs << "image_width" << imageSize.width;
    fs << "image_height" << imageSize.height;
    fs << "board_width" << boardSize.width;
    fs << "board_height" << boardSize.height;
    fs << "square_size" << squareSize;

    if( flags & CV_CALIB_FIX_ASPECT_RATIO )
        fs << "aspectRatio" << aspectRatio;

    if( flags != 0 )
    {
        sprintf( buf, "flags: %s%s%s%s",
            flags & CV_CALIB_USE_INTRINSIC_GUESS ? "+use_intrinsic_guess" : "",
            flags & CV_CALIB_FIX_ASPECT_RATIO ? "+fix_aspectRatio" : "",
            flags & CV_CALIB_FIX_PRINCIPAL_POINT ? "+fix_principal_point" : "",
            flags & CV_CALIB_ZERO_TANGENT_DIST ? "+zero_tangent_dist" : "" );
        cvWriteComment( *fs, buf, 0 );
    }

    fs << "flags" << flags;

    fs << "camera_matrix" << cameraMatrix;
    fs << "distortion_coefficients" << distCoeffs;

    fs << "avg_reprojection_error" << totalAvgErr;
    if( !reprojErrs.empty() )
        fs << "per_view_reprojection_errors" << cv::Mat(reprojErrs);

    if( !rvecs.empty() && !tvecs.empty() )
    {
        CV_Assert(rvecs[0].type() == tvecs[0].type());
        cv::Mat bigmat((int)rvecs.size(), 6, rvecs[0].type());
        for( int i = 0; i < (int)rvecs.size(); i++ )
        {
            cv::Mat r = bigmat(cv::Range(i, i+1), cv::Range(0,3));
            cv::Mat t = bigmat(cv::Range(i, i+1), cv::Range(3,6));

            CV_Assert(rvecs[i].rows == 3 && rvecs[i].cols == 1);
            CV_Assert(tvecs[i].rows == 3 && tvecs[i].cols == 1);
            //*.t() is MatExpr (not Mat) so we can use assignment operator
            r = rvecs[i].t();
            t = tvecs[i].t();
        }
        cvWriteComment( *fs, "a set of 6-tuples (rotation vector + translation vector) for each view", 0 );
        fs << "extrinsic_parameters" << bigmat;
    }

    if( !imagePoints.empty() )
    {
        cv::Mat imagePtMat((int)imagePoints.size(), (int)imagePoints[0].size(), CV_32FC2);
        for( int i = 0; i < (int)imagePoints.size(); i++ )
        {
            cv::Mat r = imagePtMat.row(i).reshape(2, imagePtMat.cols);
            cv::Mat imgpti(imagePoints[i]);
            imgpti.copyTo(r);
        }
        fs << "image_points" << imagePtMat;
    }
}


static bool runAndSave(const std::string& outputFilename,
                const std::vector<std::vector<cv::Point2f> >& imagePoints,
                cv::Size imageSize, cv::Size boardSize, Pattern patternType, float squareSize,
                float aspectRatio, int flags, cv::Mat& cameraMatrix,
                cv::Mat& distCoeffs, bool writeExtrinsics, bool writePoints )
{
    std::vector<cv::Mat> rvecs, tvecs;
    std::vector<float> reprojErrs;
    double totalAvgErr = 0;

    bool ok = runCalibration(imagePoints, imageSize, boardSize, patternType, squareSize,
                   aspectRatio, flags, cameraMatrix, distCoeffs,
                   rvecs, tvecs, reprojErrs, totalAvgErr);
    printf("%s. avg reprojection error = %.2f\n",
           ok ? "Calibration succeeded" : "Calibration failed",
           totalAvgErr);

    if( ok )
        saveCameraParams( outputFilename, imageSize,
                         boardSize, squareSize, aspectRatio,
                         flags, cameraMatrix, distCoeffs,
                         writeExtrinsics ? rvecs : std::vector<cv::Mat>(),
                         writeExtrinsics ? tvecs : std::vector<cv::Mat>(),
                         writeExtrinsics ? reprojErrs : std::vector<float>(),
                         writePoints ? imagePoints : std::vector<std::vector<cv::Point2f> >(),
                         totalAvgErr );
    return ok;
}


struct cameraData{
	jderobot::cameraClient* proxy;
	cv::Size imageSize;
    std::vector<std::vector<cv::Point2f> > imagePoints;
    std::vector<cv::Mat> images;
    cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
    std::string outputFilename;
    cv::Mat frame;
    cv::Mat raw_frame;

    //volatile
    std::vector<cv::Point2f> pointbuf;
	cv::Mat viewGray;
	bool found;
	bool calibrated;

};



int main(int argc, char** argv){

	int status,i;
	Ice::CommunicatorPtr ic;
	Ice::PropertiesPtr prop;
	cv::Size boardSize;

	Pattern pattern = CHESSBOARD;
	float squareSize;
	float aspectRatio;
	int nframes;
	bool writeExtrinsics = false;
	bool writePoints = false;
    int delay;
    clock_t prevTimestamp = 0;
    int mode = DETECTION;
    bool undistortImage = false;
    bool globalCalibrated=false;

    std::vector<cameraData> cameras;

	try{
		ic = Ice::initialize(argc,argv);
		prop = ic->getProperties();
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return 1;
	}
	catch (const char* msg) {
		std::cerr <<"Error :" << msg << std::endl;
		return 1;
	}

	std::string componentPrefix("cameraCalibrator");

	std::cout << "Ncameras: " << prop->getPropertyAsIntWithDefault(componentPrefix+".nCameras",1) << std::endl;

	for (int i=0; i< prop->getPropertyAsIntWithDefault(componentPrefix+".nCameras",1); i++){
		cameraData cam;
		std::stringstream ss;
		ss << componentPrefix << ".camera." <<i <<".";
		cam.outputFilename=prop->getProperty(ss.str()+"outFile");
		cam.proxy = new jderobot::cameraClient(ic,ss.str());
		if (cam.proxy != NULL){
			cam.proxy->start();
		}
		else{
			throw "rgbdViewer: failed to load RGB Camera";
		}
		cam.calibrated=false;
		cameras.push_back(cam);

	}



	//parsing calibration parameteres
	boardSize.height=prop->getPropertyAsIntWithDefault(componentPrefix+".pattern.height",6);
	boardSize.width=prop->getPropertyAsIntWithDefault(componentPrefix+".pattern.width",9);
	std::string patterType =prop->getProperty(componentPrefix+".pattern.type");
	if( !strcmp( patterType.c_str(), "circles" ) )
		pattern = CIRCLES_GRID;
	else if( !strcmp( patterType.c_str(), "acircles" ) )
		pattern = ASYMMETRIC_CIRCLES_GRID;
	else if( !strcmp( patterType.c_str(), "chessboard" ) )
		pattern = CHESSBOARD;
	else
		return fprintf( stderr, "Invalid pattern type: must be chessboard or circles\n" ), -1;

	squareSize=atof(prop->getProperty(componentPrefix+".pattern.size").c_str());
	aspectRatio=atof(prop->getProperty(componentPrefix+".pattern.ratio").c_str());
	nframes=prop->getPropertyAsIntWithDefault(componentPrefix+".frames",10);
	writePoints=boost::lexical_cast<bool>(prop->getProperty(componentPrefix+".writePoints"));
	writeExtrinsics=boost::lexical_cast<bool>(prop->getProperty(componentPrefix+".writeEstrinsics"));
	delay=prop->getPropertyAsIntWithDefault(componentPrefix+".delay",1000);



	//init the poling
	while (1){
		cv::Mat frame;
		for (std::vector<cameraData>::iterator it= cameras.begin(); it != cameras.end(); it++){
			while (it->frame.cols ==0){
				it->proxy->getImage(it->frame);
			}
			it->imageSize=it->frame.size();
		}
		break;
	}

	std::cout << "Poling init done" << std::endl;
	while (1){
		for (std::vector<cameraData>::iterator it= cameras.begin(); it != cameras.end(); it++){
			it->proxy->getImage(it->frame);
			it->frame.copyTo(it->raw_frame);
			it->pointbuf.resize(0);
			if (it->frame.channels() ==3)
			  cv::cvtColor(it->frame, it->viewGray, cv::COLOR_BGR2GRAY);
			else
			  it->frame.copyTo(it->viewGray);
			it->found=false;

			switch( pattern )
			{
				case CHESSBOARD:
					it->found = cv::findChessboardCorners( it->frame, boardSize, it->pointbuf,
					CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
					break;
				case CIRCLES_GRID:
					it->found = cv::findCirclesGrid( it->frame, boardSize, it->pointbuf );
					break;
				case ASYMMETRIC_CIRCLES_GRID:
					it->found = cv::findCirclesGrid( it->frame, boardSize, it->pointbuf, cv::CALIB_CB_ASYMMETRIC_GRID );
					break;
				default:
					return fprintf( stderr, "Unknown pattern type\n" ), -1;
			}

			if( pattern == CHESSBOARD && it->found)
				cv::cornerSubPix( it->viewGray, it->pointbuf, cv::Size(11,11), cv::Size(-1,-1), cv::TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
			if(it->found)
				cv::drawChessboardCorners( it->frame, boardSize, cv::Mat(it->pointbuf), it->found );
		}

		bool globalFound=true;
		for (std::vector<cameraData>::iterator it= cameras.begin(); it != cameras.end(); it++){
			globalFound=globalFound && it->found;
		}



        if( mode == CAPTURING && globalFound && (clock() - prevTimestamp > delay*1e-3*CLOCKS_PER_SEC) )
        {
    		for (std::vector<cameraData>::iterator it= cameras.begin(); it != cameras.end(); it++){
    			cv::Mat temp;
    			it->raw_frame.copyTo(temp);
    			it->images.push_back(temp);
                it->imagePoints.push_back(it->pointbuf);
    		}
            prevTimestamp = clock();
        }


		for (std::vector<cameraData>::iterator it= cameras.begin(); it != cameras.end(); it++){

			std::string msg =  mode == CAPTURING ? "100/100" : mode == CALIBRATED ? "Calibrated" : "Press 'g' to start";
			int baseLine = 0;
			cv::Size textSize = cv::getTextSize(msg, 1, 1, 1, &baseLine);

			cv::Point textOrigin(cameras[0].frame.cols - 2*textSize.width - 10, cameras[0].frame.rows - 2*baseLine - 10);


			if( mode == CAPTURING )
			{
				if(undistortImage)
					msg = cv::format( "%d/%d Undist", (int)it->imagePoints.size(), nframes );
				else
					msg = cv::format( "%d/%d", (int)it->imagePoints.size(), nframes );
			}

			cv::putText( it->frame, msg, textOrigin, 1, 1, mode != CALIBRATED ? cv::Scalar(0,0,255) : cv::Scalar(0,255,0));

			if( mode == CALIBRATED && undistortImage )
			{
				cv::Mat temp = it->frame.clone();
				undistort(temp, it->frame, it->cameraMatrix, it->distCoeffs);
			}
			std::stringstream ss;
			ss << "Image " << it-cameras.begin();


			cv::imshow(ss.str(),it->frame);
			int key=0xff & cv::waitKey(1);

			if( (key & 255) == 27 )
				break;

			if( key == 'u' && mode == CALIBRATED )
				undistortImage = !undistortImage;

			if(  key == 'g' )
			{
				mode = CAPTURING;
				it->imagePoints.clear();
			}

			if( !globalCalibrated && it->imagePoints.size() >= (unsigned)nframes )
				{
					if( runAndSave(it->outputFilename, it->imagePoints, it->imageSize,
							   boardSize, pattern, squareSize, aspectRatio,
							   0, it->cameraMatrix, it->distCoeffs,
							   writeExtrinsics, writePoints)){
						mode = CALIBRATED;
						it->calibrated=true;;
					}
					else
						mode = DETECTION;

					//save pictures
					int cameraID = std::distance(cameras.begin(),it);
					for ( std::vector<cv::Mat>::iterator itImage = it->images.begin(); itImage != it->images.end(); itImage++){
						std::stringstream ss;
						ss << "camera-" << cameraID << "-" << std::distance(it->images.begin(), itImage) << ".png";
						cv::imwrite(ss.str(),*itImage);
					}

				}


		}
		for (std::vector<cameraData>::iterator it= cameras.begin(); it != cameras.end(); it++){
			if (it == cameras.begin()){
				globalCalibrated=it->calibrated;
			}
			else
				globalCalibrated=globalCalibrated && it->calibrated;
		}



	}
}



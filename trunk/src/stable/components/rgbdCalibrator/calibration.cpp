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

#include <calibration.h>
#include <iostream> 
#include "../../libs/geometry/progeo/Progeo.h"
#include "../../libs/geometry/math/Point3D.h"
#include "../../libs/geometry/math/Segment3D.h"

using namespace cv;

namespace rgbdCalibrator{


  // This source code is a modification of camera_calibration.cpp
  // You can see in OpenCV samples: 
  //     - samples/cpp/tutorial_code/calib3d/camera_calibration/


  Calibration::Calibration()
  {}

  Calibration::~Calibration()
  {}

  // Intrinsics 
  bool Calibration::runCalibrationAndSave(Size &boardSize, 
					  float squareSize, 
					  int flag, 
					  Size imageSize,
					  Mat&  cameraMatrix, 
					  Mat& distCoeffs,
					  vector<vector<Point2f> > imagePoints )
  {
    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    double totalAvgErr = 0;
    
    bool ok = runCalibration (boardSize, squareSize, flag, imageSize, 
			      cameraMatrix, distCoeffs, imagePoints, 
			      rvecs, tvecs, reprojErrs, totalAvgErr);

    std::cout << (ok ? "Calibration succeeded" : "Calibration failed")
	 << ". avg re projection error = "  << totalAvgErr ;

    mKMatrix = Mat(cameraMatrix);


    return ok;
  }
 
  bool Calibration::runCalibration(Size &boardSize, 
				   float squareSize, 
				   int flag, 
				   Size& imageSize, 
				   Mat& cameraMatrix, 
				   Mat& distCoeffs,
				   vector<vector<Point2f> > imagePoints, 
				   vector<Mat>& rvecs, 
				   vector<Mat>& tvecs,
				   vector<float>& reprojErrs,  
				   double& totalAvgErr)
  {

    Pattern calibrationPattern = CHESSBOARD;

    cameraMatrix = Mat::eye(3, 3, CV_64F);
    if( flag & CV_CALIB_FIX_ASPECT_RATIO )
      cameraMatrix.at<double>(0,0) = 1.0;

    distCoeffs = Mat::zeros(8, 1, CV_64F);
    
    vector<vector<Point3f> > objectPoints(1);
    calcBoardCornerPositions(boardSize, squareSize, objectPoints[0], calibrationPattern);
    
    objectPoints.resize(imagePoints.size(),objectPoints[0]);
    
    //Find intrinsic and extrinsic camera parameters
    double rms = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix,
                                 distCoeffs, rvecs, tvecs, flag|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);
    
    std::cout << "Re-projection error reported by calibrateCamera: "<< rms << std::endl;
    
    bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);
    
    totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
					    rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);
    
    return ok;
}


  void Calibration::calcBoardCornerPositions(Size boardSize, 
					     float squareSize, 
					     vector<Point3f>& corners,  
					     Pattern patternType /*= Settings::CHESSBOARD*/)
  {
    corners.clear();
    
    switch(patternType)
      {
      case CHESSBOARD:
      case CIRCLES_GRID:
	for( int i = 0; i < boardSize.height; ++i )
	  for( int j = 0; j < boardSize.width; ++j )
	    corners.push_back(Point3f(float( j*squareSize ), float( i*squareSize ), 0));
	break;
	
      case ASYMMETRIC_CIRCLES_GRID:
	for( int i = 0; i < boardSize.height; i++ )
	  for( int j = 0; j < boardSize.width; j++ )
	    corners.push_back(Point3f(float((2*j + i % 2)*squareSize), float(i*squareSize), 0));
	break;
      default:
	break;
      }
  }



  double Calibration::computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints, 
						 const vector<vector<Point2f> >& imagePoints, 
						 const vector<Mat>& rvecs, 
						 const vector<Mat>& tvecs, 
						 const Mat& cameraMatrix, 
						 const Mat& distCoeffs, 
						 vector<float>& perViewErrors)
  {
    vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());
    
    for( i = 0; i < (int)objectPoints.size(); ++i )
      {
        projectPoints( Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
                       distCoeffs, imagePoints2);
        err = norm(Mat(imagePoints[i]), Mat(imagePoints2), CV_L2);
	
        int n = (int)objectPoints[i].size();
        perViewErrors[i] = (float) std::sqrt(err*err/n);
        totalErr        += err*err;
        totalPoints     += n;
      }
    
    return std::sqrt(totalErr/totalPoints);
  }

  void Calibration::getOpticalCenter (Eigen::Vector2d &center)
  {
    center(0) = mKMatrix.at<double>(0,2);
    center(1) = mKMatrix.at<double>(1,2);
  }

  // Extrinsics
  void Calibration::extrinsics (const Mat& kMatrix, 
				const jderobot::ImageDataPtr depthData)

  {

    /*
    
    TPinHoleCamera m_progeoCam;

    // Camera Position
    HPoint3D positionCam;

    positionCam.X= 0;
    positionCam.Y= 0;
    positionCam.Z= 0;
    positionCam.H= 0.0;

    m_progeoCam.position = positionCam;

    // K Matrix
    m_progeoCam.k11 = kMatrix.at<double>(0,0);
    m_progeoCam.k12 = kMatrix.at<double>(0,1);
    m_progeoCam.k13 = kMatrix.at<double>(0,2);
    m_progeoCam.k14 = 0;
    
    m_progeoCam.k21 = kMatrix.at<double>(1,0);
    m_progeoCam.k22 = kMatrix.at<double>(1,1);
    m_progeoCam.k23 = kMatrix.at<double>(1,2);
    m_progeoCam.k24 = 0;

    m_progeoCam.k31 = kMatrix.at<double>(2,0);
    m_progeoCam.k32 = kMatrix.at<double>(2,1);
    m_progeoCam.k33 = kMatrix.at<double>(2,2);
    m_progeoCam.k34 = 0;

    // RT Matrix
    m_progeoCam.rt11 = 1;
    m_progeoCam.rt12 = 0;
    m_progeoCam.rt13 = 0; 
    m_progeoCam.rt14 = 0;
    
    m_progeoCam.rt21 = 0;
    m_progeoCam.rt22 = 1;
    m_progeoCam.rt23 = 0;
    m_progeoCam.rt24 = 0;
    
    m_progeoCam.rt31 = 0;
    m_progeoCam.rt32 = 0;
    m_progeoCam.rt33 = 1;
    m_progeoCam.rt34 = 0;
    
    m_progeoCam.rt41 = 0;
    m_progeoCam.rt42 = 0;
    m_progeoCam.rt43 = 0;
    m_progeoCam.rt44 = 1;


    display_camerainfo(m_progeoCam);
    // backproject
    HPoint3D point3D;
    HPoint2D point2D;

    point2D.x = 180,0;
    point2D.y = 180.0;
    point2D.h = 1.0;
    
    printf ("Pto en 2D: %2.2f %2.2f %2.2f \n",point2D.x,point2D.y,point2D.h);

    if (backproject(&point3D,point2D,m_progeoCam)!=-1)
      printf("Backproject a 3D (de nuevo): %.2f %.2f %.2f %.2f \n",point3D.X, point3D.Y, point3D.Z, point3D.H);


    float depth = (int)depthData->pixelData[((depthData->description->width*(int)point2D.y)+(int)point2D.x)*3+1]<<8 | 
      (int)depthData->pixelData[((depthData->description->width*(int)point2D.y)+(int)point2D.x)*3+2];


    std::cout << depth << std::endl;
    std::cout << depthData->description->width << std::endl;

    HPoint2D p2;
    if (project(point3D, &p2, m_progeoCam))
      printf("Project a 2D (de nuevo): %2.2f,%2.2f,%2.2f \n",p2.x,p2.y,p2.h);

    point3D.X = 150.64;
    point3D.Y = 444.57;
    point3D.Z = 1000.0;
    point3D.H = 1.0;

    if (project(point3D, &p2, m_progeoCam))
      printf("Project a 2D (): %2.2f,%2.2f,%2.2f \n",p2.x,p2.y,p2.h);

    std::cout << "_----------------------_" << std::endl;
    point2D.x = 180,0;
    point2D.y = 180.0;
    point2D.h = 1.0;

    printf ("Pixek 2D: %2.2f %2.2f %2.2f \n",point2D.x,point2D.y,point2D.h);

    if (backproject(&point3D,point2D,m_progeoCam)!=-1)
      printf("Backproject a 3D (de nuevo): %.2f %.2f %.2f %.2f \n",point3D.X, point3D.Y, point3D.Z, point3D.H);

    HPoint3D orig;
    orig.X = 0.;
    orig.Y = 0.;
    orig.Z = 0.;
    orig.H = 1.;
    
    GeoUtils* gUtils = new GeoUtils();
    HPoint3D vector = gUtils->getVector(orig, point3D);
    
    GeoUtils::Line3D* line3D = new GeoUtils::Line3D(orig, vector);

    HPoint3D p3 = line3D->getPointByZ(2000.0);

    printf("getPointByZ : %.2f %.2f %.2f %.2f \n",p3.X, p3.Y, p3.Z, p3.H);

    if (project(p3, &p2, m_progeoCam))
      printf("Project a 2D (): %2.2f,%2.2f,%2.2f \n",p2.x,p2.y,p2.h);
    
    delete(line3D);
    delete (gUtils);
    
    
    std::cout << "_----------------------_" << std::endl;

    */

    /*

    math::Vector3H pos;
    pos.vector(0) = 0.0;
    pos.vector(1) = 0.0;
    pos.vector(2) = 0.0;
    pos.vector(3) = 0.0;

    math::Matriz3x3 K;
    K.getMatriz()(0,0) = kMatrix.at<double>(0,0);
    K.getMatriz()(0,1) = kMatrix.at<double>(0,1);
    K.getMatriz()(0,2) = kMatrix.at<double>(0,2);

    K.getMatriz()(1,0) = kMatrix.at<double>(1,0);
    K.getMatriz()(1,1) = kMatrix.at<double>(1,1);
    K.getMatriz()(1,2) = kMatrix.at<double>(1,2);

    K.getMatriz()(2,0) = kMatrix.at<double>(2,0);
    K.getMatriz()(2,1) = kMatrix.at<double>(2,1);
    K.getMatriz()(2,2) = kMatrix.at<double>(2,2);

    math::Matriz4x4 RT;
    RT.getMatrix()(0,0) = 1.;
    RT.getMatrix()(0,1) = 0.;
    RT.getMatrix()(0,2) = 0.;
    RT.getMatrix()(0,3) = 0.;

    RT.getMatrix()(1,0) = 0.;
    RT.getMatrix()(1,1) = 1.;
    RT.getMatrix()(1,2) = 0.;
    RT.getMatrix()(1,3) = 0.;

    RT.getMatrix()(2,0) = 0.;
    RT.getMatrix()(2,1) = 0.;
    RT.getMatrix()(2,2) = 1.;
    RT.getMatrix()(2,3) = 0.;

    RT.getMatrix()(3,0) = 0.;
    RT.getMatrix()(3,1) = 0.;
    RT.getMatrix()(3,2) = 0.;
    RT.getMatrix()(3,3) = 1.;

    
    Progeo::Progeo* progeo = new Progeo::Progeo(pos, K, RT, 320, 240);
    progeo->display_camerainfo();

    math::Vector3H p3D;
    math::Vector2H pixel (10.0, 100.0, 1.0); 
    
    progeo->backproject(pixel, p3D);

    std::cout << p3D << std::endl;

    HPoint3D orig;
    orig.X = 0.;
    orig.Y = 0.;
    orig.Z = 0.;
    orig.H = 1.;
    
    HPoint3D point3D;
    point3D.X = p3D.getX();
    point3D.Y = p3D.getY();
    point3D.Z = p3D.getZ();
    point3D.H = 1.;

    GeoUtils* gUtils = new GeoUtils();
    HPoint3D vector = gUtils->getVector(orig, point3D);
    
    GeoUtils::Line3D* line3D = new GeoUtils::Line3D(orig, vector);

    HPoint3D p3 = line3D->getPointByZ(2000.0);

    printf("getPointByZ : %.2f %.2f %.2f %.2f \n",p3.X, p3.Y, p3.Z, p3.H);


    p3D.setX(p3.X);
    p3D.setY(p3.Y);
    p3D.setZ(p3.Z);
    p3D.setH(1.0);

    std::cout << p3D << std::endl;

    math::Vector2H pixel2;
    progeo->project(p3D, pixel2);

    std::cout << pixel2.getX() << " " << pixel2.getY() << std::endl;

    delete(line3D);
    delete (gUtils);
    delete(progeo);

    */

    Eigen::Vector4d posCamera;
    posCamera(0) = double(0.0);
    posCamera(1) = double(0.0);
    posCamera(2) = double(0.0);


    Eigen::Matrix3d K;
    K(0,0) = kMatrix.at<double>(0,0);
    K(0,1) = kMatrix.at<double>(0,1);
    K(0,2) = kMatrix.at<double>(0,2);

    K(1,0) = kMatrix.at<double>(1,0);
    K(1,1) = kMatrix.at<double>(1,1);
    K(1,2) = kMatrix.at<double>(1,2);

    K(2,0) = kMatrix.at<double>(2,0);
    K(2,1) = kMatrix.at<double>(2,1);
    K(2,2) = kMatrix.at<double>(2,2);

    Eigen::Matrix4d RT;
    RT(0,0) = double(1.);
    RT(0,1) = double(0.);
    RT(0,2) = double(0.);
    RT(0,3) = double(0.);

    RT(1,0) = double(0.);
    RT(1,1) = double(1.);
    RT(1,2) = double(0.);
    RT(1,3) = double(0.);

    RT(2,0) = double(0.);
    RT(2,1) = double(0.);
    RT(2,2) = double(1.);
    RT(2,3) = double(0.);

    RT(3,0) = double(0.);
    RT(3,1) = double(0.);
    RT(3,2) = double(0.);
    RT(3,3) = double(1.);


    Progeo::Progeo* progeo = new Progeo::Progeo(posCamera, K, RT, 320, 240);
    progeo->display_camerainfo();   

    Eigen::Vector4d p3D;
    Eigen::Vector3d pixel;

    pixel(0) = double(180.);
    pixel(1) = double(180.); 
    pixel(2) = double(1.);

    progeo->backproject(pixel, p3D);

    std::cout << "Punto 3D: " << p3D << std::endl; 

    progeo->project(p3D, pixel);   

    std::cout << "Pixel 2D: " << pixel << std::endl;
     
    Point3D *pStart = new Point3D(0.0,0.0,0.0);
    Point3D *pEnd = new Point3D(p3D);

    Segment3D *segment = new Segment3D(*pStart,*pEnd);
    Point3D *nP3D = segment->getPointByZ(200.0);

    std::cout << "New Point 3D with Z=200.0 " << *nP3D << std::endl;
    
    progeo->project(nP3D->getPoint(), pixel);

    std::cout << "Project Pixel\n" << pixel << std::endl;

    delete(pStart);
    delete(pEnd);
    delete(nP3D);
    delete(segment);
    delete(progeo);
  }


  void
  Calibration::BackProjectWithDepth (const Eigen::Vector3d pixel,
				     const colorspaces::Image depthData,
				     Eigen::Vector4d& res3D)
  {


    Eigen::Vector4d posCamera;
    posCamera(0) = double(0.0);
    posCamera(1) = double(0.0);
    posCamera(2) = double(0.0);


    Eigen::Matrix3d K;
    K(0,0) = mKMatrix.at<double>(0,0);
    K(0,1) = mKMatrix.at<double>(0,1);
    K(0,2) = mKMatrix.at<double>(0,2);

    K(1,0) = mKMatrix.at<double>(1,0);
    K(1,1) = mKMatrix.at<double>(1,1);
    K(1,2) = mKMatrix.at<double>(1,2);

    K(2,0) = mKMatrix.at<double>(2,0);
    K(2,1) = mKMatrix.at<double>(2,1);
    K(2,2) = mKMatrix.at<double>(2,2);

    Eigen::Matrix4d RT; 
    RT(0,0) = double(1.);
    RT(0,1) = double(0.);
    RT(0,2) = double(0.);
    RT(0,3) = double(0.);

    RT(1,0) = double(0.);
    RT(1,1) = double(1.);
    RT(1,2) = double(0.);
    RT(1,3) = double(0.);

    RT(2,0) = double(0.);
    RT(2,1) = double(0.);
    RT(2,2) = double(1.);
    RT(2,3) = double(0.);

    RT(3,0) = double(0.);
    RT(3,1) = double(0.);
    RT(3,2) = double(0.);
    RT(3,3) = double(1.);

    float depth = (int)depthData.data[((depthData.cols*(int)pixel(1))+(int)pixel(0))*3+1]<<8 | (int)depthData.data[((depthData.cols*(int)pixel(1))+(int)pixel(0))*3+2];

    Progeo::Progeo* progeo = new Progeo::Progeo(posCamera, K, RT, 320, 240);
    //progeo->display_camerainfo(); 

    std::cout << "Pixel (opticas): " << pixel << std::endl;
   
    // Optical coordinates
    Eigen::Vector3d graphic;
    graphic(0) = pixel(1);
    graphic(1) = 240 - 1 - pixel(0);
    graphic(2) = 1.0;

    std::cout << "Graphic (progeo): " << graphic << std::endl;

    Eigen::Vector4d p3D;
    progeo->backproject(graphic, p3D); 

    std::cout << "P3D: " << p3D << std::endl;

    Point3D *pStart = new Point3D(0.0,0.0,0.0);
    Point3D *pEnd = new Point3D(p3D);

    Segment3D *segment = new Segment3D(*pStart,*pEnd);

    std::cout << "Depth: " << depth << std::endl;

    Point3D *nP3D = segment->getPointByZ(depth);    

    res3D = nP3D->getPoint();

    std::cout << res3D << std::endl;

    /*
    std::cout << "-------------" << std::endl;
    p3D(0) = 0.;
    p3D(1) = 0.;
    p3D(2) = 900.;
    p3D(3)= 1.;
    std::cout << p3D << std::endl;

    progeo->project(p3D, optical);
    std::cout << optical << std::endl;

    std::cout << "-------------" << std::endl;
    optical(0) = 0.;
    optical(1) = 0.;
    optical(2) = 1.;

    std::cout << optical << std::endl;
    progeo->backproject(optical, p3D);
    std::cout << p3D << std::endl;
    */

    delete(segment);
    delete(pStart);
    delete(pEnd);
    delete(nP3D);
    delete(progeo);

  }

}

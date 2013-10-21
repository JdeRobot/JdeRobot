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
#include "../../libs/geometry/math/Point3D.h"
#include "../../libs/geometry/math/Segment3D.h"
#include <../../libs/progeo/progeo.h>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h> 
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multifit.h>


using namespace cv;

namespace rgbdCalibrator{


// This source code is a modification of camera_calibration.cpp
// You can see in OpenCV samples:
//     - samples/cpp/tutorial_code/calib3d/camera_calibration/


Calibration::Calibration(): mProgeo(NULL)
{
	initPatternPoints();

}

Calibration::~Calibration()
{
	delete(mProgeo);
}

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

	initProgeo();

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

void progeo_old_test ()
{
	TPinHoleCamera myCamA;
	xmlReader(&myCamA, "./cameraB.xml");
	display_camerainfo(myCamA);

	update_camera_matrix(&myCamA);
	display_camerainfo(myCamA);

	HPoint2D pixel;
	pixel.x =224.;
	pixel.y =18.;
	pixel.h = 1.;

	std::cout << pixel.x << " , " << pixel.y << std::endl;

	HPoint3D p3D;
	backproject(&p3D, pixel, myCamA);

	std::cout << p3D.X << " , " << p3D.Y << " , " << p3D.Z << std::endl;

}


void Calibration::initProgeo()
{

	Eigen::Vector4d posCamera;
	posCamera(0) = double(0.0);
	posCamera(1) = double(0.0);
	posCamera(2) = double(0.0);
	posCamera(3) = double(1.0);


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

	mProgeo = new Progeo::Progeo(posCamera, K, RT, 320, 240);
	mProgeo->updateRTMatrix();
	mProgeo->displayCameraInfo();
	mProgeo->saveToFile("./CameraA.xml");

	Eigen::Vector3d pixel(224.0, 18., 1.0);
	Eigen::Vector4d p3d;
	mProgeo->backproject(pixel,p3d);
	std::cout << "Pixel: " << pixel << std::endl;
	std::cout << "P3d: " << p3d << std::endl;


	progeo_old_test();
}


void
Calibration::BackProjectWithDepth (const Eigen::Vector3d pixel,
		const colorspaces::Image depthData,
		Eigen::Vector4d& res3D)
{

	float depth = (int)depthData.data[((depthData.cols*(int)pixel(1))+(int)pixel(0))*3+1]<<8 | (int)depthData.data[((depthData.cols*(int)pixel(1))+(int)pixel(0))*3+2];


	//std::cout << "Pixel (opticas): " << pixel << std::endl;

	// Optical coordinates
	Eigen::Vector3d graphic;
	graphic(0) = pixel(1);
	graphic(1) = 240 - 1 - pixel(0);
	graphic(2) = 1.0;

	//std::cout << "Graphic (progeo): " << graphic << std::endl;

	Eigen::Vector4d p3D;
	mProgeo->backproject(graphic, p3D);

	//std::cout << "P3D: " << p3D << std::endl;

	Point3D *pStart = new Point3D(0.0,0.0,0.0);
	Point3D *pEnd = new Point3D(p3D);

	Segment3D *segment = new Segment3D(*pStart,*pEnd);

	//std::cout << "Depth: " << depth << std::endl;

	Point3D *nP3D = segment->getPointByZ(depth);

	res3D = nP3D->getPoint();

	delete(segment);
	delete(pStart);
	delete(pEnd);
	delete(nP3D);

}

void Calibration::initPatternPoints()
{
	// Down line, right to left
	mPatternPoints.push_back(Eigen::Vector4d (120.,0.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (60.0,0.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,0.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,0.,60.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,0.,120.,1.));

	// Up line, right to left
	mPatternPoints.push_back(Eigen::Vector4d (120.,120.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (60.,120.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,120.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,120.,60.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,120.,120.,1.));

	// Middle line, right to left
	mPatternPoints.push_back(Eigen::Vector4d (120.,60.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,60.,0.,1.));
	mPatternPoints.push_back(Eigen::Vector4d (0.,60.,120.,1.));


}

bool Calibration::addPatternPixel (Eigen::Vector3d pixel, const colorspaces::Image depthData)
{
	mPixelPoints.push_back(pixel);

	if  (mPixelPoints.size() > mPatternPoints.size())
		return false;

	if (mPixelPoints.size() == mPatternPoints.size())
	{

		std::cout << "\tPixels" << "\t\t" << "Pixel(Image)\tPixel(progeo)\tDistance" << "\t"
				  << "Backproject"  << "\t\t\t" << "Fran Point" << "\t\t" << "Pattern"
				  <<std::endl;

		for (int i=0; i<mPixelPoints.size(); i++)
		{

			std::cout << "P" << i << "\t("
					<< mPixelPoints[i](0) << ","
					<< mPixelPoints[i](1) << ")  \t";

			Eigen::Vector4d pCamera(0.,0.,1000000.,0.);

			Eigen::Vector3d pixelChoosen;

			// We use a window of 5x5 pixels to avoid errors of the depth image
			for (int x=-2; x<=2; x++) {

				for (int y=-2;y<=2; y++) {
					Eigen::Vector3d pixel(mPixelPoints[i](0)+x,mPixelPoints[i](1)+y,1.0);
					Eigen::Vector4d aux;
					BackProjectWithDepth (pixel, depthData, aux);

					if (aux(2) != 0 && aux(2) < pCamera(2)) {
						pCamera = aux;
						pixelChoosen = pixel;
					}
				}
			}

			mCameraPoints.push_back(pCamera);

			// Pixel Image
			std::cout << "(" << pixelChoosen[0] << "," << pixelChoosen[1] << ")  \t";

			mProgeo->pixel2optical(pixelChoosen);

			// Pixel Progeo
			std::cout << "(" << pixelChoosen[0] << "," << pixelChoosen[1] << ")  \t";

			// Distance
			float depth = (int)depthData.data[((depthData.cols*(int)pixelChoosen(1))+(int)pixelChoosen(0))*3+1]<<8 | (int)depthData.data[((depthData.cols*(int)pixelChoosen(1))+(int)pixelChoosen(0))*3+2];
			std::cout << depth << "\t\t";

			// Pixel backProject
			Eigen::Vector4d aux3D;
			mProgeo->backproject(pixelChoosen, aux3D);
			std::cout << "(" << aux3D[0] << "," << aux3D[1] << "," << aux3D[2] << ")\t";


			// fix bug about projection's progeo
			Eigen::Vector4d p3D;
			getRealPoint(pixelChoosen, depthData, p3D);
			std::cout << "\t(" << p3D[0] << "," << p3D[1] << "," << p3D[2] << ")\t\t";


			std::cout << "(" << mPatternPoints[i](0) << ","
					<< mPatternPoints[i](1) << ","
					<< mPatternPoints[i](2) << ")";

			std::cout << std::endl;
		}

		mPairPoints.clear();

		//Build pairs
		for (int i = 0; i< mPatternPoints.size(); i++)
			mPairPoints.push_back(std::make_pair(mPatternPoints[i] ,mCameraPoints[i]));


		for (int i = 0; i< mPatternPoints.size(); i++)
		{
			std::cout << "(" << mPairPoints[i].first(0)
				  << "," << mPairPoints[i].first(1)
				  << "," << mPairPoints[i].first(2) << ")  ->  ";

			std::cout << "(" << mPairPoints[i].second(0)
				  << "," << mPairPoints[i].second(1)
				  << "," << mPairPoints[i].second(2) << ")";

			std::cout << std::endl;
		}

		LSO();

		mCameraPoints.clear();
		mPairPoints.clear();

	}

	return true;

}


void Calibration::getRealPoint(const Eigen::Vector3d pixel,
		const colorspaces::Image depthData,
		Eigen::Vector4d& res3D)
{

	float depth = (int)depthData.data[((depthData.cols*(int)pixel(1))+(int)pixel(0))*3+1]<<8 | (int)depthData.data[((depthData.cols*(int)pixel(1))+(int)pixel(0))*3+2];

	if (depth == 0)
	{
		res3D(0) = 0.;
		res3D(1) = 0.;
		res3D(2) = 0.;
		res3D(3) = 0.;
		return;
	}

	Eigen::Vector4d p3D;
	mProgeo->backproject(pixel, p3D);

	//std::cout << "-> p3D: " << p3D << std::endl;

	Eigen::Vector4d camPosition = mProgeo->getPosition();

	//std::cout << "-> camPosition: " << camPosition << std::endl;

	float module = sqrt(double(1/(((camPosition(0)-p3D(0))*(camPosition(0)-p3D(0)))+
			((camPosition(1)-p3D(1))*(camPosition(1)-p3D(1)))+
			((camPosition(2)-p3D(2))*(camPosition(2)-p3D(2))))));

	//std::cout << "-> module: " << module << std::endl;

	Eigen::Vector4d camFoa = mProgeo->getFoa();

	//std::cout << "-> camFoa: " << camFoa << std::endl;

	float fmod = sqrt(double(1/(((camPosition(0)-camFoa(0))*(camPosition(0)-camFoa(0)))+
			((camPosition(1)-camFoa(1))*(camPosition(1)-camFoa(1)))+
			((camPosition(2)-camFoa(2))*(camPosition(2)-camFoa(2))))));

	//std::cout << "-> fmod: " << fmod << std::endl;

	Eigen::Vector4d f;
	f(0) = (camFoa(0)-camPosition(0)) * fmod;
	f(1) = (camFoa(1)-camPosition(1)) * fmod;
	f(2) = (camFoa(2)-camPosition(2)) * fmod;

	//std::cout << "-> f: " << f << std::endl;

	Eigen::Vector4d u;
	u(0) = (p3D(0)-camPosition(0)) * module;
	u(1) = (p3D(1)-camPosition(1)) * module;
	u(2) = (p3D(2)-camPosition(2)) * module;

	//std::cout << "-> u: " << u << std::endl;

	Eigen::Vector4d F;
	F(0) = depth * f(0) + camPosition(0);
	F(1) = depth * f(1) + camPosition(1);
	F(2) = depth * f(2) + camPosition(2);

	//std::cout << "-> F: " << F << std::endl;

	float t = (- (f(0)*camPosition(0)) + (f(0)*F(0)) - (f(1)*camPosition(1)) + (f(1)*F(1)) - (f(2)*camPosition(2)) + (f(2)*F(2)))/
			((f(0)*u(0)) + (f(1)*u(1)) + (f(2)*u(2)));

	//std::cout << "t=" << t << std::endl;


	res3D(0) = t*u(0) + camPosition(0);
	res3D(1) = t*u(1) + camPosition(1);
	res3D(2) = t*u(2) + camPosition(2);

}


void Calibration::test(Eigen::Vector4d pCamera)
{
	std::cout << "====== TEST ====== " << std::endl;
	std::cout << pCamera << std::endl;

	Eigen::Vector4d s = mRTsolution * pCamera;
	std::cout << s << std::endl;
}

void Calibration::LSO()
{

	gsl_matrix *ertm=gsl_matrix_alloc(4,4);
	double chisq;
	gsl_matrix *x,*cov;
	gsl_vector *y,*c;
	gsl_multifit_linear_workspace * work;

	x=gsl_matrix_calloc(mPairPoints.size()*3,12);
	cov=gsl_matrix_alloc(12,12);
	y=gsl_vector_alloc(mPairPoints.size()*3);
	c=gsl_vector_alloc(12);
	work = gsl_multifit_linear_alloc (mPairPoints.size()*3,12);
	double x1,y1,z1,x2,y2,z2;
	//preaparamos la matriz de ecuaciones

	for (int i=0; i<mPairPoints.size(); i++){
		x1=mPairPoints[i].first(0);
		y1=mPairPoints[i].first(1);
		z1=mPairPoints[i].first(2);
		x2=mPairPoints[i].second(0);
		y2=mPairPoints[i].second(1);
		z2=mPairPoints[i].second(2);

		gsl_matrix_set(x, i*3,0, x2);
		gsl_matrix_set(x, i*3,1, y2);
		gsl_matrix_set(x, i*3,2, z2);
		gsl_matrix_set(x, i*3,3, 1);
		gsl_vector_set(y,i*3, x1);

		gsl_matrix_set(x, i*3+1, 4, x2);
		gsl_matrix_set(x, i*3+1, 5, y2);
		gsl_matrix_set(x, i*3+1, 6, z2);
		gsl_matrix_set(x, i*3+1, 7, 1);
		gsl_vector_set(y,i*3+1, y1);

		gsl_matrix_set(x, i*3+2,8, x2);
		gsl_matrix_set(x, i*3+2,9, y2);
		gsl_matrix_set(x, i*3+2,10, z2);
		gsl_matrix_set(x, i*3+2,11, 1);
		gsl_vector_set(y,i*3+2, z1);
	}

	gsl_multifit_linear(x,y,c,cov,&chisq,work);
	gsl_multifit_linear_free(work);

	for (int i=0;i<3; i++) {
		for (int j=0;j<4;j++) {
			double v=c->data[i*4+j];
			if(sqrt(v*v)<0.0001)
				v=0;
			//gsl_matrix_set(ertm,i,j,v);
			mRTsolution(i,j) = v;
		}
	}

	/*
    gsl_matrix_set(ertm,3,0,0);
    gsl_matrix_set(ertm,3,1,0);
    gsl_matrix_set(ertm,3,2,0);
    gsl_matrix_set(ertm,3,3,1);
	 */

	mRTsolution(3,0) = 0.;
	mRTsolution(3,1) = 0.;
	mRTsolution(3,2) = 0.;
	mRTsolution(3,3) = 1.;

	//gsl_matrix_fprintf(stdout,ertm,"%f");

	std::cout << mRTsolution << std::endl;



}

}

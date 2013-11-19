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
 *  Authors : Alejandro Hernández <ahcorde [at] gmail [dot] com>
 *            Roberto Calvo <rocapal [at] gsyc [dot] urjc [dot] es>
 *            Eduardo Perdices <eperdices [at] gsyc [dot] es>
 *
 */



#ifndef PROGEOMM_H
#define PROGEOMM_H

#define EIGEN_DONT_ALIGN_STATICALLY True

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <libxml/parser.h>
#include <libxml/xmlreader.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <cv.h>

namespace Progeo {

/**
	\class Progeo
	\brief This class implements a pinhole camera model
	\autor Roberto Calvo <rocapal@gsyc.urjc.es>
	\date  06/10/2013
 **/



class Progeo
{

public:

	Progeo();
	Progeo(std::string filename);
	Progeo(Eigen::Vector4d posCamera,
			Eigen::Matrix3d KMatrix,
			Eigen::Matrix4d RTMatrix,
			int width, int height);

	void setPosition (Eigen::Vector4d pos);
	void setKMatrix (Eigen::Matrix3d KMatrix);
	void setRTMatrix (Eigen::Matrix4d RTMatrix);
	void setImageSize (int width, int height);
	void setFoa(Eigen::Vector4d Foa);
	void setRoll(float Roll);

	Eigen::Matrix3d getKMatrix(){return K;};
	Eigen::Matrix4d	getRTMatrix(){return RT;};
	Eigen::Vector4d getPosition() {return position; };
	Eigen::Vector4d getFoa() {return foa; };
	float getRoll() {return roll; };

	void backproject(Eigen::Vector3d point, Eigen::Vector4d& pro);
	int project(Eigen::Vector4d in, Eigen::Vector3d &out);

	void updateKMatrix();
	void updateRTMatrix();
	void displayCameraInfo();

	int getImageWidth() { return columns;};
	int getImageHeight() { return rows;};

	void pixel2optical (Eigen::Vector3d &point);
	void optical2pixel (Eigen::Vector3d &point);

	void saveToFile (std::string filename, bool updateMatrix=true);
	void readFromFile(std::string filename);
	int displayline(Eigen::Vector3d p1, Eigen::Vector3d p2, Eigen::Vector3d& a, Eigen::Vector3d& b);



private:

	/* camera 3d position in mm */
	Eigen::Vector4d position;

	/* camera 3d focus of attention in mm */
	Eigen::Vector4d foa;

	/* top right and bottom left points */
	Eigen::Vector3d tr, bl;

	/* camera roll position angle in rads */
	float roll;

	/* focus x distance in mm*/
	float fdistx;
	float fdisty;

	/* pixels */
	float u0,v0;

	/*angle between the x and y pixel axes in rads*/
	float skew

	/* image height in pixels */;
	int rows;

	/* image width in pixels */
	int columns;

	/* camera K matrix */
	float k11, k12, k13, k14, k21, k22, k23, k24, k31, k32, k33, k34;
	Eigen::Matrix3d K;

	/* camera rotation + translation matrix */
	float rt11, rt12, rt13, rt14, rt21, rt22, rt23, rt24, rt31, rt32, rt33, rt34, rt41, rt42, rt43, rt44;
	Eigen::Matrix4d RT;

	/* distortion parameters */
	float d1,d2,d3,d4,d5,d6;
	float dx,dy;

	/* name */
	std::string name;

	const char* double2char(double d);
};
}

#endif

/*
 *  Copyright (C) 2010 Julio Vega
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : Julio Vega <julio.vega@urjc.es>,
 *
 */

#ifndef TELEOPERATOR_CAMERACONF_H
#define TELEOPERATOR_CAMERACONF_H

#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <progeo/progeo.h>
#include "JointMotor.h"

/*GSL*/
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_multifit.h>

#define INIT_X 0
#define INIT_Y 0
#define INIT_Z 0
#define NECK_LENGTH 0.175
#define CAM_RIGHT_X 0.057
#define CAM_RIGHT_Y 0.0
#define CAM_RIGHT_Z 0.075

#define MAX_PAN 1.4
#define MIN_PAN -1.4
#define MAX_TILT 0.38
#define MIN_TILT -1.0

namespace teleoperator {
  class CameraConf {
  public:
    CameraConf(RoboCompJointMotor::JointMotorPrx jprx);
    virtual ~CameraConf();
    
		void drawWorld(IplImage * src);

		void getAngles(float x, float y, float z, float &pan, float &tilt);

  private:
		void calcCameraPos();

		void printMatrix(gsl_matrix * m, int rows, int cols);

		void drawLine(IplImage * src, HPoint3D pini, HPoint3D pend);

		TPinHoleCamera camera;
		RoboCompJointMotor::JointMotorPrx jprx;
  };

} /*namespace*/

#endif /*TELEOPERATOR_CAMERACONF_H*/

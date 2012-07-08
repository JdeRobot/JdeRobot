/*
 *  Copyright (C) 2010 Eduardo Perdices García
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
 *   Authors : Eduardo Perdices García <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef GIRAFFECLIENT_CONTROLLER_H
#define GIRAFFECLIENT_CONTROLLER_H

#include <string>
#include <iostream>
#include <colorspaces/colorspacesmm.h>
#include <jderobot/jointmotor.h>
#include "cameraconf.h"

namespace giraffeClient {
  class Controller {
  public:
    Controller(RoboCompJointMotor::JointMotorPrx jprx);
    virtual ~Controller();
    
    std::string getGladePath();

		void setMotorPos(int motor, float pos);

		void drawWorld(const colorspaces::Image& image);

		int lookAt(float x, float y, float z);

		const static int MOTOR_PAN = 0;
		const static int MOTOR_TILT = 1;
		const static int MOTOR_LEFT = 2;
		const static int MOTOR_RIGHT = 3;

  private:
		std::string gladepath;
		RoboCompJointMotor::JointMotorPrx jprx;
		CameraConf * camera;
  };

} /*namespace*/

#endif /*GIRAFFECLIENT_CONTROLLER_H*/

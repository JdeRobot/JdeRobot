/*
 *
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Author : Sara Marugán Alonso <smarugan@gsyc.es>
 *
 */

#ifndef IMAGE_RECORDER_H
#define IMAGE_RECORDER_H

#include "GenericRecorder.h"
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <jderobot/camera.h>

class imageRecorder : public GenericRecorder
{

	public:
		/// \brief Recorder
		imageRecorder(const jderobotice::Context& context,const jderobot::RecorderConfigPtr& recConfig,int recordingProvider);

	private:

		int doRecording();
};

#endif IMAGE_RECORDER_H

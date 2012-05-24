/*
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
 *            Eduardo Perdices <eperdes@gsyc.es>
 *            Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>	
 */




#ifndef CAMERA_ADAPTER
#define CAMERA_ADAPTER

#include <iostream>
#include "alproxy.h"
#include "alvisiondefinitions.h"
#include <pthread.h>
#include <alvideodeviceproxy.h>

namespace NaoAdapter{

/**
* \brief Class that contains all camera definitions.
*/
class NaoCamera{

	private:
		unsigned char * data;
		unsigned char * dataAux;
		int width;
		int height;
		int channels;
		std::string IP;
		int PORT;
		AL::ALProxy * NaoCameraProxy;
		int format;
		int colorSpace;
		int fps;
		std::string name;
		pthread_mutex_t cammutex;
		long time;
		int updated;

		/*Private functions*/
		void getSizeValues(int width, int height);

	public:

		NaoCamera(int width, int height, int fps, char* IP, int port) ;
		int init();
		void terminate();
		int updateImage();
		void getImage(unsigned char * data);
		int getWidth();
		int getHeight();	
		/*int check_values(T_cam_nao* cam);	
		int get_cam_values(T_cam_nao* cam);*/
		
};
}
#endif

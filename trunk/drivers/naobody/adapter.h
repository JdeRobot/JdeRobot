/*
 *  Copyright (C) 1997-2009 JDE Developers Team
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
 *  Authors : 	Eduardo Perdices <edupergar@gmail.com>
 *				Francisco Rivas <fm.rivas@alumnos.urjc.es>
 */



#ifndef ADAPTER_H
#define ADAPTER_H

#include <iostream>
#include "alproxy.h"
#include "alvisionimage.h"
#include <pthread.h>
#include "almotionproxy.h"

const float PI=3.1415926;
const float MAXY=60;
const float MAXP=44;
const float MAXVY=100;
const float MINV=-100;
const float MAXV=100;
const float CHANGE_RANGE=1;
const float MYMAXW=100;
const float MYMINW=0;
const float MYMINR=10;
const float MYMAXR=0;
const float DISTANCE_A=0.08;
const float DISTANCE_B=10;

class Camera{

	private:

		unsigned char * data;
		unsigned char * dataAux;
		int width;
		int height;
		int channels;
		std::string IP;
		int PORT;
		AL::ALProxy * cameraProxy;
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

		Camera(int width, int height, int fps);
		int init();
		void terminate();
		int updateImage();
		void getImage(unsigned char * data);
		int getWidth();
		int getHeight();		
};

class motion{
	private:
		std::string IP;
		int PORT;
		AL::ALMotionProxy * motionProxy;
		std::string name;
		float lastp;
		float lasty;
		float lastv;
		float lastw;
		int mysteps;

	public:

		motion();
		int init();
		void terminate();
		int walk_stop_to_process(float v, float w);
		int walk_stop_if_changes(float v, float w);
		int head(float y, float p,float *posy, float *posp,float vy, float vp, unsigned long int* clock);
};


/*Camera functions in C*/
extern "C" Camera* newCamera(int width, int height, int fps);
extern "C" int initCamera(Camera* c);
extern "C" void terminateCamera(Camera* c);
extern "C" void deleteCamera(Camera* c);
extern "C" int updateImageCamera(Camera* c);
extern "C" void getImageCamera(Camera* c, unsigned char * data);
extern "C" int getWidthCamera(Camera* c);
extern "C" int getHeightCamera(Camera* c);

/*Motion functions in C*/

extern "C" motion* newmotion();
extern "C" int initmotion(motion* m);
extern "C" void terminatemotion(motion* m);
extern "C" int walkprocessmotion(motion* m, float v, float w);
extern "C" int walkchangesmotion(motion* m, float v, float w);
extern "C" int headmotion(motion* m, float y, float p, float *posy, float *posp, float vy, float vp,unsigned long int* clock);
extern "C" void deletemotion(motion* m);
extern "C" int get_motionclock(motion* m);


#endif

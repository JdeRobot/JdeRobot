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
const float MAXV=100;
const float CHANGE_RANGE=1;

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
		int mysteps;

	public:

		motion();
		int init();
		void terminate();
		int walk(float v, float w);
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
extern "C" int walkmotion(motion* m, float v, float w);
extern "C" int headmotion(motion* m, float y, float p, float *posy, float *posp, float vy, float vp,unsigned long int* clock);
extern "C" void deletemotion(motion* m);
extern "C" int get_motionclock(motion* m);


#endif

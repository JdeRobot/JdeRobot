#ifndef ADAPTER_H
#define ADAPTER_H

#include <iostream>
#include "alproxy.h"
#include "alvisionimage.h"
#include <pthread.h>

#define PI 3.1415926

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

class Head{

	private:

		std::string IP;
		int PORT;
		AL::ALProxy * headProxy;
		float lastValue;

	public:

		Head();
		int init();
		void terminate();
		void moveTo(float angle);
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

/*Head functions in C*/
extern "C" Head* newHead();
extern "C" int initHead(Head* h);
extern "C" void terminateHead(Head* h);
extern "C" void deleteHead(Head* h);
extern "C" void moveToHead(Head* h, float angle);

#endif

#ifndef PERCEPTION_H
#define PERCEPTION_H

#include <iostream>
#include <cmath>
#include <pthread.h>
#include "Component.h"
#include "alvision/alvisiondefinitions.h"
#include "alproxies/alvideodeviceproxy.h"
#include "alvision/alimage.h"

#include "Singleton.h"
#include "ImageInput.h"
#include "Kinematics.h"
#include "vision/colorFilter/SimpleColorFilter.h"
#include "ImageHandler.h"

#include <IceE/IceE.h>
#include <camera.h>

using namespace std;
using namespace jderobot;

class Perception : public Component, public Singleton<Perception>
{
public:

	Perception();
	~Perception();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);
	void step();

	ImageHandler *getImageHandler() {return _ImageHandler;};

	inline SimpleColorFilter* getColorFilter() { return this->colorFilter; };
	inline int getCam() { return cam; };
	inline void lock() { pthread_mutex_lock(&mutex); };
	inline void unlock() { pthread_mutex_unlock(&mutex); };
	void setCam(const int cam);

	// External methods
	bool setCamParam(const int param, const int value);
	int getCamParam(const int param);

	virtual jderobot::ImageDataPtr getImageData(const Ice::Current& c);
	virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c);
	virtual void setCam (CameraType cam, const Ice::Current& c);
	//virtual void setImageType(FeaturesType type, const Ice::Current& c );

	long getWaitingTime();
	
	long getImageTs() {return image_ts;};

private:

	void initCamera();
	bool newImage();
	void loadCameraParams(string upper_file, string lower_file);

	static const string UPPER_CAMERA_CONFIG_FILE;
	static const string LOWER_CAMERA_CONFIG_FILE;

	int cam;
	bool camera_initialized;
	string fLEMname;
	ALValue imgDebug, filterParams;
	char * colorSrc;
	IplImage *cvImage, *cvAux;
	pthread_mutex_t mutex, mutexstep;
	Dictionary	camConfUpper, camConfLower;
	SimpleColorFilter *colorFilter;

	bool imageTaken;

	AL::ALVideoDeviceProxy *pcamera;
	jderobot::ImageDescriptionPtr imgDescription;

	Kinematics	* _Kinematics;
	ImageHandler * _ImageHandler;

	long long lastTimeStamp;
	long wtime;

	//capture thread
	pthread_t capture_thread;
	bool newImageTaken;
	char imgBuff[ImageInput::IMG_WIDTH * ImageInput::IMG_HEIGHT * ImageInput::IMG_CHANNELS_YUV];
	long image_ts;

protected:
	static void * EntryPoint(void*);
	void Capture();

};

#endif

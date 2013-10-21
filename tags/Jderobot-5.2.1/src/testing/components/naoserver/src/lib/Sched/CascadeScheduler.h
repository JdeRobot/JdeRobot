#ifndef CascadeScheduler_H
#define CascadeScheduler_H

#include "Component.h"
#include "Body.h"
#include "Head.h"
#include "Kinematics.h"
#include "Perception.h"
#include "NaoServerCamera.h"
#include "NaoServerMotors.h"
#include "NaoServerEncoders.h"
#include "NaoServerPose3DMotors.h"
#include "NaoServerPose3DEncoders.h"
#include "SensorRecorderNao.h"
#include "pthread.h"
#include <vector>

#include <IceE/IceE.h>

using namespace std;

class CascadeScheduler: public Singleton<CascadeScheduler>
{
public:

	CascadeScheduler();
	~CascadeScheduler();

	void init();
	void run();

private:

	static void* runThread(void *data);
	static pthread_t t1;
	static pthread_mutex_t mutex;
	static bool running;
};

#endif

#ifndef Kinematics_H
#define Kinematics_H

#include "Component.h"
#include "almath/tools/almath.h"
#include "alproxies/almotionproxy.h"
#include "Singleton.h"
#include "ImageInput.h"
#include "Matrix.h"
#include "progeo.h"
#include <pthread.h>

typedef struct {
	float x;
	float y;
	float z;
	float pan;
	float tilt;
	float roll;
	float foax;
	float foay;
	float foaz;
	float RT[12];
} TKinematics;

class Kinematics : public Component, public Singleton<Kinematics>
{
public:

	Kinematics();
	~Kinematics();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	void forceStep();
	void step();

	TKinematics * getKinematics();

private:

	void updateKinematics();

	AL::ALPtr<AL::ALMotionProxy> pmotion;

	int use_lower;
	float escale;
	TKinematics mykinematics;

	vector<float> torsoRT;
	MatrixCM RTrt;

	pthread_mutex_t mutex;
};

#endif

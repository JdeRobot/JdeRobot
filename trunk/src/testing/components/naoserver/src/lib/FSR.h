#ifndef FSR_H_
#define FSR_H_

#include "Component.h"
#include "Singleton.h"
#include "alproxies/almemoryproxy.h"

class FSR: public Singleton<FSR>
{
public:
	FSR();
	virtual ~FSR();

	void init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker);

	/*Return true if the robot is touching the ground*/
	int isTouchingGround();

private:

	void refreshFSR();

	static const float MIN_FSR_VALUE;
	static const int NUM_FSR = 4;
	static const int MAX_ITS_NOT_TOUCHING = 3;

	static const string fsrNamesLeft[NUM_FSR];
	static const string fsrNamesRight[NUM_FSR];

	float fsrValuesLeft[NUM_FSR];
	float fsrValuesRight[NUM_FSR];	
	int num_its_not_touching;

	AL::ALPtr<AL::ALMemoryProxy> almemory;
};

#endif /* FSR_H_ */

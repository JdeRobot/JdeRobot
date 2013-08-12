#include "FSR.h"

const string FSR::fsrNamesLeft[NUM_FSR] = {
	string("Device/SubDeviceList/LFoot/FSR/FrontLeft/Sensor/Value"),
	string("Device/SubDeviceList/LFoot/FSR/FrontRight/Sensor/Value"),
	string("Device/SubDeviceList/LFoot/FSR/RearLeft/Sensor/Value"),
	string("Device/SubDeviceList/LFoot/FSR/RearRight/Sensor/Value")
	};

const string FSR::fsrNamesRight[NUM_FSR] = {
	string("Device/SubDeviceList/RFoot/FSR/FrontLeft/Sensor/Value"),
	string("Device/SubDeviceList/RFoot/FSR/FrontRight/Sensor/Value"),
	string("Device/SubDeviceList/RFoot/FSR/RearLeft/Sensor/Value"),
	string("Device/SubDeviceList/RFoot/FSR/RearRight/Sensor/Value")
	};

const float	FSR::MIN_FSR_VALUE = 0.15; //En webots 0.0

FSR::FSR() {

	this->num_its_not_touching = 0;
}

FSR::~FSR() {
}

void
FSR::init(const string newName, AL::ALPtr<AL::ALBroker> parentBroker)
{
	try {
		this->almemory = parentBroker->getMemoryProxy();
	} catch (AL::ALError& e) {
		cerr << "FSR::init [almemory]" << e.toString() << endl;
	}
}

void
FSR::refreshFSR()
{

	int i;
	ALValue valleft, valright;

	for(i=0; i<NUM_FSR; i++) {

		valleft = almemory->getData(fsrNamesLeft[i], 0);
		valright = almemory->getData(fsrNamesRight[i], 0);

		fsrValuesLeft[i] = (float)valleft;
		fsrValuesRight[i] = (float)valright;
	}
}

int
FSR::isTouchingGround()
{
	int i;
	int found;

	this->refreshFSR();

	found = 0;
	for(i=0;i<NUM_FSR && !found;i++) {
		if(fsrValuesLeft[i] > this->MIN_FSR_VALUE || fsrValuesRight[i] > this->MIN_FSR_VALUE)
			found = 1;
	}

	if(found)
		this->num_its_not_touching = 0;	
	else
		this->num_its_not_touching++;

	if(this->num_its_not_touching >= MAX_ITS_NOT_TOUCHING)
		return 0;

	return 1;
}

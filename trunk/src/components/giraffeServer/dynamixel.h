/*
 *    Copyright (C) 2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DYNAMIXEL_H
#define DYNAMIXEL_H

#include <handler.h>
#include <q4serialport/q4serialport.h>
#include <QtCore>
#include "iostream"
#include "JointMotor.h"

#define BROADCAST 0xFE
#define SYNC_WRITE 0x83
#define WRITE_DATA 0x03
#define READ_DATA 0x02
#define ACTION 0x05
#define REG_WRTIE 0x04
#define PING 0x01
#define RESET 0x06
#define MAX_LENGTH_PACKET 30

#define MINSTEPSPOSITION 0
#define MAXSTEPSPOSITION 1023
//  
#define NO_RESPONSE
#define READ_RESPONSE
#define ALL_RESPONSE

/**
	@author Robolab <authormail>
*/

class Dynamixel : public Handler
{
typedef char* Packet;
public:
    Dynamixel( RoboCompJointMotor::BusParams  *busParams, RoboCompJointMotor::MotorParamsList *params );
    ~Dynamixel();
	QMutex mutex;

private:
	bool sendIPacket(Packet packet, char length);
	bool getSPacket( );
	QSerialPort  port;
	char checkSum(Packet packet);
	char status[MAX_LENGTH_PACKET];
	void printPacket(Packet packet);
	void ping(char motor);
	
	char packet[MAX_LENGTH_PACKET];
	RoboCompJointMotor::BusParams *busParams;
	RoboCompJointMotor::MotorParamsList *params;	
	
public:
	virtual void initialize() throw(QString);
	RoboCompJointMotor::MotorParamsList getMotorParams( uchar motor){ return this->params[motor];};	
	virtual void setPosition(uchar motor, int pos) throw(QString);
	virtual void setSyncPosition( const QVector<Handler::GoalPosition> & goals) throw(QString);
	virtual void getPosition(uchar motor, int &pos) throw(QString);
	virtual bool getVelocity(uchar motor, int &vel);
	virtual bool setVelocity(uchar motor, int vel);
	virtual bool getPower(uchar motor, int &pow);
	virtual bool getMaxPosition(uchar motor, int &pos);
	virtual bool getMinPosition(uchar motor, int &pos);
	virtual bool setMaxPosition(uchar motor, int pos);
	virtual bool setMinPosition(uchar motor, int pos);
	virtual bool isMoving(uchar motor);
	virtual bool getPGain(uchar motor, int &p){motor=motor;p=p;return false;};
	virtual bool getDGain(uchar motor, int &d){motor=motor;d=d;return false;};
	virtual bool getIGain(uchar motor, int &i){motor=motor;i=i;return false;};
	virtual bool setPGain(uchar motor, int p){motor=motor;p=p;return false;};
	virtual bool setDGain(uchar motor, int d){motor=motor;d=d;return false;};
	virtual bool setIGain(uchar motor, int i){motor=motor;i=i;return false;};
	virtual bool disablePWM(uchar motor){return false;};
	virtual bool enablePWM(uchar motor){return false;};
	virtual bool setId(uchar motor, int id);
	virtual bool reset(uchar motor){motor=motor;return false;};
	virtual bool restoreDefValues(uchar motor){motor=motor;return false;};
	virtual bool stop(uchar motor){motor=motor;return false;};
	virtual bool powerOff(uchar motor);
	virtual bool powerOn(uchar motor);
	virtual bool setBaudrate(uchar motor, int baud);
	virtual bool getBaudrate(uchar motor, int& baud);
	virtual void update(QMutex *mutex) throw(QString);

	//Out of abstract Handler. Specific of Dynamixel
	bool getCWComplianceMargin( uchar motor, int & m);
	bool getCCWComplianceMargin( uchar motor, int & m);
	bool setBothComplianceMargins( uchar motor, int m);
	bool getCWComplianceSlope( uchar motor, int & m);
	bool getCCWComplianceSlope( uchar motor, int & m);
	bool setBothComplianceSlopes( uchar motor, int  m);
	bool setReturnDelayTime( uchar motor, int t);
	bool getReturnDelayTime( uchar motor, int & t);
	bool getPunch( uchar motor, int & p);
	bool setPunch( uchar motor, int p);
	bool getVoltageLimit( uchar motor, int & v);
	bool setVoltageLimit( uchar motor, int v);
	bool getStatusReturnLevel(uchar motor,  int &level);
	bool setStatusReturnLevel(uchar motor,  int &level);
	
	//Not implemented yet
// 	bool getTemperature(uchar motor,  int &level);
// 	bool setTemperature(uchar motor,  int &level);
// 	bool getTemperatureLimit(uchar motor,  int &level);
// 	bool setTemperatureLimit(uchar motor,  int &level);
// 	bool getMaxTorque(uchar motor,  int &level);
// 	bool setMaxTorque(uchar motor,  int &level);
// 	bool getVoltage(uchar motor,  int &level);
// 	bool setVoltage(uchar motor,  int &level);
// 	bool get CWAngleLimit();
// 	bool set CWAngleLimit();
// 	bool get CCWAngleLimit();
// 	bool set CCWAngleLimit();
	
	
};

#endif

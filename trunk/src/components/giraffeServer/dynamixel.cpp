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
#include "dynamixel.h"


Dynamixel::Dynamixel(  RoboCompJointMotor::BusParams  *busParams , RoboCompJointMotor::MotorParamsList *params )
 : Handler()
{
  this->busParams = busParams;
  this->params = params;
}

Dynamixel::~Dynamixel()
{
}

void Dynamixel::initialize() throw (QString)
{
  QString device;
  device = QString::fromStdString( busParams->device);

  // Open and initialize the device
  port.setName( device );
  
  if ( port.open(device) == false)
  {
	QString error;
	QFile::Permissions p = QFile::permissions(QString::fromStdString(busParams->device));
	if ( (p | QFile::WriteOwner) != true)
		error = "JointMotor::Dynamixel::initialize() - Port " + QString::fromStdString(busParams->device) + 
					  " could not be opened. You don't have write permission on the device. Try 'sudo chmod 777 " +
					  QString::fromStdString( busParams->device ) + "'";	
	else
	  error = "JointMotor::Dynamixel::initialize() - Port " + QString::fromStdString( busParams->device ) + 
			  " could not be opened. Please check file permissions";
	throw error;
  }

  //Setting baudrate
  switch ( busParams->baudRate )
  {
	case 38400:  port.setBaudRate( QSerialPort::BAUD38400 ); 
	  port.setBaudRate( QSerialPort::BAUD38400 ); 
	  if (port.baudRate() != QSerialPort::BAUD38400)
	  {
		QString error("JointMotor::Dynamixel::initialize() - Error setting Baud Rate " + QString::number(busParams->baudRate));
		throw error;
	  }
	  std::cout << "JointMotor::Dynamixel::Dynamixel - BaudRate set to 38400" << std::endl;
	  break;
	case 115200:  
	  port.setBaudRate( QSerialPort::BAUD115200 ); 
	  if (port.baudRate() != QSerialPort::BAUD115200 )
	  {
		QString error("JointMotor::Dynamixel::initialize() - Error setting Baud Rate " + QString::number(busParams->baudRate));
		throw error;
	  }
	  std::cout << "JointMotor::Dynamixel::Dynamixel - BaudRate set to 115200" << std::endl;
	  break;
	default: 
	  port.setBaudRate( QSerialPort::BAUD115200 );
	   if (port.baudRate() != QSerialPort::BAUD115200 )
	  {
		QString error("JointMotor::Dynamixel::initialize() - Error setting Baud Rate " + QString::number(busParams->baudRate));
		throw error;
	  }
	  std::cout << "JointMotor::Dynamixel::Dynamixel - Warning: BaudRate set by default to 115200" << std::endl;
	  break;
  }
  
  ///Create servos instances in a QMap indexed by name
  for (int i = 0; i < busParams->numMotors; i++)
  {
	QString name = QString::fromStdString(params->operator[](i).name);
	motors[name] = new Servo( params->operator[](i) );
  }

  std::cout << "JointMotor::Dynamixel::Dynamixel - Motor Map created with " << busParams->numMotors << " motors: " << std::endl;
  foreach( Servo * s, motors)
  {
	  std::cout << "	" + s->params.name << std::endl; 
  }
  
  ///Initialize class variables
  bzero(packet, MAX_LENGTH_PACKET);
  packet[0]=0xFF;
  packet[1]=0xFF;
  
  ///Initialize motor params
  foreach( Servo *s, motors )
  {
	Servo::TMotorData &data = s->data;
	RoboCompJointMotor::MotorParams &params = s->params;
	
	std::cout << "JointMotor::Dynamixel::Dynamixel - Configuration data of motor " << params.name << std::endl;
	///Set Status return level to 1. Level 0: no response ; Level 1: only for reading commands. Default ; Level 2: always
	int level = 1;
	setStatusReturnLevel(params.busId, level);
	getStatusReturnLevel(params.busId, level);
	qDebug() << "	Status return level: " << level;
	
	///Return delay time
	int rt = 50;
	if (setReturnDelayTime( params.busId, rt) == true and getReturnDelayTime( params.busId, rt) == true)
	{
	  qDebug() << "	Return delay time: " << rt;
	}
	else
	  qDebug() << "Error setting delay time";
	
	///Control params
	int m;
	if (getPunch( params.busId, m ) == true)
	{
	  qDebug() << "	Punch: " << m;
	}
	else
	  qDebug() << "Error reading Punch";
	
	if (getCCWComplianceMargin( params.busId, m ) == true)
	{
	  qDebug() << "	CCWComplianceMargin: " << m;
	}
	else
	  qDebug() << "Error reading CCWComplianceMargin";  
	
	if (getCWComplianceMargin( params.busId, m ) == true)
	{
	  qDebug() << "	CWComplianceMargin: " << m;
	}
	else
	  qDebug() << "Error reading CWComplianceMargin";  
	if (getCCWComplianceSlope( params.busId, m ) == true)
	{
	  qDebug() << "	CCWComplianceSlope: " << m;
	}
	else
	  qDebug() << "Error reading CCWComplianceSlope";  
	
	if (getCWComplianceSlope( params.busId, m ) == true)
	{
	  qDebug() << "	CWComplianceSlope: " << m;
	}
	else
	  qDebug() << "Error reading CWComplianceSlope";  
	
	
	///Read current position
	int p;
	getPosition(params.busId, p );
	data.currentPos  = p;
	data.antPos = p;
	data.currentPosRads = s->steps2Rads(p);
	qDebug() << "	Current position (steps): " << data.currentPos;
	qDebug() << "	Current position (rads): " << data.currentPosRads;
	
	///Set limits
	setMaxPosition(params.busId, MAXSTEPSPOSITION);
	setMinPosition(params.busId, MINSTEPSPOSITION);	
	
	getMaxPosition( params.busId, p);
	qDebug() << "	Max position (steps): " << params.maxPos <<  p;
	getMinPosition( params.busId, p);
	qDebug() << "	Min position (steps): " << params.minPos << p;
	
	///set servos to maximum speed
	setVelocity( params.busId, params.maxVelocity);
	
	qDebug() << "	Max velocity " << params.maxVelocity;
	
 	setBothComplianceMargins(params.busId, 2);	
 	setBothComplianceSlopes(params.busId, 120);
//         setPunch(params.busId, 50);

  }
}

void Dynamixel::update(QMutex *mutex) throw(QString)
{
	int currentPos[busParams->numMotors];
	bool isMotorMoving[busParams->numMotors];
	int i;

	i=0;
	foreach( Servo *s, motors)
	{
// 	  Servo::TMotorData &data = s->data;
	  try
	  {
		isMotorMoving[i]=isMoving(s->params.busId);
		getPosition( s->params.busId, currentPos[i] );
/*		mutex->lock();
		data.antPos = data.currentPos;
		data.currentPos = currentPos;
		data.currentPosRads = s->steps2Rads(data.currentPos);
		data.isMoving = isMotorMoving;
		mutex->unlock();*/
	  }
	  catch(QString &s){ throw s; }  
	  i++;
	}

	mutex->lock();
	i=0;
	foreach( Servo *s, motors)
	{

	  Servo::TMotorData &data = s->data;
	  
	  data.isMoving = fabs(data.antPos - currentPos[i])>5;//isMotorMoving[i];		  
	  data.antPos = data.currentPos;
	  data.currentPos = currentPos[i];
	  data.currentPosRads = s->steps2Rads(data.currentPos);

	  i++;
	}
	mutex->unlock();
}

///////////Private methods/////////////

bool Dynamixel::sendIPacket( char *p, char length )
{
	if ( port.write( p , length) == length )
	  return true;
	else 
	  return false;
}

/// 0xFF 0xFF 
///	ID 
///	LENGTH : params +2
///	ERROR :
/*					Bit        Name                                      Details
					Bit 7         0                                          -
											Set to 1 if an undefined instruction is given without the
					Bit 6 Instruction Error
											reg_write instruction.
					Bit 5  Overload Error Set to 1 if the specified torque can't control the load.
											Set to 1 if the checksum of the intruction packet is
					Bit 4 Checksum Error
											incorrect
					Bit 3    Range Error    Set to 1 if the instruction is out of the usage range.
							Overheating    Set as 1 if the internal temperature of Dynamixel is out of
					Bit 2
								Error       the operative range as set in the control table.
											Set as 1 if the goal position is set outside of the range
					Bit 1 Angle Limit Error
											between CW Angle Limit and CCW Angle Limit
							Input Voltage   Set to 1 if the voltage is out of the operative range set in
					Bit 0r)
								Error       the control table.*/
///	PARAMS0--N 
///	CHECKSUM
/*					Check Sum = ~( ID + Length + Instruction + Parameter1 + … Parameter N )
					If the calculated value is bigger than 255, the lower byte becomes the checksum.
					~ represents the Not or complement operation*/

bool Dynamixel::getSPacket( )
{
	if ( port.read(status,5) != 5)
	  return false;
	
	int pars = status[3]-2;
	port.read(status+5,pars + 1);

 	char cs = checkSum( status );
 	if( cs != status[5+pars] )
	{ 	
		qDebug() << "Checksum error reading status packet";
		return false;
	}
	else return true;	
}

void Dynamixel::printPacket(Packet packet)
{
	int pars = packet[3]-2;

 	printf( "%d %d %d %d %d ",(uchar)packet[0],(uchar)packet[1],(uchar)packet[2],(uchar)packet[3],(uchar)packet[4]);
	for(int i=5; i< 5+pars;i++)
	{
		printf(" %d ",(uchar)packet[i]);
	}	
	printf(" %d \n", (uchar)packet[5+pars]);
}

char Dynamixel::checkSum(Packet packet)
{
	int sum = 0;
	sum = packet[2]+packet[3]+packet[4];
	for(int i=5; i < 5+packet[3]-2; i++)
		sum += packet[i];
	return (char)~sum;
}

void Dynamixel::ping( char motor )
{
	packet[2]=motor;
	packet[3]=0x02;
	packet[4]=PING;
	packet[5]=checkSum(packet);
	sendIPacket(packet, 6);
	qDebug()<<"Pinging motor " << motor;
	getSPacket( );

	if( status[4] == 0 ) qDebug() << "Ping OK from motor " << status[2];
	else qDebug() << "Ping error from motor " << status[2];
}


//////////Abstract class implementation//////////////////

/******************************************************************************/
/* Send a position read command to AI-motor                                   */
/* Input : ServoID                                                            */
/* Output : Position                                                          */
/******************************************************************************/
void Dynamixel::getPosition(uchar motor,  int &pos) throw(QString)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x24;
	packet[6]=0x02;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == false )
	  throw QString("");
	if ( getSPacket() == false )
	  throw QString("");
	else
	  pos = *((unsigned short *)(status+5));
	  
// 	if ( sendIPacket(packet, 8) == true and getSPacket() == true ) 	
// 	{
// 	  pos = *((unsigned short *)(status+5));
// 	  return true;
// 	}
// 	else
// 	  return false;
}

void Dynamixel::setPosition( uchar motor,  int pos ) throw(QString)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x1E;
	packet[6]=pos;
	packet[7]=pos>>8;
	packet[8]=checkSum(packet);

	if ( sendIPacket(packet, 9) == false )
	  throw QString("Dynamixel::setPosition() - Error writing to port");
	setVelocity(motor, 1024);

}

void Dynamixel::setSyncPosition( const QVector<Handler::GoalPosition> & goals) throw(QString)
{
	QMutexLocker locker(&mutex);
	int numMotors = goals.size();
	//Lenght = (L+1)xN+4 L:data length per motor; N number of motors
	uchar size = (4 + 1) * numMotors + 4; 
	int i,k;
	
	packet[2]=BROADCAST;
	packet[3]=size;	//Length
	packet[4]=SYNC_WRITE;
	packet[5]=0x1E;	//Start address to write data
	packet[6]=0x04;	//Length of data to write
	for(i = 7, k=0; i < 7 + (5 * numMotors); i = i + 5, k++)
	{
	  packet[i] = goals[k].busDir;	// Address of first motor
	  packet[i+1] = goals[k].position;	
	  packet[i+2] = goals[k].position>>8;
	  packet[i+3] = goals[k].maxVel;
	  packet[i+4] = goals[k].maxVel>>8;
	}
	packet[i]=checkSum(packet);

	if ( sendIPacket(packet, i+1) == false )
	  throw QString("Dynamixel::setSyncPosition() - Error writing to port");
	 
}


bool Dynamixel::getVelocity( uchar motor, int & vel )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x26;
	packet[6]=0x02;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  vel = *((unsigned short *)(status+5));
	  return true;
	}
	else
	  return false;
}

bool Dynamixel::setVelocity( uchar motor, int vel )
{
//	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x20;
	packet[6]=vel;
	packet[7]=vel>>8;
	packet[8]=checkSum(packet);
	
	if ( sendIPacket(packet, 9) == false)
	  return false;
	 
	return true;
}

bool Dynamixel::getMaxPosition( uchar motor, int & pos )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x08;
	packet[6]=0x02;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  pos = *((unsigned short *)(status+5));
	  return true;
	}
	return false;
}

bool Dynamixel::getMinPosition( uchar motor, int & pos )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x06;
	packet[6]=0x02;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  pos = *((unsigned short *)(status+5));
	  return true;
	}
	return false;
}

bool Dynamixel::setMaxPosition( uchar motor, int pos )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x08;
	packet[6]=pos;
	packet[7]=pos>>8;
	packet[8]=checkSum(packet);

	if (sendIPacket(packet, 9) == false)
	  return false;

	return true;
}

bool Dynamixel::setMinPosition( uchar motor, int pos )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x06;
	packet[6]=pos;
	packet[7]=pos>>8;
	packet[8]=checkSum(packet);

	if (sendIPacket(packet, 9) == false)
	  return false;

	return true;
}

bool Dynamixel::powerOn( uchar motor )
{
	QMutexLocker locker(&mutex);
	
	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x18;
	packet[6]=0x01;
	packet[7]=0x01;
	packet[8]=checkSum(packet);

	if ( sendIPacket(packet, 9) == false) 
	  return false;

	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x19;
	packet[6]=0x01;
	packet[7]=0x01;
	packet[8]=checkSum(packet);

	if( sendIPacket(packet, 9) == false);
	  return false;

	return true;
}

bool Dynamixel::powerOff( uchar motor )
{
	QMutexLocker locker(&mutex);
	
	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x18;
	packet[6]=0x00;
	packet[7]=0x00;
	packet[8]=checkSum(packet);

	if ( sendIPacket(packet, 9) == false) 
	  return false;

	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x19;
	packet[6]=0x00;
	packet[7]=0x00;
	packet[8]=checkSum(packet);
	
	if ( sendIPacket(packet, 9) == false) 
	  return false;

	return true;
}

bool Dynamixel::getPower( uchar motor, int & pow )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x28;
	packet[6]=0x02;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  char aux = status[6];
	  status[6] = status[6] & 0x04;
	  pow = *((unsigned short *)(status+5));
	  if((aux >> 2) == 1) 
		pow = -pow; 
	  return true;
	}
	return false;
}

bool Dynamixel::setId( uchar motor, int id )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x03;
	packet[6]=id;
	packet[7]=checkSum(packet);

	if (sendIPacket(packet, 8) == false)
	  return false;
	
	return false;
}

bool Dynamixel::setBaudrate( uchar motor, int baud )
{
	QMutexLocker locker(&mutex);
	
	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x04;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if( sendIPacket(packet, 8) == false)
	  return false;
	
	return true;
}

bool Dynamixel::getBaudrate(uchar motor, int &br)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x04;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  br = (int)status[5];
	  return(true);
	}
	return false;
}

/**
 * Sets Voltage Limit
 * @param motor 
 * @param limit 
 * @return 
 */
bool Dynamixel::setVoltageLimit( uchar motor, int limit )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x0C;
	packet[6]=limit;
	packet[7]=limit<<8;
	packet[8]=checkSum(packet);

	if( sendIPacket(packet, 8) == false)
	  return false;
	
	return true;
}

bool Dynamixel::getVoltageLimit( uchar motor, int & limit )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x0C;
	packet[6]=0x02;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  limit = status[5];
	  return true;
	}
	return false;
}

bool Dynamixel::isMoving( uchar motor )
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x2E;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == false )  //AMBIGUO
	  return false;	
 	if (status[5]==0) 
	  return false;
 	else return true;
}

bool Dynamixel::getPunch(uchar motor, int & d) //Named PUNCH in dynamixel manual
{
	QMutexLocker locker(&mutex);
	
	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x30;
	packet[6]=0x02;
	packet[7]=checkSum(packet);

	if (sendIPacket(packet, 8) == true and getSPacket() == true	 ) 
	{
	  d = *((unsigned short *)(status+5));
	  return true;
	}
	return false;
}

bool Dynamixel::setPunch(uchar motor, int d)
{
	QMutexLocker locker(&mutex);
	if ( d<32 or d> 1023 ) 
	{ 
		qWarning("Punch parameter out of range:%d. Try with a 0:16000 value",d);
		return false;
	}

	packet[2]=motor;
	packet[3]=0x05;
	packet[4]=WRITE_DATA;
	packet[5]=0x30;
	packet[6]=d;
	packet[7]=d>>8;
	packet[8]=checkSum(packet);

	if( sendIPacket(packet, 9) == false)
	  return false;

	return true;
}

bool Dynamixel::setBothComplianceSlopes(uchar motor, int p) //Compliance slope in both rotation senses
{
	QMutexLocker locker(&mutex);

	if ( p<0 or p> 255 ) 
	{ 
		qWarning("Compliance parameter out of range:%d. Try with a 0:255 value",p);
		return false;
	}
	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x1C;
	packet[6]=(uchar)p;
	packet[7]=checkSum(packet);
	sendIPacket(packet, 8);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x1D;
	packet[6]=(uchar)p;
	packet[7]=checkSum(packet);

	if( sendIPacket(packet, 8) == false )
	  return false;
	
	return true;
}


bool Dynamixel::setBothComplianceMargins(uchar motor, int db)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x1A;
	packet[6]=(uchar) db;
	packet[7]=checkSum(packet);
	
	if( sendIPacket(packet, 8) == false)
	  return false;

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x1B;
	packet[6]=(uchar) db;
	packet[7]=checkSum(packet);
	
	if( sendIPacket(packet, 8) == false)
	  return false;
	
	return true;
}

bool Dynamixel::getCWComplianceMargin(uchar motor, int & db)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x1A;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  db = (int)(status[5]);
	  return true;
	}
	return false;
}

bool Dynamixel::getCCWComplianceMargin(uchar motor, int & db)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x1B;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  db = (int)(status[5]);
	  return true;
	}
	return false;
}

bool Dynamixel::getCWComplianceSlope(uchar motor, int & db)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x1C;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  db = (int)(status[5]);
	  return true;
	}
	return false;
}

bool Dynamixel::getCCWComplianceSlope(uchar motor, int & db)
{
	QMutexLocker locker(&mutex);

	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x1D;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if( sendIPacket(packet, 8) == true and getSPacket() == true ) 
	{
	  db = (int)(status[5]);
	  return true;
	}
	return false;
}

bool Dynamixel::setReturnDelayTime( uchar motor, int t)
{
	QMutexLocker locker(&mutex);
	
	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x05;
	packet[6]=t;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == false) 
	  return false;
	
	return true;
}

bool Dynamixel::getReturnDelayTime( uchar motor, int & t)
{
  QMutexLocker locker(&mutex);

  packet[2]=motor;
  packet[3]=0x04;
  packet[4]=READ_DATA;
  packet[5]=0x05;
  packet[6]=0x01;
  packet[7]=checkSum(packet);

  if (sendIPacket(packet, 8) == true and getSPacket() == true ) 
  {
	t = (int)status[5];
	return true;
  }
 
  return false;
}

bool Dynamixel::getStatusReturnLevel(uchar motor,  int &level)
{
	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=READ_DATA;
	packet[5]=0x10;
	packet[6]=0x01;
	packet[7]=checkSum(packet);

	if ( sendIPacket(packet, 8) == true and getSPacket() == true)	
	{
	  level = status[5];
	  return true;
	}
	else 
	  return false;
}

bool Dynamixel::setStatusReturnLevel(uchar motor,  int &level)
{
	packet[2]=motor;
	packet[3]=0x04;
	packet[4]=WRITE_DATA;
	packet[5]=0x10;
	packet[6]=level;
	packet[7]=checkSum(packet);

	if( sendIPacket(packet, 8) == true)
	  return true;
	else
	  return false;
}

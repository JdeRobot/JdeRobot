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
#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include "servo.h"

/**
	*	Abstract class for servo motors handlers that allow bus access to several motors
	@author Pablo Bustos - Robolab - Uex
*/
class Handler
{
public:
    Handler(){};
    ~Handler(){};
	
	struct GoalPosition
	{
	  QString name;
	  uchar busDir;
	  int position;
	  int maxVel;
	  GoalPosition(){ name = "", busDir = 0; position = 0; maxVel = 0;};
	  GoalPosition( const QString & n, uchar d, float p, float mv): name(n), busDir(d), position(p), maxVel(mv) {};
	};
	
	struct GoalVelocity
	{
	  QString name;
	  float speed;
	  float maxAcc;
	};
	
	QString  tipo;
	virtual void initialize() throw(QString) =0;
	virtual void setPosition(uchar  motor, int pos)throw(QString) =0;
	virtual void getPosition(uchar  motor, int &pos) throw(QString) =0;
	virtual bool getVelocity(uchar  motor, int &pos)=0;
	virtual bool setVelocity(uchar  motor, int vel)=0;
	virtual bool getPower(uchar  motor, int &pow)=0;
	virtual bool getMaxPosition(uchar  motor, int &pos)=0;
	virtual bool getMinPosition(uchar  motor, int &pos)=0;
	virtual bool setMaxPosition(uchar  motor, int pos)=0;
	virtual bool setMinPosition(uchar  motor, int pos)=0;
	virtual bool isMoving(uchar  motor)=0;
	virtual bool getPGain(uchar  motor, int &p)=0;
	virtual bool getDGain(uchar  motor, int &d)=0;
	virtual bool getIGain(uchar  motor, int &i)=0;
	virtual bool setPGain(uchar  motor, int p)=0;
	virtual bool setDGain(uchar  motor, int d)=0;
	virtual bool setIGain(uchar  motor, int i)=0;
	virtual bool disablePWM(uchar  motor)=0;
	virtual bool enablePWM(uchar  motor)=0;
	virtual bool setId(uchar  motor, int id)=0;
	virtual bool reset(uchar  motor)=0;
	virtual bool restoreDefValues(uchar  motor)=0;
	virtual bool stop(uchar  motor)=0;
	virtual bool powerOff(uchar  motor)=0;
	virtual bool powerOn(uchar  motor)=0;
	virtual bool setBaudrate(uchar  motor, int baud)=0;
	virtual void setSyncPosition( const QVector<GoalPosition> & goalList ) throw(QString) = 0;
	virtual void update(QMutex *mutex) throw(QString) =0;
	
	QHash<QString, Servo*> motors;
	QMutex *mutex;
};

#endif


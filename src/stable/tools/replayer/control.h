/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Authors :
 *						Francisco Rivas <franciscomiguel.rivas@urjc.es>
 *
 */

#ifndef CONTROL_H_
#define CONTROL_H_
#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <log/Logger.h>

namespace replayer {

class control {
public:
	control(long long int initState);
	virtual ~control();
	void lock();
	void unlock();
	bool getPlay();
	long long int getSyncTime(); //tiempo origen para la sincronización relativa
	long long int getRelativeTime(); //tiempo relativo de ejecución
	long long int wait();
	void checkStatus();
	void stop();
	void resume();
	void setRepeat(bool active);
	void setProcesses(int procs);
	void setStep(int step);

private:
	//controladores de video
	bool repeat;
	//mutex de control de acceso a control
	IceUtil::Mutex controlMutex;
	//condicional utilizado como un semaforo
	IceUtil::Cond sem;
	//numero de procesos terminados
	int nProcFinished;
	//numero de procesos en total
	int nProcess;
	//nuevo tiempo para reiniciar la reproduccion
	long long int newTime;
	//nuevo tiempo relativo para reanudar la reproduccion
	long long int timeToResume;
	//reproduccion
	bool play;
	//numero de repeticiones
	bool nRepetitions;

};

} /* namespace replayer */
#endif /* CONTROL_H_ */

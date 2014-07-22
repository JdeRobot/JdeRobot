/*
 *    Copyright (C) 2009-2010 by RoboLab - University of Extremadura
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
/** \mainpage RoboComp::JointMotorComp
 *
 * \section intro_sec Introduction
 *
 * JointMotorComp controls an array of servomotors connected to a common communications bus. Provides a general API to access servo commands and maintains the state of all motor in the bus.
 *
 * \section interface_sec Interface
 *
 * 
 *
 * \section install_sec Installation
 *
 * \subsection install1_ssec Software depencences
 * JointMotorComp does not have specific software dependencies.
 *
 * \subsection install2_ssec Compile
 * cd Components/Robolab/Experimental/jointmotorComp
 * <br>
 * cmake . && make
 * <br>
 * To install:
 * sudo make install
 *
 * \subsection install2_ssec Compile and install
 * cd Components/Robolab/Experimental/jointnmotorComp
 *
 *
 * \section guide_sec User guide
 *
 *
 *
 * \subsection config_ssec Configuration file
 *
 * <p>
 * 
 * </p>
 *
 * \subsection execution_ssec Execution
 *
 * Just: "${PATH_TO_BINARY}/jointmotorComp --Ice.Config=${PATH_TO_CONFIG_FILE}"
 *
 * \subsection running_ssec Once running
 *
 * 
 *
 */


// QT includes
#include <QtCore>
#include <QtGui>

// ICE includes
#include <Ice/Ice.h>
#include <Ice/Application.h>

#include <rapplication/rapplication.h>

// View the config.h file for config options like
// enable/disable server, QtGui, etc...
#include "config.h"

#include "worker.h"
#include "monitor.h"

#ifdef SERVER
// Interface implementation
#include "jointmotorI.h"
#endif

// Includes for remote proxy example
// #include <Remote.h>


// User includes here

// Namespaces
using namespace std;
using namespace RoboCompJointMotor;

class JointMotorComp : public RoboComp::Application
{
private:
	MotorParamsList params;
	BusParams busParams;
	void initialize();
	int numMotors;
	string paramsStr;

public:
	virtual int run(int, char*[]);
};

void JointMotorComp::initialize()
{
}

int JointMotorComp::run(int argc, char* argv[])
{
#ifdef USE_QTGUI
	QApplication a(argc, argv);  // GUI application
#else
	QCoreApplication a(argc, argv);  // NON-GUI application
#endif
	int status=EXIT_SUCCESS;

	// Remote server proxy access example
	// RemoteComponentPrx remotecomponent_proxy;

	string proxy;

	// User variables

	initialize();

	//Semaphore to synchronize Monitor and Worker
	QWaitCondition *PARAMETERS_SET_WAIT_CONDITION = new QWaitCondition();
	//Woker thread
	Worker *worker = new Worker( &params, &busParams, PARAMETERS_SET_WAIT_CONDITION );
	worker->start();
	//Monitor thread
	Monitor *monitor = new Monitor( &params, &busParams , communicator(), PARAMETERS_SET_WAIT_CONDITION);
	monitor->start();

	usleep(10000);
    if ( worker->isFinished() )
    {
      std::cout << "worker->isFinished()" << std::endl;
      return status;
    }
    else
    	std::cout << "worker not finished" << std::endl;

	//Start up de ther server
	try
	{
		// Server adapter creation and publication
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("JointMotorComp");
		JointMotorI *jointmotorI = new JointMotorI(worker );
		adapter->add(jointmotorI, communicator()->stringToIdentity("jointmotor"));
		//CommonBehaviorI *commonbehaviorI = new CommonBehaviorI( monitor );
		//adapter->add(commonbehaviorI, communicator()->stringToIdentity("commonbehavior"));
		adapter->activate();

		cout << SERVER_FULL_NAME " started" << endl;

		// User defined QtGui elements ( main window, dialogs, etc )

#ifdef USE_QTGUI
		//ignoreInterrupt(); // Uncomment if you want the component to ignore console SIGINT signal (ctrl+c).
		a.setQuitOnLastWindowClosed( true );
#endif
		// Run QT Application Event Loop
		a.exec();
		status = EXIT_SUCCESS;
	}
	catch(const Ice::Exception& ex)
	{
		status = EXIT_FAILURE;

		cout << "[" << PROGRAM_NAME << "]: Exception raised on main thread: " << endl;
		cout << ex;

#ifdef USE_QTGUI
		a.quit();
#endif
	}

	return status;
}

int main(int argc, char* argv[])
{
	bool hasConfig = false;
	string arg;
	JointMotorComp app;

	// Search in argument list for --Ice.Config= argument
	for (int i = 1; i < argc; ++i)
	{
		arg = argv[i];
		if ( arg.find ( "--Ice.Config=", 0 ) != string::npos )
			hasConfig = true;
	}

	if ( hasConfig )
		return app.main( argc, argv );
	else
		return app.main(argc, argv, "config"); // "config" is the default config file name
}

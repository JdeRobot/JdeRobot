/*
 *  Copyright (C) 1997-2012 JDE Developers Teameldercare.camRGB
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
			
 */

/** \file remoteConfiguration.cpp
 * \brief remoteConfiguration component master file
 */


#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/remoteConfig.h>
#include <gtkmm.h>
#include <libglademm.h>
#include <gtkmm/main.h>
#include "myparser.h"



void *gui_thread(void* arg){

	try{
		while(1){
				//eldercaregui_ptx->update();
			usleep(100);
		}
	
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
	}
	pthread_exit(NULL);
}






/**
 * \brief Main program function code
 */
int main(int argc, char** argv){

	int status,i;
	Ice::CommunicatorPtr ic;
	int n_components=0;
	Ice::PropertiesPtr prop;
	std::string path;
	jderobot::remoteConfigPrx configPrx;

	try{
		ic = Ice::initialize(argc,argv);
		prop = ic->getProperties();
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return 1;
	}
	catch (const char* msg) {
		std::cerr <<"Error :" << msg << std::endl;
		return 1;
	}
	path=prop->getProperty("remoteConfiguration.Path");
	try{
		Ice::ObjectPrx baseConfig= ic->propertyToProxy("remoteConfiguration.Proxy");
		if (0==baseConfig){
			throw "remoteConfiguration: Could not create proxy with remoteConfiguration";
		}
		else {
			configPrx= jderobot::remoteConfigPrx::checkedCast(baseConfig);
			if (0==configPrx)
				throw "Invalid proxy eldercare.remoteConfiguration.Proxy";
		}
	}
	catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		status = 1;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		std::cout << "remoteConfigturation: Not remoteConfiguration provided" << std::endl;
		status = 1;
	}

	int id=configPrx->initConfiguration();
	std::cout << "id: " << id << std::endl;
	/*configPrx->write("hola mundo",id);
	configPrx->setConfiguration(id);
	*/

	pthread_t thread_gui;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&thread_gui, &attr, gui_thread,NULL);


	//Gtk::Window mainwindow;
	//mainwindow.show();

	Gtk::Main kit(argc, argv);

 	//MyWindow myw;
	eldercare::myparser myp(configPrx, id);
	myp.parsePath(path);
  	//Shows the window and returns when it is closed.
  	Gtk::Main::run(myp);


	pthread_join(thread_gui, NULL);
		

	if (ic)
		ic->destroy();
	return status;
}

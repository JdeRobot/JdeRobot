/*
 *
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Author : Roberto Calvo Palomino <rocapal@gmail.com>
 *
 */


#include <iostream>
#include <fstream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/recorder.h>
#include <jderobot/recording.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>



int main(int argc, char** argv){
  int status;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);

    // Get Proxy to RecordingManager
    Ice::ObjectPrx base = ic->propertyToProxy("Surveillance.RecordingManager.Proxy");
    if (0==base)
      throw "Could not create proxy (RecordingManager)";

    //cast to RecordingManagerPrx
    jderobot::RecordingManagerPrx recManagerPrx = jderobot::RecordingManagerPrx::checkedCast(base);
    if (0==recManagerPrx)
      throw "Invalid proxy (RecordingManager)";

    //Get properties

    Ice::PropertiesPtr prop = ic->getProperties();

    std::string dirRecordings = prop->getProperty("Surveillance.dirRecordings");
    std::string namePatter = prop->getProperty("Surveillance.namePattern");

    std::string nameRecording = dirRecordings + namePatter;

    char nameFile[nameRecording.length()+1];

    for (;;)
    {
		strncpy(nameFile,nameRecording.c_str(),nameRecording.length());
		nameFile[nameRecording.length()]='\0';

		int res = mkstemp(nameFile);
		if (res==-1)
		{
				std::cerr << "Error in mktemp: Don't create fileName!" << std::endl;
				return -1;
		}

		// Set the recording config
		jderobot::RecorderConfigPtr recConfig = new  jderobot::RecorderConfig();
		recConfig->name = prop->getProperty("Surveillance.name");
		recConfig->v4lVersion = prop->getProperty("Surveillance.v4lVersion");
		recConfig->v4lDevice = prop->getProperty("Surveillance.v4lDevice");
		recConfig->frameRate = prop->getProperty("Surveillance.fps");
		recConfig->height = prop->getProperty("Surveillance.height");
		recConfig->width = prop->getProperty("Surveillance.width");
		recConfig->path = nameFile;

		std::string timeRecording = prop->getProperty("Surveillance.duration");

		recConfig->duration = atoi(timeRecording.c_str())*60;

		int recId = recManagerPrx->startRecording(recConfig);

		std::cout << " [*] New Recording launched, PID = " << recId << " - " + timeRecording << " min." << std::endl;

		sleep( recConfig->duration );

		recManagerPrx->stopRecording(recId);
    }


  }catch (const Ice::Exception& ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }

  if (ic)
    ic->destroy();

  return status;
}



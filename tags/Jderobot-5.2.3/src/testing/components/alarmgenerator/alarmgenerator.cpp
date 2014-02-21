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
#include <jderobot/varcolor.h>
#include <jderobot/recording.h>
#include <stdlib.h>

int main(int argc, char** argv){
  int status;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);

    // Get Proxy to RecordingManager
    Ice::ObjectPrx base = ic->propertyToProxy("AlarmGenerator.RecordingManager.Proxy");
    if (0==base)
      throw "Could not create proxy (recordingManager)";

    /*cast to RecordingManagerPrx*/
    jderobot::RecordingManagerPrx rm_prx = jderobot::RecordingManagerPrx::checkedCast(base);
    if (0==rm_prx)
      throw "Invalid proxy (recordingManager)";

    // Get Proxy to VarColorServer
    Ice::ObjectPrx base2 = ic->propertyToProxy("AlarmGenerator.VarColor.Proxy");
    if (0==base)
          throw "Could not create proxy (VarColorServer)";

    /*cast to VarColorPrx*/
    jderobot::VarColorPrx vprx = jderobot::VarColorPrx::checkedCast(base2);
    if (0==vprx)
      throw "Invalid proxy (varColorServer)";


    for (;;)
    {


    	//Get Image to varColorServer
    	jderobot::ByteSeq imageVector = vprx->getData()->pixelData;

    	std::cout << imageVector.size() << std::endl;

    	std::cout << sizeof(imageVector[0]) << std::endl;
    	std::cout << sizeof(unsigned char) << std::endl;

    	std::cout << std::hex << "0x" << (unsigned short) imageVector[0] << std::endl;
    	std::cout << std::hex << "0x" << (unsigned short) imageVector[1] << std::endl;

    	// Save std:vector <bytes> in file
    	char nameFile[] = "/tmp/imgXXXXXX";
    	int res;
		std::ofstream imageFile;

		res = mkstemp(nameFile);
		if (res==-1)
		{
			std::cerr << "Error in mktemp: Don't create fileName!" << std::endl;
			continue;
		}

		imageFile.open (nameFile, std::ios::out | std::ios::binary);

		imageFile.write (reinterpret_cast<char *>(&imageVector[0]), imageVector.size () * sizeof(Ice::Byte));

		imageFile.close();

    	// Build the Event
    	jderobot::RecordingEventPtr event = new jderobot::RecordingEvent();

		event->id = 12;
		event->type = "1";
		event->producer = "alarmgenerator";
		event->resource = "cam1";
	    event->comment = "test alarm generator";
	    event->pathImage = nameFile;

	    // Send event to RecordingManager
    	rm_prx->setEvent(event, 2);

    	std::cout << "Alarm sended!. Waiting 60 seconds for the next alarm ..." << std::endl;

    	sleep(15);

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

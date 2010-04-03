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
#include <jderobot/recorder.h>
#include <stdlib.h>

int main(int argc, char** argv){
  int status;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);

    // Get Proxy to RecordingManager
    Ice::ObjectPrx base = ic->propertyToProxy("RecorderApp.Recorder.Proxy");
    if (0==base)
      throw "Could not create proxy (recorder)";

    /*cast to RecordingManagerPrx*/
    jderobot::RecorderPrx recorderPrx = jderobot::RecorderPrx::checkedCast(base);
    if (0==recorderPrx)
      throw "Invalid proxy (recorder)";


    jderobot::RecorderConfigPtr recConfig = new  jderobot::RecorderConfig();
    recConfig->path = "/tmp/tmp";
    recConfig->frameRate = "25.0";
    recConfig->time = 60;

    recorderPrx->startRecording(recConfig);


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

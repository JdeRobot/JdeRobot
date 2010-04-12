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


int main(int argc, char** argv){
  int status;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);

    // Get Proxy to RecordingManager
    Ice::ObjectPrx base = ic->propertyToProxy("Surveillance.RecordingManager.Proxy");
    if (0==base)
      throw "Could not create proxy (RecordingManager)";

    /*cast to RecordingManagerPrx*/
    jderobot::RecordingManagerPrx recManagerPrx = jderobot::RecordingManagerPrx::checkedCast(base);
    if (0==recManagerPrx)
      throw "Invalid proxy (RecordingManager)";


    // Set the recording config
    jderobot::RecorderConfigPtr recConfig = new  jderobot::RecorderConfig();
    recConfig->v4lVersion = "v4l";
    recConfig->v4lDevice = "/dev/video0";
    recConfig->frameRate = "12.5";
    recConfig->height = "240";
    recConfig->width = "320";
    recConfig->path = "/tmp/video.mpg";
    recConfig->time = 60;

    int ret = recManagerPrx->startRecording(recConfig);

    std::cout << "startRecording, ret = " << ret << std::endl;

    sleep(10);

    recManagerPrx->stopRecording(ret);


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

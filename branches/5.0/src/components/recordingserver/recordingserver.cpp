/*
 *
 *  Copyright (C) 1997-2009 JDE Developers Team
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

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/recording.h>
#include "libRecordingLog/RecordingLog.h"

class RecordingI: public jderobot::RecordingManager
{
public:
	  RecordingI(std::string& prefix, Ice::CommunicatorPtr& communicator)
	    :prefix(prefix),communicator(communicator), recLog(NULL)
	  {

	  }

	  virtual jderobot::RecordingSequence getRecordings(const Ice::Current& c){

		  RecordingLog* recLog = initRecordingHandler();

		  return recLog->getAllRecording();

	  }

	  virtual jderobot::EventSequence getEventsOfRecording(Ice::Int recordingId, const Ice::Current& c)
	  {

		  RecordingLog* recLog = initRecordingHandler();

		  return recLog->getEventsOfRecording(recordingId);
	  }

	  virtual Ice::Int setEvent (const jderobot::RecordingEventPtr& recEvent, Ice::Int recordingId, const Ice::Current& c)
	  {

		  RecordingLog* recLog = initRecordingHandler();

		  return recLog->saveRecordingEvent(recEvent, recordingId);
	  }

	  virtual jderobot::RecordingEventPtr getEvent(Ice::Int eventId, const Ice::Current&)
	  {
		  return recLog->getEvent (eventId);
	  }


private:

	  RecordingLog* initRecordingHandler()
	  {
		  if (recLog == NULL)
		  {
			  recLog = new RecordingLog();
			  recLog->connect(static_cast<string>("jderobot"),
					          static_cast<string>("localhost"),
					          static_cast<string>("root"),
					          static_cast<string>(""));
		  }

		  return recLog;
	  }

	  RecordingLog* recLog;
	  std::string prefix;
	  Ice::CommunicatorPtr communicator;
};


class RecordingSrvApp: public virtual Ice::Application{
public:
	RecordingSrvApp() :Ice::Application() {}

  virtual int run(int, char*[]) {

    std::string srvName = "RecordingSrv";
    Ice::CommunicatorPtr comm = communicator();
    Ice::PropertiesPtr prop = comm->getProperties();

    /*adapter to keep all the objects*/
    Ice::ObjectAdapterPtr adapter = comm->createObjectAdapter(srvName);

    /*VarColorI object, added with name varcolorA*/
    std::string objPrefix = srvName + ".Recording.";

    try{
      Ice::ObjectPtr object = new RecordingI(objPrefix,comm);

      adapter->add(object,
		   comm->stringToIdentity(prop->getPropertyWithDefault(objPrefix+"Id",
								       "recordManager1")));

      adapter->activate();
      comm->waitForShutdown();

    }catch(jderobot::JderobotException e){
      std::cerr << "Exception raised: " << e.what << std::endl;
    }
    if (interrupted())
      std::cerr << appName() << ": received signal, shutting down" << std::endl;
    return 0;
  }
};

int main(int argc, char** argv){
	RecordingSrvApp app;

  app.main(argc,argv);
}

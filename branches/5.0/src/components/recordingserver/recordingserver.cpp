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

int descPipe [2];

class RecordingI: public jderobot::RecordingManager
{
public:
	  RecordingI(std::string& prefix, Ice::CommunicatorPtr& communicator)
	    :prefix(prefix),communicator(communicator), recLog(NULL), mRecorderPrx(NULL)
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


	  virtual Ice::Int startRecording(const jderobot::RecorderConfigPtr& recConfig, const Ice::Current&)
	  {
		  recLog = initRecordingHandler();

		  std::cout << &recConfig << std::endl;

		  jderobot::AMI_Recorder_startRecordingPtr cb = new AMI_Recorder_startRecordingI;
		  getRecorderProxy()->startRecording_async(cb, recConfig);

		  int pid_rec;

		  // Read the PID
		  read (descPipe[0], &pid_rec, sizeof(int));

		  std::cout << "Receive PID by pipe: " << pid_rec << std::endl;
		  recConfig->id = pid_rec;

		  // Log recording
		  recLog->startRecording(recConfig);

		  IceUtil::Mutex::Lock sync(listMutex);
		  recList.push_back(recConfig);

		  return pid_rec;
	  }

	  virtual Ice::Int stopRecording(Ice::Int recId, const Ice::Current&)
	  {
		  recLog = initRecordingHandler();

		  std::cout << "Recording with PID: " << recId << " is being stopping!" << std::endl;
		  if (recId>0)
		  {
			  getRecorderProxy()->stopRecording(recId);
			  recLog->endRecording(recId);
		  }
	  }

private:

	  IceUtil::Mutex listMutex;
	  std::vector<jderobot::RecorderConfigPtr> recList;

	  // Private class to obtain the response of recording
	  class AMI_Recorder_startRecordingI : public jderobot::AMI_Recorder_startRecording
	  {
		  public:

			  virtual void ice_response(const jderobot::RecorderConfigPtr& recConfig)
			  {
				  std::cout << "Recording with PID: " << recConfig->id  << " starting correctly!" << std::endl;

				  int rec_id = recConfig->id;

				  std::cout << "Send PID by pipe: " << rec_id << std::endl;

				  write (descPipe[1],&rec_id, sizeof(int));

			  }

			  virtual void ice_exception(const Ice::Exception& ex)
			  {
				  try {
					  ex.ice_throw();
				  } catch (const Ice::LocalException& e) {
					  std::cerr << "recorder failed: " << e << std::endl;
				  }
			  }
	  };



	  // Private method to obtain the proxy to recorder component
	  jderobot::RecorderPrx getRecorderProxy ()
	  {
		  if (mRecorderPrx != NULL)
			  return mRecorderPrx;

		  // Get Proxy to RecordingManager
		  Ice::ObjectPrx base = communicator->propertyToProxy("RecordingSrv.Recorder.Proxy");
		  if (0==base)
			throw "Could not create proxy (recorder)";

		  /* Cast to RecordingManagerPrx */
		  mRecorderPrx = jderobot::RecorderPrx::checkedCast(base);
		  if (0==mRecorderPrx)
			throw "Invalid proxy (recorder)";


		  return mRecorderPrx;
	  }

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

	  jderobot::RecorderPrx mRecorderPrx;
	  RecordingLog* recLog;
	  std::string prefix;
	  Ice::CommunicatorPtr communicator;
};


class RecordingSrvApp: public virtual Ice::Application{
public:
	RecordingSrvApp() :Ice::Application() {}

  virtual int run(int, char*[]) {

	pipe (descPipe);

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

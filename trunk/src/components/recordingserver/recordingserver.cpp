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
#include <jderobot/recorder.h>
#include "libRecordingLog/RecordingLog.h"

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int descPipe [2];

class RecordingI: public jderobot::RecordingManager
{
public:
	  RecordingI(std::string& prefix, Ice::CommunicatorPtr& communicator)
	    :prefix(prefix),communicator(communicator), recLog(NULL), mRecorderPrx(NULL), mStreamingURI(), mStreamingPort(11000)
	  {

	  }

	  virtual jderobot::RecordingSequence getRecordings(Ice::Int from, Ice::Int elems, const Ice::Current& c){

		  RecordingLog* recLog = initRecordingHandler();

		  return recLog->getAllRecording(from, elems);
	  }

	  virtual jderobot::RecordingSequence getRecordingsByDate(const std::string& date, Ice::Int from, Ice::Int elems, const Ice::Current& c)
	  {

		  RecordingLog* recLog = initRecordingHandler();

		  return recLog->getRecordingsByDate(date,from,elems);

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

		  jderobot::AMI_Recorder_startRecordingPtr cb = new AMI_Recorder_startRecordingI;
		  getRecorderProxy()->startRecording_async(cb, recConfig);

		  int pid_rec;

		  // Read the PID
		  read (descPipe[0], &pid_rec, sizeof(int));

		  recConfig->id = pid_rec;

		  // Log recording
		  int recordingId = recLog->startRecording(recConfig);

		  std::cout << " [*] Recording with ID: " << recordingId  << "(" << pid_rec << ")" << " starting correctly!" << std::endl;

		  return recordingId;
	  }

	  virtual Ice::Int stopRecording(Ice::Int recId, const Ice::Current& c)
	  {
		  recLog = initRecordingHandler();

		  std::cout << " [*] Recording ID: " << recId << " is being stopping!\n" << std::endl;

		  int pid = recLog->getRecordingPID(recId);

		  std::cout << "stopRecording = " << pid << std::endl;

		  if (pid != -1)
		  {
			  // Generate Thumb
			  getThumbRecording(recId, c);

			  // Stop the recording
			  getRecorderProxy()->stopRecording(pid);

			  // Save data of recording
			  recLog->endRecording(recId);


			  return 0;
		  }

		  return -1;

	  }

	  virtual string startStreaming (Ice::Int id, const Ice::Current& c)
	  {

		  recLog = initRecordingHandler();

		  jderobot::RecorderConfigPtr myRec = recLog->getRecording(id);

		  if (myRec != NULL)
		  {
			  // Launch VLC with the video streaming
			  string vlc_command = "vlc -vvv " + myRec->path +" --play-and-exit -I dummy --sout '#transcode{vcodec=mp4v,acodec=aac}:rtp{dst=0.0.0.0,port=1234,sdp=" + getStreamingURI() +"}' & ";

			  std::cout << vlc_command << std::endl;

			  //FIXME:
			  system("killall -9 vlc");

			  int ret = system (vlc_command.c_str());

			  if (ret != -1)
				  return getStreamingURI();
			  else
				  return NULL;

		  }

		  return NULL;
	  }

	  virtual jderobot::ByteSeq getThumbRecording (Ice::Int id,  const Ice::Current&)
	  {

		  jderobot::ByteSeq emptyVector;

		  recLog = initRecordingHandler();

		  jderobot::RecorderConfigPtr myRec = recLog->getRecording(id);

		  if (myRec != NULL)
		  {
			  size_t found;
			  found = (myRec->path).find_last_of("/");

			  string dirThumb = (myRec->path).substr(0,found+1) + "thumbs/";
			  string thumb = dirThumb + (myRec->path).substr(found+1) + ".png";

			  struct stat mystat;

			  if (stat(thumb.c_str(), &mystat) == -1)
			  {
				  // Create thumb
				  string command = "ffmpeg -i " + myRec->path + " -r 1 -t 00:00:01 /tmp/thumb.jpg && convert /tmp/thumb.jpg " + thumb;
				  system (command.c_str());

			  }

			  // Read File
			  ifstream is;
			  char * buffer;

			  is.open (thumb.c_str(), ios::binary );

			  if (!is.is_open())
			  {
				  jderobot::ByteSeq thumbVector;
				  return thumbVector;
			  }

			  is.seekg (0, ios::end);
			  int length = is.tellg();
			  is.seekg (0, ios::beg);

			  // allocate memory:
			  buffer = new char [length];

			  // read data as a block:
			  is.read (buffer,length);
			  is.close();

			  //Convert to ByteSeq
			  jderobot::ByteSeq thumbVector (&buffer[0], &buffer[length]);
			  delete[] buffer;

			  return thumbVector;
		  }



		  return emptyVector;
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

				  int rec_id = recConfig->id;

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


	  string getStreamingURI ()
	  {
		  if (mStreamingURI.empty())
			  mStreamingURI = communicator->getProperties()->getProperty("RecordingSrv.StreamingUri");


		  return mStreamingURI;
	  }


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

	  string mStreamingURI;
	  jderobot::RecorderPrx mRecorderPrx;
	  RecordingLog* recLog;
	  std::string prefix;
	  Ice::CommunicatorPtr communicator;

	  int mStreamingPort;
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

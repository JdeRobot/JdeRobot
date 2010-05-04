/*
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

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/recorder.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <list>

#include "deviceRecorder.h"
#include "imageRecorder.h"

namespace RecorderProcess {

	class RecorderI: public jderobot::Recorder
	{
		public:
		  RecorderI(std::string& prefix,
					const jderobotice::Context& context)
			:prefix(prefix), context(context), mProvider()
		  {

			  Ice::PropertiesPtr prop = context.properties();
			  std::string objPrefix(context.tag() + ".Recorder.");
			  std::string recorderName = prop->getProperty(objPrefix + "Provider");

			  if (recorderName.compare("ffmpeg") == 0)
			  {
				  mProvider = RECORDING_PROVIDER_FFMPEG;
			  }
			  else if (recorderName.compare("mencoder") == 0)
			  {
				  mProvider = RECORDING_PROVIDER_MENCODER;
			  }
			  else if (recorderName.compare("mplayer") == 0)
			  {
				  mProvider = RECORDING_PROVIDER_MPLAYER;
			  }
			  else if (recorderName.compare("vlc") == 0)
			  {
				  mProvider = RECORDING_PROVIDER_VLC;
			  }
			  else
			  {
				 throw "Recording Provider Unknow!";
			  }

		  }


		  virtual jderobot::RecorderConfigPtr startRecording(const jderobot::RecorderConfigPtr& recConfig,
															 const Ice::Current& c)
		  {

			  jderobot::RecorderConfigPtr rec = new jderobot::RecorderConfig();

			  GenericRecorder* myRecorder = NULL;

			  // Initialize and launch the Recorder
			  if ( (recConfig->protocol).find(RECORDING_PROTOCOL_V4L) != std::string::npos)
			  {
				  // V4L & V4L2 protocol
				  myRecorder = new deviceRecorder (context,recConfig,mProvider);
				  myRecorder->startRecording();

			  }
			  else if ( (recConfig->protocol).compare(RECORDING_PROTOCOL_CAMERASERVER) == 0)
			  {
				  // cameraServer Protocol

				  myRecorder = new imageRecorder (context,recConfig,RECORDING_PROVIDER_MENCODER);
				  myRecorder->startRecording();

			  }
			  else
			  {
				  context.tracer().error("Recording Protocol Unknow: " + recConfig->protocol);
				  rec = recConfig;
				  rec->id = -1;
			  }

			  // The recorder is saved in a list.

			  if (myRecorder != NULL)
			  {
				  IceUtil::Mutex::Lock sync(listMutex);
				  recList.push_back(myRecorder);

				  // Return recordingId
				  jderobot::RecorderConfigPtr rec = new jderobot::RecorderConfig();

				  rec = recConfig;
				  rec->id = myRecorder->getId();

				  return rec;
			  }
			  else
			  {
				  //Error
				  context.tracer().error("Impossible initialize the recorder!");
				  rec = recConfig;
				  rec->id = -1;

				  return rec;
			  }
		  }

		  virtual Ice::Int stopRecording (Ice::Int recId, const Ice::Current& c)
		  {
			  IceUtil::Mutex::Lock sync(listMutex);


			  for (unsigned int i=0; i<recList.size(); i++)
			  {

				  // std::cout << "recList[i]->getId(): " << recList[i]->getId() << " - recId: " << recId << std::endl;

				  if (recList[i]->getId() == recId)
				  {
					  context.tracer().info("Recorder::stopRecording - Stopping recording ... ");

					  recList[i]->stopRecording();
					  delete (recList[i]);
					  recList.erase(recList.begin()+i);

					  return 0;
				  }
			  }

			  context.tracer().info("Recorder::stopRecording - Warning: the recordId was not found!");

			  return 1;
		  }

		private:

		  std::string prefix;
		  jderobotice::Context context;

		  IceUtil::Mutex listMutex;
		  std::vector<GenericRecorder*> recList;

		  int mProvider;

	};


	class Component: public jderobotice::Component
	{
	public:

		Component():jderobotice::Component("RecorderApp") {}

		virtual void start()
		{

			Ice::PropertiesPtr prop = context().properties();

			std::string objPrefix(context().tag() + ".Recorder.");
			std::string recorderName = prop->getProperty(objPrefix + "Name");

			//set the value
			prop->setProperty(objPrefix + "Name", recorderName);

			context().tracer().info("Creating recorder (" + objPrefix + "Name" +") " + recorderName);

			objRecorder = new RecorderI(objPrefix,context());
			context().createInterfaceWithString(objRecorder,recorderName);


		}

		virtual ~Component()
		{
		}

	private:

		Ice::ObjectPtr objRecorder;
	};

} //namespace

int main(int argc, char** argv)
{

	RecorderProcess::Component component;

	jderobotice::Application app(component);
	return app.jderobotMain(argc,argv);
}


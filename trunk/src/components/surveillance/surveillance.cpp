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
#include <IceUtil/Thread.h>

#include <jderobot/recorder.h>
#include <jderobot/recording.h>
#include <jderobot/surveillance.h>

#include <jderobotice/component.h>
#include <jderobotice/application.h>


std::string tag = "Surveillance";

/*
void *do_recordings_thread(void *arg)
{

	int *objId = (int*) (arg);
	std::stringstream objIdS;
	objIdS <<  *objId;

	std::cout << "Starting thread recording for cameraServer (" << objIdS.str() << ")" << std::endl;

	Ice::ObjectPrx base = ic->propertyToProxy(tag + "." + objIdS.str() +".RecordingManager.Proxy");
	if (0==base)
		throw "Could not create proxy (RecordingManager)";

	//cast to RecordingManagerPrx
	jderobot::RecordingManagerPrx recManagerPrx = jderobot::RecordingManagerPrx::checkedCast(base);
	if (0==recManagerPrx)
		throw "Invalid proxy (RecordingManager)";


	std::string dirRecordings = prop->getProperty(tag + "." + objIdS.str() + ".dirRecordings");
	std::string namePatter = prop->getProperty(tag + "." + objIdS.str() +".namePattern");

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
				throw "Error in mktemp:";
		}

		// Set the recording config
		jderobot::RecorderConfigPtr recConfig = new  jderobot::RecorderConfig();
		recConfig->name = prop->getProperty(tag + "." + objIdS.str() +".name");
		recConfig->protocol = prop->getProperty(tag + "." + objIdS.str() +".protocol");
		recConfig->device = prop->getProperty(tag + "." + objIdS.str() +".device");
		recConfig->frameRate = prop->getProperty(tag + "." + objIdS.str() +".fps");
		recConfig->height = prop->getProperty(tag + "." + objIdS.str() +".height");
		recConfig->width = prop->getProperty(tag + "." + objIdS.str() +".width");
		recConfig->path = nameFile;

		std::string timeRecording = prop->getProperty(tag + "." + objIdS.str() +".duration");

		recConfig->duration = atoi(timeRecording.c_str())*60;

		int recId = recManagerPrx->startRecording(recConfig);

		std::cout << " [*] New Recording launched, with ID = " << recId << " - " + timeRecording << " min." << std::endl;

		sleep( atoi(timeRecording.c_str()) * 60 );

		recManagerPrx->stopRecording(recId);
	}


	pthread_exit(0);
}


*/

namespace SurveillanceApp {

	class SurveillanceI: public jderobot::Surveillance
	{
		public:
			SurveillanceI(std::string& prefix,const jderobotice::Context& context) :prefix(prefix), context(context)
			{

			}

			virtual Ice::Int notifyEvent(const jderobot::RecordingEventPtr& recEvent, const Ice::Current& c)
			{
				return 0;
			}


		private:

		  std::string prefix;
		  jderobotice::Context context;


	};

	class Component: public jderobotice::Component
	{
	public:

		Component():jderobotice::Component("Surveillance") {}

		virtual void start()
		{
			Ice::PropertiesPtr prop = context().properties();

			std::string objPrefix(context().tag() + ".App.");
			std::string surveillanceName = prop->getProperty(objPrefix + "Name");


			/* Create threads for recordings */

		    //Get properties

		    int nRecordings = prop->getPropertyAsInt(context().tag() + ".NRecordings");

		    for (int i=0; i<nRecordings; i++)
		    {
				// run thread
		    	//pthread_create(&pt_rec_id,NULL,do_recordings_thread,(void*) &i);

		    	PetitionThread* pt = new PetitionThread(i, context(), context().tag());
		    	pt->start();

		    	threads.push_back(pt);
		    }


			/* Create objetct to listen de events */

			//set the value
			prop->setProperty(objPrefix + "Name", surveillanceName);

			context().tracer().info("Creating SurveillaceI (" + objPrefix + "Name" +") " + surveillanceName);

			objSurveillance = new SurveillanceI (objPrefix,context());
			context().createInterfaceWithString(objSurveillance,surveillanceName);


		}

		virtual ~Component()
		{

			for (int i=0; i<threads.size(); i++)
			{
				context().tracer().info("Surveillace:: free memory ....");
				delete(threads[i]);
			}

			threads.clear();
		}

	private:

		Ice::ObjectPtr objSurveillance;
		std::vector<IceUtil::Thread*> threads;
		std::vector<IceUtil::Thread*>::iterator it;

		class PetitionThread : public IceUtil::Thread {

			public:
				PetitionThread (int numRecording, jderobotice::Context context, std::string tag )
						: mNumRecording(numRecording), mContext(context), mTag(tag)
				{

				}

				virtual void run()
				{
					std::stringstream objIdS;
					objIdS <<  mNumRecording;

					std::cout << "Starting thread recording for RecordingServer (" << objIdS.str() << ")" << std::endl;

					Ice::CommunicatorPtr ic = mContext.communicator();
					Ice::PropertiesPtr prop = mContext.properties();

					Ice::ObjectPrx base = ic->propertyToProxy(mTag + "." + objIdS.str() +".RecordingManager.Proxy");
					if (0==base)
						throw "Could not create proxy (RecordingManager)";

					//cast to RecordingManagerPrx
					jderobot::RecordingManagerPrx recManagerPrx = jderobot::RecordingManagerPrx::checkedCast(base);
					if (0==recManagerPrx)
						throw "Invalid proxy (RecordingManager)";


					std::string dirRecordings = prop->getProperty(mTag + "." + objIdS.str() + ".dirRecordings");
					std::string namePatter = prop->getProperty(mTag + "." + objIdS.str() +".namePattern");

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
								throw "Error in mktemp:";
						}

						// Set the recording config
						jderobot::RecorderConfigPtr recConfig = new  jderobot::RecorderConfig();
						recConfig->name = prop->getProperty(mTag + "." + objIdS.str() +".name");
						recConfig->protocol = prop->getProperty(mTag + "." + objIdS.str() +".protocol");
						recConfig->device = prop->getProperty(mTag + "." + objIdS.str() +".device");
						recConfig->frameRate = prop->getProperty(mTag + "." + objIdS.str() +".fps");
						recConfig->height = prop->getProperty(mTag + "." + objIdS.str() +".height");
						recConfig->width = prop->getProperty(mTag + "." + objIdS.str() +".width");
						recConfig->path = nameFile;

						std::string timeRecording = prop->getProperty(mTag + "." + objIdS.str() +".duration");

						recConfig->duration = atoi(timeRecording.c_str())*60;

						int recId = recManagerPrx->startRecording(recConfig);

						std::cout << " [*] New Recording launched, with ID = " << recId << " - " + timeRecording << " min." << std::endl;

						sleep( atoi(timeRecording.c_str()) * 60 );

						recManagerPrx->stopRecording(recId);
					}
				}

			private:

				int mNumRecording;
				jderobotice::Context mContext;
				std::string mTag;

		};

	};



}  // Namespace


int main(int argc, char** argv){


    SurveillanceApp::Component component;

    jderobotice::Application app(component);
    return app.jderobotMain(argc,argv);


}



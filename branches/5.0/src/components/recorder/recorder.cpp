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


class RecorderI: public jderobot::Recorder
{
	public:
	  RecorderI(std::string& prefix, Ice::CommunicatorPtr& communicator)
	    :prefix(prefix),communicator(communicator)
	  {

	  }

	  virtual Ice::Int startRecording(const jderobot::RecorderConfigPtr& recConfig, const Ice::Current& c)
	  {
		  return 0;
	  }

	private:

	  std::string prefix;
	  Ice::CommunicatorPtr communicator;

};


class RecorderApp: public virtual Ice::Application{
public:
	RecorderApp() :Ice::Application() {}

  virtual int run(int, char*[]) {

    std::string srvName = "RecorderApp";
    Ice::CommunicatorPtr comm = communicator();
    Ice::PropertiesPtr prop = comm->getProperties();

    /*adapter to keep all the objects*/
    Ice::ObjectAdapterPtr adapter = comm->createObjectAdapter(srvName);

    /*VarColorI object, added with name varcolorA*/
    std::string objPrefix = srvName + ".Recorder.";

    try{
      Ice::ObjectPtr object = new RecorderI(objPrefix,comm);

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
	RecorderApp app;

	app.main(argc,argv);
}


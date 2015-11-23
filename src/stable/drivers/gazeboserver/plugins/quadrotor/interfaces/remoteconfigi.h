#ifndef _DRONE_REMOTECONFIGI_H_
#define _DRONE_REMOTECONFIGI_H_

#include "../ardrone_driver.h"
#include <jderobot/remoteConfig.h>
#include <fstream>
#include <Ice/Ice.h>

namespace remoteconfig{
	class RemoteConfigI: virtual public jderobot::remoteConfig {
		public:
			RemoteConfigI(ARDroneDriver *driver);
			virtual ~RemoteConfigI();
			virtual Ice::Int initConfiguration(const Ice::Current&);
			virtual std::string read(Ice::Int id, const Ice::Current&);
			virtual Ice::Int write(const std::string& data, Ice::Int id, const Ice::Current&);
			virtual Ice::Int setConfiguration(Ice::Int id, const Ice::Current&);
		private:
			std::ofstream f2;
			int idLocal;
			std::string path;
			ARDroneDriver *driver;
	};
}
#endif

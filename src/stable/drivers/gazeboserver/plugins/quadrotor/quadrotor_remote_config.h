#ifndef _DRONE_REMOTECONFIG_H_
#define _DRONE_REMOTECONFIG_H_

#include <remoteConfig.h>
#include <fstream>
#include <Ice/Ice.h>
#include "quadrotorplugin.hh"

namespace gazebo {

class RemoteConfigI: virtual public jderobot::remoteConfig {
	public:
		RemoteConfigI(QuadrotorPlugin *adp);
		virtual ~RemoteConfigI();
		virtual Ice::Int initConfiguration(const Ice::Current&);
		virtual std::string read(Ice::Int id, const Ice::Current&);
		virtual Ice::Int write(const std::string& data, Ice::Int id, const Ice::Current&);
		virtual Ice::Int setConfiguration(Ice::Int id, const Ice::Current&);
	private:
		std::ofstream f2;
		int idLocal;
		std::string path;
		QuadrotorPlugin *adp;
};

} //gazebo

#endif

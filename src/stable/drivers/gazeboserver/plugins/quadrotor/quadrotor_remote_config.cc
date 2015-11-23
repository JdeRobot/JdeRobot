#include "quadrotor_remote_config.h"
//#include "quadrotor_parser.h"
//#include <cstdlib>
//#include <ctime>
//#include <cstdio>

namespace gazebo {

RemoteConfigI::RemoteConfigI(QuadrotorPlugin *adp)
{
	this->adp=adp;
	idLocal=0;
}

RemoteConfigI::~RemoteConfigI()
{
	delete adp;
}

Ice::Int RemoteConfigI::initConfiguration(const Ice::Current&)
{

	std::cout << "inicializado" << std::endl;
	if (idLocal==0){
		/* initialize random seed: */
		srand ( time(NULL) );

		/* generate secret number: */
		idLocal = rand() + 1;

		std::stringstream ss;//create a stringstream
		ss << idLocal << ".xml";//add number to the stream
	
		path=ss.str();
		f2.open(ss.str().c_str(), std::ofstream::out);
		std::cout << "-----------------" <<  idLocal << std::endl;
		return idLocal;
	}
	else
		return 0;
}

std::string RemoteConfigI::read(Ice::Int id, const Ice::Current&)
{
	return std::string("");
}

Ice::Int RemoteConfigI::write(const std::string& data, Ice::Int id, const Ice::Current&)
{
	if (id == idLocal){
		f2 << data << std::endl;
		return 1;
	}
	else{
		return 0;
	}
}

Ice::Int RemoteConfigI::setConfiguration(Ice::Int id, const Ice::Current&)
{
	if (id == idLocal){
		id=0;
		idLocal=0;
		f2.close();
		std::cout << "file completed" << std::endl;
		// aqui tienes que llamar a tu parser de xml.
		QuadrotorParser parser=QuadrotorParser(5);
		QuadrotorConfig *conf=new QuadrotorConfig();
		parser.readFile(path,conf);
		adp->setConfiguration(conf);
		adp->configureDrone();

		if(remove(path.c_str())!=0){
			std::cout << "Error deleting file" << std::endl;
		}
		return 1;
	}
	return 0;
}

} //gazebo

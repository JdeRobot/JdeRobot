#include <jderobot/comm/motorsClient.hpp>

namespace Comm {

MotorsClient*
getMotorsClient(Comm::Communicator* jdrc, std::string prefix){
	MotorsClient* client = 0;

	int server;
	std::string server_name = jdrc->getConfig().asString(prefix+".Server");
	server = server2int(server_name);

	switch (server){
		case 0:
		{
			std::cout << "Motors disabled" << std::endl;
			break;
		}
		case 1:
		{
			std::cout << "Sending Velocities by ICE interfaces" << std::endl;
			MotorsIceClient* cl;
			cl = new MotorsIceClient(jdrc, prefix);
		    client = (Comm::MotorsClient*) cl;
		    break;
		}
		case 2:
		{
			#ifdef JDERROS
				std::cout << "Sending Velocities by ROS messages" << std::endl;
				std::string nodeName;
				nodeName =  jdrc->getConfig().asStringWithDefault(prefix+".Name", "MotorsNode");
				std::string topic;
				topic = jdrc->getConfig().asStringWithDefault(prefix+".Topic", "");
				PublisherMotors* pm;
				pm = new PublisherMotors(0, nullptr, nodeName, topic);
				pm->start();
				client = (Comm::MotorsClient*) pm;
            #else
				throw "ERROR: ROS is not available";
			#endif
		 	break;
		}
		default:
		{
			std::cerr << "Wrong " + prefix+".Server property" << std::endl;
			break;
		}

	}

	return client;


}

}//NS

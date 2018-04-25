#include <jderobot/comm/bumperClient.hpp>


namespace Comm {

BumperClient*
getBumperClient(Comm::Communicator* jdrc, std::string prefix){
	BumperClient* client = 0;

	int server;
	std::string server_name = jdrc->getConfig().asString(prefix+".Server");
	server = server2int(server_name);

	switch (server){
		case 0:
		{
			std::cout << "Bumper disabled" << std::endl;
			break;
		}
		case 1:
		{
			std::cout << "Receiving BumperData from ICE interfaces" << std::endl;
			BumperIceClient* cl;
			cl = new BumperIceClient(jdrc, prefix);
			cl->start();
		    client = (Comm::BumperClient*) cl;
		    break;
		}
		case 2:
		{
            #ifdef JDERROS
                std::cout << "Receiving BumperData from ROS messages" << std::endl;
                std::string nodeName;
                nodeName =  jdrc->getConfig().asStringWithDefault(prefix+".Name", "BumperNode");
				std::string topic;
				topic = jdrc->getConfig().asStringWithDefault(prefix+".Topic", "");
                ListenerBumper* lc;
                lc = new ListenerBumper(0, nullptr, nodeName, topic);
                lc->start();
                client = (Comm::BumperClient*) lc;
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

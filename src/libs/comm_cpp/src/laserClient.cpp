#include <jderobot/comm/laserClient.hpp>


namespace Comm {

LaserClient*
getLaserClient(Comm::Communicator* jdrc, std::string prefix){
	LaserClient* client = 0;

	int server;
	std::string server_name = jdrc->getConfig().asString(prefix+".Server");
	server = server2int(server_name);

	switch (server){
		case 0:
		{
			std::cout << "Laser disabled" << std::endl;
			break;
		}
		case 1:
		{
			std::cout << "Receiving LaserData from ICE interfaces" << std::endl;
			LaserIceClient* cl;
			cl = new LaserIceClient(jdrc, prefix);
			cl->start();
		    client = (Comm::LaserClient*) cl;
		    break;
		}
		case 2:
		{
            #ifdef JDERROS
                std::cout << "Receiving LaserData from ROS messages" << std::endl;
                std::string nodeName;
                nodeName =  jdrc->getConfig().asStringWithDefault(prefix+".Name", "LaserNode");
				std::string topic;
				topic = jdrc->getConfig().asStringWithDefault(prefix+".Topic", "");
                ListenerLaser* lc;
                lc = new ListenerLaser(0, nullptr, nodeName, topic);
                lc->start();
                client = (Comm::LaserClient*) lc;
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

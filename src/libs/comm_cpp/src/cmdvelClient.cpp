#include <jderobot/comm/cmdvelClient.hpp>


namespace Comm {

CMDVelClient*
getCMDVelClient(Comm::Communicator* jdrc, std::string prefix){
	CMDVelClient* client = 0;

	int server;
	std::string server_name = jdrc->getConfig().asString(prefix+".Server");
	server = server2int(server_name);

	switch (server){
		case 0:
		{
			std::cout << "CMDVel disabled" << std::endl;
			break;
		}
		case 1:
		{
			std::cout << "Sending CMDVel by ICE interfaces" << std::endl;
			CMDVelIceClient* cl;
			cl = new CMDVelIceClient(jdrc, prefix);
		    client = (Comm::CMDVelClient*) cl;
		    break;
		}
		case 2:
		{
			#ifdef JDERROS
				/*std::cout << "Sending Velocities by ROS messages" << std::endl;
				std::string nodeName;
				nodeName =  jdrc->getConfig().asStringWithDefault(prefix+".Name", "CMDVelNode");
				std::string topic;
				topic = jdrc->getConfig().asStringWithDefault(prefix+".Topic", "");
				PublisherCMDVel* pm;
				pm = new PublisherCMDVel(0, nullptr, nodeName, topic);
				pm->start();
				client = (Comm::CMDVelClient*) pm;
				*/
				throw "ERROR: CMDVel is not supported with ROS yet";
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

#include <jderobot/comm/navdataClient.hpp>


namespace Comm {

NavdataClient*
getNavdataClient(Comm::Communicator* jdrc, std::string prefix){
	NavdataClient* client = 0;

	int server;
	std::string server_name = jdrc->getConfig().asString(prefix+".Server");
	std::transform(server_name.begin(), server_name.end(), server_name.begin(), ::tolower);
	server = server2int(server_name);

	switch (server){
		case 0:
		{
			std::cout << "Navdata disabled" << std::endl;
			break;
		}
		case 1:
		{
			std::cout << "Receiving NavdataData from ICE interfaces" << std::endl;
			NavdataIceClient* cl;
			cl = new NavdataIceClient(jdrc, prefix);
			cl->start();
		    client = (Comm::NavdataClient*) cl;
		    break;
		}
		case 2:
		{
            #ifdef JDERROS
                /*std::cout << "Receiving NavdataData from ROS messages" << std::endl;
                std::string nodeName;
                nodeName =  jdrc->getConfig().asStringWithDefault(prefix+".Name", "NavdataNode");
				std::string topic;
				topic = jdrc->getConfig().asStringWithDefault(prefix+".Topic", "");
                ListenerNavdata* lc;
                lc = new ListenerNavdata(0, nullptr, nodeName, topic);
                lc->start();
                client = (Comm::NavdataClient*) lc;
                */
				throw "ERROR: Navdata is not supported with ROS yet";
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

#include <jderobot/comm/pose3dClient.hpp>


namespace Comm {

Pose3dClient*
getPose3dClient(Comm::Communicator* jdrc, std::string prefix){
	Pose3dClient* client = 0;

	int server;
	std::string server_name = jdrc->getConfig().asString(prefix+".Server");
	std::transform(server_name.begin(), server_name.end(), server_name.begin(), ::tolower);
	server = server2int(server_name);

	switch (server){
		case 0:
		{
			std::cout << "Pose3d disabled" << std::endl;
			break;
		}
		case 1:
		{
			std::cout << "Receiving Pose3D from ICE interfaces" << std::endl;
			Pose3dIceClient* cl;
			cl = new Pose3dIceClient(jdrc, prefix);
			cl->start();
		    client = (Comm::Pose3dClient*) cl;
		    break;
		}
		case 2:
		{
			#ifdef JDERROS
				std::cout << "Receiving Pose3D from ROS messages" << std::endl;
				std::string nodeName;
				nodeName =  jdrc->getConfig().asStringWithDefault(prefix+".Name", "PoseNode");
				std::string topic;
				topic = jdrc->getConfig().asStringWithDefault(prefix+".Topic", "");
				ListenerPose* lc;
				lc = new ListenerPose(0, nullptr, nodeName, topic);
				lc->start();
				client = (Comm::Pose3dClient*) lc;
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

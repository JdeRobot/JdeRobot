#include "quadrotor_parser.h"
#include <iostream>
#include <fstream>

std::string intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

std::string floatToString(float number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

std::string boolToString(bool val)
{
    if(val){
        return "1";
    }else{
        return "0";
    }
}
int stringToInt(std::string val)
{
    int temp;
    std::istringstream(val) >> temp;
    return temp;
}

float stringToFloat(std::string val)
{
    float temp;
    std::istringstream(val) >> temp;
    return temp;
}

bool stringToBool(std::string val)
{
    if(val.compare("0")==0){
        return false;
    }else{
        return true;
    }
}

QuadrotorParser::QuadrotorParser(int id)
{
    //ctor
    this->id=id;
}

QuadrotorParser::~QuadrotorParser()
{
    //dtor
}

void QuadrotorParser::parse(const xmlpp::Node* node,QuadrotorConfig *conf)
{

	//const xmlpp::ContentNode* nodeContent = dynamic_cast<const xmlpp::ContentNode*>(node);
	const xmlpp::TextNode* nodeText = dynamic_cast<const xmlpp::TextNode*>(node);
	const xmlpp::CommentNode* nodeComment = dynamic_cast<const xmlpp::CommentNode*>(node);

	if(nodeText && nodeText->is_white_space())
    	return;

  	const Glib::ustring nodename = node->get_name();


  	if(!nodeText && !nodeComment && !nodename.empty())
  	{
	        const Glib::ustring namespace_prefix = node->get_namespace_prefix();

		if(namespace_prefix.empty()){
			xmlpp::Node::NodeList list = node->get_children();

			if (list.size() > 1){
				for(xmlpp::Node::NodeList::iterator iter = list.begin(); iter != list.end(); ++iter)
				{
	  				parse(*iter,conf);
				}

			}
			else{
				const xmlpp::TextNode* nodeTextValue = dynamic_cast<const xmlpp::TextNode*>(*((node)->get_children().begin()));
				std::istringstream sTemp(nodeTextValue->get_content());
				
				if(nodename.compare("framerateN")==0){
					conf->set_interface_camera_framerateN(stringToInt(sTemp.str()));
				}else if(nodename.compare("framerateD")==0){
					conf->set_interface_camera_framerateD(stringToInt(sTemp.str()));
				}else if(nodename.compare("format")==0){
					conf->set_interface_camera_format(sTemp.str());
				}else if(nodename.compare("image_width")==0){
					conf->set_interface_camera_image_width(stringToInt(sTemp.str()));
				}else if(nodename.compare("image_height")==0){
					conf->set_interface_camera_image_height(stringToInt(sTemp.str()));
				}else if(nodename.compare("end_point")==0){
				    if(node->get_parent()->get_name().compare("interface_camera")==0){
		                        conf->set_interface_camera_end_point(sTemp.str());
				    }
				    else if(node->get_parent()->get_name().compare("interface_control")==0){
				        conf->set_interface_control_end_point(sTemp.str());
				    }
				    else if(node->get_parent()->get_name().compare("interface_remote_config")==0){
				        conf->set_interface_remote_config_end_point(sTemp.str());
				    }
				}else if(nodename.compare("adapter")==0){
				    if(node->get_parent()->get_name().compare("interface_camera")==0){
                		        conf->set_interface_camera_adapter(sTemp.str());
				    }
				    else if(node->get_parent()->get_name().compare("interface_control")==0){
				        conf->set_interface_control_adapter(sTemp.str());
				    }
				    else if(node->get_parent()->get_name().compare("interface_remote_config")==0){
				        conf->set_interface_remote_config_adapter(sTemp.str());
				    }
				}else if(nodename.compare("name")==0){
				    if(node->get_parent()->get_name().compare("interface_camera")==0){
		                        conf->set_interface_camera_name(sTemp.str());
				    }
				    else if(node->get_parent()->get_name().compare("interface_control")==0){
				        conf->set_interface_control_name(sTemp.str());
				    }
				    else if(node->get_parent()->get_name().compare("interface_remote_config")==0){
				        conf->set_interface_remote_config_name(sTemp.str());
				    }
				}else if(nodename.compare("default_camera")==0){
                    			conf->set_quadrotor_default_camera(stringToInt(sTemp.str()));
				}else if(nodename.compare("outdoor")==0){
                		    conf->set_quadrotor_outdoor(stringToBool(sTemp.str()));
				}else if(nodename.compare("max_bitrate")==0){
                		    conf->set_quadrotor_max_bitrate(stringToInt(sTemp.str()));
				}else if(nodename.compare("bitrate")==0){
                		    conf->set_quadrotor_bitrate(stringToInt(sTemp.str()));
				}else if(nodename.compare("navdata_demo")==0){
                		    conf->set_quadrotor_navdata_demo(stringToInt(sTemp.str()));
				}else if(nodename.compare("flight_without_shell")==0){
                		    conf->set_quadrotor_flight_without_shell(stringToBool(sTemp.str()));
				}else if(nodename.compare("altitude_max")==0){
                		    conf->set_quadrotor_altitude_max(stringToInt(sTemp.str()));
				}else if(nodename.compare("altitude_min")==0){
                		    conf->set_quadrotor_altitude_min(stringToInt(sTemp.str()));
				}else if(nodename.compare("euler_angle_max")==0){
                		    conf->set_quadrotor_euler_angle_max(stringToFloat(sTemp.str()));
				}else if(nodename.compare("control_vz_max")==0){
                		    conf->set_quadrotor_control_vz_max(stringToInt(sTemp.str()));
				}else if(nodename.compare("control_yaw")==0){
                		    conf->set_quadrotor_control_yaw(stringToFloat(sTemp.str()));
				}else if(nodename.compare("record")==0){
					conf ->set_video_record(stringToBool(sTemp.str()));
				}else if(nodename.compare("path")==0){
					conf->set_video_path(sTemp.str());
				}else if(nodename.compare("fps")==0){
					conf->set_video_fps(stringToInt(sTemp.str()));
				}
			}
		}
    	else{
    		//std::cout << "Node name = " << namespace_prefix << ":" << nodename << std::endl;
		}
  	}
}

void QuadrotorParser::writeFile(std::string filepath,QuadrotorConfig *conf)
{
    xmlpp::Document document;
    xmlpp::Element* nodeRoot = document.create_root_node("QuadrotorServer");
    nodeRoot->add_child_text("\n\t");

    xmlpp::Element* configNode = nodeRoot->add_child("Configuration");
    configNode->add_child_text("\n\t\t");

    xmlpp::Element* cameraNode = configNode->add_child("interface_camera");
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraEndPointNode = cameraNode->add_child("end_point");
    cameraEndPointNode->add_child_text(conf->get_interface_camera_end_point());
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraAdapNode = cameraNode->add_child("adapter");
    cameraAdapNode->add_child_text(conf->get_interface_camera_adapter());
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraNameNode = cameraNode->add_child("name");
    cameraNameNode->add_child_text(conf->get_interface_camera_name());
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraFrameNNode = cameraNode->add_child("framerateN");
    cameraFrameNNode->add_child_text(intToString(conf->get_interface_camera_framerateN()));
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraFrameDNode = cameraNode->add_child("framerateD");
    cameraFrameDNode->add_child_text(intToString(conf->get_interface_camera_framerateD()));
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraFormatNode = cameraNode->add_child("format");
    cameraFormatNode->add_child_text(conf->get_interface_camera_format());
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraImgWidthNode = cameraNode->add_child("image_width");
    cameraImgWidthNode->add_child_text(intToString(conf->get_interface_camera_image_width()));
    cameraNode->add_child_text("\n\t\t\t");
    xmlpp::Element* cameraImgHeightNode = cameraNode->add_child("image_height");
    cameraImgHeightNode->add_child_text(intToString(conf->get_interface_camera_image_height()));
    cameraNode->add_child_text("\n\t\t");

    configNode->add_child_text("\n\t\t");

    xmlpp::Element* controlNode = configNode->add_child("interface_control");
    controlNode->add_child_text("\n\t\t\t");
    xmlpp::Element* controlEndPointNode = controlNode->add_child("end_point");
    controlEndPointNode->add_child_text(conf->get_interface_control_end_point());
    controlNode->add_child_text("\n\t\t\t");
    xmlpp::Element* controlAdapNode = controlNode->add_child("adapter");
    controlAdapNode->add_child_text(conf->get_interface_control_adapter());
    controlNode->add_child_text("\n\t\t\t");
    xmlpp::Element* controlNameNode = controlNode->add_child("name");
    controlNameNode->add_child_text(conf->get_interface_control_name());
    controlNode->add_child_text("\n\t\t");

    configNode->add_child_text("\n\t\t");

    xmlpp::Element* remoteConfNode = configNode->add_child("interface_remote_config");
    remoteConfNode->add_child_text("\n\t\t\t");
    xmlpp::Element* remoteConfEndPointNode = remoteConfNode->add_child("end_point");
    remoteConfEndPointNode->add_child_text(conf->get_interface_remote_config_end_point());
    remoteConfNode->add_child_text("\n\t\t\t");
    xmlpp::Element* remoteConfAdapNode = remoteConfNode->add_child("adapter");
    remoteConfAdapNode->add_child_text(conf->get_interface_remote_config_adapter());
    remoteConfNode->add_child_text("\n\t\t\t");
    xmlpp::Element* remoteConfNameNode = remoteConfNode->add_child("name");
    remoteConfNameNode->add_child_text(conf->get_interface_remote_config_name());
    remoteConfNode->add_child_text("\n\t\t");

    configNode->add_child_text("\n\t\t");


    xmlpp::Element* quadrotorNode = configNode->add_child("quadrotor");
    quadrotorNode->add_child_text("\n\t\t\t");
    xmlpp::Element* quadrotorCamNode = quadrotorNode->add_child("default_camera");
    quadrotorCamNode->add_child_text(intToString(conf->get_quadrotor_default_camera()));
    quadrotorNode->add_child_text("\n\t\t\t");
    xmlpp::Element* arOutNode = quadrotorNode->add_child("outdoor");
    arOutNode->add_child_text(boolToString(conf->get_quadrotor_outdoor()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arMBitNode = quadrotorNode->add_child("max_bitrate");
    arMBitNode->add_child_text(intToString(conf->get_quadrotor_max_bitrate()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arBitNode = quadrotorNode->add_child("bitrate");
    arBitNode->add_child_text(intToString(conf->get_quadrotor_bitrate()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arNavNode = quadrotorNode->add_child("navdata_demo");
    arNavNode->add_child_text(intToString(conf->get_quadrotor_navdata_demo()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arFliNode = quadrotorNode->add_child("flight_without_shell");
    arFliNode->add_child_text(boolToString(conf->get_quadrotor_flight_without_shell()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arMAltNode = quadrotorNode->add_child("altitude_max");
    arMAltNode->add_child_text(intToString(conf->get_quadrotor_altitude_max()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arAltNode = quadrotorNode->add_child("altitude_min");
    arAltNode->add_child_text(intToString(conf->get_quadrotor_altitude_min()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arEulNode = quadrotorNode->add_child("euler_angle_max");
    arEulNode->add_child_text(floatToString(conf->get_quadrotor_euler_angle_max()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arConNode = quadrotorNode->add_child("control_vz_max");
    arConNode->add_child_text(intToString(conf->get_quadrotor_control_vz_max()));
    quadrotorNode->add_child_text("\n\t\t\t");

    xmlpp::Element* arConYNode = quadrotorNode->add_child("control_yaw");
    arConYNode->add_child_text(floatToString(conf->get_quadrotor_control_yaw()));
    quadrotorNode->add_child_text("\n\t\t");
    
    configNode->add_child_text("\n\t\t");

	xmlpp::Element* videoNode = configNode->add_child("video");
    controlNode->add_child_text("\n\t\t\t");
    xmlpp::Element* videoRecordNode = controlNode->add_child("record");
    videoRecordNode->add_child_text(boolToString(conf->get_video_record()));
    controlNode->add_child_text("\n\t\t\t");
    xmlpp::Element* videoPathNode = controlNode->add_child("path");
    videoPathNode->add_child_text(conf->get_video_path());
    controlNode->add_child_text("\n\t\t\t");
    xmlpp::Element* videoFpsNode = controlNode->add_child("fps");
    videoFpsNode->add_child_text(intToString(conf->get_video_fps()));
    controlNode->add_child_text("\n\t\t");
    

    configNode->add_child_text("\n\t");
    nodeRoot->add_child_text("\n");

    Glib::ustring whole = document.write_to_string();
    document.write_to_file(filepath);
}

int QuadrotorParser::readFile(std::string filepath,QuadrotorConfig *conf)
{
	#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
	try
	{
  	#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
		xmlpp::DomParser parser;
	    	parser.set_substitute_entities();
	    	parser.parse_file(filepath);

	    	if(parser)
	    	{
	      		const xmlpp::Node* pNode = parser.get_document()->get_root_node();
			parse(pNode,conf);
	   	}
		#ifdef LIBXMLCPP_EXCEPTIONS_ENABLED
	}
  	catch(const std::exception& ex)
  	{
	    	std::cout << "Exception caught: " << ex.what() << std::endl;
  	}
  	#endif //LIBXMLCPP_EXCEPTIONS_ENABLED
  	return 0;
}

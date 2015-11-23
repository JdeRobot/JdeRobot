/*
 *  Copyright (C) 1997-2015 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *  Authors : 
 *       Alberto Martín Florido <almartinflorido@gmail.com>	
 */

#include "ardrone_driver.h"
#include "components/pose3di.h"
#include "components/remoteconfigi.h"
#include "components/cameraserver.cpp"
#include "components/navdatai.h"
#include "components/cmdveli.h"
#include "components/ardroneextrai.h"
#include <signal.h>

ARDroneDriver::ARDroneDriver()
{

	parser= ArDroneParser();
	conf=new ArDroneConfig();
	inited = false;
	last_tm=0.0;	
	record_usb=false;
}

ARDroneDriver::~ARDroneDriver()
{
	delete conf;
}

void ARDroneDriver::run()
{
	
	boost::posix_time::ptime startTime=boost::posix_time::second_clock::local_time();
	float dt=0.005;
	while(true)
	{
		boost::posix_time::ptime loop_start_time=boost::posix_time::microsec_clock::local_time();
		
		if(!inited)
		{
			boost::posix_time::ptime initiedTime=boost::posix_time::second_clock::local_time();
			boost::posix_time::time_duration inited_duration = initiedTime - startTime;
			
			if(inited_duration.total_seconds()>5.0){
				inited=true;
				std::cout << "Initied!!" << std::endl;
				
				
				vp_os_mutex_lock(&navdata_lock);
				PRINT("Successfully connected to '%s' (AR-Drone %d.0 - Firmware: %s) - Battery(\%): %d\n",
				         ardrone_control_config.ardrone_name,
				         (IS_ARDRONE1) ? 1 : 2,
				         ardrone_control_config.num_version_soft,
				         shared_raw_navdata->navdata_demo.vbat_flying_percentage);
				PRINT("Navdata Publish Settings:\n");
				PRINT("    Drone Navdata Send Speed: %s\n", ardrone_application_default_config.navdata_demo==0 ? "200Hz (navdata_demo=0)" : "15Hz (navdata_demo=1)\n");				
				
				vp_os_mutex_unlock(&navdata_lock);
				
				if (ardrone_control_config.num_version_soft[0] == '0')
				{
					PRINT("The AR-Drone has a suspicious Firmware number. It usually means the network link is unreliable.\n");				    
				}
								
			}
			
			
		}else{
		
			vp_os_mutex_lock(&navdata_lock);
				navdata_unpacked_t navdata_raw = *shared_raw_navdata;
			vp_os_mutex_unlock(&navdata_lock);	
			
			if(!realtime_video)
			{
				//De momento no se plantea
			}
			
			if(!realtime_navdata)
			{
				//De momento no se plantea
	
			}
		}
		
		//looprate
		boost::posix_time::ptime loop_finish_time=boost::posix_time::microsec_clock::local_time();
		boost::posix_time::time_duration loop_diff = loop_finish_time - loop_start_time;
		
		
		if(loop_diff.total_milliseconds() > LOOPRATE)
		{
			loop_diff=boost::posix_time::milliseconds(0);
		}else{
			loop_diff=boost::posix_time::milliseconds(looprate-loop_diff.total_milliseconds());
		}
		dt=loop_diff.total_milliseconds();
		usleep(loop_diff.total_microseconds());
		if(should_exit==1)
			break;
	}
	ic->shutdown();
	std::cout << "finish ardrone_server" << std::endl;
}

bool ARDroneDriver::parseConfigFile(char* filepath)
{
	std::string file(filepath);
	if(parser.readFile(file,conf)==-1)
		return false;
	else
		return true;
}

double ARDroneDriver::getParameter(char* param, double defaultVal)
{
	std::string name(param);
	double value=conf->getParameterValue(name);
	if(value==-1)
	{
		return defaultVal;
	}
	return value;
}

void ARDroneDriver::configureDrone(char* configFile)
{
	if(!this->parseConfigFile(configFile))
        std::cerr << "Config file not found!! Establising default values." << std::endl;


	float eulerMax=this->getParameter("euler_angle_max",0.21);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(euler_angle_max, &eulerMax, NULL);
	
	int set_navdata_demo_value = this->getParameter("navdata_demo",DEFAULT_NAVDATA_DEMO);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(navdata_demo, &set_navdata_demo_value, NULL);		
	cam_state=this->getParameter("video_channel",DEFAULT_CAM_STATE);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(video_channel, &cam_state, NULL);
	int altMax=this->getParameter("altitude_max",3000);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(altitude_max, &altMax, NULL);	
	int altMin=this->getParameter("altitude_min",50);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(altitude_min, &altMin, NULL);

	float vzMax=this->getParameter("control_vz_max",700);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(control_vz_max, &vzMax, NULL);
	float yawSpeed=this->getParameter("control_yaw",1.75);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(control_yaw, &yawSpeed, NULL);
	int isOutdoor=this->getParameter("outdoor",1);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(outdoor, &isOutdoor, NULL);
	int withoutShell=this->getParameter("flight_without_shell",0);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(flight_without_shell, &withoutShell, NULL);
	int frameSize=this->getParameter("bitrate",4000);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(bitrate, &frameSize, NULL);
	int maxframeSize=this->getParameter("max_bitrate",4000);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(max_bitrate, &maxframeSize, NULL);

	int detect_type=this->getParameter("detect_type",10);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(detect_type, &detect_type, NULL);

	int detections_h=this->getParameter("detections_select_h",32);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(detections_select_h, &detections_h, NULL);

	int detections_v=this->getParameter("detections_select_v_hsync",128);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(detections_select_v_hsync, &detections_v, NULL);

	/*
	 * Orange Green : 1
	 * Orange Yellow: 2
	 * Orange Blue: 3
	 */

	int enemy_colors=this->getParameter("enemy_colors",3);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(enemy_colors, &enemy_colors, NULL);

	int enemy_shell=this->getParameter("enemy_without_shell",0);
	ARDRONE_TOOL_CONFIGURATION_ADDEVENT(enemy_without_shell, &enemy_shell, NULL);
	
}

void ARDroneDriver::initIce(int argc, char** argv)
{
	ic = Ice::initialize(argc, argv);
}

Ice::CommunicatorPtr ARDroneDriver::getCommunicator()
{
	return ic;
}

void ARDroneDriver::initInterfaces()
{
	try{
		Ice::PropertiesPtr prop = ic->getProperties();
		
		//Interface camera
		std::string CameraEndpoints = prop->getProperty("ArDrone.Camera.Endpoints");
		Ice::ObjectAdapterPtr adapterCamera =ic->createObjectAdapterWithEndpoints("ArDroneCameraServer", CameraEndpoints);
		std::string cameraName = prop->getProperty("ArDrone.Camera.Name");
		Ice::ObjectPtr object = new cameraserver::CameraI("ArDrone.Camera.", ic);
		adapterCamera->add(object, ic->stringToIdentity(cameraName));
		adapterCamera->activate();
		//Interface pose3D
		std::string pose3DName = prop->getProperty("ArDrone.Pose3D.Name");		
		std::string Pose3DEndpoints = prop->getProperty("ArDrone.Pose3D.Endpoints");
		Ice::ObjectAdapterPtr adapterPose3D =ic->createObjectAdapterWithEndpoints("ArDronePose3D", Pose3DEndpoints);
		Ice::ObjectPtr pose3DO =new pose3D::Pose3DI(this);
		adapterPose3D->add(pose3DO,ic->stringToIdentity(pose3DName));
		adapterPose3D->activate();		
		//Interface remoteConfig
		std::string remoteName = prop->getProperty("ArDrone.RemoteConfig.Name");		
		std::string remoteEndpoints = prop->getProperty("ArDrone.RemoteConfig.Endpoints");
		Ice::ObjectAdapterPtr adapterremote =ic->createObjectAdapterWithEndpoints("ArDroneRemoteConfig",remoteEndpoints);
		Ice::ObjectPtr remoteO =new remoteconfig::RemoteConfigI(this);
		adapterremote->add(remoteO,ic->stringToIdentity(remoteName));
		adapterremote->activate();			
		//Interface Navdata
		std::string navName = prop->getProperty("ArDrone.Navdata.Name");		
		std::string navEndpoints = prop->getProperty("ArDrone.Navdata.Endpoints");
		Ice::ObjectAdapterPtr adapternav =ic->createObjectAdapterWithEndpoints("ArDroneNavdata",navEndpoints);
		Ice::ObjectPtr navO =new navdata::NavdataI();
		adapternav->add(navO,ic->stringToIdentity(navName));
		adapternav->activate();		
		//Interface CMDVel
		std::string cmdName = prop->getProperty("ArDrone.CMDVel.Name");		
		std::string cmdEndpoints = prop->getProperty("ArDrone.CMDVel.Endpoints");
		Ice::ObjectAdapterPtr adaptercmd =ic->createObjectAdapterWithEndpoints("ArDroneCMDVel",cmdEndpoints);
		Ice::ObjectPtr cmdO =new cmdvel::CMDVelI();
		adaptercmd->add(cmdO,ic->stringToIdentity(cmdName));
		adaptercmd->activate();	
		//Interface Extra
		std::string extraName = prop->getProperty("ArDrone.Extra.Name");		
		std::string extraEndpoints = prop->getProperty("ArDrone.Extra.Endpoints");
		Ice::ObjectAdapterPtr adapterextra =ic->createObjectAdapterWithEndpoints("ArDroneExtra",extraEndpoints);
		Ice::ObjectPtr extraO =new ardrone_extra::ExtraI();
		adapterextra->add(extraO,ic->stringToIdentity(extraName));
		adapterextra->activate();									

	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		should_exit = 1;
		exit(-1);
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		should_exit = 1;
		exit(-1);
	} 
}

void controlCHandler (int signal)
{  
	should_exit = 1;
}
////////////////////////////////////////////////////////////////////////////////
// custom_main
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    C_RESULT res = C_FAIL;
    char * drone_ip_address = NULL;

    jdeDriver = new ARDroneDriver();


    signal (SIGABRT, &controlCHandler);
    signal (SIGTERM, &controlCHandler);
    signal (SIGINT, &controlCHandler);


    // Configure wifi
    vp_com_wifi_config_t *config = (vp_com_wifi_config_t*)wifi_config();

    if(config)
    {
        vp_os_memset( &wifi_ardrone_ip[0], 0, ARDRONE_IPADDRESS_SIZE );

        // TODO: Check if IP is valid
        if(drone_ip_address){
            printf("===================+> %s\n", drone_ip_address);
            strncpy( &wifi_ardrone_ip[0], drone_ip_address, ARDRONE_IPADDRESS_SIZE - 1);
        }else{
            printf("===================+> %s\n", config->server);
            strncpy( &wifi_ardrone_ip[0], config->server, ARDRONE_IPADDRESS_SIZE - 1);
        }
    }

    while (-1 == getDroneVersion (".", wifi_ardrone_ip, &ardroneVersion)){
        printf ("Getting AR.Drone version ...\n");
        vp_os_delay (250);
    }

    // Setup communication channels
    res = ardrone_tool_setup_com( NULL );
    if( FAILED(res) ){
        PRINT("Wifi initialization failed. It means either:\n");
        PRINT("\t* you're not root (it's mandatory because you can set up wifi connection only as root)\n");
        PRINT("\t* wifi device is not present (on your pc or on your card)\n");
        PRINT("\t* you set the wrong name for wifi interface (for example rausb0 instead of wlan0) \n");
        PRINT("\t* ap is not up (reboot card or remove wifi usb dongle)\n");
        PRINT("\t* wifi device has no antenna\n");
    }else{
        // setup the application and user profiles for the driver

        jdeDriver->initIce(argc,argv);
        char* appname = (char*) DRIVER_APPNAME;
        char* usrname = (char*) DRIVER_USERNAME;
        ardrone_gen_appid (appname, "2.0", app_id, app_name, APPLI_NAME_SIZE);
        ardrone_gen_usrid (usrname, usr_id, usr_name, USER_NAME_SIZE);

        // and finally initialize everything!
        // this will then call our sdk, which then starts the ::run() method of this file as an ardrone client thread

        res = ardrone_tool_init(wifi_ardrone_ip, strlen(wifi_ardrone_ip), NULL, app_name, usr_name, NULL, NULL, MAX_FLIGHT_STORING_SIZE, NULL);


        while( SUCCEED(res) && ardrone_tool_exit() == FALSE ){
            res = ardrone_tool_update();
        }
        res = ardrone_tool_shutdown();
    }

    return SUCCEED(res) ? 0 : -1;
}



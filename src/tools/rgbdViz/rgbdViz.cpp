/*
 *  Copyright (C) 1997-2013 JDE Developers TeamrgbdViz.camRGB
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
 *
 *  Author : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>

 */





#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/pointcloud.h>
#include <rgbd.h>
#include <jderobotutil/utils/CameraUtils.h>
#include "rgbdVizgui.h"
#include "pthread.h"
#include <jderobot/config/config.h>
#include <jderobot/comm/communicator.hpp>
#include <jderobot/comm/cameraClient.hpp>
#include <jderobot/comm/rgbdClient.hpp>
#include <jderobot/comm/tools.hpp>
#include <jderobot/types/image.h>
#include <jderobot/types/rgbd.h>
#include <logger/Logger.h>




#define MAX_COMPONENTS 20	

rgbdViz::rgbdVizgui* rgbdVizgui_ptx;


//jderobot::PointcloudClientPtr pcClient;

Comm::RgbdClient* rgbClient;
Comm::CameraClient* camRGB;
Comm::CameraClient* camDEPTH;
bool cameraRGBDActive=false;


int debug;



void *gui_thread(void* arg){
	try{
		//local data
		std::vector<jderobot::RGBPoint> cloud;
		cv::Mat rgb,depth;

		IceUtil::Time lastIT;

		lastIT=IceUtil::Time::now();
		while(rgbdVizgui_ptx->isVisible() && ! rgbdVizgui_ptx->isClosed()){

			if (cameraRGBDActive) {
				JdeRobotTypes::Rgbd data = rgbClient->getRgbd();
				rgb = data.color.data;
				depth = data.depth.data;
			}
			else {
				if (camRGB){
					JdeRobotTypes::Image rgbi = camRGB->getImage();
					rgb = rgbi.data;
				}
				if (camDEPTH){
					JdeRobotTypes::Image di = camDEPTH->getImage();
					depth = di.data;
					//std::cout<< di.format << std::endl;

				}
			}
			/*if (pcClient)
				pcClient->getData(cloud);

*/
			if ((rgb.rows!=0)&&(depth.rows!=0)){
				rgbdVizgui_ptx->updateAll(rgb,depth, cloud);
			}
			else if (rgb.rows!=0){
				rgbdVizgui_ptx->updateRGB(rgb);
			}
			else if (depth.rows!=0){
				rgbdVizgui_ptx->updateDEPTH(depth);
			}
			else{	
				rgbdVizgui_ptx->updatePointCloud(cloud);
			}
			if (((IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds()) > rgbdVizgui_ptx->getCycle() )){
				if (debug)
					;//std::cout<<"-------- rgbdViz: timeout-" << std::endl;
			}
			else{
				usleep(rgbdVizgui_ptx->getCycle() - (IceUtil::Time::now().toMicroSeconds() - lastIT.toMicroSeconds()));
			}
			lastIT=IceUtil::Time::now();


		}
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
	}
	pthread_exit(NULL);
	return NULL;
}

/**
 * \brief Main program function code
 */
int main(int argc, char** argv){

	int i;
	Ice::CommunicatorPtr ic;
	int n_components=0;
	pthread_t threads[MAX_COMPONENTS];
	pthread_attr_t attr;
	Ice::PropertiesPtr prop;
	bool create_gui=false;



	bool rgbCamSelected=false;
	bool depthCamSelected=false;
	bool pointCloudSelected=false;


	bool cameraRGBActive=false;
	bool cameraDepthActive=false;

	Config::Properties cfg = Config::load(argc, argv);
	Comm::Communicator* jdrc = new Comm::Communicator(cfg);





	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	try{
		ic = Ice::initialize(argc, argv);
	}catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		return 1;
	}
	catch (const char* msg) {
		std::cerr <<"Error :" << msg << std::endl;
		return 1;
	}


	cameraRGBActive=(bool)Comm::server2int(cfg.asStringWithDefault("rgbdViz.CameraRGB.Server","0"));
	cameraDepthActive=(bool)Comm::server2int(cfg.asStringWithDefault("rgbdViz.CameraDEPTH.Server","0"));
	cameraRGBDActive=(bool)Comm::server2int(cfg.asStringWithDefault("rgbdViz.RGBD.Server","0"));

	if (cameraRGBDActive && (cameraRGBActive ||  cameraDepthActive)){
		LOG(ERROR) << "RGBD and single cameras cannot be selected at the same time";
		return 2;
	}


	if (cameraRGBDActive) {
		rgbClient = Comm::getRgbdClient(jdrc, "rgbdViz.RGBD");
		if (rgbClient != NULL) {
				rgbCamSelected = true;
				create_gui = true;
				depthCamSelected=true;
			} else {
				throw "rgbdViz: failed to load RGBD interface";
			}
	}
	else{
		if (cameraRGBActive) {
			camRGB = Comm::getCameraClient(jdrc, "rgbdViz.CameraRGB");
			if (camRGB != NULL) {
				rgbCamSelected = true;
				create_gui = true;
			} else {
				throw "rgbdViz: failed to load RGB Camera";
			}

		}
		if (cameraDepthActive) {
			camDEPTH =Comm::getCameraClient(jdrc, "rgbdViz.CameraDEPTH");
			if (camDEPTH != NULL) {
				depthCamSelected = true;
				create_gui = true;
			} else {
				throw "rgbdViz: failed to load DEPTH Camera";
			}
		}
	}


	/*if (prop->getPropertyAsIntWithDefault("rgbdViz.pointCloudActive",0)){
		pcClient = jderobot::PointcloudClientPtr(new jderobot::pointcloudClient(ic,"rgbdViz.pointCloud."));
		if (pcClient!= NULL){
			pcClient->start();
			pointCloudSelected=true;
			create_gui=true;
		}
		else{
			throw "rgbdViz: failed to load pointCloud";
		}
	}
*/
	//set the sizes of each image
	cv::Size rgbSize(0,0);
	cv::Size depthSize(0,0);
	JdeRobotTypes::Image temp;
	//rgb
	if (cameraRGBDActive) {
		while ((rgbSize == cv::Size(0, 0)) && (depthSize == cv::Size(0, 0))) {
			JdeRobotTypes::Rgbd data = rgbClient->getRgbd();
			rgbSize = data.color.data.size();
			depthSize = data.depth.data.size();
		}
	}
	else {
		if (rgbCamSelected) {
			while (rgbSize == cv::Size(0, 0) || rgbSize == cv::Size(3, 3)) {
				temp = camRGB->getImage();
				rgbSize = temp.data.size();

			}
		}
		//depth
		if (depthCamSelected) {
			while (depthSize == cv::Size(0, 0) || depthSize == cv::Size(3, 3)) {
				temp = camDEPTH->getImage();
				depthSize = temp.data.size();
				//depthSize = cv::Size(temp.width, temp.height);

			}
		}
	}



	debug=cfg.asIntWithDefault("rgbdViz.Debug",320);
	int fps=cfg.asIntWithDefault("rgbdViz.Fps",0);
	float cycle=(float)(1/(float)fps)*1000000;

	std::string worldfile = cfg.asStringWithDefault("rgbdViz.WorldFile", "");
	std::string camRGBFile = cfg.asStringWithDefault("rgbdViz.camRGB", "");
	std::string camIRFile = cfg.asStringWithDefault("rgbdViz.camIR", "");


	rgbdVizgui_ptx = new rgbdViz::rgbdVizgui(rgbCamSelected,depthCamSelected, pointCloudSelected, worldfile, camRGBFile, camIRFile,rgbSize,depthSize, cycle);
	
	std::cout << create_gui << std::endl;
	if (create_gui){
		pthread_create(&threads[n_components], &attr, gui_thread,NULL);
		n_components++;
	}


	if (rgbdVizgui_ptx == NULL)
		throw "rgbdViz: Could not create the grafic interface";
	for (i = 0; i < n_components; i++) {
		pthread_join(threads[i], NULL);
	}

	std::cout << "final" << std::endl;
	if (ic)
		ic->destroy();
	return 0;
}

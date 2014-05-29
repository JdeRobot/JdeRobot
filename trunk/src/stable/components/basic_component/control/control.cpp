#include "control.h"


namespace basic_component {

bool cameraOn = false;
bool laserOn = false;
bool motorsOn = false;
bool encodersOn = false;

Control::Control(Ice::CommunicatorPtr ic, Shared* sm)
{
    /*Obtaining the configuration file (*.cfg) properties such as ports and IP's*/
    this->ic = ic;
    this->sm = sm;

    Ice::PropertiesPtr prop = ic->getProperties();

    /*Checking if the property has value and the creation of the proxy is possible. If the property is not set or has no value
       it will be set as "miss" and then we will know that we cannot create a proxy with the camera (or other sensors/actuators)*/
    std::string cam = prop->getPropertyWithDefault("basic_component.Camera1.Proxy", "miss");
    if (!boost::iequals(cam , "miss"))
    {

	/*Creation of a proxy to connect with cameraServer*/
	Ice::ObjectPrx base = ic->propertyToProxy("basic_component.Camera1.Proxy");
	if (0==base)
		throw "Could not create proxy";
	/*cast to CameraPrx*/
	this->cprx = jderobot::CameraPrx::checkedCast(base);
	if (0==cprx)
		throw "Invalid proxy";

	cameraOn = true;

	/*Get the image data from the camera proxy*/
	jderobot::ImageDataPtr data = cprx->getImageData();

	/*Create the first image obtained from the camera and stores in the shared memory*/
	this->sm->createImage(data);
	
    }
    else
    {
	cameraOn = false; 
	/*Create an empty image if there is no camera connected*/
	this->sm->createEmptyImage();
	std::cout << "No camera connected" << std::endl;
    }
}

void Control::update()
{
    if(cameraOn) 
    {
   	//Get de data from the camera and stores de image in the shared memory periodically (see threadcontrol)
	jderobot::ImageDataPtr data = cprx->getImageData();
	this->sm->updateImage(data);
    }
}

}

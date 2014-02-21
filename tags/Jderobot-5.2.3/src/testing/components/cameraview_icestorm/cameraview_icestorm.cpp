/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *	      Sara Marugán Alonso <smarugan@gsyc.es>
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>
#include <jderobot/camera.h>
#include <jderobot/image.h>
#include <colorspaces/colorspacesmm.h>
#include "viewer.h"

class ImageConsumerI: virtual public jderobot::ImageConsumer {
private:
    cameraview_icestorm::Viewer viewer;
public:
    virtual void report(const ::jderobot::ImageDataPtr& data,
                        const Ice::Current&) {

        //std::cout << "Cameraview_icestorm report" << std::endl;

        colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
        if (!fmt)
	  throw "Format not supported";

        colorspaces::Image image(data->description->width,
			       data->description->height,
			       fmt,
			       &(data->pixelData[0]));
        viewer.display(image);
    }
};

int main(int argc, char** argv){
  int status;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);
    std::string topicName = ic->getProperties()->getProperty("Cameraview_icestorm.Camera.TopicName");

    Ice::ObjectPrx obj=ic->propertyToProxy("Cameraview_icestorm.Camera.TopicManager");
    IceStorm::TopicManagerPrx topicManager=IceStorm::TopicManagerPrx::checkedCast(obj);

    std::string objAdapterEndpoint = ic->getProperties()->getProperty("Cameraview_icestorm.Camera.ObjectAdapter");
    Ice::ObjectAdapterPtr adapter=ic->createObjectAdapterWithEndpoints("CameraAdapter",objAdapterEndpoint);

    ImageConsumerI* imageConsumer = new ImageConsumerI;
    Ice::ObjectPrx proxy = adapter->addWithUUID(imageConsumer)->ice_oneway();

    IceStorm::TopicPrx topic;
    try {
        topic = topicManager->retrieve(topicName);
        IceStorm::QoS qos;
        topic->subscribeAndGetPublisher(qos, proxy);
    }
    catch (const IceStorm::NoSuchTopic& ex) {
        std::cerr << ex << std::endl;
    }

    adapter->activate();
    ic->waitForShutdown();

    topic->unsubscribe(proxy);

    if (ic)
    	ic->destroy();
    return status;

  }catch (const Ice::Exception& ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }
}

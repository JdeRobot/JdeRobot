//
// Created by frivas on 10/03/16.
//

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
 *  Authors : Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
 *
 */


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <IceStorm/IceStorm.h>
#include <jderobot/camera.h>
#include <jderobot/image.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include <jderobotViewer/JderobotViewer.h>
#include <pcl/common/common_headers.h>



//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string.h>
#include <sstream>
#include <cstdio>
#include <csignal>
#include <unistd.h>
#include <cstdlib>
#include <list>

#include <zlib.h>
#include <log/Logger.h>
#include <jderobot/visualization.h>
#include <ns/ns.h>

#include "easyiceconfig/EasyIce.h"

bool flag=false; /** boolean to keep a check on signal */

namespace visualization{

    class VisualizationI:  public jderobot::Visualization {
    public:
        VisualizationI(std::string propertyPrefix, Ice::CommunicatorPtr ic){
            jViewer=boost::shared_ptr<jderobot::JderobotViewer> (new jderobot::JderobotViewer("3D viewer",1,true));
            cloud = pcl::PointCloud<pcl::PointXYZRGB>::Ptr( new pcl::PointCloud<pcl::PointXYZRGB>());
            linesID=0;
        }

        ~VisualizationI(){
            jViewer.reset();
        }


        virtual void drawSegment(const jderobot::Segment& segment,const jderobot::Color& color,  const Ice::Current&){
            pcl::PointXYZ from,to;
            from.x=segment.from.x;
            from.y=segment.from.y;
            from.z=segment.from.z;
            to.x=segment.to.x;
            to.y=segment.to.y;
            to.z=segment.to.z;
            this->jViewer->addLine(from,to,color.r,color.g,color.b,boost::lexical_cast<std::string>(this->linesID));
            linesID++;

        }

        virtual void drawPoint(const jderobot::Point& point,const jderobot::Color& color,  const Ice::Current&){
            pcl::PointXYZRGB pointPcl;
            pointPcl.x = point.x;
            pointPcl.y = point.y;
            pointPcl.z = point.z;
            pointPcl.r = color.r;
            pointPcl.g = color.g;
            pointPcl.b = color.b;

            cloud->push_back(pointPcl);
            this->jViewer->addPointCloud(this->cloud);
        }
        virtual void clearAll(const Ice::Current&){
            this->jViewer->removeAllShapes();
        }


    private:
        boost::shared_ptr<jderobot::JderobotViewer> jViewer;
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud;
        int linesID;
    };

} //namespace


jderobot::ns* namingService = NULL;


void signalHandler(int signum){   /*** signal handler to handle SIGINT signal */
    flag=true;
}

int main(int argc, char** argv)
{
    Ice::ObjectPtr viewerPtr;
    //signal(SIGINT,signalHandler);
    Ice::CommunicatorPtr ic;
    try{
        ic = EasyIce::initialize(argc, argv);

        Ice::PropertiesPtr prop = ic->getProperties();
        std::string Endpoints = prop->getProperty("Visualization.Endpoints");

        // Naming Service
        int nsActive = prop->getPropertyAsIntWithDefault("NamingService.Enabled", 0);

        if (nsActive)
        {
            std::string ns_proxy = prop->getProperty("NamingService.Proxy");
            try
            {
                namingService = new jderobot::ns(ic, ns_proxy);
            }
            catch (Ice::ConnectionRefusedException& ex)
            {
                jderobot::Logger::getInstance()->error("Impossible to connect with NameService!");
                exit(-1);
            }
        }

        Ice::ObjectAdapterPtr adapter =ic->createObjectAdapterWithEndpoints("Visualization", Endpoints);
        std::string objPrefix("Visualization.");
        std::string viewerName = prop->getProperty(objPrefix + "Name");
        Ice::ObjectPtr object = new visualization::VisualizationI(objPrefix, ic);

        adapter->add(object, ic->stringToIdentity(viewerName));

        if (namingService)
            namingService->bind(viewerName, Endpoints, object->ice_staticId());


        adapter->activate();
        ic->waitForShutdown();

    }catch (const Ice::Exception& ex) {
        std::cerr << ex<<" 1 " << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg<< " 2 " << std::endl;
        exit(-1);
    }

}

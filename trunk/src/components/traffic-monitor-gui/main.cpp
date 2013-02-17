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
 *
 */

#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include <traffic_monitor.h> 

#include "viewgtk.h"
#include "trafficmonitor_config.h"

using namespace trafficmonitor;

void get_current_frame(Ice::CommunicatorPtr ic, colorspaces::Image& current_frame)
{
   Ice::ObjectPrx base = ic->stringToProxy("redotest:tcp -h 0.0.0.0 -p 10000");
   if (0==base)
   {
      throw "Could not create REDO proxy";
   }

   /*cast to CameraPrx*/
   trafficmonitor::TrafficMonitorIPrx cprx = trafficmonitor::TrafficMonitorIPrx::checkedCast(base);
   if (0==cprx)
      throw "Invalid proxy";
            
   jderobot::ImageDataPtr data = cprx->getImageData();
   colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
            
   if (!fmt)
   {
      throw "Format not supported";
   }
   
   colorspaces::Image image(data->description->width,
                            data->description->height,
                            fmt,
                            &(data->pixelData[0]));

   current_frame = image.clone();
}

int main(int argc, char** argv)
{
   colorspaces::Image current_frame;
   ViewGtkPtr vp(new ViewGtk(current_frame));

   int status;
   Ice::CommunicatorPtr ic;

   try
   {
      ic = Ice::initialize(argc,argv);
      Ice::ObjectPrx base = ic->stringToProxy("admintest/admin -f admin-facet:default -p 5555");
      
      if (!base)
      {
         throw "Could not create proxy";
      }

      Ice::PropertiesAdminPrx propAdmin = Ice::PropertiesAdminPrx::checkedCast(base, "Properties");
      if (propAdmin)
      {
         TrafficMonitorAlgorithmConfig cfg(propAdmin);
         
         Ice::PropertyDict props = propAdmin->getPropertiesForPrefix("");

         printf("!!!!!!!!!! Updating properties !!!!!!!!!!!!\n");
         for(Ice::PropertyDict::const_iterator p = props.begin(); p != props.end(); ++p)
         {
            std::cout << "  " << p->first << "=" << p->second << std::endl;
         }

         cfg.readProperties(props);

         vp->set_cfg(&cfg);
         vp->read_cfg();

         for(;;)
         {
            get_current_frame(ic, current_frame);
            vp->iteration();
         }

      }
      else
      {
         throw "Invalid proxy";
      }
   }
   catch (const Ice::Exception& ex)
   {
      std::cerr << ex << std::endl;
      status = 1;
   }
   catch (const char* msg)
   {
      std::cerr << msg << std::endl;
      status = 1;
   }

   if (ic)
   {
      ic->destroy();
   }
   
   return status;
}


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


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <colorspaces/colorspaces++.h>
#include <colorspacesice/image.h>
#include <jderobot/varcolor.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include "gstpipeline.h"

namespace varcolorserver{
  class VarColorI: virtual public jderobot::VarColor,
		   virtual public gbxiceutilacfr::SafeThread {
  public:
    VarColorI(std::string& propertyPrefix, const jderobotice::Context& context)
      : gbxiceutilacfr::SafeThread(context.tracer()),
	prefix(propertyPrefix),context(context),pipeline(0),description()
    {
      /*initializes gst*/
      gst_init(0,0);
      
      Ice::PropertiesPtr prop = context.communicator()->getProperties();
      pipelineCfg.uri = prop->getProperty(prefix+"Uri");
      pipelineCfg.framerateN = prop->getPropertyAsIntWithDefault(prefix+"FramerateN",25);
      pipelineCfg.framerateD = prop->getPropertyAsIntWithDefault(prefix+"FramerateD",1);
      pipelineCfg.width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
      pipelineCfg.height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);
      pipelineCfg.format = prop->getPropertyWithDefault(prefix+"Format","YUY2");

      const Format *fmt = searchFormat(pipelineCfg.format.c_str());
      if (fmt == 0)
	throw jderobot::JderobotException("Configured format not suported");
      
      description.reset(new colorspacesice::ImageDescription(pipelineCfg.width,
							     pipelineCfg.height,
							     pipelineCfg.width * 
							     pipelineCfg.height * 
							     fmt->pixelSize,
							     fmt));
      pipeline.reset(new GSTPipeline(pipelineCfg));
      start();//start thread
    }

    virtual ~VarColorI(){
      gbxiceutilacfr::stopAndJoin(this);
    }
    
    virtual void walk(){
      jderobot::ImageDataPtr datap(0);
      while(!isStopping()){
	colorspacesice::ImagePtr img;
	GstBuffer* buff = pipeline->pull_buffer();
	if (!buff){//restart pipeline, we will send last image again for this iteration, 0 if we are in the first
	  pipeline.reset(new GSTPipeline(pipelineCfg));
	}else{
	  img.reset(new colorspacesice::Image(*description,(char*)buff->data));
	  datap = (jderobot::ImageDataPtr)(*img);/*cast+copy*/
	  img.reset();
	  gst_buffer_unref(buff);/*cast copy data, so now we can release it*/
	}

	{//critical region start
	  IceUtil::Mutex::Lock sync(requestsMutex);
	  while(!requests.empty()){
	    jderobot::AMD_VarColor_getDataPtr cb = requests.front();
	    requests.pop_front();
	    //reply
	    if (!datap)
	      std::cerr << "Sending empty image\n";
	    cb->ice_response(datap);
	  }
	}//critical region end
      }
    }
    
    virtual jderobot::ImageDescriptionPtr getDescription(const Ice::Current& c){
      return (jderobot::ImageDescriptionPtr)(*description);/*cast operator*/
    }

    virtual void getData_async(const jderobot::AMD_VarColor_getDataPtr& cb,
			       const Ice::Current& c){
      IceUtil::Mutex::Lock sync(requestsMutex);
      requests.push_back(cb);
    }

  private:
    std::string prefix;
    jderobotice::Context context;
    std::auto_ptr<GSTPipeline> pipeline;
    Config pipelineCfg;
    colorspacesice::ImageDescriptionPtr description;
    IceUtil::Mutex requestsMutex;
    std::list<jderobot::AMD_VarColor_getDataPtr> requests;
  };


  class Component: public jderobotice::Component{
  public:
    Component()
      :jderobotice::Component("VarColorSrv"), varcolorSrv(0) {}

    virtual void start(){
      std::string objPrefix = context().tag()+".VarColor.";
      Ice::ObjectPtr varcolorSrv = new VarColorI(objPrefix,context());

      context().createInterfaceWithString(varcolorSrv,context().properties()->getPropertyWithDefault(objPrefix+"Id","varcolorA"));
    }

    virtual ~Component(){
      varcolorSrv = 0;//join thread
    }

  private:
    Ice::ObjectPtr varcolorSrv;
  };

} //namespace

int main(int argc, char** argv){
  varcolorserver::Component component;

  jderobotice::Application app(component);
  return app.jderobotMain(argc,argv);
}

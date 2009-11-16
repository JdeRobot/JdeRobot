#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <formats.h>
#include <colorspaces.h>
#include <jderobot/varcolor.h>
#include "gstpipeline.h"


/*derive from Shared to use Handle*/
class GSTPipeline: public gstvideopipeline::GSTPipeline, public IceUtil::Shared {
public:
  GSTPipeline(gstvideopipeline::Config& cfg)
    :gstvideopipeline::GSTPipeline(cfg) {}
};
typedef IceUtil::Handle<GSTPipeline> GSTPipelinePtr;


class VarColorI: public jderobot::VarColor {
public:
  VarColorI(std::string& prefix, Ice::CommunicatorPtr& communicator)
    : prefix(prefix),communicator(communicator)
  {
    /*initializes gst*/
    gstvideopipeline::gst_init(0,0);
      
    loadPipeline();
  }

  void loadPipeline()
    throw (jderobot::HardwareFailedException) {
    gstvideopipeline::Config pipelineCfg;
    Ice::PropertiesPtr prop = communicator->getProperties();

    pipeline = 0;/*delete pipeline*/

    pipelineCfg.uri = prop->getProperty(prefix+"Uri");
    pipelineCfg.framerateN = prop->getPropertyAsIntWithDefault(prefix+"FramerateN",25);
    pipelineCfg.framerateD = prop->getPropertyAsIntWithDefault(prefix+"FramerateD",1);
    pipelineCfg.width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
    pipelineCfg.height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);
    pipelineCfg.format = prop->getPropertyWithDefault(prefix+"Format","YUV2");

    descp = new jderobot::ImageDescription();
    descp->width = pipelineCfg.width;
    descp->height = pipelineCfg.height;
    descp->size = descp->width * descp->height * 2;
    descp->format = pipelineCfg.format;
    formatDesc = searchPixelFormat(descp->format.c_str());

    pipeline = new GSTPipeline(pipelineCfg);
  }

  virtual jderobot::ImageDescriptionPtr getDescription(const Ice::Current& c){
    return descp;
  }

  virtual jderobot::ImageDataPtr getData(const Ice::Current&)
    throw (jderobot::DataNotExistException, jderobot::HardwareFailedException) {
    jderobot::ImageDataPtr datap = new jderobot::ImageData();

    GstBuffer* buff = pipeline->pull_buffer();
    if (0 == buff){/*restart pipeline*/
      loadPipeline();
      buff = pipeline->pull_buffer();
    }

    if (buff){
      datap->pixelData.resize(descp->width*descp->height*formatDesc->size/sizeof(datap->pixelData[0]));
      memmove( &(datap->pixelData[0]), buff->data, buff->size );
      gst_buffer_unref(buff);
    }else
      throw jderobot::HardwareFailedException("pipeline couldn't be restarted");


    IceUtil::Time t = IceUtil::Time::now();
    datap->timeStamp.seconds = (int)t.toSeconds();
    datap->timeStamp.useconds = (int)t.toMicroSeconds() - datap->timeStamp.seconds*1000000;
  
    datap->description = descp;

    return datap;
    //IceUtil::ThreadControl::sleep(IceUtil::Time::milliSeconds(20));
  }
private:
  std::string prefix;
  Ice::CommunicatorPtr communicator;
  GSTPipelinePtr pipeline;
  jderobot::ImageDescriptionPtr descp;
  const Format *formatDesc;
};


class VarColorSrvApp: public virtual Ice::Application{
public:
  VarColorSrvApp()
    :Ice::Application() {}

  virtual int run(int, char*[]) {
    std::string srvName = "VarColorSrv";
    Ice::CommunicatorPtr comm = communicator();
    Ice::PropertiesPtr prop = comm->getProperties();
    
    /*adapter to keep all the objects*/
    Ice::ObjectAdapterPtr adapter = comm->createObjectAdapter(srvName);

    /*VarColorI object, added with name varcolorA*/
    std::string objPrefix = srvName + ".VarColor.";
    Ice::ObjectPtr object = new VarColorI(objPrefix,comm);
    adapter->add(object,comm->stringToIdentity(prop->getPropertyWithDefault(objPrefix+"Id","varcolorA")));

    adapter->activate();
    comm->waitForShutdown();
    if (interrupted())
      std::cerr << appName() << ": received signal, shutting down" << std::endl;
    return 0;
  }
};

int main(int argc, char** argv){
  VarColorSrvApp app;
  
  app.main(argc,argv);
}

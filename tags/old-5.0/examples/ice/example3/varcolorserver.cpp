#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <varcolor.h>
#include "gstpipeline.h"


/*derive from Shared to use Handle*/
class GSTPipeline: public gstvideopipeline::GSTPipeline, public IceUtil::Shared {
public:
  GSTPipeline(gstvideopipeline::Config& cfg)
    :gstvideopipeline::GSTPipeline(cfg) {}
};
typedef IceUtil::Handle<GSTPipeline> GSTPipelinePtr;


class VarColorI: public jde::VarColor {
public:
  VarColorI(std::string& prefix, Ice::CommunicatorPtr& communicator)
    : prefix(prefix),communicator(communicator)
  {
    /*initializes gst*/
    gstvideopipeline::gst_init(0,0);
      
    loadPipeline();
  }

  void loadPipeline()
    throw (jde::HardwareFailedException) {
    gstvideopipeline::Config pipelineCfg;
    Ice::PropertiesPtr prop = communicator->getProperties();

    pipeline = 0;/*delete pipeline*/

    pipelineCfg.uri = prop->getProperty(prefix+"Uri");
    pipelineCfg.bpp = prop->getPropertyAsIntWithDefault(prefix+"Bpp",24);
    pipelineCfg.framerateN = prop->getPropertyAsIntWithDefault(prefix+"FramerateN",25);
    pipelineCfg.framerateD = prop->getPropertyAsIntWithDefault(prefix+"FramerateD",1);
    pipelineCfg.mime = prop->getPropertyWithDefault(prefix+"Mime","video/x-raw-rgb");
    pipelineCfg.width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
    pipelineCfg.height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);

    descp = new jde::ImageDescription();
    descp->width = pipelineCfg.width;
    descp->height = pipelineCfg.height;
    descp->size = descp->width * descp->height * (pipelineCfg.bpp >> 3);
    descp->format = pipelineCfg.mime;

    pipeline = new GSTPipeline(pipelineCfg);
  }

  virtual jde::ImageDescriptionPtr getDescription(const Ice::Current& c){
    return descp;
  }

  virtual jde::ImageDataPtr getData(const Ice::Current&)
    throw (jde::DataNotExistException, jde::HardwareFailedException) {
    jde::ImageDataPtr datap = new jde::ImageData();

    GstBuffer* buff = pipeline->pull_buffer();
    if (0 == buff){/*restart pipeline*/
      loadPipeline();
      buff = pipeline->pull_buffer();
    }

    if (buff){
      datap->pixelData.resize(buff->size);
      memcpy( &(datap->pixelData[0]), buff->data, buff->size );
      gst_buffer_unref(buff);
    }else
      throw jde::HardwareFailedException("pipeline couldn't be restarted");


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
  jde::ImageDescriptionPtr descp;
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

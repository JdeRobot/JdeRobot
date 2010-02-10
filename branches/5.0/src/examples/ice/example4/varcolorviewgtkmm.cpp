#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <varcolor.h>
#include "viewer.h"


class VarColorViewApp: public virtual Ice::Application{
public:
  VarColorViewApp()
    :Ice::Application() {}

  virtual int run(int, char*[]) {
    Viewer viewer;
    Ice::CommunicatorPtr comm = communicator();
    
    /*we get the proxy using a property in cfg file*/
    Ice::ObjectPrx base = comm->propertyToProxy("VarColorView.VarColor.Proxy");
    if (0==base)
      throw "Could not create proxy";

    /*cast to VarColorPrx*/
    jde::VarColorPrx vprx = jde::VarColorPrx::checkedCast(base);
    if (0==vprx)
      throw "Invalid proxy";
    
    while(viewer.isVisible()){
      jde::ImageDataPtr image;

      /*remote procedure call*/
      image = vprx->getData();

      viewer.display(image);
    }
  return 0;
  }
};

int main(int argc, char** argv){
  VarColorViewApp app;
  
  app.main(argc,argv);
}

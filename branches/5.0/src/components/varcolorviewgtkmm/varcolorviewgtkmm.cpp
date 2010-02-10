#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <jderobot/varcolor.h>
#include <colorspacesice/image.h>
#include "viewer.h"


int main(int argc, char** argv){
  int status;
  Viewer viewer;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);
    Ice::ObjectPrx base = ic->propertyToProxy("VarColorView.VarColor.Proxy");
    if (0==base)
      throw "Could not create proxy";

    /*cast to VarColorPrx*/
    jderobot::VarColorPrx vprx = jderobot::VarColorPrx::checkedCast(base);
    if (0==vprx)
      throw "Invalid proxy";
    
    while(viewer.isVisible()){
      colorspacesice::ImagePtr image(new colorspacesice::Image(vprx->getData()));
      viewer.display(*image);
    }
  }catch (const Ice::Exception& ex) {
    std::cerr << ex << std::endl;
    status = 1;
  } catch (const char* msg) {
    std::cerr << msg << std::endl;
    status = 1;
  }

  if (ic)
    ic->destroy();
  return status;
}

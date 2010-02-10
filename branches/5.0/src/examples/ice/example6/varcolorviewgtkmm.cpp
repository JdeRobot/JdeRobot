#include <iostream>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <varcolor.h>
#include "viewer.h"


int main(int argc, char** argv){
  int status;
  Viewer viewer;
  Ice::CommunicatorPtr ic;

  try{
    ic = Ice::initialize(argc,argv);
    /*inicializamos proxy a pelo*/
    Ice::ObjectPrx base = ic->stringToProxy("varcolorA:tcp -h 127.0.0.1 -p 9999");
    /*lo correcto es hacerlo con propiedades para poder modificar 
      el proxy sin tener que recompilar*/
    
    /*Ice::ObjectPrx base = ic->propertyToProxy("VarColorView.VarColor.Proxy");*/
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

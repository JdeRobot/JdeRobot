#include <iostream>
#include <string>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <gbxsickacfr/gbxiceutilacfr/store.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <jderobotutil/observer.h>
#include <jderobot/varcolor.h>
#include "view.h"
#include "model.h"

namespace opencvdemo {
  class Component: public jderobotice::Component{
  public:
    Component()
      : jderobotice::Component("Opencvdemo"),model(0),controller(0) {
      model = new Model();
      controller = new Controller(model);
    }

    virtual void start(){
      /*inicializamos proxy a pelo*/
      Ice::ObjectPrx base = context().communicator()->stringToProxy("varcolorA:tcp -h 127.0.0.1 -p 9999");
      /*lo correcto es hacerlo con propiedades para poder modificar 
	el proxy sin tener que recompilar*/
      
      /*Ice::ObjectPrx base = ic->propertyToProxy("VarColorView.VarColor.Proxy");*/
      if (0==base)
	throw "Could not create proxy";
      
      /*cast to VarColorPrx*/
      jderobot::VarColorPrx vprx = jderobot::VarColorPrx::checkedCast(base);
      if (0==vprx)
	throw "Invalid proxy";
      
      while(controller.isRunning()){
	/*remote procedure call*/
	model.setImage(vprx->getData());
      }
    }
  private:
    Model* model;
    Controller* controller;
  };
} //namespace

int main(int argc, char * argv[]){
  opencvdemo::Component component;

  jderobotice::Application app( component );
  return app.jderobotMain(argc, argv);
}

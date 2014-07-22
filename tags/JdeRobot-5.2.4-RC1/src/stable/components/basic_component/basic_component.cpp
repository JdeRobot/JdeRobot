//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <pthread.h>

#include "gui/threadgui.h"
#include "gui/gui.h"
#include "control/control.h"
#include "control/threadcontrol.h"

// Global members

basic_component::ThreadControl* threadControl;
basic_component::ThreadGui* threadGui;

//Launching the control "thread"
void controlStart() {

    threadControl->start();
}

//Launching the gui thred
void* guiThread(void*) {

    threadGui->start();
}

int main(int argc, char* argv[]) {

    try{

	//We initialize Ice here to be able to add some other options to the configuration file such as
        //the posibility to show or not show the GUI.
        Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);
        
        //Shared memory object
        basic_component::Shared* sm = new basic_component::Shared();
	
        //Creates the control&processing thread manager
        threadControl = new basic_component::ThreadControl(ic, sm);

	Ice::PropertiesPtr prop = ic->getProperties();

	//Let's check if the user want to show the gui or not. This setting must be in the .cfg file
	std::string gui = prop->getPropertyWithDefault("basic_component.Gui", "miss");
    	if (!boost::iequals(gui , "miss") && !boost::iequals(gui, "OFF")) {
		
		pthread_t t_gui;

		//Creates the gui thread manager
		threadGui = new basic_component::ThreadGui(sm);

		//Creates the thread for the control&processing
		pthread_create(&t_gui, NULL, &guiThread, NULL);	
	
	}else 
		std::cout << "No Gui mode" << std::endl;

	//We use the main thread to manage the control&processing thread, so the gui has it's own thread that is
	//periodically updated.
	controlStart();
		
	
	
    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }
    return 0;

}

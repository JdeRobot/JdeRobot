//Qt
#include <QtGui>

//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <pthread.h>
#include "gui/threadgui.h"
#include "gui/gui.h"
#include "control/control.h"
#include "control/threadcontrol.h"
#include "shared.h"

basic_component_qt::ThreadControl* threadControl;
basic_component_qt::ThreadGui* threadGui;

//Launching the control thread
void* controlThread(void*) {

    threadControl->start();
}

//Launching the gui thred
void* guiThread(void *) {

    threadGui->start();
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    int ret;

    try{


        //We initialize Ice here to be able to add some other options to the configuration file such as
        //the posibility to show or not show the GUI.
        Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);
        
        //Shared memory object
        basic_component_qt::Shared* sm = new basic_component_qt::Shared();

        //Creates the control&processing thread manager
        threadControl = new basic_component_qt::ThreadControl(ic, sm);

	Ice::PropertiesPtr prop = ic->getProperties();

	//Checking if the user want to show the gui or not. This setting is included in the .cfg file
	std::string gui = prop->getPropertyWithDefault("basic_component_qt.Gui", "miss");
    	if (!boost::iequals(gui , "miss") && !boost::iequals(gui, "OFF")) {
		
		pthread_t t_control;
		pthread_t t_gui;
		
		//Creates the gui thread manager
		threadGui = new basic_component_qt::ThreadGui(sm);

		//Creates the threads for gui and control&processing
		pthread_create(&t_control, NULL, &controlThread, NULL);
        	pthread_create(&t_gui, NULL, &guiThread, NULL);	

		//Starting Qt mainloop
		ret = a.exec();
	
	}else {
		//If there is no gui, it's not necessary to create a thread because the main thread can do the job
		std::cout << "No Gui mode" << std::endl;
		controlThread(NULL);	
		ret = 0;
	}

	
    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }

    return ret;

}

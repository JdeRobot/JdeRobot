

//ICE
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <dlfcn.h>

#include <pthread.h>
#include "gui/threadgui.h"
#include "gui/igui.h"
#include "control/control.h"
#include "control/threadcontrol.h"
#include "shared.h"

using namespace basic_component;

//Global variables
ThreadControl* threadControl;
ThreadGui* threadGui;
int argc;
char** argv;
Shared* sm;

//Launching the control function (using main loop)
void loadControl() {

    threadControl->start();
}

//Launching the gui thread
void* guiThread(void*) {

    threadGui->start();
}

//Loading the gui plugin
void* loadGui(void* data) {

    //Getting the gui mode read from the conf file
    std::string* userData = reinterpret_cast<std::string*>(data);

    void* handle;

    if (boost::iequals(*userData , "qt")) {
	std::cout << "Loading Qt...";
    	handle = dlopen("libguiqt.so", RTLD_LAZY);
    }
    else if (boost::iequals(*userData , "gtk")) {
	std::cout << "Loading Gtk...";
	handle = dlopen("libguigtk.so", RTLD_LAZY);
    }else {
	std::cout << "Wrong Gui" << std::endl;
	return 0;
    }

    IGui* (*create)(int, char**);
    void (*destroy)(IGui*);

    create = (IGui* (*)(int, char**))dlsym(handle, "create_gui");
    destroy = (void (*)(IGui*))dlsym(handle, "destroy_gui");

    IGui* gui = (IGui*)create(argc, argv);
    
    pthread_t t_guiU;
    threadGui = new ThreadGui(sm, gui);
    pthread_create(&t_guiU, NULL, &guiThread, NULL);

    std::cout << "Done!" << std::endl;

    gui->runGui(sm);
	
    //if we are using Qt lib we have not to destroy the gui or it will fail when the gui window closes,
    //I guess Qt mainloop has its own free() (¿?)
    destroy(gui);
}

int main(int argc, char* argv[])
{
    try{
	
	//We need these arguments for instantiate a QApplication (only Qt)
	argc = argc;
	argv = argv;

        //Shared memory object
	sm = new Shared();

	//pthread_t t_gui;
        //We initialize Ice here to be able to add some other options to the configuration file such as
        //the posibility to choose the gui mode.
        Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);

	Ice::PropertiesPtr prop = ic->getProperties();

	std::string guiMode = prop->getPropertyWithDefault("basic_component.Gui", "miss");

	//Here we check what type of Gui the user wants to show (No Gui, QtGui or GtkGui)
    	if (!boost::iequals(guiMode , "miss") && !boost::iequals(guiMode, "OFF")) {
       
            //Create the threads for the gui and for the control and communication
            //pthread_create(&t_gui, NULL, &guiThread, NULL);
	    pthread_t t_gui;
    	    pthread_create(&t_gui, NULL, &loadGui, (void*) &guiMode);
	}

	//Creating the updater control function (not a different thread, we use the main thread to do this stuff)
        threadControl = new ThreadControl(ic, sm);
	loadControl();
	
    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }
    
    return 0;

}

#include "control.h"
#include "API.h"
#include "gui.h"
#define cycle_control 100 //miliseconds
#define cycle_gui 50 //miliseconds


//Global Memory
basic_component::Api *api;

void *showGui(void*) {

    struct timeval a, b;
    long totalb, totala;
    int cont = 0;
    long diff;
    basic_component::Gui *gui;

    gui = new basic_component::Gui(api);


    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        gui->display(api);


        //Sleep Algorithm
        gettimeofday(&b, NULL);
        totalb = b.tv_sec * 1000000 + b.tv_usec;
        diff = (totalb - totala) / 1000;
        if (diff < 0 || diff > cycle_gui)
            diff = cycle_gui;
        else
            diff = cycle_gui - diff;

        /*Sleep Algorithm*/
        usleep(diff * 1000);
        if (diff < 33)
            usleep(33 * 1000);
        //printf("GUI %.30lf seconds elapsed, %d\n", diff, cont);
        cont++;

    }
}

int main(int argc, char** argv) {
    pthread_t thr_gui;

    basic_component::Control *control;
    int status;
    Ice::CommunicatorPtr ic;
    struct timeval a, b;
    long diff;
    long totalb, totala;
    bool guiActivated = 0;
    bool controlActivated = 0;


    api = new basic_component::Api();
    control = new basic_component::Control();

    pthread_mutex_init(&api->controlGui, NULL);


    try {

        //-----------------ICE----------------//
        ic = Ice::initialize(argc, argv);

        // Get driver camera
        Ice::ObjectPrx camara1 = ic->propertyToProxy("basic_component.Camera1.Proxy");
        if (0 == camara1)
            throw "Could not create proxy to camera1 server";

        // cast to CameraPrx
        control->cprx1 = jderobot::CameraPrx::checkedCast(camara1);
        if (0 == control->cprx1)
            throw "Invalid proxy";

        //-----------------END ICE----------------//

        //****************************** Processing the Control ******************************///
        api->guiVisible = true;
        control->UpdateSensorsICE(api);

            pthread_create(&thr_gui, NULL, &showGui, NULL);

        while (api->guiVisible) {
            gettimeofday(&a, NULL);
            totala = a.tv_sec * 1000000 + a.tv_usec;


            control->UpdateSensorsICE(api); // Update sensors               

            //Sleep Algorithm
            gettimeofday(&b, NULL);
            totalb = b.tv_sec * 1000000 + b.tv_usec;
            diff = (totalb - totala) / 1000;
            if (diff < 0 || diff > cycle_control)
                diff = cycle_control;
            else
                diff = cycle_control - diff;

            /*Sleep Algorithm*/
            usleep(diff * 1000);
            if (diff < 33)
                usleep(33 * 1000);
            //printf("CONTROL %.15lf seconds elapsed\n", diff);     
        }

        //****************************** END Processing the Control ******************************///
        if (guiActivated)
            pthread_join(thr_gui, NULL);

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        status = 1;
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        status = 1;
    }
    if (ic)
        ic->destroy();
    return 0;
}


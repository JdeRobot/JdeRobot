#include <Ice/Application.h>
#include <jderobot/wiimote.h>
#include <Ice/Application.h>
#include "control.h"
#include "API.h"
#include "gui.h"



using namespace std;
using namespace jderobot;

#define cycle_control 50//miliseconds
#define cycle_gui 100//miliseconds

//Global Memory
wiimoteClient::Api *api;

//class Client : virtual public Ice::Application {
//
//    virtual int run(int, char*[]) {
//        Ice::ObjectPrx base = communicator()->
//                stringToProxy("wiiMote1:default -h localhost -p 9999");
//        //introrob.Motors.Proxy=motors1:tcp -h localhost -p 9999
//        wiiMotePrx wiiMotePrx = wiiMotePrx::checkedCast(base);
//        //wiiMotePrx->saludar();
//        //wiiMotePrx->despedir();
//        //jderobot::AccelerometerDataPtr dataAcc = wiiMotePrx->getAccData();
//        //wiiMotePrx->setValue(4);
//        //cout << value << endl;
////        wiiMotePrx->changeNunchukMode();
////        wiiMotePrx->changeAccMode();
////        wiiMotePrx->changeButtonMode();
////        wiiMotePrx->changeIrMode();
//                cout << "baterry status: " << wiiMotePrx->getBatteryStatus() << "%" << endl;
//
////        while (1) {
////            jderobot::AccelerometerDataPtr dataAcc = wiiMotePrx->getAccData();
////            cout << "Acc: x= " << dataAcc->accelerometer[0] << " y= " << dataAcc->accelerometer[1] << " z= " << dataAcc->accelerometer[2] << endl;
////            sleep(0.5);
////        }
////
////        while (1) {
////            cout << "button: " << wiiMotePrx->getButtonData() << endl;
////            sleep(0.5);
////        }
////
////
////
////        while (1) {
////            jderobot::InfraredDataPtr dataIr = wiiMotePrx->getIrData();
////            if (dataIr->sourceDetected)
////                cout << "Ir: x= " << dataIr->infrared[0] << " y= " << dataIr->infrared[1] << endl;
////            else
////                cout << "Ir: No Sources Detected" << endl;
////            sleep(0.5);
////        }
//        
////        cout << "NUNCHUCK INFO:" << endl;
////        
////        while (1) {
////            jderobot::NunchukDataPtr dataNunchuk = wiiMotePrx->getNunchukData();
////                
////                cout << "Acc: x= " << dataNunchuk->acc[0] << " y= " << dataNunchuk->acc[1] << " z= " << dataNunchuk->acc[2] << endl;;
////                cout << "Stick: x= " << dataNunchuk->stick[0] << " y= " << dataNunchuk->stick[1] << endl;;
////                cout << "Button: " << dataNunchuk->button << endl;
////                
////            
////            
////            sleep(0.5);
////        }        
//
//
//        communicator()->waitForShutdown();
//        return 0;
//    }
//};

void *showGui(void*) {
    struct timeval a, b;
    long totalb, totala;
    int cont = 0;
    long diff;
    wiimoteClient::Gui *gui;

    gui = new wiimoteClient::Gui(api);

    while (true) {
        gettimeofday(&a, NULL);
        totala = a.tv_sec * 1000000 + a.tv_usec;

        //LLAMAMOS A LA FUNCION QUE MUETRA EL GUI

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

int main(int argc, char *argv[]) {
    pthread_t thr_gui;
    Ice::CommunicatorPtr ic;
    int status;

    wiimoteClient::Control *control;
    struct timeval a, b;
    long diff;
    long totalb, totala;

    api = new wiimoteClient::Api();

    control = new introrob::Control();

    try {

        //-----------------ICE----------------//
        ic = Ice::initialize(argc, argv);


        // Contact to WIIMOTE interface
        Ice::ObjectPrx baseWiimote = ic->propertyToProxy("wiimoteClient.Wiimote.Proxy");
        if (0 == baseWiimote)
            throw "Could not create proxy with Wiimote";
        // Cast to wiimote
        control->wiiprx = wiiMotePrx::checkedCast(baseWiimote);
        if (0 == control->wiiprx)
            throw "Invalid proxy wiimoteClient.Wiimote.Proxy";

        //-----------------END ICE----------------//

        control->updateData(api);

        pthread_create(&thr_gui, NULL, &showGui, NULL);

        while (api->guiVisible) {

            gettimeofday(&a, NULL);
            totala = a.tv_sec * 1000000 + a.tv_usec;

            control->updateData(api);

            //Sleep Algorithm
            gettimeofday(&b, NULL);
            totalb = b.tv_sec * 1000000 + b.tv_usec;
            diff = (totalb - totala) / 1000;
            if (diff < 0 || diff > cycle_control)
                diff = cycle_control;
            else
                diff = cycle_control - diff;
            usleep(diff * 1000);
            if (diff < 33)
                usleep(33 * 1000);
            //printf("CONTROL %.15lf seconds elapsed\n", diff);            


        }

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

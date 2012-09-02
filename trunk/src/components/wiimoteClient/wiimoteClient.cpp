#include <Ice/Application.h>
#include <jderobot/wiimote.h>
#include <Ice/Application.h>

using namespace std;
using namespace jderobot;

class Client : virtual public Ice::Application {

    virtual int run(int, char*[]) {
        Ice::ObjectPrx base = communicator()->
                stringToProxy("wiiMote1:default -h localhost -p 9999");
        //introrob.Motors.Proxy=motors1:tcp -h localhost -p 9999
        wiiMotePrx wiiMotePrx = wiiMotePrx::checkedCast(base);
        //wiiMotePrx->saludar();
        //wiiMotePrx->despedir();
        //jderobot::AccelerometerDataPtr dataAcc = wiiMotePrx->getAccData();
        //wiiMotePrx->setValue(4);
        //cout << value << endl;
        wiiMotePrx->changeAccMode();
        wiiMotePrx->changeButtonMode();
        wiiMotePrx->changeIrMode();


//                        while (1) {
//                            jderobot::AccelerometerDataPtr dataAcc = wiiMotePrx->getAccData();
//                            cout << "Acc: x= " << dataAcc->accelerometer[0] << " y= " << dataAcc->accelerometer[1] << " z= " << dataAcc->accelerometer[2] << endl;
//                            sleep(0.5);
//                        }

        //                while (1) {                    
        //                    cout << "button: " << wiiMotePrx->getButtonData() << endl;
        //                    sleep(0.5);
        //                }        



//                while (1) {            
//                    jderobot::InfraredDataPtr dataIr = wiiMotePrx->getIrData();
//                    if(dataIr->sourceDetected)
//                        cout << "Ir: x= " << dataIr->infrared[0] << " y= " << dataIr->infrared[1] << endl;
//                    else
//                        cout << "Ir: No Sources Detected" << endl;
//                    sleep(0.5);
//                }


        communicator()->waitForShutdown();
        return 0;
    }
};

int main(int argc, char *argv[]) {
    Client client;
    return client.main(argc, argv);
}

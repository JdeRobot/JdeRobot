#include <Ice/Application.h>
#include <jderobot/wiimote.h>
#include <Ice/Application.h>

using namespace std;
using namespace jderobot;

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

int main(int argc, char *argv[]) {

    Ice::CommunicatorPtr ic;
    int status;

    try {

        //-----------------ICE----------------//
        ic = Ice::initialize(argc, argv);


        // Contact to WIIMOTE interface
        Ice::ObjectPrx baseWiimote = ic->propertyToProxy("wiimoteClient.Wiimote.Proxy");
        if (0 == baseWiimote)
            throw "Could not create proxy with motors";
        // Cast to wiimote
        wiiMotePrx wiiMoteClient = wiiMotePrx::checkedCast(baseWiimote);
        //control->mprx = jderobot::MotorsPrx::checkedCast(baseMotors);
        if (0 == wiiMoteClient)
            throw "Invalid proxy introrob.Motors.Proxy";
        
                wiiMoteClient->changeNunchukMode();
        
        while (1) {
            jderobot::NunchukDataPtr dataNunchuk = wiiMoteClient->getNunchukData();
                
                cout << "Acc: x= " << dataNunchuk->acc[0] << " y= " << dataNunchuk->acc[1] << " z= " << dataNunchuk->acc[2] << endl;;
                cout << "Stick: x= " << dataNunchuk->stick[0] << " y= " << dataNunchuk->stick[1] << endl;;
                cout << "Button: " << dataNunchuk->button << endl;
                
            
            
            sleep(0.5);
        }        
        
        
        
        
        
        

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

    //    Client client;
    //    return client.main(argc, argv);
}

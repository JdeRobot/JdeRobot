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
        wiiMotePrx->saludar();
        wiiMotePrx->despedir();        
        int value = wiiMotePrx->getValue();
        wiiMotePrx->setValue(4);
        cout << value << endl;
        
        while(1){
            int value = wiiMotePrx->getValue();
            cout << value << endl;            
            sleep(0.5);
        }
        
        communicator()->waitForShutdown();
        return 0;
    }
};

int main(int argc, char *argv[]) {
    Client client;
    return client.main(argc, argv);
}

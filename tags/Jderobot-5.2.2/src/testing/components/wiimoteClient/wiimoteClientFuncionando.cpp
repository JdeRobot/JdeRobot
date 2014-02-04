#include <Ice/Application.h>
#include <jderobot/wiimote.h>
#include <Ice/Application.h>
using namespace std;
using namespace jderobotice;

class Client : virtual public Ice::Application {

    virtual int run(int, char*[]) {
        Ice::ObjectPrx base = communicator()->
                stringToProxy("HolaMundo:default -p 10000");
        HolaMundoPrx holaMundoPrx = HolaMundoPrx::checkedCast(base);
        holaMundoPrx->saludar();
        holaMundoPrx->despedir();        
        int value = holaMundoPrx->getValue();
        holaMundoPrx->setValue(4);
        cout << value << endl;
        communicator()->waitForShutdown();
        return 0;
    }
};

int main(int argc, char *argv[]) {
    Client client;
    return client.main(argc, argv);
}

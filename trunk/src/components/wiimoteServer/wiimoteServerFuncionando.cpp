#include <jderobot/wiimote.h>
#include <Ice/Application.h>
#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

using namespace std;

//using namespace Demo;



    class HolaMundoI : public jderobotice::HolaMundo {
    public:
        virtual void saludar(const Ice::Current&);
        virtual void despedir(const Ice::Current&);
        virtual int getValue(const Ice::Current&);
        virtual void setValue(Ice::Int value, const Ice::Current&);
    };

    void
    HolaMundoI::saludar(const Ice::Current&) {
        cout << "¡Hola Mundo!" << endl;
    }

    void HolaMundoI::despedir(const Ice::Current&) {
        cout << "!Adios Mundo!" << endl;
    }

    int HolaMundoI::getValue(const Ice::Current&) {
        return 5;
    }

    void HolaMundoI::setValue(Ice::Int value, const Ice::Current&) {
        cout << value << endl;
    }

    class Server : virtual public Ice::Application {

        virtual int run(int, char*[]) {
            Ice::ObjectAdapterPtr adapter = communicator()->
                    createObjectAdapterWithEndpoints("HolaMundoAdapter",
                    "default -p 10000");
            Ice::ObjectPtr object = new HolaMundoI;
            adapter->add(object, communicator()->stringToIdentity("HolaMundo"));
            adapter->activate();
            communicator()->waitForShutdown();
            return 0;
        }
    };

    int main(int argc, char *argv[]) {
        Server server;
        return server.main(argc, argv);
    }


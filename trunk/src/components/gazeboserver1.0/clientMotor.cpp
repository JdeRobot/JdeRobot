#include <Ice/Ice.h>
#include <jderobot/laser.h>

#include <jderobot/motors.h>

using namespace std;

int
main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy("Motors:default -p 9997");
        jderobot::MotorsPrx motors = jderobot::MotorsPrx::checkedCast(base);
        if (!motors)
            throw "Invalid proxy";

		motors->setV(0.01);
		motors->setW(0.05);
        std::cout << "vel: " << motors->getV() << std::endl;
    } catch (const Ice::Exception& ex) {
        cerr << ex << endl;
        status = 1;
    } catch (const char* msg) {
        cerr << msg << endl;
        status = 1;
    }
    if (ic)
        ic->destroy();
    return status;
}

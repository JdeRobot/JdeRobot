#include <Ice/Ice.h>
#include <jderobot/laser.h>

#include <jderobot/encoders.h>

using namespace std;

int
main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy("MotorsEncoders:default -p 9997");
        jderobot::EncodersPrx baserEncoders = jderobot::EncodersPrx::checkedCast(base);
        if (!baserEncoders)
            throw "Invalid proxy";
   		jderobot::EncodersDataPtr ed;
		ed = baserEncoders->getEncodersData();
        std::cout << "x: " << ed->robotx << " y: " <<ed->roboty << " z: " << ed->robottheta << std::endl;
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

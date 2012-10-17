#include <Ice/Ice.h>
#include <jderobot/laser.h>

using namespace std;

int
main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy("Laser:default -p 10001");
        jderobot::LaserPrx laser = jderobot::LaserPrx::checkedCast(base);
        if (!laser)
            throw "Invalid proxy";
   		jderobot::LaserDataPtr ld;   
		ld = laser->getLaserData();
		for(int i = 0; i< 180; i++ ){
			if(i%10==0)
				std::cout << std::endl;
			std:cout << ld->distanceData[i]<< "\t";
				
		}
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

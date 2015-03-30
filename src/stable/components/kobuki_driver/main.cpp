/*****************************************************************************
 Includes
 ****************************************************************************/
#include "thread_control.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
/*****************************************************************************
 Signal Handler
*****************************************************************************/

bool shutdown_req = false;
void signalHandler(int signum) {
  shutdown_req = true;
}

/*****************************************************************************
 Main
*****************************************************************************/


int main(int argc, char** argv)
{

    KobukiManager* kobuki_manager = new KobukiManager();

    Ice::CommunicatorPtr ic;

    try {
        //-----------------ICE----------------//
        ic = Ice::initialize(argc, argv);

        Thread_control* thread = new Thread_control(ic, kobuki_manager);
        thread->run();

    } catch (const Ice::Exception& ex) {
        std::cerr << ex << std::endl;
        exit(-1);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(-1);
    }

//  signal(SIGINT, signalHandler);

//  std::cout << "Demo : Example of simple control loop." << std::endl;
//  KobukiManager kobuki_manager;

//  ecl::Sleep sleep(1);
//  ecl::Pose2D<double> pose;
//  try {
//    while (!shutdown_req){
//      sleep();
//      pose = kobuki_manager.getPose();
//      std::cout << "current pose: [" << pose.x() << ", " << pose.y() << ", " << pose.heading() << "]" << std::endl;
//    }
//  } catch ( ecl::StandardException &e ) {
//    std::cout << e.what();
//  }
//  return 0;
}

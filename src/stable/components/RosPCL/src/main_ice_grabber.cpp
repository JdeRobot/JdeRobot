
#include "KinectClient.h"

int main (int argc, char **argv)
{
    ros::init(argc, argv, "test2");

    ros::AsyncSpinner spinner(4);

    spinner.start();

    int status;
    Ice::CommunicatorPtr ic;

    KinectClient kinectClient(argc,argv,"openni_grabber","openni");

    try
    {
        ic = Ice::initialize(argc, argv);

        kinectClient.addIceProxy("KinectServer.Proxy",ic,1);


        while(ros::ok())
        {
            kinectClient.publishROS();
        }


    }
    catch (const Ice::Exception& ex)
    {
        std::cerr << ex << std::endl;
        status = 1;
    }
    catch (const char* msg)
    {
        std::cerr << msg << std::endl;
        status = 1;
    }
    if (ic)
    {
        ic->destroy();
    }


  return 0;
}

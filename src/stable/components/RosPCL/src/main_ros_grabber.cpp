#include "RosKinectGrabber.h"

int main (int argc, char **argv)
{
    ros::init(argc, argv, "test");

    ros::AsyncSpinner spinner(4);

    spinner.start();

    RosKinectGrabber v(argc,argv,"openni");

    v.run ();

    return 0;
}

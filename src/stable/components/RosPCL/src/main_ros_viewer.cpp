#include "RosOpenNIViewer.h"

int main (int argc, char **argv)
{
    ros::init(argc, argv, "test");

    ros::AsyncSpinner spinner(4);

    spinner.start();

    RosOpenNIViewer v(argc,argv,"openni_viewer","openni");

    while(ros::ok())
    {

    }

  return 0;
}


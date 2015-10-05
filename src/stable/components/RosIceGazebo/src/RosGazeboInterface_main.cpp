#include "EncodersClient.h"
#include "Pose3DClient.h"
#include "MotorClient.h"
#include "CameraClient.h"
#include "LaserClient.h"
#include "Pose3DMotorsClient.h"
#include "Pose3DEncodersClient.h"
#include "TeleOperator.h"
#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{

    int status;
    Ice::CommunicatorPtr ic;

    CameraClient cameraClient1(argc,argv,"cam_pioneer_left");
    CameraClient cameraClient2(argc,argv,"cam_pioneer_right");

    EncodersClient encodersClient(argc,argv,"encoders");

    Pose3DClient pose3DClient(argc,argv,"pose3d_pioneer");


    LaserClient laserClient(argc,argv,"laser");

    MotorClient motorClient(argc, argv, "motor");

    Pose3DEncodersClient pose3DEncodersClient_left(argc,argv,"pioneer_pose3dencoders_left_encoders");
    Pose3DEncodersClient pose3DEncodersClient_right(argc,argv,"pioneer_pose3dencoders_right_encoders");

    TeleOperator teleOperator(argc, argv, "pioneer");


    ros::AsyncSpinner RosSpinner(4);
    RosSpinner.start();




    try
    {
        ic = Ice::initialize(argc, argv);

        cameraClient1.addIceProxy("introrob.Camera1.Proxy",ic,1);
        cameraClient2.addIceProxy("introrob.Camera2.Proxy",ic,1);

        encodersClient.addIceProxy("introrob.Encoders.Proxy",ic,1);

        pose3DClient.addIceProxy("introrob.Pose3D.Proxy",ic,1);

        laserClient.addIceProxy("introrob.Laser.Proxy",ic,1);

        pose3DEncodersClient_left.addIceProxy("introrob.Pose3Dencoders1.Proxy",ic,1);
        pose3DEncodersClient_right.addIceProxy("introrob.Pose3Dencoders2.Proxy",ic,1);

        motorClient.addIceProxy("introrob.Motors.Proxy",ic,1);

        RosIceMessage::MotorData motorMsg;
        RosIceMessage::EncodersData encodersMsg;
        geometry_msgs::Pose pose3DMsg;

        motorMsg.motorW = -10.0;
        motorMsg.motorL = 25.0;
        motorMsg.motorV = 25.0;

        while(ros::ok())
        {
            cameraClient1.publishROS();
            cameraClient2.publishROS();

            encodersMsg = encodersClient.publishROS();
            pose3DMsg = pose3DClient.publishROS();

            //laserClient.publishROS(encodersMsg);
            laserClient.publishROS(pose3DMsg);

            pose3DEncodersClient_left.publishROS();
            pose3DEncodersClient_right.publishROS();

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

    return 0;
}

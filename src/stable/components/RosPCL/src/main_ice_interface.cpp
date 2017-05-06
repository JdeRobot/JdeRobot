#include "KinectInterface.h"



int main (int argc, char **argv)
{
    ros::init(argc, argv, "test2");

    ros::AsyncSpinner spinner(4);

    spinner.start();

    Ice::CommunicatorPtr ic;
    Ice::PropertiesPtr prop;

    std::string str = "--Ice.Config=pioneer2dx.cfg";



    try
    {

        ic = Ice::initialize(argc, argv);

        prop = ic->getProperties();

        std::string Endpoints = prop->getProperty("KinectInterface.Endpoints");

        Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("KinectInterface", Endpoints);

        Ice::ObjectPtr object_rgb = new KinectInterface(argc,argv,"openni_RGB",sensor_msgs::image_encodings::RGB8);
        adapter->add(object_rgb, ic->stringToIdentity(prop->getProperty("KinectInterface.CameraRGB.Name")));


        Ice::ObjectPtr object_depth = new KinectInterface(argc,argv,"openni_Depth",sensor_msgs::image_encodings::RGB8);
        adapter->add(object_depth, ic->stringToIdentity(prop->getProperty("KinectInterface.CameraDEPTH.Name")));


        adapter->activate();


        ic->waitForShutdown();

        adapter->destroy();

    }

    catch (const Ice::Exception& e)
    {
        std::cerr << e << std::endl;
    }
    catch (const char* msg)
    {
        std::cerr << msg << std::endl;
    }

    if (ic)
    {
        try
        {
            ic->destroy();
        }
        catch (const Ice::Exception& e)
        {
            std::cerr << e << std::endl;
        }
    }

    return 0;


}

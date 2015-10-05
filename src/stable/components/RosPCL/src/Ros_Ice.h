#ifndef ROS_ICE_H
#define ROS_ICE_H

#include <iostream>
#include "ros/ros.h"
#include <Ice/Ice.h>
#include "image_transport/image_transport.h"


/**
 * @brief Base class for the ROS Ice Bridge
 */

template<class IcePrx >
class Ros_Ice
{

private:

    //ros::NodeHandle* RosNode;
    ros::Publisher*  RosPublisher;
    ros::Subscriber* RosSubscriber;
    ros::ServiceClient* RosServiceClient;
    ros::ServiceServer* RosServiceServer;

    //image_transport::ImageTransport* ImageNode;
    image_transport::Publisher* ImagePublisher;
    image_transport::Subscriber* ImageSubscriber;

    /**
     * @brief This is the object that will run the ROS Spinner in order to run the callbacks
     */
    ros::AsyncSpinner* RosSpinner;

    Ice::CommunicatorPtr IceCommunicator;
    bool isCommunicator;

protected:

    IcePrx Proxy;
    image_transport::ImageTransport* ImageNode;
    ros::NodeHandle* RosNode;

public:

    Ros_Ice()
    {
        RosNode = NULL;
        RosSpinner = NULL;
        RosPublisher = NULL;
        RosSubscriber = NULL;
        RosServiceClient = NULL;
        RosServiceServer = NULL;

        ImageNode = NULL;
        ImagePublisher = NULL;
        ImageSubscriber = NULL;

        isCommunicator = false;
    }

    Ros_Ice(int argc, char **argv, std::string nodeName)
    {

        initializeROS(argc,argv,nodeName);
        RosSpinner = NULL;


        initializeIce(argc,argv);

    }


    virtual ~Ros_Ice()
    {

        //ROS_INFO("Destructor Bridge\n");


        if(RosServiceClient)
        {
            delete RosServiceClient;
        }

        if(RosServiceServer)
        {
            delete RosServiceServer;
        }

        if(RosPublisher)
        {
            delete RosPublisher;
        }

        if(RosSubscriber)
        {
            delete RosSubscriber;
        }

        if(ImagePublisher)
        {
            delete ImagePublisher;
        }

        if(ImageSubscriber)
        {
            delete ImageSubscriber;
        }


        if(RosSpinner)
        {
            RosSpinner->stop();
            delete RosSpinner;
        }



        if(isCommunicator)
        {

            try
            {
                IceCommunicator->destroy();
            }
            catch (const Ice::Exception& e)
            {
                std::cerr << e << std::endl;
            }

        }

        delete ImageNode;
        delete RosNode;


    }

    /**
     * @brief Method for initializing the ROS components
     *
     */

    int initializeROS(int argc, char **argv, std::string nodeName)
    {

        ros::init(argc, argv, nodeName);
        RosNode = new ros::NodeHandle;
        ImageNode = new image_transport::ImageTransport(*RosNode);

        RosPublisher = NULL;
        RosSubscriber = NULL;

        RosServiceClient = NULL;
        RosServiceServer = NULL;

        ImagePublisher = NULL;
        ImageSubscriber = NULL;


        return 0;
    }

    void runROSSpinner()
    {
        RosSpinner = new ros::AsyncSpinner(4);
        RosSpinner->start();

    }

    /**
     * @brief Method for initializing the Ice components
     *
     */

    int initializeIce(int argc, char **argv)
    {
        int status = 1;
        try
        {
            IceCommunicator = Ice::initialize(argc, argv);

        }
        catch (const Ice::Exception& ex)
        {
            std::cerr << ex << std::endl;
            status = 1;
        }

        isCommunicator = true;

        return status;
    }


    /**
     * @brief Method to create a ROS Publisher for a given topic
     */

    template <class ROS_DATA>
    void addRosPublisher(std::string rosTopic, int queueSize)
    {
        RosPublisher = new ros::Publisher(RosNode->advertise<ROS_DATA>(rosTopic, queueSize));

    }


    /**
     * @brief Method for adding a Subscriber with a simple function as callback
     */
    template <class ROS_DATA>
    void addRosSubscriber(std::string rosTopic, int queueSize,void(*callback)(ROS_DATA))
    {
        RosSubscriber = new ros::Subscriber(RosNode->subscribe(rosTopic, queueSize, callback));

    }


    /**
     * @brief Method for adding a Subscriber with a method of a user implemented class
     */

    template <class ROS_DATA, class ROS_CLASS>
    void addRosSubscriber(std::string rosTopic, int queueSize,void(ROS_CLASS::*callback)(ROS_DATA), ROS_CLASS *rosObject)
    {
        RosSubscriber = new ros::Subscriber(RosNode->subscribe(rosTopic, queueSize, callback,rosObject));

    }


    /**
     * @brief Method to create an Image_Transport Publisher for a given topic
     */


    void addRosImagePublisher(std::string rosTopic, int queueSize)
    {
        ImagePublisher = new image_transport::Publisher(ImageNode->advertise(rosTopic, queueSize));

    }


    /**
     * @brief Method for adding an Image_Transport Subscriber with a simple function as callback
     */

    void addRosImageSubscriber(std::string rosTopic, int queueSize,void(*callback)(sensor_msgs::ImageConstPtr& image_message))
    {
        ImageSubscriber = new image_transport::Subscriber(ImageNode->subscribe(rosTopic, queueSize, callback));

    }


    /**
     * @brief Method for adding an Image_Transport Subscriber with a method of a user implemented class
     */

    template <class ROS_CLASS>
    void addRosImageSubscriber(std::string rosTopic, int queueSize,void(ROS_CLASS::*callback)(const sensor_msgs::ImageConstPtr& image_message), ROS_CLASS *rosObject)
    {
        ImageSubscriber = new image_transport::Subscriber(ImageNode->subscribe<ROS_CLASS>(rosTopic, queueSize, callback,rosObject));

    }

    template <class ROS_DATA, class ROS_CLASS>
    void addRosServiceServer(std::string rosTopic, bool(ROS_CLASS::*callback)(ROS_DATA), ROS_CLASS *rosObject)
    {
        RosServiceServer = new ros::ServiceServer(RosNode->advertiseService(rosTopic, callback,rosObject));
    }

    template <class ROS_DATA>
    void addRosServiceClient(std::string rosTopic)
    {
        RosServiceClient = new ros::ServiceClient(RosNode->serviceClient<ROS_DATA>(rosTopic));
    }


    /**
     * @brief Method to publish a ROS message
     */

    template <class ROS_DATA>
    void rosPublish(ROS_DATA message)
    {
        RosPublisher->publish(message);
    }

    /**
     * @brief Method to publish a ROS service
     */

    template <class ROS_DATA>
    void rosServiceCall(ROS_DATA &message)
    {
        RosServiceClient->call(message);
    }

    /**
     * @brief Method to publish an Image using the image_transport API
     */

    void rosImagePublish(sensor_msgs::ImagePtr& image_message)
    {
        ImagePublisher->publish(image_message);
    }



    /**
     * @brief Method to configure the Ice Proxy
     */


    void addIceProxy(std::string ProxyString, Ice::CommunicatorPtr IceComm, int FunctionType)
    {
        if(IceComm)
        {
            IceCommunicator = IceComm;
        }

        try
        {
            if(FunctionType == 0)
            {
                Proxy = IcePrx::checkedCast(IceCommunicator->stringToProxy(ProxyString));
            }

            if(FunctionType == 1)
            {
                Proxy = IcePrx::checkedCast(IceCommunicator->propertyToProxy(ProxyString));
            }


            if (!Proxy)
            {
                throw "Invalid proxy";
            }
        }
        catch (const Ice::Exception& ex)
        {
            std::cerr << ex << std::endl;
        }
    }





};

#endif // ROSICEBRIDGE_H

#include "MotorsInterface.h"

MotorsInterface::MotorsInterface(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    //addRosSubscriber<RosIceMessage::MotorData>("laser",500,&LaserInterface::rosCallback,this);

    addRosPublisher<RosIceMessage::MotorData>("motors_subscriber",500);
    addRosServiceClient<RosIceMessage::MotorsSrv>("motors_server");


    //myfile.open("Ros.txt");


}

MotorsInterface::~MotorsInterface()
{

}

float MotorsInterface::getL(const Ice::Current &)
{
    RosIceMessage::MotorsSrv srv;
    rosServiceCall(srv);
    return srv.response.motorMsg.motorL;

}

float MotorsInterface::getW(const Ice::Current &)
{
    RosIceMessage::MotorsSrv srv;
    rosServiceCall(srv);
    return srv.response.motorMsg.motorW;
}

float MotorsInterface::getV(const Ice::Current &)
{
    RosIceMessage::MotorsSrv srv;
    rosServiceCall(srv);
    return srv.response.motorMsg.motorV;

}

Ice::Int MotorsInterface::setV(Ice::Float v, const Ice::Current&)
{
    RosIceMessage::MotorsSrv srv;
    rosServiceCall(srv);
    srv.response.motorMsg.motorV = v;
    rosPublish(srv.response.motorMsg);
}

Ice::Int MotorsInterface::setW(Ice::Float w, const Ice::Current&)
{
    RosIceMessage::MotorsSrv srv;
    rosServiceCall(srv);
    srv.response.motorMsg.motorW = -w;
    rosPublish(srv.response.motorMsg);
}

Ice::Int MotorsInterface::setL(Ice::Float l, const Ice::Current&)
{
    RosIceMessage::MotorsSrv srv;
    rosServiceCall(srv);
    srv.response.motorMsg.motorL = l;
    rosPublish(srv.response.motorMsg);

}




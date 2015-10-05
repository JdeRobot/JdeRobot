#ifndef ROSLASERPLUGIN_H
#define ROSLASERPLUGIN_H

#include "ros/ros.h"
#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/physics/MultiRayShape.hh>
#include <gazebo/common/common.hh>
#include <gazebo/transport/transport.hh>

#include <gazebo/plugins/RayPlugin.hh>

#include <boost/algorithm/string.hpp>
#include <string>

#include <gazebo/plugins/CameraPlugin.hh>

#include <RosIceMessage/Laser.h>



namespace gazebo
{

class RosLaserPlugin : public RayPlugin
{

public:
    RosLaserPlugin();
    ~RosLaserPlugin();

    void Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf);

protected:
    virtual void OnNewLaserScans();

private:

    sensors::RaySensorPtr parentSensor;

    ros::NodeHandle* RosNode;
    ros::Publisher* RosPublisher;



};

GZ_REGISTER_SENSOR_PLUGIN(RosLaserPlugin)



}



#endif // ROSLASERPLUGIN_H

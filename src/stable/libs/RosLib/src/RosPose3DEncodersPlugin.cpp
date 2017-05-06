
#include "RosPose3DEncodersPlugin.h"
namespace gazebo
{

RosPose3DEncodersPlugin::RosPose3DEncodersPlugin()
{
    std::cout << "RosPose3DEncodersPlugin constructor"<< std::endl;
}

RosPose3DEncodersPlugin::~RosPose3DEncodersPlugin()
{
    delete RosSubscriber_Camera_Left_Motor;
    delete RosSubscriber_Camera_Right_Motor;
    delete RosPublisher_Camera_Left_Encoder;
    delete RosPublisher_Camera_Right_Encoder;
    delete RosPublisher_Camera_Left_Motor;
    delete RosPublisher_Camera_Right_Motor;
    delete RosNode;
}


void RosPose3DEncodersPlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {

    this->model = _model;

    if (!_sdf->HasElement("left_joint_pose3dencoders_pan"))
    {
        gzerr << "pose3dencoders plugin missing <left_joint_pose3dencoders_pan> element\n";
    }

    if (!_sdf->HasElement("left_joint_pose3dencoders_tilt"))
    {
        gzerr << "pose3dencoders plugin missing <left_joint_pose3dencoders_tilt> element\n";
    }

    joint_pose3dencoders_pan_left_camera = model->GetJoint("pan_joint_left");
    joint_pose3dencoders_tilt_left_camera = model->GetJoint("tilt_joint_left");

    if (!joint_pose3dencoders_pan_left_camera)
    {
        gzerr << "Unable to find left joint pose3dencoders_pan[" << _sdf->GetElement("left_joint_pose3dencoders_pan")->Get<std::string>() << "]\n";
    }

    if (!joint_pose3dencoders_tilt_left_camera)
    {
        gzerr << "Unable to find left joint pose3dencoders_tilt[" << _sdf->GetElement("left_joint_pose3dencoders_tilt")->Get<std::string>() << "]\n";
    }

    left_camera_link_pan = model->GetLink("camera_column_body_left");
    left_camera_link_tilt = model->GetLink("camera_top_body_left");


    if (!_sdf->HasElement("right_joint_pose3dencoders_pan"))
    {
        gzerr << "Motors plugin missing <right_joint_pose3dencoders_pan> element\n";
    }

    if (!_sdf->HasElement("right_joint_pose3dencoders_tilt"))
    {
        gzerr << "Motors plugin missing <right_joint_pose3dencoders_tilt> element\n";
    }


    joint_pose3dencoders_pan_right_camera = model->GetJoint("pan_joint_right");
    joint_pose3dencoders_tilt_right_camera = model->GetJoint("tilt_joint_right");


    if (!joint_pose3dencoders_pan_right_camera)
    {
        gzerr << "Unable to find right joint pose3dencoders_pan[" << _sdf->GetElement("right_joint_pose3dencoders_pan")->Get<std::string>() << "]\n";
    }


    if (!joint_pose3dencoders_tilt_right_camera)
    {
        gzerr << "Unable to find right joint pose3dencoders_tilt[" << _sdf->GetElement("right_joint_pose3dencoders_tilt")->Get<std::string>() << "]\n";
    }

    right_camera_link_pan = model->GetLink("camera_column_body_right");
    right_camera_link_tilt = model->GetLink("camera_top_body_right");


    if (_sdf->HasElement("torque"))
    {
        torque = _sdf->GetElement("torque")->Get<double>();
    }

    else
    {
        gzwarn << "No torque value set for the DiffDrive plugin.\n";
        torque = 5.0;
    }


    this->updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&RosPose3DEncodersPlugin::OnUpdate, this));

    RosNode = new ros::NodeHandle;
    RosPublisher_Camera_Left_Encoder = new ros::Publisher(RosNode->advertise<RosIceMessage::Pose3DEncodersData>("pioneer_pose3dencoders_left_encoders",500));
    RosPublisher_Camera_Right_Encoder = new ros::Publisher(RosNode->advertise<RosIceMessage::Pose3DEncodersData>("pioneer_pose3dencoders_right_encoders",500));

    RosPublisher_Camera_Left_Motor = new ros::Publisher(RosNode->advertise<RosIceMessage::Pose3DMotorsData>("pioneer_pose3dencoders_left_motors",500));
    RosPublisher_Camera_Right_Motor = new ros::Publisher(RosNode->advertise<RosIceMessage::Pose3DMotorsData>("pioneer_pose3dencoders_right_motors",500));

    RosSubscriber_Camera_Left_Motor = new ros::Subscriber(RosNode->subscribe("pioneer_pose3dencoders_left_motors_sub",500,&RosPose3DEncodersPlugin::rosCallback_Left_Camera,this));
    RosSubscriber_Camera_Right_Motor = new ros::Subscriber(RosNode->subscribe("pioneer_pose3dencoders_right_motors_sub",500,&RosPose3DEncodersPlugin::rosCallback_Right_Camera,this));


}

void RosPose3DEncodersPlugin::Init()
{



}


void RosPose3DEncodersPlugin::OnUpdate()
{



    encoder_left_camera.pan = left_camera_link_pan->GetRelativePose().rot.GetAsEuler().z * 180.0 / M_PI;

    if (encoder_left_camera.pan > 0)
    {
        encoder_left_camera.pan = 180 - encoder_left_camera.pan;
    }

    if (encoder_left_camera.pan < 0)
    {
        encoder_left_camera.pan = -(180 + encoder_left_camera.pan);
    }

    encoder_left_camera.tilt = left_camera_link_tilt->GetRelativePose().rot.GetAsEuler().y * 180.0 / M_PI;



    encoder_right_camera.pan = right_camera_link_pan->GetRelativePose().rot.GetAsEuler().z * 180.0 / M_PI;

    if (encoder_right_camera.pan > 0)
    {
        encoder_right_camera.pan = 180 - encoder_right_camera.pan;
    }

    if (encoder_right_camera.pan < 0)
    {
        encoder_right_camera.pan = -(180 + encoder_right_camera.pan);
    }

    encoder_right_camera.tilt = right_camera_link_tilt->GetRelativePose().rot.GetAsEuler().y * 180.0 / M_PI;




    if (motor_left_camera.pan >= 0)
    {
        if (encoder_left_camera.pan < motor_left_camera.pan)
        {
            joint_pose3dencoders_pan_left_camera->SetParam("vel", 0, -0.1);
            joint_pose3dencoders_pan_left_camera->SetParam("fmax", 0, this->torque);
        }
        else
        {
            joint_pose3dencoders_pan_left_camera->SetParam("vel", 0, 0);
            joint_pose3dencoders_pan_left_camera->SetParam("fmax", 0, this->torque);
        }
    }

    else
    {
        if (encoder_left_camera.pan > motor_left_camera.pan)
        {
            joint_pose3dencoders_pan_left_camera->SetParam("vel", 0, 0.1);
            joint_pose3dencoders_pan_left_camera->SetParam("fmax", 0, this->torque);
        }
        else
        {
            joint_pose3dencoders_pan_left_camera->SetParam("vel", 0, 0);
            joint_pose3dencoders_pan_left_camera->SetParam("fmax", 0, this->torque);
        }
    }

    if (motor_right_camera.pan >= 0)
    {
        if (encoder_right_camera.pan < motor_right_camera.pan)
        {
            joint_pose3dencoders_pan_right_camera->SetParam("vel",0, -0.1);
            joint_pose3dencoders_pan_right_camera->SetParam("fmax",0, this->torque);
        }

        else
        {
            joint_pose3dencoders_pan_right_camera->SetParam("vel",0, 0);
            joint_pose3dencoders_pan_right_camera->SetParam("fmax",0, this->torque);
        }
    }

    else
    {
        if (encoder_right_camera.pan > motor_right_camera.pan)
        {
            joint_pose3dencoders_pan_right_camera->SetParam("vel",0, 0.1);
            joint_pose3dencoders_pan_right_camera->SetParam("fmax",0, this->torque);
        }
        else
        {
            joint_pose3dencoders_pan_right_camera->SetParam("vel",0, 0);
            joint_pose3dencoders_pan_right_camera->SetParam("fmax",0, this->torque);
        }
    }

    if (motor_left_camera.tilt >= 0)
    {
        if (encoder_left_camera.tilt < motor_left_camera.tilt)
        {
            joint_pose3dencoders_tilt_left_camera->SetParam("vel", 0, -0.1);
            joint_pose3dencoders_tilt_left_camera->SetParam("fmax", 0, this->torque);
        }
        else
        {
            joint_pose3dencoders_tilt_left_camera->SetParam("vel", 0, 0);
            joint_pose3dencoders_tilt_left_camera->SetParam("fmax", 0, this->torque);
        }
    }
    else
    {
        if (encoder_left_camera.tilt > motor_left_camera.tilt)
        {
            joint_pose3dencoders_tilt_left_camera->SetParam("vel", 0, 0.1);
            joint_pose3dencoders_tilt_left_camera->SetParam("fmax",0, this->torque);
        }
        else
        {
            joint_pose3dencoders_tilt_left_camera->SetParam("vel", 0, 0);
            joint_pose3dencoders_tilt_left_camera->SetParam("fmax",0, this->torque);
        }
    }
    if (motor_right_camera.tilt >= 0)
    {
        if (encoder_right_camera.tilt < motor_right_camera.tilt)
        {
            joint_pose3dencoders_tilt_right_camera->SetParam("vel", 0, -0.1);
            joint_pose3dencoders_tilt_right_camera->SetParam("fmax",0, this->torque);
        }
        else
        {
            joint_pose3dencoders_tilt_right_camera->SetParam("vel", 0, 0);
            joint_pose3dencoders_tilt_right_camera->SetParam("fmax",0, this->torque);
        }
    }
    else
    {
        if (encoder_right_camera.tilt > motor_right_camera.tilt)
        {
            joint_pose3dencoders_tilt_right_camera->SetParam("vel", 0, 0.1);
            joint_pose3dencoders_tilt_right_camera->SetParam("fmax",0, this->torque);
        }
        else
        {
            joint_pose3dencoders_tilt_right_camera->SetParam("vel", 0, 0);
            joint_pose3dencoders_tilt_right_camera->SetParam("fmax",0, this->torque);
        }
    }

    RosPublisher_Camera_Left_Encoder->publish(encoder_left_camera);
    RosPublisher_Camera_Right_Encoder->publish(encoder_right_camera);

    RosIceMessage::Pose3DMotorsData motor_left_camera_local;
    RosIceMessage::Pose3DMotorsData motor_right_camera_local;

    motor_left_camera_local.pan = encoder_left_camera.pan;
    motor_left_camera_local.tilt = encoder_left_camera.tilt;


    motor_right_camera_local.pan = encoder_right_camera.pan;
    motor_right_camera_local.tilt = encoder_right_camera.tilt;

    RosPublisher_Camera_Left_Motor->publish(motor_left_camera_local);
    RosPublisher_Camera_Right_Motor->publish(motor_right_camera_local);




}

void RosPose3DEncodersPlugin::rosCallback_Left_Camera(RosIceMessage::Pose3DMotorsData pose3DMotor)
{
    this->motor_left_camera = pose3DMotor;
}


void RosPose3DEncodersPlugin::rosCallback_Right_Camera(RosIceMessage::Pose3DMotorsData pose3DMotor)
{
    this->motor_right_camera = pose3DMotor;
}


}

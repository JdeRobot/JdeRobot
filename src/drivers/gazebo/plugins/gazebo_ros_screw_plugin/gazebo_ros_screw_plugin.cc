#ifndef _GAZEBO_ROS_SCREW_PLUGIN_HH_
#define _GAZEBO_ROS_SCREW_PLUGIN_HH_

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float32.h"

namespace gazebo
{
  /// \brief A plugin to control a Screw joint.
  class GazeboROSScrewPlugin : public ModelPlugin
  {
    /// \brief Constructor
    public: GazeboROSScrewPlugin() {}

    /// \brief The load function is called by Gazebo when the plugin is
    /// inserted into simulation
    /// \param[in] _model A pointer to the model that this plugin is
    /// attached to.
    /// \param[in] _sdf A pointer to the plugin's SDF element.
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      // We output a msg to see if the plugin is connected
      std::cerr << "\nThe Screw plugin is attach to model[" <<
        _model->GetName() << "]\n";

      // Safety check
      if (_model->GetJointCount() == 0)
      {
        std::cerr << "Invalid joint count ( == 0 ), Screw plugin not loaded\n";
        return;
      }

      // Store the model pointer for convenience.
      this->model = _model;

      // Get the required joint from gazebo

      this->joint = _model->GetJoint(GetScrewJointNameString());

      // Default to zero force on joint
      double jointForce = 0;

      // Check that the jointForce element exists, then read the value
      if (_sdf->HasElement("jointForce"))
        jointForce = _sdf->Get<double>("jointForce");

      // Set the joint's target force.
      this->joint->SetForce(0,jointForce);
      // For Data Transmission, first create the node
      this->node = transport::NodePtr(new transport::Node());
      #if GAZEBO_MAJOR_VERSION < 8
      this->node->Init(this->model->GetWorld()->GetName());
      #else
      this->node->Init(this->model->GetWorld()->Name());
      #endif

      // Create a topic name
      std::string topicName = "~/amazon_robot_warehouse/joint_cmd";
      // std::string topicName = "~/" + this->model->GetName() + "/vel_cmd";
      // Subscribe to the topic, and register a callback
      this->sub = this->node->Subscribe(topicName,
         &GazeboROSScrewPlugin::OnMsg, this);



      // Initialize ros, if it has not already been initialized.
      if (!ros::isInitialized())
      {
        int argc = 0;
        char **argv = NULL;
        ros::init(argc, argv, "gazebo_client",
            ros::init_options::NoSigintHandler);
      }

      // Create our ROS node. This acts in a similar manner to
      // the Gazebo node
      this->rosNode.reset(new ros::NodeHandle("gazebo_client"));

      // Create a named topic, and subscribe to it.
      ros::SubscribeOptions so =
        ros::SubscribeOptions::create<std_msgs::Float32>(
            "/" + this->model->GetName() + "/joint_cmd",
            1,
            boost::bind(&GazeboROSScrewPlugin::OnRosMsg, this, _1),
            ros::VoidPtr(), &this->rosQueue);
      this->rosSub = this->rosNode->subscribe(so);

      // Spin up the queue helper thread.
      this->rosQueueThread =
        std::thread(std::bind(&GazeboROSScrewPlugin::QueueThread, this));
    }

      /// \brief Set the jointForce of the Screw Joint
      /// \param[in] _jForce New target joint Force
      public: void SetJointForce(const double &_jForce)
      {
        // Set the joint's target force.
        this->joint->SetForce(0,_jForce);
      }

      /// \brief Function to use the name of screw joint in our robot
      const std::string& GetScrewJointNameString()
      {
          // Initialize the static variable
          static std::string foo("amazon_warehouse_robot::top_screw_lift_hinge");
          return foo;
      }

      /// \brief Handle an incoming message from ROS
      /// \param[in] _msg A float value that is used to set the force
      public: void OnRosMsg(const std_msgs::Float32ConstPtr &_msg)
      {
        this->SetJointForce(_msg->data);
      }

      /// \brief ROS helper function that processes messages
      private: void QueueThread()
      {
        static const double timeout = 0.01;
        while (this->rosNode->ok())
        {
          this->rosQueue.callAvailable(ros::WallDuration(timeout));
        }
      }
    /// \brief Pointer to the model.
    private: physics::ModelPtr model;

    /// \brief Pointer to the joint.
    private: physics::JointPtr joint;


    /// \brief A node used for transport
    private: transport::NodePtr node;

    /// \brief A subscriber to a named topic.
    private: transport::SubscriberPtr sub;

    /// \brief Handle incoming message
    /// \param[in] _msg Repurpose a vector3 message. This function will
    /// only use the x component.
    private: void OnMsg(ConstVector3dPtr &_msg)
    {
        this->SetJointForce(_msg->x());
    }


      /// \brief A node use for ROS transport
      private: std::unique_ptr<ros::NodeHandle> rosNode;

      /// \brief A ROS subscriber
      private: ros::Subscriber rosSub;

      /// \brief A ROS callbackqueue that helps process messages
      private: ros::CallbackQueue rosQueue;

      /// \brief A thread the keeps running the rosQueue
      private: std::thread rosQueueThread;
  };

  // Tell Gazebo about this plugin, so that Gazebo can call Load on this plugin.
  GZ_REGISTER_MODEL_PLUGIN(GazeboROSScrewPlugin)
}
#endif

#include "poseplugin.hh"

namespace gazebo {

GZ_REGISTER_MODEL_PLUGIN(PosePlugin)

PosePlugin::PosePlugin() {
    std::cout << "Constructor PosePlugin\r\n";
}

PosePlugin::~PosePlugin() {}

void PosePlugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
    this->model = _model;
    this->sdf = _sdf;
    this->world = _model->GetWorld();

    if(!_sdf->HasElement("poseTopic"))
        this->pose_topic_ = "~/pose_topic";
    else
        this->pose_topic_ = _sdf->GetElement("poseTopic")->GetValue()->GetAsString();

    transport::NodePtr node(new transport::Node());
    node->Init();
    pub_ = node->Advertise<msgs::Pose>(pose_topic_);

    this->updateConnection = event::Events::ConnectWorldUpdateBegin(
          boost::bind(&PosePlugin::OnUpdate, this));
}

void PosePlugin::Init() {
    //pub_->WaitForConnection();
}

void PosePlugin::OnUpdate() {
    math::Pose pose = model->GetWorldPose();
    msgs::Pose msg;
    msgs::Set(&msg, pose);
    pub_->Publish(msg);
}

} // gazebo

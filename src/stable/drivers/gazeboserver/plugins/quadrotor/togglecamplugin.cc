#include "togglecamplugin.hh"

namespace gazebo {

GZ_REGISTER_SENSOR_PLUGIN(ToggleCamPlugin)

/*
bool toggle;
bool recentlyToggled[2];
*/

void ToggleCamPlugin::ToggleCallback(
        const boost::shared_ptr<const msgs::Vector2d> &_msg) {
    pthread_mutex_lock(&mutex);
    //toggle = true;
    on_ = !on_;
    pthread_mutex_unlock(&mutex);
}

ToggleCamPlugin::ToggleCamPlugin() : CameraPlugin(), count(0) {
	pthread_mutex_init (&mutex, NULL);
	n = round(rand()*1000);
	std::cout << "Constructor ToggleCamPlugin\n";
}

void ToggleCamPlugin::Load(sensors::SensorPtr _parent, sdf::ElementPtr _sdf) {
	// Don't forget to load the camera plugin
	CameraPlugin::Load(_parent,_sdf);

    if (!_sdf->HasElement("initialStatus"))
        on_ = 0;
    else {
        std::string initial_status = _sdf->GetElement("initialStatus")
                                            ->GetValue()->GetAsString();
    	std::string on_string("on");
        on_ = !strcmp(initial_status.c_str(), on_string.c_str());
    }
    if (!_sdf->HasElement("cameraId"))
        camera_id_ = 0;
    else {
        std::string camera_id = _sdf->GetElement("cameraId")
                                            ->GetValue()->GetAsString();
        std::istringstream(camera_id) >> camera_id_;
    }
    if (!_sdf->HasElement("imgTopic"))
        img_topic_ = "~/image_topic";
    else
        img_topic_ = _sdf->GetElement("imgTopic")->GetValue()->GetAsString();
    if (!_sdf->HasElement("toggleTopic"))
        toggle_topic_ = "~/toggle_topic";
    else
        toggle_topic_ = _sdf->GetElement("toggleTopic")->GetValue()->GetAsString();

    //recentlyToggled[camera_id_] = false;

    transport::NodePtr sub_node(new transport::Node());
    sub_node->Init();
    sub_ = sub_node->Subscribe(toggle_topic_, &ToggleCamPlugin::ToggleCallback, this);

    transport::NodePtr pub_node(new transport::Node());
    pub_node->Init();
    pub_ = pub_node->Advertise<msgs::Image>(img_topic_);

    //If we make SetActive(false), OnNewFrame won't be invoked.
    this->parentSensor->SetActive(true);

	std::cout << "Load: " <<n << " "
            << this->parentSensor->GetCamera()->GetName()
            << " On: " << on_ << ". Camera Id: " << camera_id_
            << std::endl;
}

// Update the controller
void ToggleCamPlugin::OnNewFrame(const unsigned char *_image, 
		unsigned int _width, unsigned int _height, unsigned int _depth, 
		const std::string &_format) {
    pthread_mutex_lock (&mutex);

/*
    if(toggle && !recentlyToggled[camera_id_]) {
        on_ = !on_;
        recentlyToggled[camera_id_] = true;
        // Si soy el último en cambiar, reseteo todos los valores:
        if (recentlyToggled[0] && recentlyToggled[1]) {
            toggle = recentlyToggled[0] = recentlyToggled[1] = false;
        }
        pthread_mutex_unlock(&mutex);
        return;
    }
*/
    if (!on_) {
        pthread_mutex_unlock (&mutex);
        return;
    }
	if(count==0){
		image.create(_height, _width, CV_8UC3);
		count++;
	}
	memcpy((unsigned char *) image.data, &(_image[0]), _width*_height * 3);
		common::Image msg_image;
		msg_image.SetFromData((unsigned char *)image.data,
                              _width,
                              _height,
                              common::Image::RGB_INT8);
	    msgs::Image msg;
	    msgs::Set(&msg, msg_image);
		//msg.step = image.step[0];
	    pub_->Publish(msg);
	pthread_mutex_unlock (&mutex);
}

} //gazebo

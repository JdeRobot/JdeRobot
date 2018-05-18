#include "sensors.h"

Sensors::Sensors(Comm::Communicator* jdrc)
{
    this-> jdrc = jdrc;
    

    this->poseClient = Comm::getPose3dClient(jdrc, "carViz.Pose3D");


    ////////////////////////////// CAMERA1 /////////////////////////////

	this->camera1 = Comm::getCameraClient(jdrc, "carViz.Camera1");

    ////////////////////////////// CAMERA2 /////////////////////////////
	this->camera2 = Comm::getCameraClient(jdrc, "carViz.Camera2");

    ////////////////////////////// LASER //////////////////////////////
	// Contact to LASER interface

	this->laserClient = Comm::getLaserClient(jdrc, "carViz.Laser");
}

JdeRobotTypes::Image Sensors::getImage1()
{
	JdeRobotTypes::Image img;

	if (this->camera1){
    	img = this->camera1->getImage();
    }

    return img;

}

JdeRobotTypes::Image Sensors::getImage2()
{
    JdeRobotTypes::Image img;

	if (this->camera2){
    	img = this->camera2->getImage();
    }

    return img;
}

//magnitude = sqrt(this->pose3ddata->q0 * this->pose3ddata->q0 + this->pose3ddata->q1 * this->pose3ddata->q1 + this->pose3ddata->q2 * this->pose3ddata->q2 + this->pose3ddata->q3 * this->pose3ddata->q3);

JdeRobotTypes::Pose3d Sensors::getPose()
{

	JdeRobotTypes::Pose3d pose3d;

	if (this->poseClient){
    	pose3d = this->poseClient->getPose();
    }

    return pose3d;
}


JdeRobotTypes::LaserData Sensors::getLaserData()
{
	JdeRobotTypes::LaserData laser;

	if (this->laserClient){
    	laser = this->laserClient->getLaserData();
    }

    return laser;
}


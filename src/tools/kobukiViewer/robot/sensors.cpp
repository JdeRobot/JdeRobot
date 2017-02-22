#include "sensors.h"

Sensors::Sensors(Ice::CommunicatorPtr ic)
{
    this-> ic = ic;
    Ice::PropertiesPtr prop = ic->getProperties();

    ////////////////////////////// Pose3D //////////////////////////////
    // Contact to POSE3D interface
    Ice::ObjectPrx basePose3D = ic->propertyToProxy("kobukiViewer.Pose3D.Proxy");
    if (0 == basePose3D) {
		pose3dON = false;
		std::cout << "Pose3D configuration not specified" <<std::endl;
        //throw "Could not create proxy with pose3D";
	}else{
		// Cast to pose3D
		try {
			p3dprx = jderobot::Pose3DPrx::checkedCast(basePose3D);
			if (0 == p3dprx)
				throw "Invalid proxy kobukiViewer.Pose3D.Proxy";

			pose3dON = true;
			std::cout << "Pose3D connected" << std::endl;
		}catch (Ice::ConnectionRefusedException& e){
			pose3dON=false;
			std::cout << "Pose3D inactive" << std::endl;
		}
	}


    ////////////////////////////// CAMERA1 /////////////////////////////2

	jderobot::ImageDataPtr data;

    Ice::ObjectPrx baseCamera1 = ic->propertyToProxy("kobukiViewer.Camera1.Proxy");
    if (0==baseCamera1) {
		camera1ON = false;
		image1.create(400, 400, CV_8UC3);
		std::cout << "Camera1 configuration not specified" <<std::endl;
      //throw "Could not create proxy";
	}else{
    /*cast to CameraPrx*/
	try {
		camera1 = jderobot::CameraPrx::checkedCast(baseCamera1);
		if (0==camera1)
		  throw "Invalid proxy";

		camera1ON = true;
		std::cout << "Camera1 connected" << std::endl;

		data = camera1->getImageData(camera1->getImageFormat().at(0));
		image1.create(data->description->height, data->description->width, CV_8UC3);
	}catch (Ice::ConnectionRefusedException& e){
		camera1ON=false;
		std::cout << "Camera1 inactive" << std::endl;

		//create an empty image if no camera connected (avoid seg. fault)
		image1.create(400, 400, CV_8UC3);
	}}

    ////////////////////////////// CAMERA2 /////////////////////////////2
	Ice::ObjectPrx baseCamera2 = ic->propertyToProxy("kobukiViewer.Camera2.Proxy");
    if (0==baseCamera2) {
		camera2ON = false;
		image2.create(400, 400, CV_8UC3);
		std::cout << "Camera2 configuration not specified" <<std::endl;
      //throw "Could not create proxy";
	}else{
    /*cast to CameraPrx*/
	try {
		camera2 = jderobot::CameraPrx::checkedCast(baseCamera2);
		if (0==camera2)
		  throw "Invalid proxy";

		camera2ON = true;
		std::cout << "Camera2 connected" << std::endl;

		data = camera2->getImageData(camera2->getImageFormat().at(0));
		image2.create(data->description->height, data->description->width, CV_8UC3);
	}catch (Ice::ConnectionRefusedException& e){
		camera2ON=false;
		std::cout << "Camera2 inactive" << std::endl;

		//create an empty image if no camera connected (avoid seg. fault)
		image2.create(400, 400, CV_8UC3);
	}}

    ////////////////////////////// LASER //////////////////////////////
	// Contact to LASER interface

	this->laserClient = JdeRobotComm::getLaserClient(ic, "kobukiViewer.Laser");
}

cv::Mat Sensors::getCamera1()
{
    mutex.lock();
    cv::Mat result = image1.clone();
    mutex.unlock();
    return result;

}

cv::Mat Sensors::getCamera2()
{
    mutex.lock();
    cv::Mat result = image2.clone();
    mutex.unlock();
    return result;
}

void Sensors::update()
{
	if (pose3dON) {
    	pose3ddata = this->p3dprx->getPose3DData();
	    mutex.lock();
		robotx = pose3ddata->x;
		roboty = pose3ddata->y;

		//theta
		double magnitude,w,x,y,z,squ,sqx,sqy,sqz;
		magnitude = sqrt(this->pose3ddata->q0 * this->pose3ddata->q0 + this->pose3ddata->q1 * this->pose3ddata->q1 + this->pose3ddata->q2 * this->pose3ddata->q2 + this->pose3ddata->q3 * this->pose3ddata->q3);

		w = this->pose3ddata->q0 / magnitude;
		x = this->pose3ddata->q1 / magnitude;
		y = this->pose3ddata->q2 / magnitude;
		z = this->pose3ddata->q3 / magnitude;

		squ = w * w;
		sqx = x * x;
		sqy = y * y;
		sqz = z * z;

		double angle;

		angle = atan2( 2 * (x * y + w * z), squ + sqx - sqy - sqz) * 180.0 / M_PI;

		if(angle < 0)
		{
		    angle += 360.0;
		}

		this->robottheta = angle;

	    mutex.unlock();
	}

	if (camera1ON) {
	    jderobot::ImageDataPtr data = camera1->getImageData(camera1->getImageFormat().at(0));
		mutex.lock();
	    memcpy((unsigned char *) image1.data ,&(data->pixelData[0]), image1.cols*image1.rows * 3);
		mutex.unlock();
	}

	if (camera2ON) {
	    jderobot::ImageDataPtr data2 = camera2->getImageData(camera2->getImageFormat().at(0));
		mutex.lock();
	    memcpy((unsigned char *) image2.data ,&(data2->pixelData[0]), image2.cols*image2.rows * 3);
	    mutex.unlock();
	}
	//if (this->laserClient->on){
	//	this->laserClient->resume();
	//}

    
}

float Sensors::getRobotPoseX()
{

	float x;
	mutex.lock();
	if (pose3dON) 
	    x = this->robotx;
   	else
		x = 0;
	mutex.unlock();

    return x;
}

float Sensors::getRobotPoseY()
{
    float y;
	mutex.lock();
	if (pose3dON) 
	    y = this->roboty;
   	else
		y = 0;
	mutex.unlock();

    return y;
}

float Sensors::getRobotPoseTheta()
{
    float theta;
	mutex.lock();
	if (pose3dON) 
	    theta = this->robottheta;
   	else
		theta = 0;
	mutex.unlock();

    return theta;
}

JdeRobotTypes::LaserData Sensors::getLaserData()
{
	return this->laserClient->getLaserData();
    /*JdeRobotTypes::LaserData laserDataAux;
    mutex.lock();
	if (laserON)
	    laserDataAux = laserData;
    mutex.unlock();
    return laserDataAux;
    */
}


#include "sensors.h"

Sensors::Sensors(Ice::CommunicatorPtr ic)
{
    this-> ic = ic;
    Ice::PropertiesPtr prop = ic->getProperties();

    ////////////////////////////// ENCODERS //////////////////////////////
    // Contact to ENCODERS interface
		try {
			eprx = easyiceconfig::proxies::createProxy<jderobot::EncodersPrx>(ic, "introrob.Encoders.Proxy", false);
			encodersON = true;
		}catch (Ice::Exception& e){
			encodersON=false;
			std::cout << "Encoders inactive." << std::endl;
			std::cout << "Reason: "<< e << std::endl << std::endl;
		}


    ////////////////////////////// CAMERA1 /////////////////////////////2

	jderobot::ImageDataPtr data;
	camera1 = EasyIce::EasyProxy<jderobot::CameraPrx>(ic, "introrob.Camera1.Proxy", false);
	if (camera1){
		camera1ON = true;
		data = camera1->getImageData(camera1->getImageFormat().at(0));
		image1.create(data->description->height, data->description->width, CV_8UC3);
	}else{
		camera1ON=false;
		std::cout << "Camera1 inactive." << std::endl;
		std::cout << "Reason: " << camera1.exception() << std::endl << std::endl;

		//create an empty image if no camera connected (avoid seg. fault)
		image1.create(400, 400, CV_8UC3);
	}

    ////////////////////////////// CAMERA2 /////////////////////////////
	camera2 = EasyIce::EasyProxy<jderobot::CameraPrx>(ic, "introrob.Camera2.Proxy", false);
	if (camera2){
		camera2ON = true;
		data = camera2->getImageData(camera2->getImageFormat().at(0));
		image2.create(data->description->height, data->description->width, CV_8UC3);
	}else{
		camera2ON=false;
		std::cout << "Camera2 inactive." << std::endl;
		std::cout << "Reason: " << camera2.exception() << std::endl << std::endl;

		//create an empty image if no camera connected (avoid seg. fault)
		image2.create(400, 400, CV_8UC3);
	}

    ////////////////////////////// LASER //////////////////////////////
	// Contact to LASER interface
	laserprx = EasyIce::EasyProxy<jderobot::LaserPrx>(ic, "introrob.Laser.Proxy", false);
	if (laserprx) {
		laserON = true;
		std::cout << "Laser connected" << std::endl;
	}else{
		laserON=false;
		std::cout << "Laser inactive" << std::endl;
		std::cout << "Reason: " << laserprx.exception() << std::endl << std::endl;
	}

    /*boolLaser = prop->getPropertyAsInt("introrob.Laser");

    std::cout << "Laser " <<  boolLaser << std::endl;
    if(boolLaser){
        // Contact to LASER interface
        Ice::ObjectPrx laserICE = ic->propertyToProxy("introrob.Laser.Proxy");
        if (0 == laserICE)
            throw "Could not create proxy with Laser";

        // Cast to LASER
        laserprx = jderobot::LaserPrx::checkedCast(laserICE);
        if (0 == laserprx){
           throw std::string("Invalid proxy introrob.Laser.Proxy");
        }
    }*/
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
	if (encodersON) {
    	encodersData = this->eprx->getEncodersData();
	    mutex.lock();
		robotx = encodersData->robotx;
		roboty = encodersData->roboty;
		robottheta = encodersData->robottheta;
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

	if (laserON) {
		ld = laserprx->getLaserData();
		mutex.lock();
		laserData.resize(ld->numLaser);
        for(int i = 0; i< ld->numLaser; i++ ){
            laserData[i] = ld->distanceData[i];
        }
		mutex.unlock();
	}

    
}

float Sensors::getRobotPoseX()
{

	float x;
	mutex.lock();
	if (encodersON) 
	    x = robotx;
   	else
		x = 0;
	mutex.unlock();

    return x;
}

float Sensors::getRobotPoseY()
{
    float y;
	mutex.lock();
	if (encodersON) 
	    y = roboty;
   	else
		y = 0;
	mutex.unlock();

    return y;
}

float Sensors::getRobotPoseTheta()
{
    float theta;
	mutex.lock();
	if (encodersON) 
	    theta = robottheta;
   	else
		theta = 0;
	mutex.unlock();

    return theta;
}

std::vector<float> Sensors::getLaserData()
{
    std::vector<float> laserDataAux;
    mutex.lock();
	if (laserON)
	    laserDataAux = laserData;
	else
		laserDataAux = {0};
    mutex.unlock();
    return laserDataAux;
}


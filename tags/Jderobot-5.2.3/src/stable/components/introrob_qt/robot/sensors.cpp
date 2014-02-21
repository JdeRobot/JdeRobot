#include "sensors.h"

Sensors::Sensors(Ice::CommunicatorPtr ic)
{
    this-> ic = ic;
    Ice::PropertiesPtr prop = ic->getProperties();


    ////////////////////////////// ENCODERS //////////////////////////////
    // Contact to ENCODERS interface
    Ice::ObjectPrx baseEncoders = ic->propertyToProxy("introrob.Encoders.Proxy");
    if (0 == baseEncoders)
        throw "Could not create proxy with encoders";

    // Cast to encoders
    eprx = jderobot::EncodersPrx::checkedCast(baseEncoders);
    if (0 == eprx)
        throw "Invalid proxy introrob.Encoders.Proxy";

    ////////////////////////////// CAMERA1 /////////////////////////////2

    Ice::ObjectPrx baseCamera1 = ic->propertyToProxy("introrob.Camera1.Proxy");
    if (0==baseCamera1)
      throw "Could not create proxy";

    /*cast to CameraPrx*/
    camera1 = jderobot::CameraPrx::checkedCast(baseCamera1);
    if (0==camera1)
      throw "Invalid proxy";

    jderobot::ImageDataPtr data = camera1->getImageData();
    image1.create(data->description->height, data->description->width, CV_8UC3);

    ////////////////////////////// CAMERA2 /////////////////////////////2
    Ice::ObjectPrx baseCamera2 = ic->propertyToProxy("introrob.Camera2.Proxy");
    if (0==baseCamera2)
      throw "Could not create proxy";

    /*cast to CameraPrx*/
    camera2 = jderobot::CameraPrx::checkedCast(baseCamera2);
    if (0==camera2)
      throw "Invalid proxy";

    data = camera2->getImageData();
    image2.create(data->description->height, data->description->width, CV_8UC3);

    ////////////////////////////// LASER //////////////////////////////
    boolLaser = prop->getPropertyAsInt("introrob.Laser");

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
    }
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
    encodersData = this->eprx->getEncodersData();
    jderobot::ImageDataPtr data = camera1->getImageData();
    jderobot::ImageDataPtr data2 = camera2->getImageData();


    mutex.lock();
    robotx = encodersData->robotx;
    roboty = encodersData->roboty;
    robottheta = encodersData->robottheta;

    if(boolLaser){
        ld = laserprx->getLaserData();
        laserData.resize(ld->numLaser);
        for(int i = 0; i< ld->numLaser; i++ ){
            laserData[i] = ld->distanceData[i];
        }
    }

    memcpy((unsigned char *) image1.data ,&(data->pixelData[0]), image1.cols*image1.rows * 3);

    memcpy((unsigned char *) image2.data ,&(data2->pixelData[0]), image2.cols*image2.rows * 3);

    mutex.unlock();
}

bool Sensors::getBoolLaser()
{
    mutex.lock();
    float x = boolLaser;
    mutex.unlock();
    return x;
}


float Sensors::getRobotPoseX()
{
    mutex.lock();
    float x = robotx;
    mutex.unlock();
    return x;
}

float Sensors::getRobotPoseY()
{
    mutex.lock();
    float y = roboty;
    mutex.unlock();
    return y;
}

float Sensors::getRobotPoseTheta()
{
    mutex.lock();
    float theta = robottheta;
    mutex.unlock();
    return theta;
}

std::vector<float> Sensors::getLaserData()
{
    std::vector<float> laserDataAux;
    mutex.lock();
    laserDataAux = laserData;
    mutex.unlock();
    return laserDataAux;
}


#include "sensors.h"

Sensors::Sensors(Ice::CommunicatorPtr ic)
{
    this-> ic = ic;
    Ice::PropertiesPtr prop = ic->getProperties();

    Ice::ObjectPrx base = ic->propertyToProxy("Cameraview.Camera.Proxy");
    if (0==base)
      throw "Could not create proxy";

    /*cast to CameraPrx*/
    cprx = jderobot::CameraPrx::checkedCast(base);
    if (0==cprx)
      throw "Invalid proxy";

    jderobot::ImageDataPtr data = cprx->getImageData();
    image.create(data->description->height, data->description->width, CV_8UC3);


}

void Sensors::update()
{
    jderobot::ImageDataPtr data = cprx->getImageData();
    mutex.lock();
    memcpy((unsigned char *) image.data ,&(data->pixelData[0]), image.cols*image.rows * 3);
    mutex.unlock();

}

cv::Mat Sensors::getImage()
{
    mutex.lock();

    cv::Mat result = image.clone();

    mutex.unlock();

    return result;
}

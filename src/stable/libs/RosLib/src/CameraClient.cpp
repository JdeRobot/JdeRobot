#include "CameraClient.h"

CameraClient::CameraClient(int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    addRosImagePublisher(nodeName,1000);

    ImageWindow = nodeName;

}

CameraClient::~CameraClient()
{
}

void CameraClient::publishROS()
{
    cv::Mat image;
    jderobot::ImageDataPtr imageData;

    imageData = this->Proxy->getImageData( colorspaces::ImageRGB8::FORMAT_RGB8.get()->name );

    image.create(imageData->description->height, imageData->description->width, CV_8UC3);
    memcpy((unsigned char *) image.data, &(imageData->pixelData[0]), image.cols*image.rows*3);

    sensor_msgs::ImagePtr img_message = cv_bridge::CvImage(std_msgs::Header(), "rgb8", image).toImageMsg();


    rosImagePublish(img_message);
}
/*

void CameraClient::rosCallback(const sensor_msgs::ImageConstPtr& image_message)
{
    cv_bridge::CvImagePtr cv_ptr;

    try
    {
        cv_ptr = cv_bridge::toCvCopy(image_message, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::Mat image = cv_ptr->image;

    if(image.cols > 0 && image.rows > 0)
    {

        cv::imshow(ImageWindow, image);
        cv::waitKey(3);

    }


}

*/


#include "CameraInterface.h"

CameraInterface::CameraInterface(int argc, char **argv, std::string nodeName)
{

    imageDescription = (new jderobot::ImageDescription());
    cameraDescription = (new jderobot::CameraDescription());
    cameraDescription->name = "camera Introrob";


    initializeROS(argc,argv,nodeName);
    addRosImageSubscriber(nodeName,500,&CameraInterface::rosCallback,this);


    replyTask = new ReplyTask(this);
    replyTask->start();



}

CameraInterface::~CameraInterface()
{

}

jderobot::ImageDescriptionPtr CameraInterface::getImageDescription(const Ice::Current& c)
{

    return imageDescription;

}

jderobot::CameraDescriptionPtr CameraInterface::getCameraDescription(const Ice::Current& c)
{
    return cameraDescription;
}

Ice::Int CameraInterface::setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c)
{
    return 0;
}


void CameraInterface::getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c)
{
    replyTask->pushJob(cb, format);

}


jderobot::ImageFormat CameraInterface::getImageFormat(const Ice::Current &c)
{
    jderobot::ImageFormat mFormats;
    mFormats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);

    return mFormats;
}

std::string CameraInterface::startCameraStreaming(const Ice::Current&)
{

}

void CameraInterface::stopCameraStreaming(const Ice::Current&)
{

}

void CameraInterface::reset(const Ice::Current&)
{

}

void CameraInterface::rosCallback(const sensor_msgs::ImageConstPtr &image_message)
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

    image.copyTo(image_camera);
/*
    if(image.cols > 0 && image.rows > 0)
    {

        imageDescription->width = image.cols;
        imageDescription->height = image.rows;
        imageDescription->size = image.cols * image.rows*3;
        imageDescription->format = "RGB8";

        reply->description = imageDescription;


        IceUtil::Time t = IceUtil::Time::now();
        reply->timeStamp.seconds = (long)t.toSeconds();
        reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;

        reply->pixelData.resize(image.rows * image.cols * 3);

        memcpy( &(reply->pixelData[0]), (unsigned char *) image.data, image.rows * image.cols*3);


    }


*/
}

CameraInterface::ReplyTask::ReplyTask(CameraInterface *camera)
{
    mycamera = camera;
}

void CameraInterface::ReplyTask::pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format)
{
    mFormat = format;
    IceUtil::Mutex::Lock sync(requestsMutex);
    requests.push_back(cb);
}

void CameraInterface::ReplyTask::run()
{

    jderobot::ImageDataPtr reply(new jderobot::ImageData);
    struct timeval a, b;
    int cycle = 48;
    long totalb,totala;
    long diff;

    int count = 0;

    while(1)
    {

        if(!mycamera->image_camera.data)
        {
            usleep(100);
            continue;
        }

        if(count==0)
        {
            mycamera->imageDescription->width = mycamera->image_camera.cols;
            mycamera->imageDescription->height = mycamera->image_camera.rows;
            mycamera->imageDescription->size = mycamera->image_camera.cols * mycamera->image_camera.rows * 3;

            mycamera->imageDescription->format = "RGB8";

            reply->description = mycamera->imageDescription;
            count++;
        }


        gettimeofday(&a,NULL);
        totala=a.tv_sec*1000000+a.tv_usec;


        IceUtil::Time t = IceUtil::Time::now();
        reply->timeStamp.seconds = (long)t.toSeconds();
        reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;

        reply->pixelData.resize(mycamera->image_camera.rows * mycamera->image_camera.cols * 3);

        memcpy( &(reply->pixelData[0]), (unsigned char *) mycamera->image_camera.data, mycamera->image_camera.rows * mycamera->image_camera.cols * 3);

        {
            IceUtil::Mutex::Lock sync(requestsMutex);
            while(!requests.empty())
            {
                jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
                requests.pop_front();
                cb->ice_response(reply);
            }
        }

        gettimeofday(&b,NULL);
        totalb=b.tv_sec*1000000+b.tv_usec;

        diff = (totalb-totala)/1000;
        diff = cycle-diff;


        if(diff < 33)
            diff = 33;


        /*Sleep Algorithm*/
        usleep(diff*1000);
    }

}

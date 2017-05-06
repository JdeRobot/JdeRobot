#include "KinectInterface.h"

KinectInterface::KinectInterface(int argc, char **argv, std::string nodeName, std::string encoding)
{

    imageDescription = (new jderobot::ImageDescription());
    cameraDescription = (new jderobot::CameraDescription());
    cameraDescription->name = "camera Introrob";


    initializeROS(argc,argv,nodeName);
    addRosImageSubscriber(nodeName,500,&KinectInterface::rosCallback,this);


    replyTask = new ReplyTask(this);
    replyTask->start();

    image_encoding = encoding;



}

KinectInterface::~KinectInterface()
{

}

jderobot::ImageDescriptionPtr KinectInterface::getImageDescription(const Ice::Current& c)
{

    return imageDescription;

}

jderobot::CameraDescriptionPtr KinectInterface::getCameraDescription(const Ice::Current& c)
{
    return cameraDescription;
}

Ice::Int KinectInterface::setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c)
{
    return 0;
}


void KinectInterface::getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c)
{
    replyTask->pushJob(cb, format);

}


jderobot::ImageFormat KinectInterface::getImageFormat(const Ice::Current &c)
{
    jderobot::ImageFormat mFormats;
    mFormats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);

    return mFormats;
}

std::string KinectInterface::startCameraStreaming(const Ice::Current&)
{

}

void KinectInterface::stopCameraStreaming(const Ice::Current&)
{

}

void KinectInterface::reset(const Ice::Current&)
{

}

void KinectInterface::rosCallback(const sensor_msgs::ImageConstPtr &image_message)
{
    cv_bridge::CvImagePtr cv_ptr;

    try
    {
        cv_ptr = cv_bridge::toCvCopy(image_message, image_encoding);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::Mat image = cv_ptr->image;

    image.copyTo(image_camera);

}

KinectInterface::ReplyTask::ReplyTask(KinectInterface *camera)
{
    mycamera = camera;
}

void KinectInterface::ReplyTask::pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format)
{
    mFormat = format;
    IceUtil::Mutex::Lock sync(requestsMutex);
    requests.push_back(cb);
}

void KinectInterface::ReplyTask::run()
{

    jderobot::ImageDataPtr reply(new jderobot::ImageData);
    struct timeval a, b;
    int cycle = 48;
    long totalb,totala;
    long diff;
    int frame_size;

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
/*
            if(mycamera->image_encoding == sensor_msgs::image_encodings::BGR8)
            {
                frame_size = mycamera->image_camera.cols * mycamera->image_camera.rows * 3;
            }

            else
            {
                frame_size = mycamera->image_camera.cols * mycamera->image_camera.rows;
            }*/

            frame_size = mycamera->image_camera.cols * mycamera->image_camera.rows * 3;


            mycamera->imageDescription->size = frame_size;


            mycamera->imageDescription->format = "RGB8";

            reply->description = mycamera->imageDescription;
            count++;
        }


        gettimeofday(&a,NULL);
        totala=a.tv_sec*1000000+a.tv_usec;


        IceUtil::Time t = IceUtil::Time::now();
        reply->timeStamp.seconds = (long)t.toSeconds();
        reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;

        reply->pixelData.resize(frame_size);

        memcpy( &(reply->pixelData[0]), (unsigned char *) mycamera->image_camera.data, frame_size);

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

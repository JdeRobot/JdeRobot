#ifndef CAMERAINTERFACE_H
#define CAMERAINTERFACE_H

#include <jderobot/camera.h>
#include <visionlib/colorspaces/colorspacesmm.h>
#include "Ros_Ice.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <IceUtil/IceUtil.h>

#include <fstream>

class CameraInterface : virtual public jderobot::Camera, public Ros_Ice< Ice::ObjectPrx >
{

public:
    CameraInterface(int argc, char **argv, std::string nodeName);

    virtual ~CameraInterface();

    virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c);

    virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c);

    virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c);

    virtual void getImageData_async (const jderobot::AMD_ImageProvider_getImageDataPtr& cb,const std::string& format, const Ice::Current& c);


    virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c);


    virtual std::string startCameraStreaming(const Ice::Current&);

    virtual void stopCameraStreaming(const Ice::Current&);

    virtual void reset(const Ice::Current&);

    cv::Mat image_camera;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;



private:

    void rosCallback(const sensor_msgs::ImageConstPtr &image_message);



    class ReplyTask: public IceUtil::Thread
    {
    public:

        ReplyTask(CameraInterface* camera);

        void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, std::string format);

        virtual void run();

        CameraInterface* mycamera;
        IceUtil::Mutex requestsMutex;
        std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;

        std::string mFormat;
    };

    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
    ReplyTaskPtr replyTask;


};


#endif // CAMERAINTERFACE_H

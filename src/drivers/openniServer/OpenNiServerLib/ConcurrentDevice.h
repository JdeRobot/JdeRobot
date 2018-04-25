//
// Created by frivas on 23/01/17.
//

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <OpenNI.h>
#include <OpenNiServerLib/OpenNIDeviceListener.h>
#include <OpenNITypes.h>
#include <opencv2/opencv.hpp>
#include <boost/shared_ptr.hpp>


#ifndef SAMPLERGENERATOR_CONCURRENTDEVICE_H
#define SAMPLERGENERATOR_CONCURRENTDEVICE_H


class ConcurrentDevice:public IceUtil::Thread {
public:
    ConcurrentDevice(int fps,int cameraIdx, DeviceConfig config, const cv::Size& size);
    ~ConcurrentDevice();
    cv::Mat getRGBImage(bool withlock=true);
    cv::Mat getDepthImage(bool withlock=true);
    void getSyncData(cv::Mat& rgb, cv::Mat& depth);
    void getDistances(std::vector<int>& distances);
    virtual void run();
    void stop();
    DeviceConfig getConfig();
    VideoMode getVideoMode();
    bool isValid();


private:

    bool componentAlive;
    int fps;
    int cameraIdx;
    DeviceConfig config;
    cv::Size workingSize;
    const char* deviceUri;

    DeviceParameter g_Registration;
    DeviceParameter g_Resolution;
    bool g_bIsDepthOn;
    bool g_bIsColorOn;
    bool g_bIsIROn;
    bool g_isValid;

    openni::Device g_device;
    openni::PlaybackControl* g_pPlaybackControl;

    openni::VideoStream g_depthStream;
    openni::VideoStream g_colorStream;
    openni::VideoStream g_irStream;

    openni::VideoFrameRef g_depthFrame;
    openni::VideoFrameRef g_colorFrame;
    openni::VideoFrameRef g_irFrame;

    const openni::SensorInfo* g_depthSensorInfo = NULL;
    const openni::SensorInfo* g_colorSensorInfo = NULL;
    const openni::SensorInfo* g_irSensorInfo = NULL;

    IceUtil::Mutex mutex;
    VideoMode videoMode;
    float cycle;
    IceUtil::Time lastIT;





    int openStream(const char* name, openni::SensorType sensorType, SensorOpenType openType, openni::VideoStream& stream, const openni::SensorInfo** ppSensorInfo, bool* pbIsStreamOn);
    void initConstants();
    void readFrame();
    int openCommon();
    void changeRegistration(int value);

};

typedef boost::shared_ptr<ConcurrentDevice> ConcurrentDevicePtr;


#endif //SAMPLERGENERATOR_CONCURRENTDEVICE_H

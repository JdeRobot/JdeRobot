#include "RosKinectGrabber.h"


RosKinectGrabber::RosKinectGrabber (int argc, char **argv, std::string nodeName)
{
    initializeROS(argc,argv,nodeName);
    ImagePublisher1 = new image_transport::Publisher(ImageNode->advertise(nodeName + "_RGB", 400));
    ImagePublisher2 = new image_transport::Publisher(ImageNode->advertise(nodeName + "_Depth", 400));

    std::cout << nodeName + "_RGB" << std::endl;
    std::cout << nodeName + "_Depth" << std::endl;
}

RosKinectGrabber::~RosKinectGrabber ()
{
    delete ImagePublisher1;
    delete ImagePublisher2;
}

void RosKinectGrabber::cloud_cb_ (const boost::shared_ptr<openni_wrapper::Image> &image, const boost::shared_ptr<openni_wrapper::DepthImage> &depth_image, float focallength_inverse)
{
    //RosPCL::RGB_Depth_Image rgb_depth_message;

    int rows = image->getHeight();
    int cols = image->getWidth();

    cv::Mat frameRGB = cv::Mat(rows, cols, CV_8UC3);
    image->fillRGB(cols, rows, frameRGB.data, frameRGB.step);

    cv::Mat frameDepth = cv::Mat(depth_image->getHeight(), depth_image->getWidth(), CV_16U);
    depth_image->fillDepthImageRaw(frameDepth.cols, frameDepth.rows, (unsigned short *)frameDepth.data, frameDepth.step);

    cv::Mat frameDepth8bit = frameDepth.clone();
    cv::Mat frameDepthRgb(depth_image->getHeight(), depth_image->getWidth(), CV_8UC3);

    frameDepth8bit.convertTo(frameDepth8bit, CV_8UC1, 0.0625);

    cv::cvtColor(frameDepth8bit, frameDepthRgb, CV_BayerGR2RGB);



    //std::cout << rows << " " << cols << " " << depth_image->getHeight() << " " << depth_image->getWidth() << std::endl;

    sensor_msgs::ImagePtr rgb_message = cv_bridge::CvImage(std_msgs::Header(), sensor_msgs::image_encodings::RGB8, frameRGB).toImageMsg();
    sensor_msgs::ImagePtr depth_message = cv_bridge::CvImage(std_msgs::Header(), sensor_msgs::image_encodings::RGB8, frameDepthRgb).toImageMsg();

    ImagePublisher1->publish(rgb_message);
    ImagePublisher2->publish(depth_message);

}

void RosKinectGrabber::run ()
{
  pcl::Grabber* interface = new pcl::OpenNIGrabber();

  boost::function<void (const boost::shared_ptr<openni_wrapper::Image> &image, const boost::shared_ptr<openni_wrapper::DepthImage> &depth_image, float focallength_inverse)> f =
    boost::bind (&RosKinectGrabber::cloud_cb_, this, _1, _2, _3);



  interface->registerCallback (f);

  interface->start ();

  while (true)
  {
    boost::this_thread::sleep (boost::posix_time::seconds (1));
  }

  interface->stop ();
}

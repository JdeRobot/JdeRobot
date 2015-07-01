#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/background_segm.hpp>
#include <cv.h>
#include <QtGui>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <visionlib/cvBlob/cvblob.h>
#include <jderobot/ardroneextra.h>
#include <jderobot/pose3d.h>
#include <jderobot/cmdvel.h>
#include <jderobot/navdata.h>

using namespace cvb;

class Sensors;

class Myalgorithm {
	public:
		Myalgorithm();
		void processImage(cv::Mat& image);
		void run(QMutex& mutex_, QMutex& mutexDrone_, cv::Mat& image, jderobot::ArDroneExtraPrx& arextraprx_, jderobot::Pose3DPrx& poseprx_, jderobot::CMDVelPrx& cmdprx_, jderobot::NavdataPrx& navprx_);
		void morphologicalTransform(cv::Mat& image);
		void setBlobParams();
		std::string to_string(int n);

	private:
		cv::Mat fgMaskMOG;
		cv::Ptr<cv::BackgroundSubtractor> pMOG;
		cv::SimpleBlobDetector* simpleblobdetector;
		cv::SimpleBlobDetector::Params params;
		std::vector<cv::KeyPoint> keypoints;
		IplImage* bin;
		IplImage* frame;
		IplImage *labelImg;
		CvBlobs blobs;
		CvTracks tracks;
		int line_pos;
		int count;
		int countUD;
		int countDU;
		int* count_arr;
		CvPoint2D64f last_pos;
		CvPoint2D64f cur_pos;
		std::map<CvID, CvPoint2D64f> last_poses;
		jderobot::Pose3DDataPtr pose;
		jderobot::CMDVelDataPtr vel;
		bool initiated;
		bool dynamic;
		std::vector<cv::Point> landmarks;
		int nframes;
		int lmindex;
};



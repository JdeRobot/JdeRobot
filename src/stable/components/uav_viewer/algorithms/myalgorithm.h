#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/background_segm.hpp>
#include <cv.h>

#include <iostream>

#include <visionlib/cvBlob/cvblob.h>

using namespace cvb;

class Myalgorithm {
	public:
		Myalgorithm();
		void processImage(cv::Mat image);
		void morphologicalTransform(cv::Mat image);
		void setBlobParams();

	private:
		cv::Mat fgMaskMOG;
		cv::Ptr<cv::BackgroundSubtractor> pMOG;
		cv::SimpleBlobDetector* simpleblobdetector;
		cv::SimpleBlobDetector::Params params;
		std::vector<cv::KeyPoint> keypoints;
		IplImage* bin1;
		IplImage* frame;
		IplImage *labelImg;
		CvBlobs blobs;
		CvTracks tracks;	
};
	


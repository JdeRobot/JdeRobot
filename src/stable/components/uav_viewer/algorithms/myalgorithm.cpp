#include "myalgorithm.h"

Myalgorithm::Myalgorithm() {
	pMOG = new cv::BackgroundSubtractorMOG;
}

void Myalgorithm::processImage(cv::Mat image) {
	pMOG->operator()(image, fgMaskMOG);
	
	morphologicalTransform(fgMaskMOG);
	
	//setBlobParams();
	//simpleblobdetector = new cv::SimpleBlobDetector(params);
	//simpleblobdetector->detect(fgMaskMOG, keypoints);
	//cv::drawKeypoints( image, keypoints, image, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
	
	//cv::Mat image1;
	//cvtColor(image,image1,cv::COLOR_BGR2GRAY);
	
	
	/* Blob Detection*/
	bin1 = new IplImage(fgMaskMOG);
	frame = new IplImage(image);
	labelImg=cvCreateImage(cvSize(image.cols,image.rows),IPL_DEPTH_LABEL,1);
	
	unsigned int result=cvLabel(bin1,labelImg,blobs);
	cvRenderBlobs(labelImg,blobs,frame,frame);
	cvFilterByArea(blobs,1500,40000);
	cvUpdateTracks(blobs,tracks,200.,5);
	cvRenderTracks(tracks,frame,frame,CV_TRACK_RENDER_ID);//|CV_TRACK_RENDER_BOUNDING_BOX);

	int c = 0;
	for (CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it) {
		double moment10 = it->second->m10;
		double moment01 = it->second->m01;
		double area = it->second->area;
		//Variable for holding position
		int x1;
		int y1;
		//Calculating the current position
		x1 = moment10/area;
		y1 = moment01/area;
		//Printing the position information
		std::cout<<"X: "<<x1<<" Y: "<<y1<<std::endl;
		c++;
		std::cout<<"Area: "<<area<<"\n";
	}

	std::cout<<"Cars: "<<c<<"\n\n";
	cv::imshow("Blobs", image);
	cv::imshow("fgMaskMOG", fgMaskMOG);
}

void Myalgorithm::morphologicalTransform(cv::Mat image) {
	/*Morphological opening - erosion followed by dilation*/
	//cv::erode(image,image,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	//cv::dilate(image,image,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));

	/*Morphological closing - dilation followed by erosion*/
	cv::dilate(image,image,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(15,15)));
	//cv::erode(image,image,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(15,15)));
}

void Myalgorithm::setBlobParams() {

	params.minThreshold=0;
	params.maxThreshold=10;
	params.thresholdStep=10;

	params.filterByColor=true;
	params.blobColor=255;

	params.filterByArea = false;
	params.minArea = 500;
 
	params.filterByCircularity = false;

	params.filterByInertia = false;
}


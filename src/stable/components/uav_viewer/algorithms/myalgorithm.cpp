#include "myalgorithm.h"

#define MARGIN 120
#define FR_W 240
#define FR_H 240

Myalgorithm::Myalgorithm() {
	pMOG = new cv::BackgroundSubtractorMOG;
	count = 0;
	countUD = 0;
	countDU = 0;
	line_pos = FR_H - MARGIN;
}

void Myalgorithm::processImage(cv::Mat image) {
	if (image.empty())
		return;

	pMOG->operator()(image, fgMaskMOG);
	morphologicalTransform(fgMaskMOG);

	// Blob Detection
	bin = new IplImage(fgMaskMOG);
	frame = new IplImage(image);
	labelImg = cvCreateImage(cvSize(image.cols,image.rows),IPL_DEPTH_LABEL,1);

	unsigned int result = cvLabel(bin, labelImg, blobs);
	cvRenderBlobs(labelImg, blobs, frame, frame, CV_BLOB_RENDER_BOUNDING_BOX|CV_BLOB_RENDER_CENTROID|CV_BLOB_RENDER_ANGLE);
	cvFilterByArea(blobs, 1500, 40000);
	cvUpdateTracks(blobs, tracks, 200., 5);
	cvRenderTracks(tracks, frame, frame, CV_TRACK_RENDER_ID);

	for (std::map<CvID, CvTrack*>::iterator track_it = tracks.begin(); track_it!=tracks.end(); track_it++) {
		CvID id = (*track_it).first;
		CvTrack* track = (*track_it).second;
		cur_pos = track->centroid;

		if (track->inactive == 0) {
			if (last_poses.count(id)) {
				std::map<CvID, CvPoint2D64f>::iterator pose_it = last_poses.find(id);
				last_pos = pose_it -> second;
				last_poses.erase(pose_it);
			}
			last_poses.insert(std::pair<CvID, CvPoint2D64f>(id, cur_pos));

			if (cur_pos.y>line_pos && last_pos.y<line_pos) {
				count++;
				countUD++;
			}
			if (cur_pos.y<line_pos && last_pos.y>line_pos) {
				count++;
				countDU++;
			}
		} else {
			if (last_poses.count(id)) {
				last_poses.erase(last_poses.find(id));
			}
		}
	}

	//display
	cv::line(image, cv::Point(0, line_pos), cv::Point(FR_W, line_pos), cv::Scalar(0,255,0),2);
	cv::putText(image, "COUNT: "+to_string(count), cv::Point(10, 15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
	cv::putText(image, "UP->DOWN: "+to_string(countUD), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
	cv::putText(image, "DOWN->UP: "+to_string(countDU), cv::Point(10, 45), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
	cv::imshow("Blobs", image);
	//cv::imshow("fgMaskMOG", fgMaskMOG);
}

void Myalgorithm::morphologicalTransform(cv::Mat image) {
	//cv::erode(image,image,cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5,5)));
	//cv::dilate(image,image,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5,5)));
	cv::dilate(image,image,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(15,15)));
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


std::string Myalgorithm::to_string(int number){
    std::string number_string = "";
    char ones_char;
    int ones = 0;
    while(true){
        ones = number % 10;
        switch(ones){
            case 0: ones_char = '0'; break;
            case 1: ones_char = '1'; break;
            case 2: ones_char = '2'; break;
            case 3: ones_char = '3'; break;
            case 4: ones_char = '4'; break;
            case 5: ones_char = '5'; break;
            case 6: ones_char = '6'; break;
            case 7: ones_char = '7'; break;
            case 8: ones_char = '8'; break;
            case 9: ones_char = '9'; break;
            default : std::cout << "Trouble converting number to string.";
        }
        number -= ones;
        number_string = ones_char + number_string;
        if(number == 0){
            break;
        }
        number = number/10;
    }
    return number_string;
}

/*
 *  Copyright (C) 1997-2015 JDERobot Developers Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Author: Satyaki Chakraborty (satyaki [dot] cs15 [at] gmail [dot] com)
 *
 *  Note: This source code has been developed in GSoC'2015
 *
 */

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>

#include <jderobot/camera.h>
#include <jderobot/pose3d.h>
#include <jderobot/cmdvel.h>
#include <jderobot/ardroneextra.h>

#include <visionlib/cvBlob/cvblob.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/background_segm.hpp>
#include <cv.h>

#include <iostream>
#include <vector>
#include <stdlib.h>

#define DRONE_HEIGHT 5
#define DRONE_VEL 0.5
#define EPS 0.5
#define N_FRAMES 500
#define FR_H 240
#define FR_W 240
#define MARGIN 120

using namespace cvb;

bool initiated, dynamic;
int lmindex, countUD, countDU, count, count_active, line_pos, nframes;
int* count_arr;
float* avg_vel_arr;
float yaw;
double avg_vel;

std::vector<cv::Point> landmarks;
cv::Mat image, fgMaskMOG, heat_map, heat_mapfg;
cv::Ptr<cv::BackgroundSubtractor> pMOG;
IplImage* bin;
IplImage* frame;
IplImage* labelImg;
CvBlobs blobs;
CvTracks tracks;
CvPoint2D64f last_pos;
CvPoint2D64f cur_pos;
std::map<CvID, CvPoint2D64f> last_poses;

void processImage(cv::Mat& image);

int main (int argc, char** argv) {

	initiated = false;
	dynamic = false;
	lmindex = 0;
	countUD = 0;
	countDU = 0;
	count = 0;
	nframes = 0;
	avg_vel = 0;
	line_pos = FR_H - MARGIN;

#ifdef _OPENCV3
	pMOG = cv::createBackgroundSubtractorMOG2();
#else
	pMOG = new cv::BackgroundSubtractorMOG;
#endif
	heat_map = cv::Mat::zeros(FR_W, FR_H, CV_8UC3);
	heat_mapfg = cv::Mat::zeros(FR_W, FR_H, CV_8UC3);
	cv::rectangle(heat_map, cv::Point(FR_W/2-40, 0), cv::Point(FR_W/2+40, FR_H), cv::Scalar(50, 50, 50), -1);
	cv::line(heat_map, cv::Point(FR_W/2, 0), cv::Point(FR_W/2, FR_H), cv::Scalar(100, 100, 100), 1);

	Ice::CommunicatorPtr ic;
	jderobot::ArDroneExtraPrx arextraprx;
	jderobot::Pose3DPrx poseprx;
	jderobot::CMDVelPrx cmdprx;
	jderobot::CameraPrx camprx;
	jderobot::Pose3DDataPtr pose;
	jderobot::ImageDataPtr img;
	jderobot::CMDVelDataPtr vel = new jderobot::CMDVelData();

	//prespecify checkpoints
	landmarks.push_back(cv::Point(40.0, 0.0));
	landmarks.push_back(cv::Point(10.0, 0.0));
	landmarks.push_back(cv::Point(-20.0, 0.0));

	count_arr = new int[landmarks.size()]();
	avg_vel_arr = new float[landmarks.size()]();
	cv::namedWindow("BLOBS", cv::WINDOW_AUTOSIZE);
	count_active = 0;
	avg_vel = 0;

	try {
		ic = Ice::initialize(argc, argv);
		Ice::PropertiesPtr prop = ic->getProperties();

		Ice::ObjectPrx baseextra = ic->propertyToProxy("BSCounter.Extra.Proxy");
		if (0==baseextra)
			throw "Could not create proxy";
		arextraprx = jderobot::ArDroneExtraPrx::checkedCast(baseextra);
		if (0==arextraprx)
			throw "ArDroneExtra -> Invalid proxy";

		Ice::ObjectPrx basepose = ic->propertyToProxy("BSCounter.Pose3D.Proxy");
		if (0==basepose)
			throw "Could not create proxy";
		poseprx = jderobot::Pose3DPrx::checkedCast(basepose);
		if (0==poseprx)
			throw "Pose3D -> Invalid proxy";

		Ice::ObjectPrx basecmd = ic->propertyToProxy("BSCounter.CMDVel.Proxy");
		if (0==basecmd)
			throw "Could not create proxy";
		cmdprx = jderobot::CMDVelPrx::checkedCast(basecmd);
		if (0==cmdprx)
			throw "CMDVel -> Invalid proxy";

		Ice::ObjectPrx basecam = ic->propertyToProxy("BSCounter.Camera.Proxy");
		if (0==basecam)
			throw "Could not create proxy";
		camprx = jderobot::CameraPrx::checkedCast(basecam);
		if (0==camprx)
			throw "Camera -> Invalid proxy";
		img = camprx->getImageData("RGB8");

	} catch (const Ice::Exception& ex) {
		std::cerr << ex << std::endl;
		exit(-1);
	} catch (const char* msg) {
		std::cerr << msg << std::endl;
		exit(-1);
	}

	std::cout << "taking off..\n";
	arextraprx->takeoff();

	std::cout << "reaching desired height..\n";
	vel->linearZ = DRONE_VEL; vel->linearX = 0; vel->linearY = 0;
	cmdprx->setCMDVelData(vel);

	while (1) {
		pose = poseprx->getPose3DData();
		img = camprx->getImageData("RGB8");
                image.create(img->description->height, img->description->width, CV_8UC3);
                memcpy((unsigned char*) image.data, &(img->pixelData[0]), image.cols*image.rows*3);
		cv::imshow("BLOBS", image);
		cv::imshow("HEATMAP", heat_map + heat_mapfg);
		cv::waitKey(33);

		if (!initiated && abs(pose->z - DRONE_HEIGHT) < EPS) {
			vel->linearZ = 0;
			cmdprx->setCMDVelData(vel);
			initiated = true;
			std::cout<< "reached height: "<<pose->z<<" m.\n";
			std::cout<< "[STATUS] initialized: moving to 1st checkpoint..\n";
		}

		if (initiated && !dynamic) {
			// set velocity for next checkpoint
			vel->linearX = (landmarks[lmindex].x-pose->x)/sqrt((landmarks[lmindex].x-pose->x)*(landmarks[lmindex].x-pose->x)+(landmarks[lmindex].y-pose->y)*(landmarks[lmindex].y-pose->y)+(DRONE_HEIGHT-pose->z)*(DRONE_HEIGHT-pose->z));
			vel->linearY = (landmarks[lmindex].y-pose->y)/sqrt((landmarks[lmindex].x-pose->x)*(landmarks[lmindex].x-pose->x)+(landmarks[lmindex].y-pose->y)*(landmarks[lmindex].y-pose->y)+(DRONE_HEIGHT-pose->z)*(DRONE_HEIGHT-pose->z));
			vel->linearZ = (DRONE_HEIGHT-pose->z)/sqrt((landmarks[lmindex].x-pose->x)*(landmarks[lmindex].x-pose->x)+(landmarks[lmindex].y-pose->y)*(landmarks[lmindex].y-pose->y)+(DRONE_HEIGHT-pose->z)*(DRONE_HEIGHT-pose->z));
			float yaw = (float) atan2(2.0*(pose->q0*pose->q3 + pose->q1*pose->q2), 1 - 2.0*(pose->q2*pose->q2 + pose->q3*pose->q3));
			float tempX = cos(yaw)*(vel->linearX) + sin(yaw)*(vel->linearY);
			float tempY = -sin(yaw)*(vel->linearX) + cos(yaw)*(vel->linearY);
			vel->linearX = tempX;
			vel->linearY = tempY;
			cmdprx->setCMDVelData(vel);

			dynamic = true;

			// Update heatmap
			std::cout << "[HEATMAP] Current Heatmap: \n";
			for (int i=0; i<landmarks.size(); i++)
				std::cout << "Cars counted at checkpoint ["<< i+1 <<"]: "<<count_arr[i]<<"\n";
			heat_mapfg = cv::Mat::zeros(FR_H, FR_W, CV_8UC3);

			for (int i=0; i<landmarks.size(); i++) {
				cv::circle(heat_mapfg, cv::Point((landmarks[i].y + 50)*2.4, (landmarks[i].x + 50)*2.4), count_arr[i]*3, cv::Scalar(0, 16*avg_vel_arr[i], 255 - 16*avg_vel_arr[i]), -1);
			}
			cv::GaussianBlur(heat_mapfg, heat_mapfg, cv::Size(15, 15), 3);
			std::cout << "Average speed of cars: "<<avg_vel<<"\n";
			std::cout<<"\n";

			avg_vel = 0;
			count_active = 0;
		}

		if (dynamic && abs(landmarks[lmindex].x-pose->x)<EPS && abs(landmarks[lmindex].y-pose->y)<EPS) {
			std::cout << "Reached checkpoint ["<<lmindex+1<<"]: X="<<pose->x<<"m, Y="<<pose->y<<"m, Z="<<pose->z<<"m\n";
			std::cout << "[STATUS] Processing started..\n";

			vel->linearX = 0; vel->linearY = 0; vel->linearZ = 0;
			cmdprx->setCMDVelData(vel);

			// process Image for N Frames
			while (nframes<N_FRAMES) {
				nframes++;
				img = camprx->getImageData("RGB8");
				image.create(img->description->height, img->description->width, CV_8UC3);
				memcpy((unsigned char*) image.data, &(img->pixelData[0]), image.cols*image.rows*3);
				processImage(image);
			}

			count_arr[lmindex]=count;
			if (count_active) avg_vel/=count_active;
			avg_vel_arr[lmindex] = avg_vel;
			++lmindex%=landmarks.size();
			nframes = 0;
			count = 0;
			countDU = 0;
			countUD = 0;
			std::cout << "[STATUS] Processed "<<N_FRAMES<<" frames. Moving to next checkpoint.\n";
			dynamic = false;
		}
	}

	return 0;
}

std::string to_string(int number){
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
            default : std::cout << "Trouble converting number to string. number: "<<ones;
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

void processImage(cv::Mat& image) {
	if (image.empty())
		return;

#ifdef _OPENCV3
	pMOG->apply(image, fgMaskMOG, 0.05);
#else
	pMOG->operator()(image, fgMaskMOG, 0.05);
#endif
	cv::dilate(fgMaskMOG,fgMaskMOG,cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(15,15)));

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
			if (line_pos+25>cur_pos.y && cur_pos.y>line_pos && line_pos-25<last_pos.y && last_pos.y<line_pos) {
				count++;
				countUD++;
			}
			if (line_pos-25<cur_pos.y && cur_pos.y<line_pos && line_pos+25>last_pos.y && last_pos.y>line_pos) {
				count++;
				countDU++;
			}

			if ( cur_pos.y<line_pos+50 && cur_pos.y>line_pos-50) {
				avg_vel += abs(cur_pos.y-last_pos.y);
				count_active++;
			}

			//update heatmapfg
			heat_mapfg = cv::Mat::zeros(FR_H, FR_W, CV_8UC3);
			count_arr[lmindex] = count;
			avg_vel_arr[lmindex] = avg_vel/count_active ;
			for (int i=0; i<landmarks.size(); i++) {
				cv::circle(heat_mapfg, cv::Point((landmarks[i].y + 50)*2.4, (landmarks[i].x + 50)*2.4), count_arr[i]*3, cv::Scalar(0, 16*avg_vel_arr[i], 255 - 16*avg_vel_arr[i]), -1);
			}
			cv::GaussianBlur(heat_mapfg, heat_mapfg, cv::Size(15, 15), 5);
		} else {
			if (last_poses.count(id)) {
				last_poses.erase(last_poses.find(id));
			}
		}
	}

	cv::line(image, cv::Point(0, line_pos), cv::Point(FR_W, line_pos), cv::Scalar(0,255,0),2);
	cv::putText(image, "COUNT: "+to_string(count), cv::Point(10, 15), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
	cv::putText(image, "UP->DOWN: "+to_string(countUD), cv::Point(10, 30), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
	cv::putText(image, "DOWN->UP: "+to_string(countDU), cv::Point(10, 45), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255,255,255));
	cv::imshow("BLOBS", image);
	cv::imshow("HEATMAP", heat_map + heat_mapfg);
	cv::waitKey(33);
}

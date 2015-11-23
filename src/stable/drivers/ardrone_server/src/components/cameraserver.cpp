/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *	      Sara Marugán Alonso <smarugan@gsyc.es>
 *  Modified by: Alberto Martín <almartinflorido@gmail.com>
 *
 */


#include "../teleop_twist.h"
#include "../video.h"

#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>


#include <jderobot/camera.h>
#include <jderobot/image.h>
#include <visionlib/colorspaces/colorspacesmm.h>


//Opencv
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>



#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <stdlib.h>
#include <list>


namespace cameraserver{

class CameraI: virtual public jderobot::Camera {


    public:
    std::string name;
    std::string uri;
    int framerateN;
    int framerateD;

        CameraI(std::string propertyPrefix, Ice::CommunicatorPtr ic)
               : prefix(propertyPrefix) {

		std::cout << "cameraserver start" << std::endl;

		imageDescription = (new jderobot::ImageDescription());
		cameraDescription = (new jderobot::CameraDescription());

		Ice::PropertiesPtr prop = ic->getProperties();

		//fill cameraDescription
		name = prop->getProperty(prefix+"Name");
		if (name.size() == 0)
			throw "Camera name not configured";

		cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");
		cameraDescription->streamingUri = prop->getProperty(prefix+"StreamingUri");

		std::string dronecam;
		//fill imageDescription
		if (IS_ARDRONE1){
			dronecam=prefix+"ArDrone1.";
		}else{
			dronecam=prefix+"ArDrone2.";		
		}
		imageDescription->width = prop->getPropertyAsIntWithDefault(dronecam+"ImageWidth",340);
		imageDescription->height = prop->getPropertyAsIntWithDefault(dronecam+"ImageHeight",280);
		//we use formats acording to colorspaces
		std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
		imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
		if (!imageFmt)
			throw  "Format " + fmtStr + " unknown";

		imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
		imageDescription->format = imageFmt->name;

		//fill pipeline cfg
		uri = prop->getProperty(prefix+"Uri");
		framerateN = prop->getPropertyAsIntWithDefault(prefix+"FramerateN",25);
		framerateD = prop->getPropertyAsIntWithDefault(prefix+"FramerateD",1);
		
		// mirror image
		mirror = prop->getPropertyAsIntWithDefault(prefix+"Mirror",0);
		replyTask = new ReplyTask(this);
		replyTask->start(); // my own thread

        }

        std::string getName () {
            return (cameraDescription->name);
        }

        std::string getRobotName () {
//            return ((context.properties())->getProperty(context.tag()+".RobotName"));
        }

        virtual ~CameraI() {
//            context.tracer().info("Stopping and joining thread for camera: " + cameraDescription->name);
//            gbxiceutilacfr::stopAndJoin(replyTask);
        }

        virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
            return imageDescription;
        }

        virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
            return cameraDescription;
        }

        virtual Ice::Int setCameraDescription(const jderobot::CameraDescriptionPtr &description, const Ice::Current& c) {
            return 0;
        }

	virtual jderobot::ImageFormat getImageFormat(const Ice::Current& c)
	{
		jderobot::ImageFormat formats;

		formats.push_back(colorspaces::ImageRGB8::FORMAT_RGB8.get()->name);

		return formats;
	}
	virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb, const std::string& format, const Ice::Current& c){
            replyTask->pushJob(cb);
        }

        virtual std::string startCameraStreaming(const Ice::Current&){
        }

        virtual void stopCameraStreaming(const Ice::Current&) {
        }

    	virtual void reset(const Ice::Current&)
    	{
    	}

    private:

        class ReplyTask: public IceUtil::Thread {
            public:
                ReplyTask(CameraI* camera)
                {
                    std::cout << "cameraserver -> replytask start" << std::endl;
                    mycamera = camera;
                }

                void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
                    IceUtil::Mutex::Lock sync(requestsMutex);
                    requests.push_back(cb);
                }

                virtual void run(){
                    jderobot::ImageDataPtr reply(new jderobot::ImageData);
                    struct timeval a, b;
                    int cycle = 48;
                    long totalb,totala;
                    long diff;

                    int count = 0 ;
                    cv::Mat frame;
		cv::Mat image;
                    int cycle_control = 1000/mycamera->framerateN;

                    while(1){
                        gettimeofday(&a,NULL);
                        totala=a.tv_sec*1000000+a.tv_usec;

			
			if (IS_ARDRONE1){
				vp_os_mutex_lock(&video_lock);
				image=cv::Mat(D1_STREAM_HEIGHT,D1_STREAM_WIDTH,CV_8UC3,(void*)buffer);
				vp_os_mutex_unlock(&video_lock);			
			    	if (cam_state == ZAP_CHANNEL_HORI)
				{
					frame=image(cv::Range(0,0),cv::Range(D1_STREAM_HEIGHT,D1_STREAM_WIDTH));
					image.copyTo(frame);
					frame.rows=D1_STREAM_HEIGHT;
					frame.cols=D1_STREAM_WIDTH;
					mycamera->imageDescription->width=D1_STREAM_WIDTH;
					mycamera->imageDescription->height=D1_STREAM_HEIGHT;				    
				}
				else if (cam_state == ZAP_CHANNEL_VERT)
				{
					frame=image(cv::Range(0,0),cv::Range(D1_VERTSTREAM_HEIGHT,D1_VERTSTREAM_WIDTH));
					image.copyTo(frame);
					frame.rows=D1_VERTSTREAM_HEIGHT;
					frame.cols=D1_VERTSTREAM_WIDTH;
					mycamera->imageDescription->width=D1_VERTSTREAM_WIDTH+1;
					mycamera->imageDescription->height=D1_VERTSTREAM_HEIGHT+1;				    
				}
			}
			if (IS_ARDRONE2){
				vp_os_mutex_lock(&video_lock);
				image=cv::Mat(D2_STREAM_HEIGHT,D2_STREAM_WIDTH,CV_8UC3,(void*)buffer);	
				vp_os_mutex_unlock(&video_lock);			
				frame=image(cv::Range(0,0),cv::Range(D2_STREAM_HEIGHT,D2_STREAM_WIDTH));
				image.copyTo(frame);
				frame.rows=D2_STREAM_HEIGHT;
				frame.cols=D2_STREAM_WIDTH;
				mycamera->imageDescription->width=D2_STREAM_WIDTH;
				mycamera->imageDescription->height=D2_STREAM_HEIGHT;			
			}
          		

                        if(mycamera->imageDescription->width!=frame.rows &&
                           mycamera->imageDescription->height!=frame.cols)
                            cv::resize(frame, frame,
                                       cv::Size(mycamera->imageDescription->width,
                                                mycamera->imageDescription->height));

                        if(count==0){
                            reply->description = mycamera->imageDescription;
                            count++;
                        }

			
			if (mycamera->mirror)
			{
				cv::Mat dst;
				cv::flip(frame, frame, 1);
			}
			

                        IceUtil::Time t = IceUtil::Time::now();
                        reply->timeStamp.seconds = (long)t.toSeconds();
                        reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;

                        reply->pixelData.resize(frame.rows*frame.cols*3);

                        memcpy( &(reply->pixelData[0]), (unsigned char *) frame.data, frame.rows*frame.cols*3);


                       { //critical region start
                           IceUtil::Mutex::Lock sync(requestsMutex);
                           while(!requests.empty()) {
                               jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
                               requests.pop_front();
                               cb->ice_response(reply);
                           }
                       } //critical region end

                        gettimeofday(&b,NULL);
                        totalb=b.tv_sec*1000000+b.tv_usec;

                        diff = (totalb-totala)/1000;
                        diff = cycle-diff;
			//std::cout << "CameraServer takes " << diff << " ms ";
                        
                        if (diff < 0 || diff > cycle_control)
                            diff = 0.;
                        else
                            diff = cycle_control - diff;

			//std::cout <<  " and sleep " << diff << " ms " << std::endl;
                        /*Sleep Algorithm*/
                        usleep(diff * 1000);

                    }
                }

                CameraI* mycamera;
                IceUtil::Mutex requestsMutex;
                std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
        };

        typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;
        std::string prefix;
        colorspaces::Image::FormatPtr imageFmt;
        jderobot::ImageDescriptionPtr imageDescription;
        jderobot::CameraDescriptionPtr cameraDescription;
        ReplyTaskPtr replyTask;
	int mirror;

}; // end class CameraI

} //namespace

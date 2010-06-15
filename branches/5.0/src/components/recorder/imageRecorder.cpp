/*
 *
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Author : Sara Marugán Alonso <smarugan@gsyc.es>
 *
 */

#include "imageRecorder.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <strings.h>
#include <jderobot/image.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <Ice/Ice.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <IceUtil/IceUtil.h>
#include <IceUtil/Handle.h>
#include <IceStorm/IceStorm.h>
#include <iostream>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include <ctime>


//////////////////////////////////////////////////////// IMAGE CONSUMER INTERFACE IMPLEMENTATION
class ImageConsumerI: virtual public jderobot::ImageConsumer {
	private:
            jderobot::Time timeStampOld;
            jderobot::Time timeStampNew;
	    int size;
  	    int fifo_fd;
    	    char *imageData;
	public:
	    ImageConsumerI(int width,int height){
		size=width*height*3;
	        imageData = (char*) malloc(size);
		timeStampOld.seconds=timeStampNew.seconds=0;
		timeStampOld.useconds=timeStampNew.useconds=0;
		
		// open fifo
		if ((fifo_fd=open("fifovid", O_WRONLY))<0){
			fprintf (stderr, "imageRecorder error: could not open fifo\n");
		}
	    }

	    ~ImageConsumerI(){
		// close fifo
		close (fifo_fd);
		unlink("fifovid");

		free(imageData); 
	    }


	    virtual void report(const ::jderobot::ImageDataPtr& data,
		                const Ice::Current&) {

		colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(data->description->format);
		if (!fmt)
		  throw "Format not supported";

		colorspaces::Image img(data->description->width,
				       data->description->height,
				       fmt,
				       &(data->pixelData[0]));

		timeStampOld=timeStampNew;
		timeStampNew=data->timeStamp;

		if(timeStampOld!=timeStampNew){
			int s=size/3;
			if (write (fifo_fd, img.data, size)>size){
				fprintf (stderr, "imageRecorder error: could not write on fifo\n");
			}
		}
	    }
};


//////////////////////////////////////////////////////////// THREAD

  void* callback(void* obj);

  class Thread { 
  private:
    Ice::CommunicatorPtr communicator;
    jderobot::RecorderConfigPtr configuration;

  public:
    int main()
    {
	int imgwidth= atoi((char*)configuration->width.c_str());
	int imgheight= atoi((char*)configuration->height.c_str());
	int duration= atoi((char*)configuration->duration.c_str());
	IceStorm::TopicPrx topic;

        //IMAGE CONSUMERS INIT

	// generate camera adapter endpoint randomly
    	srand((unsigned)time(0));
    	char adapter_endpoint [256];

    	int adapter_port = 10010 + (rand()%500)+1;
    	sprintf(adapter_endpoint,"default -t 5000 -p %d",adapter_port); 

	Ice::ObjectAdapterPtr adapter= communicator->createObjectAdapterWithEndpoints(adapter_endpoint,adapter_endpoint);

	if (adapter==0){
		fprintf(stderr,"imageRecorder error: could not create adapter for camera");
		return -1;
	}

        Ice::ObjectPrx obj = communicator->stringToProxy(configuration->device);

        if (obj==0){
		fprintf(stderr,"imageRecorder error: could not create proxy\n");
		return(-1);
        }

	std::string topicName = configuration->name;

	IceStorm::TopicManagerPrx topicManager=IceStorm::TopicManagerPrx::checkedCast(obj);

	ImageConsumerI *imageConsumer = new ImageConsumerI(imgwidth,imgheight);
	Ice::ObjectPrx proxy = adapter->addWithUUID(imageConsumer)->ice_oneway();

	try {
		topic = topicManager->retrieve(topicName);
		IceStorm::QoS qos;
		topic->subscribeAndGetPublisher(qos, proxy);
	}
	catch (const IceStorm::NoSuchTopic& ex) {
		std::cerr << ex << std::endl;
		return -1;
	}

	adapter->activate();

        //communicator->waitForShutdown();
	sleep(duration);

        topic->unsubscribe(proxy);

	adapter->deactivate();

	delete imageConsumer;
	adapter=NULL;

	pthread_exit(0);
    }
	  
    void run(Ice::CommunicatorPtr comm,jderobot::RecorderConfigPtr config)
    {
      communicator=comm;
      configuration=config;
      pthread_create(&thread, 0, &callback, this);
      sleep(1);
    }

    void stop(){
    }

    int join()
    {
      return pthread_join(thread, ret);
    }
    
    pthread_t thread;
    void** ret;
  }; // class Thread


  void* callback(void* obj)
  {
    static_cast<Thread*>(obj)->main();
    return(0);
  } // callback


///////////////////////////////////////// IMAGE_RECORDER FUCNTIONS

imageRecorder::imageRecorder(const jderobotice::Context& context,
		const jderobot::RecorderConfigPtr& recConfig,int recordingProvider) : GenericRecorder(context,recConfig,recordingProvider)
{
	// create fifo
	unlink ("fifovid");
	if ( (mkfifo ("fifovid", 0600) != 0) ){
		 fprintf (stderr, "imageRecorder error: could not create fifo\n");
	}

	// create thread for getting images
	Thread thread;
	thread.run(getContext().communicator(),recConfig);
}

int imageRecorder::doRecording()
{

	int imgwidth= atoi((char*)getConfig()->width.c_str());
	int imgheight= atoi((char*)getConfig()->height.c_str());


	if (getProvider() == RECORDING_PROVIDER_MENCODER)
	{
		getContext().tracer().info ( "starting recording: Path = " +
										 getConfig()->path + " - FrameRate = " +
										 getConfig()->frameRate + " fps - ");

		char str[50];
		int file;
		file = open("/dev/null",O_RDWR);
		close(0); dup(file);
		close(1); dup(file);
		close(2); dup(file);

		// execute mencoder
		sprintf(str,"fps=%.1f:w=%d:h=%d:format=%s",atof((char*)getConfig()->frameRate.c_str()),imgwidth,imgheight, "rgb24");
		execlp("mencoder","mencoder","fifovid","-demuxer","rawvideo", "-rawvideo",
			str, "-o", (char*) (getConfig()->path).c_str(), "-ovc", "lavc" ,NULL);

		printf("imageRecorder error: cannot execute mencoder\n");
		return -1;
	}
	else
	{
		getContext().tracer().error ("Recording Provider Unknow! - ");
		return -1;
	}
}




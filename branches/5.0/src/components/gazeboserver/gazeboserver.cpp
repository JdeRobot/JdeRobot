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
 *  Authors : Javier Vazquez Pereda <javiervazper@yahoo.es>
 *
 *  DATE					COMMENT
 *  21/03/2010				Initial version. It's an adaptation of cameraserver component
 *  						It only supports 1 camera from gazebo interface.
 *  26/04/2010				Extension with laser interface
 *  15/5/2010				Extension with motors, sonaras, ptmotors, ptencoders, encoders.
 *
 */


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include <gbxsickacfr/gbxiceutilacfr/safethread.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include <jderobotice/component.h>
#include <jderobotice/application.h>
#include <tr1/memory>
#include <list>
#include <sstream>
#include <jderobotice/exceptions.h>
#include <jderobot/motors.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>
#include <jderobot/ptencoders.h>
#include <jderobot/ptmotors.h>
#include <jderobot/sonars.h>
#include <math.h>
#include <gazebo.h>

#define NUM_LASER 180
#define NUM_SONARS 16
#define DEGTORAD 0.01745327
#define RADTODEG 57.29582790

/** gazebo pantilt max pan, degrees.*/
float MAX_PAN = 54;
/** gazebo pantilt min pan, degrees.*/
float MIN_PAN = -54;
/** gazebo pantilt max tilt, degrees.*/
float MAX_TILT = 44;
/** gazebo pantilt min tilt, degrees.*/
float MIN_TILT = -44;


namespace gazeboserver{
  class CameraI: virtual public jderobot::Camera {
  public:
    CameraI(std::string& propertyPrefix, const jderobotice::Context& context)
      : prefix(propertyPrefix),context(context),
	imageFmt(),
	imageDescription(new jderobot::ImageDescription()),
	cameraDescription(new jderobot::CameraDescription()),
	replyTask()
    {
      
      
      Ice::PropertiesPtr prop = context.properties();

      //fill cameraDescription
      cameraDescription->name = prop->getProperty(prefix+"Name");
      if (cameraDescription->name.size() == 0)
	throw jderobotice::ConfigFileException(ERROR_INFO,"Camera name not configured");
      cameraDescription->shortDescription = prop->getProperty(prefix+"ShortDescription");

      //fill imageDescription
      imageDescription->width = prop->getPropertyAsIntWithDefault(prefix+"ImageWidth",340);
      imageDescription->height = prop->getPropertyAsIntWithDefault(prefix+"ImageHeight",280);
      //we use formats acording to colorspaces
      std::string fmtStr = prop->getPropertyWithDefault(prefix+"Format","YUY2");//default format YUY2
      imageFmt = colorspaces::Image::Format::searchFormat(fmtStr);
      if (!imageFmt)
	throw jderobotice::ConfigFileException(ERROR_INFO, "Format " + fmtStr + " unknown");
      imageDescription->size = imageDescription->width * imageDescription->height * CV_ELEM_SIZE(imageFmt->cvType);
      imageDescription->format = imageFmt->name;

      context.tracer().info("Starting thread for camera: " + cameraDescription->name);
      replyTask = new ReplyTask(this);

      replyTask->start();//my own thread
    }

    virtual ~CameraI(){
      context.tracer().info("Stopping and joining thread for camera: " + cameraDescription->name);
      gbxiceutilacfr::stopAndJoin(replyTask);
    }
    
    virtual jderobot::ImageDescriptionPtr getImageDescription(const Ice::Current& c){
      return imageDescription;
    }

    virtual jderobot::CameraDescriptionPtr getCameraDescription(const Ice::Current& c){
      return cameraDescription;
    }

    virtual void getImageData_async(const jderobot::AMD_ImageProvider_getImageDataPtr& cb,
			       const Ice::Current& c){
      replyTask->pushJob(cb);
    }

    virtual std::string startCameraStreaming(const Ice::Current&){
      context.tracer().info("Should be made anything to start camera streaming: " + cameraDescription->name);
	}

	virtual void stopCameraStreaming(const Ice::Current&) {
		context.tracer().info("Should be made anything to stop camera streaming: " + cameraDescription->name);
	}

  private:
    class ReplyTask: public gbxiceutilacfr::SafeThread{
    public:
      ReplyTask(CameraI* camera)
	: gbxiceutilacfr::SafeThread(camera->context.tracer()), mycamera(camera) {
    	        gazeboserver_id=0;
    	        gazeboclient_id=0;
    	        // Create a client object
    	        gazeboclient = gz_client_alloc();

    	        // Connect to the server
    	        if (gz_client_connect_wait(gazeboclient, gazeboserver_id, gazeboclient_id) != 0) {
    	      	  printf("ERROR: Connecting to Gazebo server.\n");
    	        }

    	        gazebocamera = gz_camera_alloc();

    	        printf("Connecting to camera device on server...\n");
    	        if (gz_camera_open(gazebocamera, gazeboclient, "camera1") != 0) {
    	      	  printf("ERROR: Connecting to camera device on server.\n");
    	        }

    	        gazeboCamData = gazebocamera->data;
    	        gazeboCamImage = gazeboCamData->image;
    	        printf("Image width= %d, height= %d\n", gazeboCamData->width, gazeboCamData->height);

      }

      void pushJob(const jderobot::AMD_ImageProvider_getImageDataPtr& cb){
	IceUtil::Mutex::Lock sync(requestsMutex);
	requests.push_back(cb);
      }

      virtual void walk(){
	jderobot::ImageDataPtr reply(new jderobot::ImageData);
	reply->description = mycamera->imageDescription;
	
	while(!isStopping()){
	    IceUtil::Time t = IceUtil::Time::now();
	    reply->timeStamp.seconds = (long)t.toSeconds();
	    reply->timeStamp.useconds = (long)t.toMicroSeconds() - reply->timeStamp.seconds*1000000;
	    reply->pixelData.resize(gazeboCamData->width*gazeboCamData->height*3);

	    memmove( &(reply->pixelData[0]), gazeboCamImage, mycamera->imageDescription->size);
	    
	    {//critical region start
	      IceUtil::Mutex::Lock sync(requestsMutex);
	      while(!requests.empty()){
		jderobot::AMD_ImageProvider_getImageDataPtr cb = requests.front();
		requests.pop_front();
		cb->ice_response(reply);
	      }
	    }//critical region end
	}
      }
      CameraI* mycamera;
      IceUtil::Mutex requestsMutex;
      std::list<jderobot::AMD_ImageProvider_getImageDataPtr> requests;
      gz_client_t *gazeboclient;
      int gazeboserver_id;
      int gazeboclient_id;
      gz_camera_t *gazebocamera;
      gz_camera_data_t *gazeboCamData;
      unsigned char *gazeboCamImage;
    };
    typedef IceUtil::Handle<ReplyTask> ReplyTaskPtr;


    std::string prefix;
    jderobotice::Context context;
    colorspaces::Image::FormatPtr imageFmt;
    jderobot::ImageDescriptionPtr imageDescription;
    jderobot::CameraDescriptionPtr cameraDescription;
    ReplyTaskPtr replyTask;


  };


  class MotorsI: virtual public jderobot::Motors {
  	public:
  		MotorsI(std::string& propertyPrefix, const jderobotice::Context& context)
       : prefix(propertyPrefix),context(context)
     {


       Ice::PropertiesPtr prop = context.properties();

			   gazeboserver_id=0;
              	        gazeboclient_id=0;

              	        // Create a client object
              	        gazeboclient = gz_client_alloc();

              	        // Connect to the server
              	        if (gz_client_connect_wait(gazeboclient, gazeboserver_id, gazeboclient_id) != 0) {
              	      	  printf("ERROR: Connecting to Gazebo server.\n");
              	      	}

              	     gazeboPosition = gz_position_alloc ();
              	     if (gz_position_open (gazeboPosition, gazeboclient, "robot1") != 0) {
              	    	 gazeboPosition=NULL;
              	    	 printf("Error while opening position\n");
              	     }

     }

     virtual ~MotorsI(){};

     virtual float getV(const Ice::Current&){
  	   //waiting for next gazebo camera update
  	      		if ( gz_client_wait(gazeboclient) != 0) {
  	      		   		printf("Error waiting for Gazebo server\n");
  	      		}

  	      		if(!gazeboPosition){
  	      			printf("Gazebo Position model not opened\n");
  	      		}
  	      		gazeboPositionData = gazeboPosition->data;


  	      		double v_double;
  	      		gz_position_lock(gazeboPosition,1);
  	      		  v_double = gazeboPositionData->vel_pos[0];
  	      		gz_position_unlock(gazeboPosition);
			return (float)v_double;
     };

     virtual float getW(const Ice::Current&){
  	   //waiting for next gazebo camera update
  	      		if ( gz_client_wait(gazeboclient) != 0) {
  	      		   		printf("Error waiting for Gazebo server\n");
  	      		}

  	      		if(!gazeboPosition){
  	      			printf("Gazebo Position model not opened\n");
  	      		}
  	      		gazeboPositionData = gazeboPosition->data;

  	      		double w_double;
  	      		gz_position_lock(gazeboPosition,1);
  	      		  w_double = gazeboPositionData->vel_rot[2];
  	      		gz_position_unlock(gazeboPosition);
			return (float)w_double;
     };

     virtual  Ice::Int setV(Ice::Float v, const Ice::Current&){
		   if ( gz_client_wait(gazeboclient) != 0) {
  	   	      		   		printf("Error waiting for Gazebo server\n");
  	   	      		}

  	   	      		if(!gazeboPosition){
  	   	      			printf("Gazebo Position model not opened\n");
  	   	      		}
  	   	      		gazeboPositionData = gazeboPosition->data;

  	   	      		gz_position_lock(gazeboPosition,1);
  	   	      		gazeboPositionData->cmd_vel_pos[0]=(v/1000);
  	   	      		gz_position_unlock(gazeboPosition);

  	   return 0;
     };

     virtual  Ice::Int setW(Ice::Float w, const Ice::Current&){

     		   if ( gz_client_wait(gazeboclient) != 0) {
       	   	      		   		printf("Error waiting for Gazebo server\n");
       	   	      		}

       	   	      		if(!gazeboPosition){
       	   	      			printf("Gazebo Position model not opened\n");
       	   	      		}
       	   	      		gazeboPositionData = gazeboPosition->data;

       	   	      		gz_position_lock(gazeboPosition,1);
       	   	      		gazeboPositionData->cmd_vel_rot[2]=w;
       	   	      		gz_position_unlock(gazeboPosition);
	       	   return 0;
          };

         std::string prefix;
         jderobotice::Context context;

                  gz_client_t *gazeboclient;
                  int gazeboserver_id;
                  int gazeboclient_id;
                  gz_position_t * gazeboPosition;
                  gz_position_data_t * gazeboPositionData;

    };


  class LaserI: virtual public jderobot::Laser {
  	public:
  		LaserI(std::string& propertyPrefix, const jderobotice::Context& context)
       : prefix(propertyPrefix),context(context),
         laserData(new jderobot::LaserData())

     {


       Ice::PropertiesPtr prop = context.properties();

			   gazeboserver_id=0;
			   gazeboclient_id=0;
              	        // Create a client object
			   gazeboclient = gz_client_alloc();

              	        // Connect to the server
			   if (gz_client_connect_wait(gazeboclient, gazeboserver_id, gazeboclient_id) != 0) {
				   printf("ERROR: Connecting to Gazebo server.\n");
			   }

			 //  gazeboPosition = gz_position_alloc ();
			   gazeboLaser = gz_laser_alloc ();
              	     if (gz_laser_open (gazeboLaser, gazeboclient, "laser1") != 0) {
              	    	 gazeboLaser=NULL;
              	    	 printf("Error while opening laser\n");
              	     }

           	      laserData->numLaser=NUM_LASER;
             	  laserData->distanceData.resize(sizeof(int)*laserData->numLaser);

     }

     virtual ~LaserI(){};

    virtual jderobot::LaserDataPtr getLaserData(const Ice::Current&){
  	 double relation; //(gazebo ray number / jde rays number)
  	 int cont2;
  	 int angle=0;
  	 int laserValue;

  	      		if ( gz_client_wait(gazeboclient) != 0) {
  	      		   		printf("Error waiting for Gazebo server\n");
  	      		}

  	      		if(!gazeboLaser){
  	      			printf("Gazebo Laser model not opened\n");
  	      		}
  	      	  ;
  	      		gz_laser_lock(gazeboLaser,1);

  	      	relation=(float)gazeboLaser->data->range_count /(float)NUM_LASER;
  	      	for (angle=0;angle<laserData->numLaser;angle++){
  	      		cont2 = rint(relation * angle);
  	      		laserData->distanceData[angle]=(int) (gazeboLaser->data->ranges[cont2] * 1000);
  	      	}
  	      	gz_laser_unlock(gazeboLaser);
      		return laserData;
     };

  	private:
         std::string prefix;
         jderobotice::Context context;
         jderobot::LaserDataPtr laserData;

                  gz_client_t *gazeboclient;
                  int gazeboserver_id;
                  int gazeboclient_id;
                  gz_laser_t *gazeboLaser;
    };


  class EncodersI: virtual public jderobot::Encoders {
  	public:
  		EncodersI(std::string& propertyPrefix, const jderobotice::Context& context)
       : prefix(propertyPrefix),context(context),
         encodersData(new jderobot::EncodersData())

     {


       Ice::PropertiesPtr prop = context.properties();

			   gazeboserver_id=0;
			   gazeboclient_id=0;
              	        // Create a client object
			   gazeboclient = gz_client_alloc();

              	        // Connect to the server
			   if (gz_client_connect_wait(gazeboclient, gazeboserver_id, gazeboclient_id) != 0) {
				   printf("ERROR: Connecting to Gazebo server.\n");
			   }

              	     gazeboPosition = gz_position_alloc ();
              	     if (gz_position_open (gazeboPosition, gazeboclient, "robot1") != 0) {
              	    	 gazeboPosition=NULL;
              	    	 printf("Error while opening position\n");
              	     }
		 correcting_x = 0.; /* mm */
		 correcting_y = 0.; /* mm */
		 correcting_theta = 0.; /* deg */

     }

     virtual ~EncodersI(){};

    virtual jderobot::EncodersDataPtr getEncodersData(const Ice::Current&){
  	   printf("\ngetEncodersDataV\n");
  	   //waiting for next gazebo camera update
  	      		if ( gz_client_wait(gazeboclient) != 0) {
  	      		   		printf("Error waiting for Gazebo server\n");
  	      		}

  	      		if(!gazeboPosition){
  	      			printf("Gazebo Position model not opened\n");
  	      		}
  	      		gazeboPositionData = gazeboPosition->data;
			float robotx, roboty, robottheta;
  	      		gz_position_lock(gazeboPosition,1);
		encodersData->robotx =
		    (gazeboPositionData->pos[0]) * 1000 * (float) cos (DEGTORAD * correcting_theta) -
		    (gazeboPositionData->pos[1]) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
		    correcting_x;
		encodersData->roboty =
		    (gazeboPositionData->pos[1]) * 1000 * (float) cos (DEGTORAD * correcting_theta) +
		    (gazeboPositionData->pos[0]) * 1000 * (float) sin (DEGTORAD * correcting_theta) +
		    correcting_y;
		encodersData->robottheta = (gazeboPositionData->rot[2] * RADTODEG) + correcting_theta;

		if (encodersData->robottheta <= 0) encodersData->robottheta = encodersData->robottheta + 360;
		else if (encodersData->robottheta > 360) encodersData->robottheta = encodersData->robottheta - 360;
  	      		gz_position_unlock(gazeboPosition);
  
		encodersData->robotcos=cos(encodersData->robottheta);
		encodersData->robotsin=sin(encodersData->robottheta);
      		return encodersData;
     };

  	private:
         std::string prefix;
         jderobotice::Context context;
         jderobot::EncodersDataPtr encodersData;
                  gz_client_t *gazeboclient;
                  int gazeboserver_id;
                  int gazeboclient_id;
                 gz_position_t * gazeboPosition;
                  gz_position_data_t * gazeboPositionData;
		/* Variables put to 0.0 and no change during the execution */
		float correcting_x; /* mm */
		float correcting_y; /* mm */
		float correcting_theta; /* deg */


    };

//PTMOTORS
  class PTMotorsI: virtual public jderobot::PTMotors {
  	public:
  		PTMotorsI(std::string& propertyPrefix, const jderobotice::Context& context)
       : prefix(propertyPrefix),context(context)
     {


       Ice::PropertiesPtr prop = context.properties();

			   gazeboserver_id=0;
              	        gazeboclient_id=0;

              	        // Create a client object
              	        gazeboclient = gz_client_alloc();

       	        // Connect to the server
       	        if (gz_client_connect_wait(gazeboclient, gazeboserver_id, gazeboclient_id) != 0) {
       	      	  printf("ERROR: Connecting to Gazebo server.\n");
      	      	}
		gazeboPTZ = gz_ptz_alloc();
      			if (gz_ptz_open (gazeboPTZ, gazeboclient, "robot1") != 0)
		{
			  fprintf (stderr, "Error opening \"%s\" ptz\n","robot1");
			  //return (-1);
		}
     }

     virtual ~PTMotorsI(){};

     virtual Ice::Int setPTMotorsData(const jderobot::PTMotorsDataPtr & data, const Ice::Current&){
  	   //waiting for next gazebo camera update
  	      		if ( gz_client_wait(gazeboclient) != 0) {
  	      		   		printf("Error waiting for Gazebo server\n");
  	      		}

  	      		if(!gazeboPTZ){
  	      			printf("Gazebo PTMOTORS model not opened\n");
  	      		}
		gz_ptz_lock(gazeboPTZ,1);
		ptMotorsData->longitude=data->longitude;
		if (data->longitude > MAX_PAN)
		  ptMotorsData->longitude=MAX_PAN;
		else if (data->longitude < -54)
		  ptMotorsData->longitude= MIN_PAN;
		
		ptMotorsData->latitude=data->latitude;
		if (data->latitude > MAX_TILT)
		  ptMotorsData->latitude= MAX_TILT;
		else if (data->latitude < MIN_TILT)
		  ptMotorsData->latitude= MIN_TILT;
		
		gazeboPTZ->data->cmd_pan=-ptMotorsData->longitude * DEGTORAD;
		gazeboPTZ->data->cmd_tilt=-ptMotorsData->latitude * DEGTORAD;
		gz_ptz_unlock(gazeboPTZ);

		return 0; 
     };


         std::string prefix;
         jderobotice::Context context;
	jderobot::PTMotorsDataPtr ptMotorsData;

                  gz_client_t *gazeboclient;
                  int gazeboserver_id;
                  int gazeboclient_id;
		  gz_ptz_t * gazeboPTZ;
                  
    };


//PTENCODERS
  class PTEncodersI: virtual public jderobot::PTEncoders {
  	public:
  		PTEncodersI(std::string& propertyPrefix, const jderobotice::Context& context)
       : prefix(propertyPrefix),context(context)
     {


       Ice::PropertiesPtr prop = context.properties();

			   gazeboserver_id=0;
              	        gazeboclient_id=0;

              	        // Create a client object
              	        gazeboclient = gz_client_alloc();

       	        // Connect to the server
       	        if (gz_client_connect_wait(gazeboclient, gazeboserver_id, gazeboclient_id) != 0) {
       	      	  printf("ERROR: Connecting to Gazebo server.\n");
      	      	}
		gazeboPTZ = gz_ptz_alloc();
      			if (gz_ptz_open (gazeboPTZ, gazeboclient, "robot1") != 0)
		{
			  fprintf (stderr, "Error opening \"%s\" ptz\n","robot1");
			  //return (-1);
		}
     }

     virtual ~PTEncodersI(){};

     virtual jderobot::PTEncodersDataPtr getPTEncodersData(const Ice::Current&){
  	   //waiting for next gazebo camera update
  	      		if ( gz_client_wait(gazeboclient) != 0) {
  	      		   		printf("Error waiting for Gazebo server\n");
  	      		}

  	      		if(!gazeboPTZ){
  	      			printf("Gazebo PTENCODERS model not opened\n");
  	      		}
		gz_ptz_lock(gazeboPTZ,1);
		ptEncodersData->panAngle=-1 * gazeboPTZ->data->pan * RADTODEG;
		ptEncodersData->tiltAngle= -1 * gazeboPTZ->data->tilt * RADTODEG;
		gz_ptz_unlock(gazeboPTZ);

		return ptEncodersData; 
     };


         std::string prefix;
         jderobotice::Context context;
	jderobot::PTEncodersDataPtr ptEncodersData;

                  gz_client_t *gazeboclient;
                  int gazeboserver_id;
                  int gazeboclient_id;
		  gz_ptz_t * gazeboPTZ;
                  
    };

//SONARS
  class SonarsI: virtual public jderobot::Sonars {
  	public:
  		SonarsI(std::string& propertyPrefix, const jderobotice::Context& context)
       : prefix(propertyPrefix),context(context)
     {


       Ice::PropertiesPtr prop = context.properties();

			   gazeboserver_id=0;
              	        gazeboclient_id=0;

              	        // Create a client object
              	        gazeboclient = gz_client_alloc();

       	        // Connect to the server
       	        if (gz_client_connect_wait(gazeboclient, gazeboserver_id, gazeboclient_id) != 0) {
       	      	  printf("ERROR: Connecting to Gazebo server.\n");
      	      	}
		gazeboSonar = gz_sonar_alloc();
      			if (gz_sonar_open (gazeboSonar, gazeboclient, "robot1") != 0)
		{
			  fprintf (stderr, "Error opening \"%s\" sonar\n","robot1");
			  //return (-1);
		}
		gazeboSonar->data->cmd_enable_sonar = 1;

		  sonarsData->numSonars=NUM_SONARS;
             	  sonarsData->us.resize(sizeof(int)*sonarsData->numSonars);
     }

     virtual ~SonarsI(){};

     virtual jderobot::SonarsDataPtr getSonarsData(const Ice::Current&){
  	   //waiting for next gazebo camera update
		int j;
  	      		if ( gz_client_wait(gazeboclient) != 0) {
  	      		   		printf("Error waiting for Gazebo server\n");
  	      		}

  	      		if(!gazeboSonar){
  	      			printf("Gazebo SONARS model not opened\n");
  	      		}
		  gz_sonar_lock (gazeboSonar, 1);
		  for (j = 0; j < NUM_SONARS; j++)
		    {
		      sonarsData->us[j] = (int) gazeboSonar->data->sonar_ranges[j]*1000;
		    }
		  gz_sonar_unlock (gazeboSonar);
		return sonarsData; 
     };


         std::string prefix;
         jderobotice::Context context;
	jderobot::SonarsDataPtr sonarsData;

                  gz_client_t *gazeboclient;
                  int gazeboserver_id;
                  int gazeboclient_id;
		  gz_sonar_t * gazeboSonar;
                  
    };


//COMPONENT
  class Component: public jderobotice::Component{
  public:
    Component()
      :jderobotice::Component("GazeboServer"), cameras(0), motors1(0), laser1(0), encoders1(0), ptmotors1(0), ptencoders1(0), sonars1(0) {}

    virtual void start(){
      Ice::PropertiesPtr prop = context().properties();
      int nCameras = prop->getPropertyAsInt(context().tag() + ".NCameras");
      cameras.resize(nCameras);
      for (int i=0; i<nCameras; i++){//build camera objects
	std::stringstream objIdS;
	objIdS <<  i;
	std::string objId = objIdS.str();// should this be something unique??
	std::string objPrefix(context().tag() + ".Camera." + objId + ".");
	std::string cameraName = prop->getProperty(objPrefix + "Name");

	if (cameraName.size() == 0){//no name specified, we create one using the index
	  cameraName = "camera" + objId;
	  prop->setProperty(objPrefix + "Name",cameraName);//set the value
	}
	context().tracer().info("Creating camera " + cameraName);
	cameras[i] = new CameraI(objPrefix,context());
	context().createInterfaceWithString(cameras[i],cameraName);

	//Motors
	std::string objPrefix2="motors1";
	std::string gazeboactName = "motors1";
	context().tracer().info("Creating motors1 " + gazeboactName);
	motors1 = new MotorsI(objPrefix2,context());
	context().createInterfaceWithString(motors1,gazeboactName);

	//Laser
	std::string objPrefix3="laser1";
		std::string laserName = "laser1";
		context().tracer().info("Creating laser1 " + laserName);
		laser1 = new LaserI(objPrefix3,context());
		context().createInterfaceWithString(laser1,laserName);

	//Encoders
	std::string objPrefix4="encoders1";
		std::string encodersName = "encoders1";
		context().tracer().info("Creating encoders1 " + encodersName);
		encoders1 = new EncodersI(objPrefix4,context());
		context().createInterfaceWithString(encoders1,encodersName);

	//PTMotors
	std::string objPrefix5="ptmotors1";
	std::string ptmotorsName = "ptmotors1";
	context().tracer().info("Creating ptmotors1 " + ptmotorsName);
	ptmotors1 = new PTMotorsI(objPrefix5,context());
	context().createInterfaceWithString(ptmotors1,ptmotorsName);

	//PTEncoders
	std::string objPrefix6="ptencoders1";
	std::string ptencodersName = "ptencoders1";
	context().tracer().info("Creating ptencoders1 " + ptencodersName);
	ptencoders1 = new PTEncodersI(objPrefix6,context());
	context().createInterfaceWithString(ptencoders1,ptencodersName);

	//Sonars
/*	std::string objPrefix7="sonars1";
	std::string sonarsName = "sonars1";
	context().tracer().info("Creating sonars1 " + sonarsName);
	sonars1 = new SonarsI(objPrefix7,context());
	context().createInterfaceWithString(sonars1,sonarsName);
*/
      }
    }

    virtual ~Component(){
    }

  private:
    std::vector<Ice::ObjectPtr> cameras;
    Ice::ObjectPtr motors1;
    Ice::ObjectPtr laser1;
    Ice::ObjectPtr encoders1;
    Ice::ObjectPtr ptmotors1;
    Ice::ObjectPtr ptencoders1;
    Ice::ObjectPtr sonars1;
  };

} //namespace

int main(int argc, char** argv){

  gazeboserver::Component component;

  jderobotice::Application app(component);

  return app.jderobotMain(argc,argv);

}

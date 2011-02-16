#include "camera.h"


   //Constructor
   camera::camera(){
   	cam= new EVI_D100P();
   }

   // camera init
   int camera::init(){
	if(cam->Init() != 1) {
      		return -1;
	}
	return 1;
   }
	
   int camera::open(int mode,char* evi_device){
	if(cam->Open(mode, evi_device) != 1) {		
      		return -1;
        }
        return 1;
   }

   int camera::powerInq(){
	return cam->PowerInq();
   }

   int camera::power(int mode){
	if(cam->Power(mode) != 1) {         	
         	return -1;
        }
        return 1;
   }

   int camera::home(){
	if(cam->Home() != 1)
     		return -1;
        return 1;
   }

   int camera::pan_TiltPosInq(float &longitude,float &latitude){
	if(cam->Pan_TiltPosInq(longitude, latitude) != 1)
      		return -1;
        return 1;
  }
   int camera::zoomPosInq(float &zoom){
	if(cam->ZoomPosInq(zoom) != 1)
      		return -1;
        return 1;
  }

   int camera::Pan_TiltDrive(int type, int pan_speed, int tilt_speed, float pan_pos, float tilt_pos, int waitC) {
      return cam->Pan_TiltDrive(type, pan_speed, tilt_speed, pan_pos, tilt_pos, waitC);
   }

   int camera::Zoom(int type, int speed, float zoom, int waitC) {
      return cam->Zoom(type, speed, zoom, waitC);
   }


  // camera actions: pantiltdrive, zoomdrive...






//FUNCIONES EXPORTADAS A C
   camera* NewCamera(){
	return new camera();
   }

   void DeleteCamera(camera* p){
	delete p;
   }

  int InitCamera(camera* p){
	return p->init();
  }

  int OpenCamera(camera* p,int mode,char* device){
	return p->open(1, device);
  }

  int PowerInqCamera(camera* p){
	return p->powerInq();
  }
  
  int PowerCamera(camera* p,int mode){
	return p->power(mode);	
  }

  int HomeCamera(camera* p){
	return p->home();
  }

  int Pan_TiltPosInqCamera(camera* p,float *longitude,float *latitude){
	return p->pan_TiltPosInq(*longitude, *latitude);
  }

  int ZoomPosInqCamera(camera* p,float *zoom){
	return p->zoomPosInq(*zoom);
  }

   int Pan_TiltDriveCamera(camera *p, int type, int pan_speed, int tilt_speed, float pan_pos, float tilt_pos, int waitC) {
      return p->Pan_TiltDrive(type, pan_speed, tilt_speed, pan_pos, tilt_pos, waitC);
   }

   int ZoomCamera(camera *p, int type, int speed, float zoom, int waitC) {
      return p->Zoom(type, speed, zoom, waitC);
   }

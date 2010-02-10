#ifndef CAMERA_H
#define CAMERA_H

#include "./EVILib/EVI-D100P.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

class camera{
   private:
   	EVI_D100P* cam;

   public:

   //Constructores
   camera();
   int init();
   int open(int,char*);
   int powerInq();
   int power(int);
   int home();
   int pan_TiltPosInq(float&,float&);
   int zoomPosInq(float&);
   int Pan_TiltDrive(int, int, int, float, float, int);
   int Zoom(int, int, float, int);
};

extern "C" camera* NewCamera();
extern "C" void DeleteCamera(camera* p);
extern "C" int InitCamera(camera* p);
extern "C" int OpenCamera(camera* p,int mode,char* device);
extern "C" int PowerInqCamera(camera* p);
extern "C" int PowerCamera(camera* p,int mode);
extern "C" int HomeCamera(camera* p);
extern "C" int Pan_TiltPosInqCamera(camera* p,float *longitude,float *latitude);
extern "C" int ZoomPosInqCamera(camera* p,float *zoom);
extern "C" int Pan_TiltDriveCamera(camera* p,int type, int pan_speed, int tilt_speed, float pan_pos, float tilt_pos, int waitC);
extern "C" int ZoomCamera(camera* p,int type, int speed, float zoom, int waitC);

#endif

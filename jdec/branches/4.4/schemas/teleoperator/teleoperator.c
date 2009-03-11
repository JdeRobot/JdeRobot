/*
 *  Copyright (C) 2006 José María Cañas Plaza 
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
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 */

#define DEBUG 1

#include <jde.h>
#include <forms.h>
#include <glcanvas.h>
#include <graphics_xforms.h>
#include <teleoperatorgui.h>
#include <pioneer.h>
#include <pioneeropengl.h>

#define v3f glVertex3f
#include <GL/gl.h>              
#include <GL/glx.h>
#include <GL/glu.h>

int finish_flag=0;

#define MOUSELEFT 1
#define MOUSEMIDDLE 2
#define MOUSERIGHT 3
#define MOUSEWHEELUP 4
#define MOUSEWHEELDOWN 5

#define DISPLAY_ROBOT 0x01UL
#define DISPLAY_PANTILTENCODERS 0x02UL
#define DISPLAY_SONARS 0x04UL
#define DISPLAY_LASER 0x08UL
#define DISPLAY_COLORIMAGEA 0x10UL
#define DISPLAY_COLORIMAGEB 0x20UL
#define DISPLAY_COLORIMAGEC 0x40UL
#define DISPLAY_COLORIMAGED 0x80UL
#define BASE_TELEOPERATOR 0x100UL
#define PANTILT_TELEOPERATOR 0x200UL
#define DISPLAY_ZOOMENCODERS 0x400UL
#define ZOOM_TELEOPERATOR 0x800UL

#define PI 3.141592654
#define MAXWORLD 30.
typedef struct SoRtype{
  struct SoRtype *father;
  float posx;
  float posy;
  float posz;
  float foax;
  float foay;
  float foaz;
  float roll;
}SofReference;

static SofReference mypioneer,virtualcam;
/* robot and virtual camera in the absolute FrameOfReference */
static int track_robot=TRUE;
static int toggle=FALSE;

Display *mydisplay;
int  *myscreen;

/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;

#define PUSHED 1
#define RELEASED 0


static char *samplesource=NULL;
static int vmode;
static XImage *imagenA,*imagenB,*imagenC,*imagenD,*sampleimage; 
static char *imagenA_buf, *imagenB_buf, *imagenC_buf, *imagenD_buf, *sampleimage_buf; /* puntero a memoria para la imagen a visualizar en el servidor X. No compartida con el servidor */
static long int tabla[256]; 
/* tabla con la traduccion de niveles de gris a numero de pixel en Pseudocolor-8bpp. Cada numero de pixel apunta al valor adecuado del ColorMap, con el color adecuado instalado */
static int pixel8bpp_rojo, pixel8bpp_blanco, pixel8bpp_amarillo;


int teleoperator_id=0; 
int teleoperator_brothers[MAX_SCHEMAS];
arbitration teleoperator_callforarbitration;
int teleoperator_cycle=100; /* ms */

#define joystick_maxRotVel 30 /* deg/sec */
#define joystick_maxTranVel 500 /* mm/sec */
float joystick_x, joystick_y;
float pt_joystick_x, pt_joystick_y, pt_joystick_z;
int back=0;

float *mysonars=NULL;
int *mylaser=NULL;
float *myencoders=NULL;
runFn laserrun, sonarsrun, encodersrun;
stopFn laserstop, sonarsstop, encodersstop;

float *myv=NULL;
float *myw=NULL;
runFn motorsrun;
stopFn motorsstop;

float *pan_angle=NULL, *tilt_angle=NULL;  /* degs */
float *mylongitude=NULL; /* degs, pan angle */
float *longitude_max=NULL, *longitude_min=NULL;
float *mylatitude=NULL; /* degs, tilt angle */
float *latitude_max=NULL, *latitude_min=NULL;
float *longitude_speed=NULL;
float *longitude_speed_max=NULL;
float *latitude_speed=NULL;
runFn ptmotorsrun, ptencodersrun;
stopFn ptmotorsstop, ptencodersstop;

float *myzoom_position=NULL;
float *myzoom=NULL;
float *myzoom_max=NULL, *myzoom_min=NULL;
float *myzoom_speed=NULL;
float *myzoom_speed_max=NULL;
runFn zmotorsrun, zencodersrun;
stopFn zmotorsstop, zencodersstop;

char **mycolorA;
runFn colorArun;
stopFn colorAstop;
char **mycolorB;
runFn colorBrun;
stopFn colorBstop;
char **mycolorC;
runFn colorCrun;
stopFn colorCstop;
char **mycolorD;
runFn colorDrun;
stopFn colorDstop;


FD_teleoperatorgui *fd_teleoperatorgui=NULL;
GC teleoperatorgui_gc;
Window teleoperatorgui_win; 
unsigned long display_state;


void teleoperator_iteration()
{  
  double delta, deltapos;
  float speed_coef;

  speedcounter(teleoperator_id);

  if ((display_state & BASE_TELEOPERATOR)!=0)
    {
      /* ROTACION=ejeX: Ajusta a un % de joystick_maxRotVel. OJO no funcion lineal del desplazamiento visual, sino con el al cuadrado, para aplanarla en el origen y evitar cabeceos, conseguir suavidad en la teleoperacion */
      delta = (joystick_x-0.5)*2; /* entre +-1 */
      deltapos = fabs(delta); /* Para que no moleste el signo de delta en el factor de la funcion de control */

      if (delta<0) (*myw) = (float) joystick_maxRotVel*deltapos*deltapos*deltapos; 
      else (*myw) = (float) -1.*joystick_maxRotVel*deltapos*deltapos*deltapos;

      /* TRASLACION=ejeY: Ajusta a un % de +-joystick_maxTranVel. OJO no funcion lineal del desplazamiento visual, sino con el a la cuarta, para aplanarla en el origen */
      
      delta = (joystick_y-0.5)*2; /* entre +-1 */
      deltapos = fabs(delta);/* Para que no moleste el signo de delta en el factor de la funcion de control */
      if (delta<0) (*myv) = (float) -1.*joystick_maxTranVel*deltapos*deltapos*deltapos;
      else (*myv) = (float) joystick_maxTranVel*deltapos*deltapos*deltapos;
    }
  
  if ((display_state & PANTILT_TELEOPERATOR)!=0)      
    {
      /* pt_joystick_x and pt_joystick_y fall in [0,1], the default limits from Xforms */  
      *mylatitude=pt_joystick_y*((*latitude_max)-(*latitude_min))+(*latitude_min);
      *mylongitude=pt_joystick_x*((*longitude_max)-(*longitude_min))+(*longitude_min);
      
      speed_coef = fl_get_slider_value(fd_teleoperatorgui->ptspeed);
      *longitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
      *latitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
    }

   if ((display_state & ZOOM_TELEOPERATOR)!=0){
      if (myzoom && myzoom_max && myzoom_min){
         *myzoom=(*myzoom_min)+pt_joystick_z*((*myzoom_max)-(*myzoom_min));
      }
      
      if (myzoom_speed && myzoom_speed_max){
         *myzoom_speed=(1.-speed_coef)*(*myzoom_speed_max);
      }
   }
  
}


void teleoperator_stop()
{
  pthread_mutex_lock(&(all[teleoperator_id].mymutex));
  put_state(teleoperator_id,slept);
  /* printf("teleoperator: off\n");*/
  pthread_mutex_unlock(&(all[teleoperator_id].mymutex));
  
  if ((display_state & DISPLAY_SONARS)!=0)
     sonarsstop();
  if ((display_state & DISPLAY_LASER)!=0)
     laserstop();
  if ((display_state & DISPLAY_ROBOT)!=0)
     encodersstop();
  if ((display_state & BASE_TELEOPERATOR)!=0)
     motorsstop();
  if ((display_state & DISPLAY_COLORIMAGEA)!=0)
     colorAstop();
  if ((display_state & DISPLAY_COLORIMAGEB)!=0)
     colorBstop();
  if ((display_state & DISPLAY_COLORIMAGEC)!=0)
     colorCstop();
  if ((display_state & DISPLAY_COLORIMAGED)!=0)
     colorDstop();
  if ((display_state & DISPLAY_PANTILTENCODERS)!=0)
     ptencodersstop();
  if ((display_state & PANTILT_TELEOPERATOR)!=0)
     ptmotorsstop();
}


void teleoperator_run(int father, int *brothers, arbitration fn)
{
  int i;
 
  pthread_mutex_lock(&(all[teleoperator_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[teleoperator_id].children[i]=FALSE;
 
  all[teleoperator_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) teleoperator_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {teleoperator_brothers[i]=brothers[i];i++;}
    }
  teleoperator_callforarbitration=fn;
  put_state(teleoperator_id,notready);

 
  /*  printf("teleoperator: on\n");*/
  pthread_cond_signal(&(all[teleoperator_id].condition));
  pthread_mutex_unlock(&(all[teleoperator_id].mymutex));
}

void *teleoperator_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;finish_flag==0;)
    {
      pthread_mutex_lock(&(all[teleoperator_id].mymutex));

      if (all[teleoperator_id].state==slept) 
	{
	  pthread_cond_wait(&(all[teleoperator_id].condition),&(all[teleoperator_id].mymutex));
	  pthread_mutex_unlock(&(all[teleoperator_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[teleoperator_id].state==notready) put_state(teleoperator_id,ready);
	  else if (all[teleoperator_id].state==ready)	  /* check brothers and arbitrate. For now this is the only winner */
	    { 
	      put_state(teleoperator_id,winner);
	 
	      /* start the winner state from controlled motor values */
              /*
	      if ((display_state & DISPLAY_SONARS)!=0) 
		{
		  mysonars=(float *)myimport("sonars","us");
		  sonarsrun=(runFn)myimport("sonars","run");
		  sonarsstop=(stopFn)myimport("sonars","stop");
		  sonarsrun(teleoperator_id,NULL,NULL);
                  if (myv!=NULL)
                     *myv=0;
                  if (myw!=NULL)
                     *myw=0;
		}
	      
	      if ((display_state & DISPLAY_LASER)!=0) 
		{
		  mylaser=(int *)myimport("laser","laser");
		  laserrun=(runFn)myimport("laser","run");
		  laserstop=(stopFn)myimport("laser","stop");
		  laserrun(teleoperator_id,NULL,NULL);
		}
	      
	      if ((display_state & DISPLAY_ROBOT)!=0)
		{
		  myencoders=(float *)myimport("encoders","jde_robot");
		  encodersrun=(runFn)myimport("encoders","run");
		  encodersstop=(stopFn)myimport("encoders","stop");
		  encodersrun(teleoperator_id,NULL,NULL);
		}
	      
	      if ((display_state & BASE_TELEOPERATOR)!=0)
		{
		  myv=(float *)myimport("motors","v");
		  myw=(float *)myimport("motors","w");
		  motorsrun=(runFn)myimport("motors","run");
		  motorsstop=(stopFn)myimport("motors","stop");
		  motorsrun(teleoperator_id,NULL,NULL);
		}*/
	    }
	  else if (all[teleoperator_id].state==winner);

	  if (all[teleoperator_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[teleoperator_id].mymutex));
	      gettimeofday(&a,NULL);
	      teleoperator_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = teleoperator_cycle*1000-diff-10000; 
	      if (next>0) 
		/* discounts 10ms taken by calling usleep itself */
		usleep(teleoperator_cycle*1000-diff);
	      else 
		/* just let this iteration go away. overhead time negligible */
		{/*printf("time interval violated: teleoperator\n"); */
		usleep(teleoperator_cycle*1000);
		}
	    }
	  else 
	    {
	      pthread_mutex_unlock(&(all[teleoperator_id].mymutex));
	      usleep(teleoperator_cycle*1000);
	    }
	}
    }
}

void teleoperator_terminate()
{
  /*
  pthread_mutex_lock(&(all[teleoperator_id].mymutex));
  teleoperator_stop();  
  pthread_mutex_unlock(&(all[teleoperator_id].mymutex));
  sleep(2);
  */
 if (fd_teleoperatorgui!=NULL)
 {
    if (all[teleoperator_id].guistate==on){
       teleoperator_hide();
       all[teleoperator_id].guistate=off;
    }
 }
 teleoperator_stop();
 finish_flag=1;
 printf ("teleoperator close\n");

  /*free(imagenA_buf);
  free(imagenB_buf);
  free(imagenC_buf);
  free(imagenD_buf);*/  
}


void teleoperator_init(char *configfile)
{
  samplesource=NULL;
  virtualcam.posx=-150;
  virtualcam.posy=-150.;
  virtualcam.posz=150.;
  virtualcam.foax=0.;
  virtualcam.foay=0.;
  virtualcam.foaz=0.;
  virtualcam.roll=0.;
  init_pioneer();
  pthread_mutex_lock(&(all[teleoperator_id].mymutex));
  myexport("teleoperator","id",&teleoperator_id);
  myexport("teleoperator","run",(void *) &teleoperator_run);
  myexport("teleoperator","stop",(void *) &teleoperator_stop);
  printf("teleoperator schema started up\n");
  put_state(teleoperator_id,slept);
  pthread_create(&(all[teleoperator_id].mythread),NULL,teleoperator_thread,NULL);

  if (myregister_buttonscallback==NULL){
     if ((myregister_buttonscallback=(registerbuttons)myimport ("graphics_xforms", "register_buttonscallback"))==NULL){
        printf ("I can't fetch register_buttonscallback from graphics_xforms\n");
        jdeshutdown(1);
     }
     if ((mydelete_buttonscallback=(deletebuttons)myimport ("graphics_xforms", "delete_buttonscallback"))==NULL){
        printf ("I can't fetch delete_buttonscallback from graphics_xforms\n");
        jdeshutdown(1);
     }
     if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_xforms", "register_displaycallback"))==NULL){
        printf ("I can't fetch register_displaycallback from graphics_xforms\n");
        jdeshutdown(1);
     }
     if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_xforms", "delete_displaycallback"))==NULL){
        jdeshutdown(1);
        printf ("I can't fetch delete_displaycallback from graphics_xforms\n");
     }
  }

  if ((myscreen=(int *)myimport("graphics_xforms", "screen"))==NULL){
     fprintf (stderr, "teleoperator: I can't fetch screen from graphics_xforms\n");
     jdeshutdown(1);
  }
  if ((mydisplay=(Display *)myimport("graphics_xforms", "display"))==NULL){
     fprintf (stderr, "teleoperator: I can't fetch display from graphics_xforms\n");
     jdeshutdown(1);
  }
  pthread_mutex_unlock(&(all[teleoperator_id].mymutex));
}

static int InitOGL(FL_OBJECT *ob, Window win,int w,int h, XEvent *xev, void *ud)
 /* Inicializa OpenGL con los parametros que diran como se visualiza. */
{
   GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat position[] = {0.0, 3.0, 3.0, 0.0};
   GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat local_view[] = {0.0};
 
   glViewport(0,0,(GLint)w,(GLint)h);  
   /*  glDrawBuffer(GL_FRONT);*/
   glDrawBuffer(GL_BACK);
   glClearColor(0.6f, 0.8f, 1.0f, 0.0f);  
   glClearDepth(1.0);                           /* Enables Clearing Of The Depth Buffer */
   /* This Will Clear The Background Color To Light Blue */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
   /* With this, the pioneer appears correctly, but the cubes don't */
   glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
   glLightfv (GL_LIGHT0, GL_POSITION, position);
   glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
   glEnable (GL_LIGHT0);
   /*glEnable (GL_LIGHTING);*/
   
   glEnable(GL_TEXTURE_2D);     /* Enable Texture Mapping */
   glEnable (GL_AUTO_NORMAL);
   glEnable (GL_NORMALIZE);  
   glEnable(GL_DEPTH_TEST);     /* Enables Depth Testing */
   glDepthFunc(GL_LESS);  
   glShadeModel(GL_SMOOTH);     /* Enables Smooth Color Shading */
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   return 0;
}


static int image_displaysetup() 
/* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/ 
{
    XGCValues gc_values;
    XWindowAttributes win_attributes;
    XColor nuevocolor;
    int pixelNum, numCols;
    int allocated_colors=0, non_allocated_colors=0;

    teleoperatorgui_win= FL_ObjWin(fd_teleoperatorgui->ventanaA);
    XGetWindowAttributes(mydisplay, teleoperatorgui_win, &win_attributes);
    XMapWindow(mydisplay, teleoperatorgui_win);
    /*XSelectInput(display, teleoperatorgui_win, ButtonPress|StructureNotifyMask);*/   
    gc_values.graphics_exposures = False;
    teleoperatorgui_gc = XCreateGC(mydisplay, teleoperatorgui_win, GCGraphicsExposures, &gc_values);  

    /* Utilizan el Visual (=estructura de color) y el colormap con que este operando el programa principal con su Xforms. No crea un nuevo colormap, sino que modifica el que se estaba usando a traves de funciones de Xforms*/
    vmode= fl_get_vclass();
    if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
      {printf("teleoperator: truecolor 16 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),win_attributes.depth, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenB = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),win_attributes.depth, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenC = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),win_attributes.depth, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenD = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),win_attributes.depth, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      return win_attributes.depth;
      sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2);    
      sampleimage = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),win_attributes.depth, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      }
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24)) 
      { printf("teleoperator: truecolor 24 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenB = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenC = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenD = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      sampleimage = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      return win_attributes.depth;
      }
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32)) 
      { printf("teleoperator: truecolor 24 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenB = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenC = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenD = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      sampleimage = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      return win_attributes.depth;
      }
    else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)) 
      {
	numCols = 256;
	for (pixelNum=0; pixelNum<numCols; pixelNum++) 
	  {
	    nuevocolor.pixel=0;
	    nuevocolor.red=pixelNum<<8;
	    nuevocolor.green=pixelNum<<8;
	    nuevocolor.blue=pixelNum<<8;
	    nuevocolor.flags=DoRed|DoGreen|DoBlue;
	    
	    /*if (XAllocColor(display,DefaultColormap(display,*myscreen),&nuevocolor)==False) tabla[pixelNum]=tabla[pixelNum-1];*/
	    if (XAllocColor(mydisplay,fl_state[vmode].colormap,&nuevocolor)==False) {tabla[pixelNum]=tabla[pixelNum-1]; non_allocated_colors++;}
	    else {tabla[pixelNum]=nuevocolor.pixel;allocated_colors++;}
	  }
	printf("teleoperator: depth= %d\n", fl_state[vmode].depth); 
	printf("teleoperator: colormap got %d colors, %d non_allocated colors\n",allocated_colors,non_allocated_colors);

	imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenB = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenC = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenD = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS);    
	sampleimage = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);

	pixel8bpp_rojo = fl_get_pixel(FL_RED);
	pixel8bpp_blanco = fl_get_pixel(FL_WHITE);
	pixel8bpp_amarillo = fl_get_pixel(FL_YELLOW);
	return win_attributes.depth;
      }
    else 
      {
	perror("Unsupported color mode in X server");exit(1);
      }
    return win_attributes.depth;
}


int freeobj_ventanaAA_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGEA)!=0))
    samplesource=*mycolorA;
  return 0;
}

int freeobj_ventanaBB_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGEB)!=0))
    samplesource=*mycolorB;
  return 0;
}

int freeobj_ventanaCC_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGEC)!=0)) 
    samplesource=*mycolorC;
  return 0;
}

int freeobj_ventanaDD_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGED)!=0))
    samplesource=*mycolorD;
  return 0;
}


extern void teleoperator_hide(void);

void teleoperator_guibuttons(void *obj1){
   FL_OBJECT *obj;
   float dpan=0.5,dtilt=0.5, speed_coef;
   float r,lati,longi,guix,guiy,guiz;
   float dx,dy,dz;
   obj=(FL_OBJECT *)obj1;

   if (track_robot==TRUE){
      if (myencoders!=NULL){
         virtualcam.foax=myencoders[0]/100.;
         virtualcam.foay=myencoders[1]/100.;
      }
      else{
         virtualcam.foax=0.;
         virtualcam.foay=0.;
      }
      virtualcam.foaz=0.;

      if ((fl_get_button(fd_teleoperatorgui->selectfoa)==PUSHED) ||
           (fl_get_button(fd_teleoperatorgui->selectfoaRel)==PUSHED))
      {
         if (fl_get_button(fd_teleoperatorgui->selectfoa)==PUSHED){
            guix=virtualcam.foax;
            guiy=virtualcam.foay;
            guiz=virtualcam.foaz;
         }
         else if (fl_get_button(fd_teleoperatorgui->selectfoaRel)==PUSHED){
            guix=virtualcam.foax-virtualcam.posx;
            guiy=virtualcam.foay-virtualcam.posy;
            guiz=virtualcam.foaz-virtualcam.posz;
         }
         fl_set_slider_value(fd_teleoperatorgui->posX,(double)guix);
         fl_set_slider_value(fd_teleoperatorgui->posY,(double)guiy);
         fl_set_slider_value(fd_teleoperatorgui->posZ,(double)guiz);
         r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
         lati=(float)asin(guiz/r)*360./(2.*PI);
         longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
         fl_set_positioner_xvalue(fd_teleoperatorgui->latlong,(double) longi);
         fl_set_positioner_yvalue(fd_teleoperatorgui->latlong,(double) lati);
         fl_set_slider_value(fd_teleoperatorgui->posR,(double)r);
      }
   }

   if (obj==fd_teleoperatorgui->posOrigin) {
      guix=0.; guiy=0.; guiz=0.;
      fl_set_slider_value(fd_teleoperatorgui->posX,(double)guix);
      fl_set_slider_value(fd_teleoperatorgui->posY,(double)guiy);
      fl_set_slider_value(fd_teleoperatorgui->posZ,(double)guiz);
      fl_set_positioner_xvalue(fd_teleoperatorgui->latlong,(double)0.);
      fl_set_positioner_yvalue(fd_teleoperatorgui->latlong,(double)90.);
      fl_set_slider_value(fd_teleoperatorgui->posR,(double)0.);
      if (fl_get_button(fd_teleoperatorgui->selectposition)==PUSHED){
         if (toggle==TRUE){
            dx=virtualcam.foax - virtualcam.posx;
            dy=virtualcam.foay - virtualcam.posy;
            dz=virtualcam.foaz - virtualcam.posz;
            virtualcam.foax=guix+dx;
            virtualcam.foay=guiy+dy;
            virtualcam.foaz=guiz+dz;
         }
         virtualcam.posx=guix; virtualcam.posy=guiy; virtualcam.posz=guiz;
      }
      else if (fl_get_button(fd_teleoperatorgui->selectfoa)==PUSHED){
         virtualcam.foax=guix;
         virtualcam.foay=guiy;
         virtualcam.foaz=guiz;
      }
      else if (fl_get_button(fd_teleoperatorgui->selectfoaRel)==PUSHED){
         virtualcam.foax=virtualcam.posx+guix;
         virtualcam.foay=virtualcam.posy+guiy;
         virtualcam.foaz=virtualcam.posz+guiz;
      }
   }
   else if ((obj==fd_teleoperatorgui->posX) ||
             (obj==fd_teleoperatorgui->posY) ||
             (obj==fd_teleoperatorgui->posZ))
   {
      guix=(float) fl_get_slider_value(fd_teleoperatorgui->posX);
      guiy=(float) fl_get_slider_value(fd_teleoperatorgui->posY);
      guiz=(float) fl_get_slider_value(fd_teleoperatorgui->posZ);
      if (fl_get_button(fd_teleoperatorgui->selectposition)==PUSHED){
         if (toggle==TRUE){
            dx=virtualcam.foax - virtualcam.posx;
            dy=virtualcam.foay - virtualcam.posy;
            dz=virtualcam.foaz - virtualcam.posz;
            virtualcam.foax=guix+dx;
            virtualcam.foay=guiy+dy;
            virtualcam.foaz=guiz+dz;
         }
         virtualcam.posx=guix; virtualcam.posy=guiy; virtualcam.posz=guiz;
      }
      else if (fl_get_button(fd_teleoperatorgui->selectfoa)==PUSHED){
         virtualcam.foax=guix;
         virtualcam.foay=guiy;
         virtualcam.foaz=guiz;
      }
      else if (fl_get_button(fd_teleoperatorgui->selectfoaRel)==PUSHED){
         virtualcam.foax=guix+virtualcam.posx;
         virtualcam.foay=guiy+virtualcam.posy;
         virtualcam.foaz=guiz+virtualcam.posz;
      }
      r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
      lati=(float)asin(guiz/r)*360./(2.*PI);
      longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
      fl_set_positioner_xvalue(fd_teleoperatorgui->latlong,(double) longi);
      fl_set_positioner_yvalue(fd_teleoperatorgui->latlong,(double) lati);
      fl_set_slider_value(fd_teleoperatorgui->posR,(double)r);
   }
   else if ((obj==fd_teleoperatorgui->posR) ||
             (obj==fd_teleoperatorgui->latlong))
   {
      longi=2*PI*fl_get_positioner_xvalue(fd_teleoperatorgui->latlong)/360.;
      lati=2*PI*fl_get_positioner_yvalue(fd_teleoperatorgui->latlong)/360.;
      r=fl_get_slider_value(fd_teleoperatorgui->posR);
      guix=r*cos(lati)*cos(longi);
      guiy=r*cos(lati)*sin(longi);
      guiz=r*sin(lati);
      fl_set_slider_value(fd_teleoperatorgui->posX,(double)guix);
      fl_set_slider_value(fd_teleoperatorgui->posY,(double)guiy);
      fl_set_slider_value(fd_teleoperatorgui->posZ,(double)guiz);
      if (fl_get_button(fd_teleoperatorgui->selectposition)==PUSHED){
         if (toggle==TRUE){
            dx=virtualcam.foax - virtualcam.posx;
            dy=virtualcam.foay - virtualcam.posy;
            dz=virtualcam.foaz - virtualcam.posz;
            virtualcam.foax=guix+dx;
            virtualcam.foay=guiy+dy;
            virtualcam.foaz=guiz+dz;
         }
         virtualcam.posx=guix; virtualcam.posy=guiy; virtualcam.posz=guiz;
      }
      else if (fl_get_button(fd_teleoperatorgui->selectfoa)==PUSHED){
         virtualcam.foax=guix;
         virtualcam.foay=guiy;
         virtualcam.foaz=guiz;
      }
      else if (fl_get_button(fd_teleoperatorgui->selectfoaRel)==PUSHED){
         virtualcam.foax=guix+virtualcam.posx;
         virtualcam.foay=guiy+virtualcam.posy;
         virtualcam.foaz=guiz+virtualcam.posz;
      }
   }
   else if (obj == fd_teleoperatorgui->track_robot) {
      if (fl_get_button(obj)==PUSHED){
         track_robot=TRUE;
         fl_set_button(fd_teleoperatorgui->toggle,RELEASED);
         toggle=FALSE;
      }
      else track_robot=FALSE;
   }
   else if (obj == fd_teleoperatorgui->toggle){
      if (fl_get_button(obj)==PUSHED){
         toggle=TRUE;
         fl_set_button(fd_teleoperatorgui->track_robot,RELEASED);
         track_robot=FALSE;
      }
      else toggle=FALSE;
   }
   else if (obj == fd_teleoperatorgui->selectposition){
      if (fl_get_button(fd_teleoperatorgui->selectposition)==PUSHED){
         fl_set_button(fd_teleoperatorgui->selectfoa,RELEASED);
         fl_set_button(fd_teleoperatorgui->selectfoaRel,RELEASED);
         guix=virtualcam.posx; guiy=virtualcam.posy; guiz=virtualcam.posz;
         r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
         lati=(float)asin(guiz/r)*360./(2.*PI);
         longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
         fl_set_positioner_xvalue(fd_teleoperatorgui->latlong,(double) longi);
         fl_set_positioner_yvalue(fd_teleoperatorgui->latlong,(double) lati);
         fl_set_slider_value(fd_teleoperatorgui->posR,(double)r);
         fl_set_slider_value(fd_teleoperatorgui->posX,(double)guix);
         fl_set_slider_value(fd_teleoperatorgui->posY,(double)guiy);
         fl_set_slider_value(fd_teleoperatorgui->posZ,(double)guiz);
      }
   }
   else if (obj == fd_teleoperatorgui->selectfoa){
      if (fl_get_button(fd_teleoperatorgui->selectfoa)==PUSHED){
         fl_set_button(fd_teleoperatorgui->selectposition,RELEASED);
         fl_set_button(fd_teleoperatorgui->selectfoaRel,RELEASED);
         guix=virtualcam.foax;
         guiy=virtualcam.foay;
         guiz=virtualcam.foaz;
         r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
         lati=(float)asin(guiz/r)*360./(2.*PI);
         longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
         fl_set_positioner_xvalue(fd_teleoperatorgui->latlong,(double) longi);
         fl_set_positioner_yvalue(fd_teleoperatorgui->latlong,(double) lati);
         fl_set_slider_value(fd_teleoperatorgui->posR,(double)r);
         fl_set_slider_value(fd_teleoperatorgui->posX,(double)guix);
         fl_set_slider_value(fd_teleoperatorgui->posY,(double)guiy);
         fl_set_slider_value(fd_teleoperatorgui->posZ,(double)guiz);
      }
   }
   else if (obj == fd_teleoperatorgui->selectfoaRel){
      if (fl_get_button(fd_teleoperatorgui->selectfoaRel)==PUSHED){
         fl_set_button(fd_teleoperatorgui->selectposition,RELEASED);
         fl_set_button(fd_teleoperatorgui->selectfoa,RELEASED);
         guix=virtualcam.foax-virtualcam.posx;
         guiy=virtualcam.foay-virtualcam.posy;
         guiz=virtualcam.foaz-virtualcam.posz;
         r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
         lati=(float)asin(guiz/r)*360./(2.*PI);
         longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
         fl_set_positioner_xvalue(fd_teleoperatorgui->latlong,(double) longi);
         fl_set_positioner_yvalue(fd_teleoperatorgui->latlong,(double) lati);
         fl_set_slider_value(fd_teleoperatorgui->posR,(double)r);
         fl_set_slider_value(fd_teleoperatorgui->posX,(double)guix);
         fl_set_slider_value(fd_teleoperatorgui->posY,(double)guiy);
         fl_set_slider_value(fd_teleoperatorgui->posZ,(double)guiz);
      }
   }

   else if (obj == fd_teleoperatorgui->hide) {
      teleoperator_hide();
      all[teleoperator_id].guistate=off;
      /* it stops the robot when the teleoperatorGUI is disabled */
      if ((display_state & BASE_TELEOPERATOR)!=0){
         joystick_x=0.5;
         joystick_y=0.5;
      }
   }
   else if (obj == fd_teleoperatorgui->joystick) {
      if ((display_state & BASE_TELEOPERATOR)!=0) {
         if (fl_get_button(fd_teleoperatorgui->back)==RELEASED)
            joystick_y=0.5+0.5*fl_get_positioner_yvalue(fd_teleoperatorgui->joystick);
         else
            joystick_y=0.5-0.5*fl_get_positioner_yvalue(fd_teleoperatorgui->joystick);
         joystick_x=fl_get_positioner_xvalue(fd_teleoperatorgui->joystick);
      }
   }
   else if (obj == fd_teleoperatorgui->back) {
      if (fl_get_button(fd_teleoperatorgui->back)==RELEASED)
         joystick_y=0.5+0.5*fl_get_positioner_yvalue(fd_teleoperatorgui->joystick);
      else
         joystick_y=0.5-0.5*fl_get_positioner_yvalue(fd_teleoperatorgui->joystick);
      joystick_x=fl_get_positioner_xvalue(fd_teleoperatorgui->joystick);
   }
   else if (obj == fd_teleoperatorgui->stop){
      fl_set_positioner_xvalue(fd_teleoperatorgui->joystick,0.5);
      fl_set_positioner_yvalue(fd_teleoperatorgui->joystick,0.);
      joystick_x=0.5;
      joystick_y=0.5;
   }
   else if (obj == fd_teleoperatorgui->pantilt_joystick){
      if ((display_state & PANTILT_TELEOPERATOR)!=0){
	pt_joystick_y=fl_get_positioner_yvalue(fd_teleoperatorgui->pantilt_joystick);
	pt_joystick_x=fl_get_positioner_xvalue(fd_teleoperatorgui->pantilt_joystick);
      }
   }
   else if (obj == fd_teleoperatorgui->pantilt_origin){
     if ((display_state & PANTILT_TELEOPERATOR)!=0){
       if (((*longitude_max) - (*longitude_min)) > 0.05)
	 pt_joystick_x= (0.-(*longitude_min))/((*longitude_max)-(*longitude_min));
       if (((*latitude_max) - (*latitude_min)) > 0.05)
	 pt_joystick_y= (0.-(*latitude_min))/((*latitude_max)-(*latitude_min));
     }
   }
   else if (obj == fd_teleoperatorgui->pantilt_stop) {
     if ((display_state & PANTILT_TELEOPERATOR)!=0){
       /* current pantilt position as initial command, to avoid any movement */
       if (((*longitude_max) - (*longitude_min)) > 0.05)
	 pt_joystick_x= ((*pan_angle)-(*longitude_min))/((*longitude_max)-(*longitude_min));
       if (((*latitude_max) - (*latitude_min)) > 0.05)
	 pt_joystick_y= ((*tilt_angle)-(*latitude_min))/((*latitude_max)-(*latitude_min));
     }
   }
   else if (obj == fd_teleoperatorgui->ptspeed){
      speed_coef = fl_get_slider_value(fd_teleoperatorgui->ptspeed);
      if (longitude_speed!=NULL)
         *longitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
      if (latitude_speed!=NULL)
         *latitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
      if (myzoom_speed && myzoom_speed_max)
         *myzoom_speed=(1.-speed_coef)*(*myzoom_speed_max);
   }
   else if (obj == fd_teleoperatorgui->zoom){
     if ((display_state & ZOOM_TELEOPERATOR)!=0)
       pt_joystick_z = fl_get_slider_value(obj);
   }
   else if (obj == fd_teleoperatorgui->sonars){
      if (fl_get_button(fd_teleoperatorgui->sonars)==PUSHED){
         display_state = display_state | DISPLAY_SONARS;
         mysonars=(float *)myimport("sonars","us");
         sonarsrun=(runFn)myimport("sonars","run");
         sonarsstop=(stopFn)myimport("sonars","stop");
         if (sonarsrun!=NULL){
            sonarsrun(teleoperator_id,NULL,NULL);
         }
         else{
            display_state = display_state & ~DISPLAY_SONARS;
            fl_set_button(fd_teleoperatorgui->sonars, RELEASED);
         }
      }
      else {
         display_state = display_state & ~DISPLAY_SONARS;
         if (sonarsstop!=NULL)
            sonarsstop();
      }
   }
   else if (obj == fd_teleoperatorgui->laser){
      if (fl_get_button(fd_teleoperatorgui->laser)==PUSHED){
         display_state = display_state | DISPLAY_LASER;
         mylaser=(int *)myimport("laser","laser");
         laserrun=(runFn )myimport("laser","run");
         laserstop=(stopFn)myimport("laser","stop");
         if (laserrun!=NULL){
            laserrun(teleoperator_id,NULL,NULL);
         }
         else{
            fl_set_button(obj, RELEASED);
            display_state = display_state & ~DISPLAY_LASER;
         }
      }
      else {
         display_state = display_state & ~DISPLAY_LASER;
         if (laserstop!=NULL)
            laserstop();
      }
   }
   else if (obj == fd_teleoperatorgui->encoders)
   {
      if (fl_get_button(fd_teleoperatorgui->encoders)==PUSHED)
      {
         display_state = display_state | DISPLAY_ROBOT;
         myencoders=(float *)myimport("encoders","jde_robot");
         encodersrun=(runFn)myimport("encoders","run");
         encodersstop=(stopFn)myimport("encoders","stop");
         if (encodersrun!=NULL)
            encodersrun(teleoperator_id,NULL,NULL);
         else{
            fl_set_button(obj, RELEASED);
            display_state = display_state & ~DISPLAY_ROBOT;
         }
      }
      else
      {
         display_state = display_state & ~DISPLAY_ROBOT;
         if (encodersstop!=NULL)
            encodersstop();
      }
   }
   else if (obj == fd_teleoperatorgui->motors)
   {
      if (fl_get_button(fd_teleoperatorgui->motors)==PUSHED)
      {
         display_state = display_state | BASE_TELEOPERATOR;
         myv=(float *)myimport("motors","v");
         myw=(float *)myimport("motors","w");
         motorsrun=(runFn)myimport("motors","run");
         motorsstop=(stopFn)myimport("motors","stop");
         if (motorsrun!=NULL)
            motorsrun(teleoperator_id,NULL,NULL);
         else{
            fl_set_button(fd_teleoperatorgui->motors, RELEASED);
            display_state = display_state & ~BASE_TELEOPERATOR;
         }
      }
      else
      {
         /*safety stop when disabling the motors teleoperator */
         if (myv!=NULL)
            *myv=0.;
         if (myw!=NULL)
            *myw=0.;
         display_state = display_state & ~BASE_TELEOPERATOR;
         if (motorsstop!=NULL)
            motorsstop();
      }
   }
   else if (obj == fd_teleoperatorgui->PTmotors){
      if (fl_get_button(fd_teleoperatorgui->PTmotors)==PUSHED){
         mylongitude=myimport("ptmotors", "longitude");
	 longitude_max=myimport("ptmotors", "max_longitude");
	 longitude_min=myimport("ptmotors", "min_longitude");
         mylatitude=myimport ("ptmotors", "latitude");
	 latitude_max=myimport ("ptmotors", "max_latitude");
	 latitude_min=myimport ("ptmotors", "min_latitude");
         longitude_speed=myimport("ptmotors", "longitude_speed");
         latitude_speed=myimport("ptmotors","latitude_speed");
	 if((mylongitude==NULL)||(mylatitude==NULL)||
	    (longitude_max==NULL)||(longitude_min==NULL)||
	    (latitude_max==NULL)||(latitude_min==NULL)||
	    (longitude_speed==NULL)||(latitude_speed==NULL))
	   printf("Teleoperator can't import all relevant PTmotors symbols\n");
         ptmotorsrun=(runFn)myimport("ptmotors","run");
         ptmotorsstop=(stopFn)myimport("ptmotors","stop");

	 /* initial value to pt_positioner in the GUI */
	 if (((*longitude_max) - (*longitude_min)) > 0.05)
	   pt_joystick_x= (0.-(*longitude_min))/((*longitude_max)-(*longitude_min));
	 if (((*latitude_max) - (*latitude_min)) > 0.05)
	   pt_joystick_y= (0.-(*latitude_min))/((*latitude_max)-(*latitude_min));

         if (ptmotorsrun!=NULL){
            display_state = display_state | PANTILT_TELEOPERATOR;
            ptmotorsrun(teleoperator_id, NULL, NULL);
         }
         else{
            display_state = display_state & ~PANTILT_TELEOPERATOR;
            fl_set_button(obj, RELEASED);
         }
      }
      else {
         /*safety stop when disabling the teleoperator */
         /* current pantilt position as initial command, to avoid any movement *
         if (((*longitude_max) - (*longitude_min)) > 0.05){
            if (pan_angle!=NULL)
               pt_joystick_x= 1.-(*pan_angle-(*longitude_min))/((*longitude_max)-(*longitude_min));
            else
               pt_joystick_x= 1.-(0-(*longitude_min))/((*longitude_max)-(*longitude_min));
         }
         if (((*latitude_max) - (*latitude_min)) > 0.05){
            if (tilt_angle!=NULL)
               pt_joystick_y= (*tilt_angle-(*latitude_min))/((*latitude_max)-(*latitude_min));
            else
               pt_joystick_y= (0-(*latitude_min))/((*latitude_max)-(*latitude_min));
         }
	 */
         if (ptmotorsstop!=NULL){
            ptmotorsstop();
         }
         display_state = display_state & ~PANTILT_TELEOPERATOR;
      }
   }
   else if (obj == fd_teleoperatorgui->PTencoders){
      if (fl_get_button(fd_teleoperatorgui->PTencoders)==PUSHED){
         pan_angle=myimport("ptencoders", "pan_angle");
         tilt_angle=myimport("ptencoders", "tilt_angle");
         ptencodersrun=(runFn)myimport("ptencoders", "run");
         ptencodersstop=(stopFn)(stopFn)myimport("ptencoders", "stop");
         if (ptencodersrun!=NULL){
            ptencodersrun(teleoperator_id, NULL, NULL);
            display_state = display_state | DISPLAY_PANTILTENCODERS;
         }
         else{
            fl_set_button(obj, RELEASED);
            display_state = display_state & ~DISPLAY_PANTILTENCODERS;
         }
      }
      else{
         display_state = display_state & ~DISPLAY_PANTILTENCODERS;
         if (ptencodersstop!=NULL)
            ptencodersstop();
      }
   }
   else if (obj == fd_teleoperatorgui->Zmotors){
      if (fl_get_button(obj)==PUSHED){
         myzoom=myimport("zmotors", "zoom");
         myzoom_speed=myimport("zmotors", "zoom_speed");
         zmotorsrun=(runFn)myimport("zmotors","run");
         zmotorsstop=(stopFn)myimport("zmotors","stop");
         myzoom_speed_max=(float *)myimport("zmotors", "max_zoom_speed");
         myzoom_max=(float *)myimport("zmotors", "max_zoom");
         myzoom_min=(float *)myimport("zmotors", "min_zoom");

         if (zmotorsrun!=NULL || myzoom!=NULL || myzoom_speed!=NULL ||
             zmotorsstop!=NULL || myzoom_speed_max!=NULL ||
             myzoom_max!=NULL || myzoom_min!=NULL)
         {
            display_state = display_state | ZOOM_TELEOPERATOR;
            zmotorsrun(teleoperator_id, NULL, NULL);
         }
         else{
            display_state = display_state & ~ZOOM_TELEOPERATOR;
            fl_set_button(obj, RELEASED);
         }
      }
      else {
         /*safety stop when disabling the teleoperator */
         /* current pantilt position as initial command, to avoid any movement */
         if (zmotorsstop!=NULL){
            zmotorsstop();
         }
         display_state = display_state & ~ZOOM_TELEOPERATOR;
      }
   }
   else if (obj == fd_teleoperatorgui->Zencod){
      if (fl_get_button(obj)==PUSHED){
         myzoom_position=myimport("zencoders", "zoom_position");
         zencodersrun=(runFn)myimport("zencoders", "run");
         zencodersstop=(stopFn)(stopFn)myimport("zencoders", "stop");
         if (zencodersrun!=NULL){
            zencodersrun(teleoperator_id, NULL, NULL);
            display_state = display_state | DISPLAY_ZOOMENCODERS;
         }
         else{
            fl_set_button(obj, RELEASED);
            display_state = display_state & ~DISPLAY_ZOOMENCODERS;
         }
      }
      else{
         display_state = display_state & ~DISPLAY_ZOOMENCODERS;
         if (zencodersstop!=NULL)
            zencodersstop();
      }
   }
   else if (obj == fd_teleoperatorgui->colorA){
      if (fl_get_button(fd_teleoperatorgui->colorA)==PUSHED){
         mycolorA=myimport ("colorA", "colorA");
         colorArun=(runFn)myimport("colorA", "run");
         colorAstop=(stopFn)myimport("colorA", "stop");
         if (colorArun!=NULL){
            display_state = display_state | DISPLAY_COLORIMAGEA;
            colorArun(teleoperator_id, NULL, NULL);
         }
         else{
            display_state = display_state & ~DISPLAY_COLORIMAGEA;
            fl_set_button(obj, RELEASED);
         }
      }
      else {
         display_state = display_state & ~DISPLAY_COLORIMAGEA;
         if (colorAstop!=NULL)
            colorAstop();
      }
   }
   else if (obj == fd_teleoperatorgui->colorB){
      if (fl_get_button(fd_teleoperatorgui->colorB)==PUSHED){
         mycolorB=myimport ("colorB", "colorB");
         colorBrun=(runFn)myimport("colorB", "run");
         colorBstop=(stopFn)myimport("colorB", "stop");
         if (colorBrun!=NULL){
            display_state = display_state | DISPLAY_COLORIMAGEB;
            colorBrun(teleoperator_id, NULL, NULL);
         }
         else{
            display_state = display_state & ~DISPLAY_COLORIMAGEB;
            fl_set_button(obj, RELEASED);
         }
      }
      else {
         display_state = display_state & ~DISPLAY_COLORIMAGEB;
         if (colorBstop!=NULL)
            colorBstop();
      }
   }

   else if (obj == fd_teleoperatorgui->colorC){
      if (fl_get_button(fd_teleoperatorgui->colorC)==PUSHED){
         mycolorC=myimport ("colorC", "colorC");
         colorCrun=(runFn)myimport("colorC", "run");
         colorCstop=(stopFn)myimport("colorC", "stop");
         if (colorCrun!=NULL){
            display_state = display_state | DISPLAY_COLORIMAGEC;
            colorCrun(teleoperator_id, NULL, NULL);
         }
         else{
            display_state = display_state & ~DISPLAY_COLORIMAGEC;
            fl_set_button(obj, RELEASED);
         }
      }
      else {
         display_state = display_state & ~DISPLAY_COLORIMAGEC;
         if (colorCstop!=NULL)
            colorCstop();
      }
   }

  else if (obj == fd_teleoperatorgui->colorD)
    {
      if (fl_get_button(fd_teleoperatorgui->colorD)==PUSHED){
         mycolorD=myimport ("colorD", "colorD");
         colorDrun=(runFn)myimport("colorD", "run");
         colorDstop=(stopFn)myimport("colorD", "stop");
         if (colorDrun!=NULL){
            display_state = display_state | DISPLAY_COLORIMAGED;
            colorDrun(teleoperator_id, NULL, NULL);
         }
         else{
            display_state = display_state & ~DISPLAY_COLORIMAGED;
            fl_set_button(obj, RELEASED);
         }
      }
      else {
         display_state = display_state & ~DISPLAY_COLORIMAGED;
         if (colorDstop!=NULL)
            colorDstop();
      }
    } 
}

void teleoperator_guidisplay()    
{
  int i,c,row,j,k;
  Tvoxel start,end;
  float r,lati,longi,dx,dy,dz;
  float matColors[4];
  float  Xp_sensor, Yp_sensor;
  float dpan=0.5,dtilt=0.5;

  if (((display_state&DISPLAY_PANTILTENCODERS)!=0)&&
      ((display_state&PANTILT_TELEOPERATOR)!=0))
    {  
      if (pan_angle!=NULL){
	if (((*longitude_max) - (*longitude_min)) > 0.05)
	  dpan= (*pan_angle-(*longitude_min))/((*longitude_max)-(*longitude_min));
      }
      else{
	if (((*longitude_max) - (*longitude_min)) > 0.05)
	  dpan= (0-(*longitude_min))/((*longitude_max)-(*longitude_min));
      }
      fl_set_positioner_xvalue(fd_teleoperatorgui->pantilt_joystick,dpan);
      if (tilt_angle!=NULL){
	if (((*latitude_max) - (*latitude_min)) > 0.05)
	  dtilt= (*tilt_angle-(*latitude_min))/((*latitude_max)-(*latitude_min));
      }
      else{
	if (((*latitude_max) - (*latitude_min)) > 0.05)
	  dtilt= (0-(*latitude_min))/((*latitude_max)-(*latitude_min));
      }
      fl_set_positioner_yvalue(fd_teleoperatorgui->pantilt_joystick,dtilt);
      
      if (latitude_speed!=NULL)
	fl_set_slider_value(fd_teleoperatorgui->ptspeed,(double)(1.-(*latitude_speed)/MAX_SPEED_PANTILT));
    }

  fl_redraw_object(fd_teleoperatorgui->joystick);
  fl_redraw_object(fd_teleoperatorgui->pantilt_joystick);
  fl_redraw_object(fd_teleoperatorgui->latlong);

  fl_activate_glcanvas(fd_teleoperatorgui->canvas);
  /* Set the OpenGL state machine to the right context for this display */
  /* reset of the depth and color buffers */
  InitOGL(fd_teleoperatorgui->canvas, FL_ObjWin(fd_teleoperatorgui->canvas),fd_teleoperatorgui->canvas->w,fd_teleoperatorgui->canvas->h,NULL,NULL);
    
  /* Virtual camera */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  /* proyección ortográfica 
     glOrtho(-5.0,5.0,-5.0,5.0,1.0,100.0);
     glTranslatef(0,0,-5); */
  /* perspective projection. intrinsic parameters + frustrum */
  gluPerspective(45.,(GLfloat)640/(GLfloat)480,1.0,500.0);
  /* extrinsic parameters */
  gluLookAt(virtualcam.posx,virtualcam.posy,virtualcam.posz,virtualcam.foax,virtualcam.foay,virtualcam.foaz,0.,0.,1.);
  

  /** Absolute Frame of Reference **/  
  /* floor */ 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f( 0.6, 0.6, 0.6 );
  glBegin(GL_LINES);
  for(i=0;i<((int)MAXWORLD+1);i++)
    {
      v3f(-(int)MAXWORLD*10/2.+(float)i*10,-(int)MAXWORLD*10/2.,0.);
      v3f(-(int)MAXWORLD*10/2.+(float)i*10,(int)MAXWORLD*10/2.,0.);
      v3f(-(int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10,0.);
      v3f((int)MAXWORLD*10/2.,-(int)MAXWORLD*10/2.+(float)i*10,0.);
    }
  glEnd();
  
  /* absolute axis */
  glLineWidth(3.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor3f( 0.7, 0., 0. );
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 10.0, 0.0, 0.0 );
  glEnd();
  glColor3f( 0.,0.7,0. );
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.0, 10.0, 0.0 );
  glEnd();
  glColor3f( 0.,0.,0.7 );
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.0, 0.0, 10.0 );
  glEnd();
  glLineWidth(1.0f);

  
  /** Robot Frame of Reference **/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (myencoders!=NULL){
     mypioneer.posx=myencoders[0]/100.;
     mypioneer.posy=myencoders[1]/100.;
     mypioneer.posz=0.;
     mypioneer.foax=myencoders[0]/100.;
     mypioneer.foay=myencoders[1]/100.;
     mypioneer.foaz=10.;
     mypioneer.roll=myencoders[2]*RADTODEG;
  }
  else{
     mypioneer.posx=0.;
     mypioneer.posy=0.;
     mypioneer.posz=0.;
     mypioneer.foax=0.;
     mypioneer.foay=0.;
     mypioneer.foaz=10.;
     mypioneer.roll=0.;
  }
  glTranslatef(mypioneer.posx,mypioneer.posy,mypioneer.posz);
  dx=(mypioneer.foax-mypioneer.posx);
  dy=(mypioneer.foay-mypioneer.posy);
  dz=(mypioneer.foaz-mypioneer.posz);
  longi=(float)atan2(dy,dx)*360./(2.*PI);
  glRotatef(longi,0.,0.,1.);
  r=sqrt(dx*dx+dy*dy+dz*dz);
  if (r<0.00001) lati=0.;
  else lati=acos(dz/r)*360./(2.*PI);
  glRotatef(lati,0.,1.,0.);
  glRotatef(mypioneer.roll,0.,0.,1.);

  /* X axis */
  glColor3f( 1., 0., 0. );
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 5.0, 0.0, 0.0 );
  glEnd();
  /* Y axis */
  glColor3f( 0., 1., 0. );  
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.0, 5.0, 0.0 );
  glEnd();
  /* Z axis */
  glColor3f( 0., 0., 1.);
  glBegin(GL_LINES);
  v3f( 0.0, 0.0, 0.0 );   
  v3f( 0.0, 0.0, 5.0 );
  glEnd();
  /* robot body */
  if ((display_state & DISPLAY_ROBOT)!=0)
    {
      glEnable (GL_LIGHTING);
      glPushMatrix();
      glTranslatef(1.,0.,0.);
      /* the body it is not centered. With this translation we center it */  
      loadModel();
      glPopMatrix();
      glDisable (GL_LIGHTING);
    }

  /* sonars */
  if ((display_state & DISPLAY_SONARS)!=0) 
    {
      glColor3f( 0., 0.8, 0. );
      glLineWidth(2.0f);
      for (k = 0; k < NUM_SONARS; k++) {
	start.x=us_coord[k][0];
	start.y=us_coord[k][1];
        if (mysonars!=NULL)
           Xp_sensor = mysonars[k]; /* mm */
        else
           Xp_sensor = 0;
	Yp_sensor = 0.;
	/* Coordenadas del punto detectado por el US con respecto al sistema del sensor, eje x+ normal al sensor */
	end.x = us_coord[k][0] + Xp_sensor*us_coord[k][3] - Yp_sensor*us_coord[k][4];
	end.y = us_coord[k][1] + Yp_sensor*us_coord[k][3] + Xp_sensor*us_coord[k][4];
	glBegin(GL_LINES);
	glVertex3f (start.x/100., start.y/100., 2.0f);
	glVertex3f (end.x/100., end.y/100., 2.0f);
	glEnd();
      }
      glLineWidth(1.0f);
    }

  /* laser */
  if ((display_state & DISPLAY_LASER)!=0) 
    {
      glEnable (GL_LIGHTING);
      matColors[0] = 1.0;
      matColors[1] = 0.0;
      matColors[2] = 0.0;
      matColors[3] = 0.5;
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,matColors);
      /*
	glBegin(GL_POLYGON); 
	glVertex3f (laser_coord[0]*10./100., laser_coord[1]/100., 3.2);
      */
      start.x=laser_coord[0]*10.;
      start.y=laser_coord[1];
      for(k=0;k<NUM_LASER;k++) 
	{
           if (mylaser!=NULL){
              Xp_sensor = mylaser[k]*cos(((float)k-90.)*DEGTORAD);
              Yp_sensor = mylaser[k]*sin(((float)k-90.)*DEGTORAD);
           }
           else{
              Xp_sensor = 0.*cos(((float)k-90.)*DEGTORAD);
              Yp_sensor = 0.*sin(((float)k-90.)*DEGTORAD);
           }
	  /* Coordenadas del punto detectado por el US con respecto al sistema del sensor, eje x+ normal al sensor */
	  end.x = laser_coord[0]*10. + Xp_sensor*laser_coord[3] - Yp_sensor*laser_coord[4];
	  end.y = laser_coord[1] + Yp_sensor*laser_coord[3] + Xp_sensor*laser_coord[4];
	  glBegin(GL_POLYGON); 
	  glVertex3f (laser_coord[0]*10./100., laser_coord[1]/100., 3.2);
	  glVertex3f (start.x/100., start.y/100., 3.2);
	  glVertex3f (end.x/100., end.y/100., 3.2);
	  glEnd();
	  start.x=end.x;
	  start.y=end.y;
	  /* glVertex3f (end.x/100., end.y/100., 3.2);*/
	}
      /*  glVertex3f (laser_coord[0]*10./100., laser_coord[1]/100., 3.2);*/
      glDisable (GL_LIGHTING);
    }
  
  /** intercambio de buffers **/
  glXSwapBuffers(fl_display, fl_get_canvas_id(fd_teleoperatorgui->canvas));

  /* sample image display */
  {
    /* Pasa de la imagen capturada a la imagen para visualizar (sampleimage_buf), "cambiando" de formato adecuadamente */
     if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){
           if (samplesource!=NULL)
              sampleimage_buf[i]= (unsigned char)tabla[(unsigned char)(samplesource[i*3])];
           else
              sampleimage_buf[i]= (unsigned char)tabla[(unsigned char)(0)];
        }
     }
     else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){
           if (samplesource!=NULL){
              sampleimage_buf[i*2+1]=(0xf8&(samplesource[i*3+2]))+((0xe0&(samplesource[i*3+1]))>>5);
              sampleimage_buf[i*2]=((0xf8&(samplesource[i*3]))>>3)+((0x1c&(samplesource[i*3+1]))<<3);
           }
           else{
              sampleimage_buf[i*2+1]=(0xf8&(0))+((0xe0&(0))>>5);
              sampleimage_buf[i*2]=((0xf8&(0))>>3)+((0x1c&(0))<<3);
           }
        }
     }
     else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
                ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
     {
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){
           if(samplesource!=NULL){
              sampleimage_buf[i*4]=samplesource[i*3]; /* Blue Byte */
              sampleimage_buf[i*4+1]=samplesource[i*3+1]; /* Green Byte */
              sampleimage_buf[i*4+2]=samplesource[i*3+2]; /* Red Byte */
           }
           else{
              sampleimage_buf[i*4]=0; /* Blue Byte */
              sampleimage_buf[i*4+1]=0; /* Green Byte */
              sampleimage_buf[i*4+2]=0; /* Red Byte */
           }
           sampleimage_buf[i*4+3]=UCHAR_MAX; /* dummy byte */  }
     }
  }

  /* color imageA display */
  if ((display_state&DISPLAY_COLORIMAGEA)!=0){
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenA_buf),
     "cambiando" de formato adecuadamente */
     if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorA!=NULL)
              imagenA_buf[i]= (unsigned char)tabla[(unsigned char)((*mycolorA)[j*3])];
           else
              imagenA_buf[i]= (unsigned char)tabla[(unsigned char)(0)];
        }
     }
     else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){ 
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorA!=NULL){
              imagenA_buf[i*2+1]=(0xf8&((*mycolorA)[j*3+2]))+((0xe0&((*mycolorA)[j*3+1]))>>5);
              imagenA_buf[i*2]=((0xf8&((*mycolorA)[j*3]))>>3)+((0x1c&((*mycolorA)[j*3+1]))<<3);
           }
           else{
              imagenA_buf[i*2+1]=(0xf8&(0))+((0xe0&(0))>>5);
              imagenA_buf[i*2]=((0xf8&(0))>>3)+((0x1c&(0))<<3);
           }
        }
     }
     else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
                ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
     {
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorA!=NULL){
              imagenA_buf[i*4]=(*mycolorA)[j*3]; /* Blue Byte */
              imagenA_buf[i*4+1]=(*mycolorA)[j*3+1]; /* Green Byte */
              imagenA_buf[i*4+2]=(*mycolorA)[j*3+2]; /* Red Byte */
           }
           else{
              imagenA_buf[i*4]=0; /* Blue Byte */
              imagenA_buf[i*4+1]=0; /* Green Byte */
              imagenA_buf[i*4+2]=0; /* Red Byte */
           }
           imagenA_buf[i*4+3]=UCHAR_MAX; /* alpha Byte */
        }
     }
  }

  /* color imageB display */
  if ((display_state&DISPLAY_COLORIMAGEB)!=0){
     /* Pasa de la imagen capturada a la imagen para visualizar (imagenB_buf),
      "cambiando" de formato adecuadamente */
     if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorB!=NULL)
              imagenB_buf[i]= (unsigned char)tabla[(unsigned char)((*mycolorB)[j*3])];
           else
              imagenB_buf[i]= (unsigned char)tabla[(unsigned char)(0)];
        }
     }
     else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorB!=NULL){
              imagenB_buf[i*2+1]=(0xf8&((*mycolorB)[j*3+2]))+((0xe0&((*mycolorB)[j*3+1]))>>5);
              imagenB_buf[i*2]=((0xf8&((*mycolorB)[j*3]))>>3)+((0x1c&((*mycolorB)[j*3+1]))<<3);
           }
           else{
              imagenB_buf[i*2+1]=(0xf8&(0))+((0xe0&(0))>>5);
              imagenB_buf[i*2]=((0xf8&(0))>>3)+((0x1c&(0))<<3);
           }
        }
     }
     else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
                ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
     {
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorB!=NULL){
              imagenB_buf[i*4]=(*mycolorB)[j*3]; /* Blue Byte */
              imagenB_buf[i*4+1]=(*mycolorB)[j*3+1]; /* Green Byte */
              imagenB_buf[i*4+2]=(*mycolorB)[j*3+2]; /* Red Byte */
           }
           else{
              imagenB_buf[i*4]=0; /* Blue Byte */
              imagenB_buf[i*4+1]=0; /* Green Byte */
              imagenB_buf[i*4+2]=0; /* Red Byte */
           }
           imagenB_buf[i*4+3]=UCHAR_MAX; /* alpha byte */
        }
     }
  }

  /* color imageC display */
  if ((display_state&DISPLAY_COLORIMAGEC)!=0){
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenC_buf),
     "cambiando" de formato adecuadamente */
     if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorC!=NULL)
              imagenC_buf[i]= (unsigned char)tabla[(unsigned char)((*mycolorC)[j*3])];
           else{
              imagenC_buf[i]= (unsigned char)tabla[(unsigned char)(0)];
           }
        }
     }
     else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) {
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorC!=NULL){
              imagenC_buf[i*2+1]=(0xf8&((*mycolorC)[j*3+2]))+((0xe0&((*mycolorC)[j*3+1]))>>5);
              imagenC_buf[i*2]=((0xf8&((*mycolorC)[j*3]))>>3)+((0x1c&((*mycolorC)[j*3+1]))<<3);
           }
           else{
              imagenC_buf[i*2+1]=(0xf8&(0))+((0xe0&(0))>>5);
              imagenC_buf[i*2]=((0xf8&(0))>>3)+((0x1c&(0))<<3);
           }
        }
     }
     else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
                ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
     {
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           if (mycolorC!=NULL){
              imagenC_buf[i*4]=(*mycolorC)[j*3]; /* Blue Byte */
              imagenC_buf[i*4+1]=(*mycolorC)[j*3+1]; /* Green Byte */
              imagenC_buf[i*4+2]=(*mycolorC)[j*3+2]; /* Red Byte */
           }
           else{
              imagenC_buf[i*4]=0; /* Blue Byte */
              imagenC_buf[i*4+1]=0; /* Green Byte */
              imagenC_buf[i*4+2]=0; /* Red Byte */
           }
           imagenC_buf[i*4+3]=0; /* dummy byte */
        }
     }
  }

  /* color imageD display */
  if ((display_state&DISPLAY_COLORIMAGED)!=0){
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenD_buf),
     "cambiando" de formato adecuadamente */
     if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           imagenD_buf[i]= (unsigned char)tabla[(unsigned char)((*mycolorD)[j*3])];
        }
     }
     else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)){
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           imagenD_buf[i*2+1]=(0xf8&((*mycolorD)[j*3+2]))+((0xe0&((*mycolorD)[j*3+1]))>>5);
           imagenD_buf[i*2]=((0xf8&((*mycolorD)[j*3]))>>3)+((0x1c&((*mycolorD)[j*3+1]))<<3);
        }
     }
     else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
                ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
     {
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++){
           c=i%(SIFNTSC_COLUMNS/2);
           row=i/(SIFNTSC_COLUMNS/2);
           j=2*row*SIFNTSC_COLUMNS+2*c;
           imagenD_buf[i*4]=(*mycolorD)[j*3]; /* Blue Byte */
           imagenD_buf[i*4+1]=(*mycolorD)[j*3+1]; /* Green Byte */
           imagenD_buf[i*4+2]=(*mycolorD)[j*3+2]; /* Red Byte */
           imagenD_buf[i*4+3]=0; /* dummy byte */  }
     }
  }

  if ((display_state&DISPLAY_COLORIMAGEA)!=0)
    {    /* Draw *myscreen onto display */
      XPutImage(mydisplay,teleoperatorgui_win,teleoperatorgui_gc,imagenA,0,0,fd_teleoperatorgui->ventanaA->x+1, fd_teleoperatorgui->ventanaA->y+1, SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2);
    }
  
  if ((display_state&DISPLAY_COLORIMAGEB)!=0)
    {    /* Draw *myscreen onto display */
      XPutImage(mydisplay,teleoperatorgui_win,teleoperatorgui_gc,imagenB,0,0,fd_teleoperatorgui->ventanaB->x+1, fd_teleoperatorgui->ventanaB->y+1, SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2);
    }
  
  if ((display_state&DISPLAY_COLORIMAGEC)!=0)
    {    /* Draw *myscreen onto display */
      XPutImage(mydisplay,teleoperatorgui_win,teleoperatorgui_gc,imagenC,0,0,fd_teleoperatorgui->ventanaC->x+1, fd_teleoperatorgui->ventanaC->y+1, SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2);
    }
  
  if ((display_state&DISPLAY_COLORIMAGED)!=0){    /* Draw *myscreen onto display */
     XPutImage(mydisplay,teleoperatorgui_win,teleoperatorgui_gc,imagenD,0,0,
               fd_teleoperatorgui->ventanaD->x+1,
               fd_teleoperatorgui->ventanaD->y+1, SIFNTSC_COLUMNS/2,
               SIFNTSC_ROWS/2);
  }

  /* Draw sample image onto display */
    XPutImage(mydisplay,teleoperatorgui_win,teleoperatorgui_gc,sampleimage,0,0,fd_teleoperatorgui->sampleimage->x+1, fd_teleoperatorgui->sampleimage->y+1, SIFNTSC_COLUMNS, SIFNTSC_ROWS);

}


void teleoperator_hide_aux(void)
{
  if ((display_state & BASE_TELEOPERATOR)!=0)
    {
      (*myv)=0; 
      (*myw)=0;
    }
  /* to make a safety stop when the robot is being teleoperated from GUI */
  mydelete_buttonscallback(teleoperator_guibuttons);
  mydelete_displaycallback(teleoperator_guidisplay);
  fl_hide_form(fd_teleoperatorgui->teleoperatorgui);
}

void teleoperator_hide(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)teleoperator_hide_aux);
      }
   }
   else{
      fn ((gui_function)teleoperator_hide_aux);
   }
}

void teleoperator_show_aux(void)
{
  static int k=0;
  float r,lati,longi,guix,guiy,guiz;

  if (k==0) /* not initialized */
    {
      k++;
      fd_teleoperatorgui = create_form_teleoperatorgui();
      fl_set_form_position(fd_teleoperatorgui->teleoperatorgui,400,50);
      fl_show_form(fd_teleoperatorgui->teleoperatorgui,FL_PLACE_POSITION,FL_FULLBORDER,"teleoperator");
      image_displaysetup(); /* Tiene que ir despues de la inicializacion de Forms, pues hace uso de informacion que la libreria rellena en tiempo de ejecucion al iniciarse */
      fl_add_canvas_handler(fd_teleoperatorgui->canvas,Expose,InitOGL,0);
      
    }
  else 
    {
      fl_show_form(fd_teleoperatorgui->teleoperatorgui,FL_PLACE_POSITION,FL_FULLBORDER,"teleoperator");
    }

  fl_set_slider_bounds(fd_teleoperatorgui->posX,MAXWORLD*10,-MAXWORLD*10);
  fl_set_slider_bounds(fd_teleoperatorgui->posY,MAXWORLD*10,-MAXWORLD*10);
  fl_set_slider_bounds(fd_teleoperatorgui->posZ,MAXWORLD*10,-MAXWORLD*10);
  fl_set_positioner_xbounds(fd_teleoperatorgui->latlong,-180,180.);
  fl_set_positioner_ybounds(fd_teleoperatorgui->latlong,-89.99,89.99); 
  fl_set_slider_bounds(fd_teleoperatorgui->posR,MAXWORLD*10*2,0.);
  fl_set_button(fd_teleoperatorgui->selectfoaRel,RELEASED);
  fl_set_button(fd_teleoperatorgui->selectfoa,RELEASED);
  fl_set_button(fd_teleoperatorgui->selectposition,PUSHED);
  guix=(float) virtualcam.posx;
  guiy=(float) virtualcam.posy;
  guiz=(float) virtualcam.posz;
  r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
  lati=(float)asin(guiz/r)*360./(2.*PI);
  longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
  fl_set_slider_value(fd_teleoperatorgui->posX,virtualcam.posx);
  fl_set_slider_value(fd_teleoperatorgui->posY,virtualcam.posy);
  fl_set_slider_value(fd_teleoperatorgui->posZ,virtualcam.posz);
  fl_set_positioner_xvalue(fd_teleoperatorgui->latlong,(double) longi);
  fl_set_positioner_yvalue(fd_teleoperatorgui->latlong,(double) lati);
  fl_set_slider_value(fd_teleoperatorgui->posR,(double)r);
  if (track_robot==TRUE) fl_set_button(fd_teleoperatorgui->track_robot,PUSHED);
  else fl_set_button(fd_teleoperatorgui->track_robot,RELEASED);
  if (toggle==TRUE) fl_set_button(fd_teleoperatorgui->toggle,PUSHED);
  else fl_set_button(fd_teleoperatorgui->toggle,RELEASED);
  teleoperatorgui_win = FL_ObjWin(fd_teleoperatorgui->ventanaA);
  /* the window (teleoperatorgui_win) changes every time the form is hided and showed again. They need to be updated before displaying anything again */
  
  fl_set_positioner_xvalue(fd_teleoperatorgui->joystick,0.5);
  fl_set_positioner_yvalue(fd_teleoperatorgui->joystick,0.);
  joystick_x=0.5;
  joystick_y=0.5;

  if (myregister_buttonscallback!=NULL){
     myregister_buttonscallback(teleoperator_guibuttons);
     myregister_displaycallback(teleoperator_guidisplay);
  }
  else{
     fprintf (stderr, "teleoperator: myregister_buttonscallback not fetched\n");
     jdeshutdown(1);
  }
}

void teleoperator_show(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)teleoperator_show_aux);
      }
   }
   else{
      fn ((gui_function)teleoperator_show_aux);
   }
}

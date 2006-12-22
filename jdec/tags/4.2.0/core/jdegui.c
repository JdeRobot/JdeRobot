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

#include "jde.h"
#include "jdegui.h"
#include "mastergui.h"
#include "sensorsmotorsgui.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#define v3f glVertex3f

#include <GL/gl.h>              
#include <GL/glx.h>
#include <GL/glu.h>

#include <forms.h>
#include <glcanvas.h>
#include "pioneer.h"

#define DISPLAY_ROBOT 0x01UL
#define DISPLAY_PANTILTENCODERS 0x20UL
#define DISPLAY_SONARS 0x04UL
#define DISPLAY_LASER 0x08UL
#define DISPLAY_COLORIMAGEA 0x10UL
#define DISPLAY_COLORIMAGEB 0x20UL
#define DISPLAY_COLORIMAGEC 0x40UL
#define DISPLAY_COLORIMAGED 0x80UL
#define BASE_TELEOPERATOR 0x100UL
#define PANTILT_TELEOPERATOR 0x200UL

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

SofReference mypioneer,virtualcam;
/* robot and virtual camera in the absolute FrameOfReference */
static int track_robot=TRUE;
static int toggle=FALSE;

/* jdegui has the same schema API, but it is not an schema, and so it is not stored at all[]. It is just a service thread. */
pthread_mutex_t jdegui_mymutex;
pthread_cond_t jdegui_condition;
pthread_t jdegui_mythread;
int jdegui_debug=0;
int jdegui_state;

guibuttons buttonscallbacks[MAX_SCHEMAS];
int num_buttonscallbacks=0;
int register_buttonscallback(guibuttons f)
{
  int i;
  int found=0;
  if (f!=NULL) 
    {
      for(i=0;i<num_buttonscallbacks;i++)
	if (buttonscallbacks[i]==NULL) 
	  {
	    buttonscallbacks[i]=f;
	    found=1;
	    break;
	  }
      if ((found==0)&&(num_buttonscallbacks<MAX_SCHEMAS)) 
	{
	  buttonscallbacks[num_buttonscallbacks]=f;
	  num_buttonscallbacks++;
	}
      else if ((found==0)&&(num_buttonscallbacks>=MAX_SCHEMAS))
	printf("Warning no space for registering a buttonscallback\n");
    }
  return 1;
}

int delete_buttonscallback(guibuttons f)
{
  int i;
  if (f!=NULL) 
    {
      for(i=0;i<num_buttonscallbacks;i++)
	if (buttonscallbacks[i]==f) 
	  {
	    buttonscallbacks[i]=NULL;
	    break;
	  }
    }
  return 1;
}


guidisplay displaycallbacks[MAX_SCHEMAS];
int num_displaycallbacks=0;
int register_displaycallback(guidisplay f)
{
  int i;
  int found=0;
  if (f!=NULL) 
    {
      for(i=0;i<num_displaycallbacks;i++)
	if (displaycallbacks[i]==NULL) 
	  {
	    displaycallbacks[i]=f;
	    found=1;
	    break;
	  }
      if ((found==0)&&(num_displaycallbacks<MAX_SCHEMAS)) 
	{
	  displaycallbacks[num_displaycallbacks]=f;
	  num_displaycallbacks++;
	}
      else if ((found==0)&&(num_displaycallbacks>=MAX_SCHEMAS))
	printf("Warning no space for registering a displaycallback\n");
    }
  return 1;
}

int delete_displaycallback(guidisplay f)
{
  int i;
  if (f!=NULL) 
    {
      for(i=0;i<num_displaycallbacks;i++)
	if (displaycallbacks[i]==f) 
	  {
	    displaycallbacks[i]=NULL;
	    break;
	  }
    }
  return 1;
}

/* GUI entries for dynamically loaded schemas */ 
FL_OBJECT *vis[MAX_LOADEDSCHEMAS];
FL_OBJECT *act[MAX_LOADEDSCHEMAS];
FL_OBJECT *fps[MAX_LOADEDSCHEMAS];
int associated_ID[MAX_LOADEDSCHEMAS];

float fpsgui=0;
int kgui=0;
int jdegui_cycle=70; /* ms */
unsigned long display_state;

#define joystick_maxRotVel 30 /* deg/sec */
#define joystick_maxTranVel 500 /* mm/sec */

float joystick_x, joystick_y;
float pt_joystick_x, pt_joystick_y;
int back=0;

#define PUSHED 1
#define RELEASED 0

/* Necesarias para las Xlib, para todos los guis */
Display* display;
int screen;

/* necesarias para los sensorsmotorsgui y mastergui */
int mastergui_on=FALSE;
int mastergui_request=FALSE;

int sensorsmotorsgui_on=FALSE;
int sensorsmotorsgui_request=FALSE;

FD_mastergui *fd_mastergui;
Window  hierarchy_win;
FD_sensorsmotorsgui *fd_sensorsmotorsgui;
GC sensorsmotorsgui_gc;
Window  sensorsmotorsgui_win; 

char *samplesource;
int vmode;
XImage *imagenA,*imagenB,*imagenC,*imagenD,*sampleimage; 
char *imagenA_buf, *imagenB_buf, *imagenC_buf, *imagenD_buf, *sampleimage_buf; /* puntero a memoria para la imagen a visualizar en el servidor X. No compartida con el servidor */
long int tabla[256]; 
/* tabla con la traduccion de niveles de gris a numero de pixel en Pseudocolor-8bpp. Cada numero de pixel apunta al valor adecuado del ColorMap, con el color adecuado instalado */
int pixel8bpp_rojo, pixel8bpp_blanco, pixel8bpp_amarillo;



/* to display the hierarchy */
int state_dpy[MAX_SCHEMAS];
int sizeY=20;
int sizeX=20;
int iteracion_display=0;
#define FORCED_REFRESH 5000 /* ms */ 
/*Every forced_refresh the hierarchy is drawn from scratch.*/




int InitOGL(FL_OBJECT *ob, Window win,int w,int h, XEvent *xev, void *ud)
{
  /*  printf("capullo %d %d\n",w,h);*/

  /* Inicializa OpenGL con los parametros que diran como se visualiza. */
   GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat position[] = {0.0, 3.0, 3.0, 0.0};
   GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat local_view[] = {0.0};

  glViewport(0,0,(GLint)w,(GLint)h);
  /*  glDrawBuffer(GL_FRONT);*/
  glDrawBuffer(GL_BACK);
  glClearColor(0.6f, 0.8f, 1.0f, 0.0f);    
  /* This Will Clear The Background Color To Black */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearDepth(1.0);                           /* Enables Clearing Of The Depth Buffer */

 
 /* Enable Texture Mapping */
  glEnable(GL_TEXTURE_2D);    
  glLightfv (GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv (GL_LIGHT0, GL_POSITION, position);
  glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv (GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
 
  /* With this, the pioneer appears correctly, but the cubes don't */
  glEnable (GL_LIGHTING);
  glEnable (GL_LIGHT0);
  glEnable (GL_AUTO_NORMAL);
  glEnable (GL_NORMALIZE);  
  glEnable(GL_DEPTH_TEST);                     /* Enables Depth Testing */
  glDepthFunc(GL_LESS);  
  glShadeModel(GL_SMOOTH);                     /* Enables Smooth Color Shading */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return 0;
}


int image_displaysetup() 
/* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/ 
{
    XGCValues gc_values;
    XWindowAttributes win_attributes;
    XColor nuevocolor;
    int pixelNum, numCols;
    int allocated_colors=0, non_allocated_colors=0;
   
    sensorsmotorsgui_win= FL_ObjWin(fd_sensorsmotorsgui->ventanaA);
    XGetWindowAttributes(display, sensorsmotorsgui_win, &win_attributes);  
    XMapWindow(display, sensorsmotorsgui_win);
    /*XSelectInput(display, sensorsmotorsgui_win, ButtonPress|StructureNotifyMask);*/   
    gc_values.graphics_exposures = False;
    sensorsmotorsgui_gc = XCreateGC(display, sensorsmotorsgui_win, GCGraphicsExposures, &gc_values);  
    
    /* Utilizan el Visual (=estructura de color) y el colormap con que este operando el programa principal con su Xforms. No crea un nuevo colormap, sino que modifica el que se estaba usando a traves de funciones de Xforms*/
    vmode= fl_get_vclass();
    if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
      {printf("jdegui: truecolor 16 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenA = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenB = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenC = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2/4);    
      imagenD = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      return win_attributes.depth;
      sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2);    
      sampleimage = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      }
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24)) 
      { printf("jdegui: truecolor 24 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenA = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenB = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenC = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenD = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      sampleimage = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      return win_attributes.depth;
      }
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32)) 
      { printf("jdegui: truecolor 24 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenA = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenB = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenC = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4/4); 
      imagenD = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
      sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      sampleimage = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
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
	    
	    /*if (XAllocColor(display,DefaultColormap(display,screen),&nuevocolor)==False) tabla[pixelNum]=tabla[pixelNum-1];*/
	    if (XAllocColor(display,fl_state[vmode].colormap,&nuevocolor)==False) {tabla[pixelNum]=tabla[pixelNum-1]; non_allocated_colors++;}
	    else {tabla[pixelNum]=nuevocolor.pixel;allocated_colors++;}
	  }
	printf("jdegui: depth= %d\n", fl_state[vmode].depth); 
	printf("jdegui: colormap got %d colors, %d non_allocated colors\n",allocated_colors,non_allocated_colors);

	imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenA = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenB = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenC = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS/4);    
	imagenD = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2,8,0);
	sampleimage_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS);    
	sampleimage = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,sampleimage_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);

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


int freeobj_ventanaA_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGEA)!=0))
    samplesource=colorA;
  return 0;
}

int freeobj_ventanaB_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGEB)!=0))
    samplesource=colorB;
  return 0;
}

int freeobj_ventanaC_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGEC)!=0)) 
    samplesource=colorC;
  return 0;
}

int freeobj_ventanaD_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  if ((event==FL_PUSH)&&(key==MOUSELEFT)&&
      ((display_state&DISPLAY_COLORIMAGED)!=0))
    samplesource=colorD;
  return 0;
}

void sensorsmotorsgui_suspend(void)
{
  if (sensorsmotorsgui_on==TRUE)
    {
      if (jdegui_debug) printf("sensorsmotorsgui suspend\n");
      sensorsmotorsgui_request=FALSE; 
    }
}

void sensorsmotorsgui_resume(void)
{
  if (sensorsmotorsgui_on==FALSE)
    {
      /* printf("sensorsmotorsgui resume\n"); */
      sensorsmotorsgui_request=TRUE;
    }
}


void sensorsmotorsgui_buttons(FL_OBJECT *obj) 
{
  float dpan=0.5,dtilt=0.5, speed_coef;
  float r,lati,longi,guix,guiy,guiz;
  float dx,dy,dz;


  if (track_robot==TRUE)
    {  
      virtualcam.foax=jde_robot[0]/100.;
      virtualcam.foay=jde_robot[1]/100.;
      virtualcam.foaz=0.;
      if ((fl_get_button(fd_sensorsmotorsgui->selectfoa)==PUSHED) ||
	  (fl_get_button(fd_sensorsmotorsgui->selectfoaRel)==PUSHED))
	{
	  if (fl_get_button(fd_sensorsmotorsgui->selectfoa)==PUSHED)
	    {
	      guix=virtualcam.foax; 
	      guiy=virtualcam.foay; 
	      guiz=virtualcam.foaz;
	    }
	  else if (fl_get_button(fd_sensorsmotorsgui->selectfoaRel)==PUSHED)
	    {
	      guix=virtualcam.foax-virtualcam.posx;
	      guiy=virtualcam.foay-virtualcam.posy;
	      guiz=virtualcam.foaz-virtualcam.posz;
	    }
	  fl_set_slider_value(fd_sensorsmotorsgui->posX,(double)guix);
	  fl_set_slider_value(fd_sensorsmotorsgui->posY,(double)guiy);
	  fl_set_slider_value(fd_sensorsmotorsgui->posZ,(double)guiz);
	  r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
	  lati=(float)asin(guiz/r)*360./(2.*PI);
	  longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);      
	  fl_set_positioner_xvalue(fd_sensorsmotorsgui->latlong,(double) longi);
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->latlong,(double) lati);
	  fl_set_slider_value(fd_sensorsmotorsgui->posR,(double)r);  
	}
    }

  if (obj==fd_sensorsmotorsgui->posOrigin) 
    { 
      guix=0.; guiy=0.; guiz=0.;
      fl_set_slider_value(fd_sensorsmotorsgui->posX,(double)guix);
      fl_set_slider_value(fd_sensorsmotorsgui->posY,(double)guiy);
      fl_set_slider_value(fd_sensorsmotorsgui->posZ,(double)guiz);
      fl_set_positioner_xvalue(fd_sensorsmotorsgui->latlong,(double)0.);
      fl_set_positioner_yvalue(fd_sensorsmotorsgui->latlong,(double)90.);
      fl_set_slider_value(fd_sensorsmotorsgui->posR,(double)0.); 
      if (fl_get_button(fd_sensorsmotorsgui->selectposition)==PUSHED)
	{
	  if (toggle==TRUE)
	    {
	      dx=virtualcam.foax - virtualcam.posx;
	      dy=virtualcam.foay - virtualcam.posy;
	      dz=virtualcam.foaz - virtualcam.posz;
	      virtualcam.foax=guix+dx;
	      virtualcam.foay=guiy+dy;
	      virtualcam.foaz=guiz+dz;
	    }
	  virtualcam.posx=guix; virtualcam.posy=guiy; virtualcam.posz=guiz;
	} 
      else if (fl_get_button(fd_sensorsmotorsgui->selectfoa)==PUSHED)
	{
	  virtualcam.foax=guix; 
	  virtualcam.foay=guiy; 
	  virtualcam.foaz=guiz;
	}
      else if (fl_get_button(fd_sensorsmotorsgui->selectfoaRel)==PUSHED)
	{
	  virtualcam.foax=virtualcam.posx+guix; 
	  virtualcam.foay=virtualcam.posy+guiy; 
	  virtualcam.foaz=virtualcam.posz+guiz;
	}
    }
  else if ((obj==fd_sensorsmotorsgui->posX) ||
	   (obj==fd_sensorsmotorsgui->posY) || 
	   (obj==fd_sensorsmotorsgui->posZ))
    {
      guix=(float) fl_get_slider_value(fd_sensorsmotorsgui->posX);
      guiy=(float) fl_get_slider_value(fd_sensorsmotorsgui->posY);
      guiz=(float) fl_get_slider_value(fd_sensorsmotorsgui->posZ);
      if (fl_get_button(fd_sensorsmotorsgui->selectposition)==PUSHED)
	{
	  if (toggle==TRUE)
	    {
	      dx=virtualcam.foax - virtualcam.posx;
	      dy=virtualcam.foay - virtualcam.posy;
	      dz=virtualcam.foaz - virtualcam.posz;
	      virtualcam.foax=guix+dx;
	      virtualcam.foay=guiy+dy;
	      virtualcam.foaz=guiz+dz;
	    }
	  virtualcam.posx=guix; virtualcam.posy=guiy; virtualcam.posz=guiz;
	} 
      else if (fl_get_button(fd_sensorsmotorsgui->selectfoa)==PUSHED)
	{
	  virtualcam.foax=guix; 
	  virtualcam.foay=guiy; 
	  virtualcam.foaz=guiz;
	}
      else if (fl_get_button(fd_sensorsmotorsgui->selectfoaRel)==PUSHED)
	{
	  virtualcam.foax=guix+virtualcam.posx;
	  virtualcam.foay=guiy+virtualcam.posy;
	  virtualcam.foaz=guiz+virtualcam.posz;
	}
      r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
      lati=(float)asin(guiz/r)*360./(2.*PI);
      longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);      
      fl_set_positioner_xvalue(fd_sensorsmotorsgui->latlong,(double) longi);
      fl_set_positioner_yvalue(fd_sensorsmotorsgui->latlong,(double) lati);
      fl_set_slider_value(fd_sensorsmotorsgui->posR,(double)r);  
    }
  else if ((obj==fd_sensorsmotorsgui->posR) ||
	   (obj==fd_sensorsmotorsgui->latlong))
    {
      longi=2*PI*fl_get_positioner_xvalue(fd_sensorsmotorsgui->latlong)/360.;
      lati=2*PI*fl_get_positioner_yvalue(fd_sensorsmotorsgui->latlong)/360.;
      r=fl_get_slider_value(fd_sensorsmotorsgui->posR);
      guix=r*cos(lati)*cos(longi);
      guiy=r*cos(lati)*sin(longi);
      guiz=r*sin(lati);
      fl_set_slider_value(fd_sensorsmotorsgui->posX,(double)guix);
      fl_set_slider_value(fd_sensorsmotorsgui->posY,(double)guiy);
      fl_set_slider_value(fd_sensorsmotorsgui->posZ,(double)guiz);
      if (fl_get_button(fd_sensorsmotorsgui->selectposition)==PUSHED)
	{
	  if (toggle==TRUE)
	    {
	      dx=virtualcam.foax - virtualcam.posx;
	      dy=virtualcam.foay - virtualcam.posy;
	      dz=virtualcam.foaz - virtualcam.posz;
	      virtualcam.foax=guix+dx;
	      virtualcam.foay=guiy+dy;
	      virtualcam.foaz=guiz+dz;
	    }
	  virtualcam.posx=guix; virtualcam.posy=guiy; virtualcam.posz=guiz;
	} 
      else if (fl_get_button(fd_sensorsmotorsgui->selectfoa)==PUSHED)
	{
	  virtualcam.foax=guix; 
	  virtualcam.foay=guiy; 
	  virtualcam.foaz=guiz;
	}
      else if (fl_get_button(fd_sensorsmotorsgui->selectfoaRel)==PUSHED)
	{
	  virtualcam.foax=guix+virtualcam.posx;
	  virtualcam.foay=guiy+virtualcam.posy;
	  virtualcam.foaz=guiz+virtualcam.posz;
	}
    }
  else if (obj == fd_sensorsmotorsgui->track_robot) 
    {if (fl_get_button(obj)==PUSHED) 
      {
	track_robot=TRUE;
	fl_set_button(fd_sensorsmotorsgui->toggle,RELEASED);
	toggle=FALSE;
      }
    else track_robot=FALSE;
    } 
  else if (obj == fd_sensorsmotorsgui->toggle) 
    {if (fl_get_button(obj)==PUSHED) 
      {
	toggle=TRUE;
	fl_set_button(fd_sensorsmotorsgui->track_robot,RELEASED);
	track_robot=FALSE;
      }
    else toggle=FALSE;
    } 
  else if (obj == fd_sensorsmotorsgui->selectposition)
    {
      if (fl_get_button(fd_sensorsmotorsgui->selectposition)==PUSHED)
	{
	  fl_set_button(fd_sensorsmotorsgui->selectfoa,RELEASED);
	  fl_set_button(fd_sensorsmotorsgui->selectfoaRel,RELEASED);
	  guix=virtualcam.posx; guiy=virtualcam.posy; guiz=virtualcam.posz;	  
	  r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
	  lati=(float)asin(guiz/r)*360./(2.*PI);
	  longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
	  fl_set_positioner_xvalue(fd_sensorsmotorsgui->latlong,(double) longi);
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->latlong,(double) lati);
	  fl_set_slider_value(fd_sensorsmotorsgui->posR,(double)r); 
	  fl_set_slider_value(fd_sensorsmotorsgui->posX,(double)guix);
	  fl_set_slider_value(fd_sensorsmotorsgui->posY,(double)guiy);
	  fl_set_slider_value(fd_sensorsmotorsgui->posZ,(double)guiz);
	}
    }
  else if (obj == fd_sensorsmotorsgui->selectfoa)
    {
      if (fl_get_button(fd_sensorsmotorsgui->selectfoa)==PUSHED)
	{
	  fl_set_button(fd_sensorsmotorsgui->selectposition,RELEASED);
	  fl_set_button(fd_sensorsmotorsgui->selectfoaRel,RELEASED);
	  guix=virtualcam.foax; 
	  guiy=virtualcam.foay; 
	  guiz=virtualcam.foaz;
	  r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
	  lati=(float)asin(guiz/r)*360./(2.*PI);
	  longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
	  fl_set_positioner_xvalue(fd_sensorsmotorsgui->latlong,(double) longi);
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->latlong,(double) lati);
	  fl_set_slider_value(fd_sensorsmotorsgui->posR,(double)r); 
	  fl_set_slider_value(fd_sensorsmotorsgui->posX,(double)guix);
	  fl_set_slider_value(fd_sensorsmotorsgui->posY,(double)guiy);
	  fl_set_slider_value(fd_sensorsmotorsgui->posZ,(double)guiz);
	}
    }
  else if (obj == fd_sensorsmotorsgui->selectfoaRel)
    {
      if (fl_get_button(fd_sensorsmotorsgui->selectfoaRel)==PUSHED)
	{
	  fl_set_button(fd_sensorsmotorsgui->selectposition,RELEASED);
	  fl_set_button(fd_sensorsmotorsgui->selectfoa,RELEASED);
	  guix=virtualcam.foax-virtualcam.posx; 
	  guiy=virtualcam.foay-virtualcam.posy; 
	  guiz=virtualcam.foaz-virtualcam.posz;
	  r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
	  lati=(float)asin(guiz/r)*360./(2.*PI);
	  longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
	  fl_set_positioner_xvalue(fd_sensorsmotorsgui->latlong,(double) longi);
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->latlong,(double) lati);
	  fl_set_slider_value(fd_sensorsmotorsgui->posR,(double)r); 
	  fl_set_slider_value(fd_sensorsmotorsgui->posX,(double)guix);
	  fl_set_slider_value(fd_sensorsmotorsgui->posY,(double)guiy);
	  fl_set_slider_value(fd_sensorsmotorsgui->posZ,(double)guiz);
	}
    }

  else if (obj == fd_sensorsmotorsgui->hide) 
    {
      display_state = display_state & ~DISPLAY_LASER;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->vislaser,RELEASED);
      display_state = display_state & ~DISPLAY_SONARS;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->vissonars,RELEASED);
      display_state = display_state & ~DISPLAY_ROBOT;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->visrobot,RELEASED);
      display_state = display_state & ~DISPLAY_COLORIMAGEA;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->viscolorimageA,RELEASED);
      display_state = display_state & ~DISPLAY_COLORIMAGEB;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->viscolorimageB,RELEASED);
      display_state = display_state & ~DISPLAY_COLORIMAGEC;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->viscolorimageC,RELEASED);
      display_state = display_state & ~DISPLAY_COLORIMAGED;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->viscolorimageD,RELEASED);
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->vispantiltencoders,RELEASED);      
      display_state = display_state & ~BASE_TELEOPERATOR;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->vismotors,RELEASED);
      display_state = display_state & ~PANTILT_TELEOPERATOR;
      if (mastergui_on==TRUE) fl_set_button(fd_mastergui->vispantiltmotors,RELEASED);
      sensorsmotorsgui_suspend();
    }  
  else if (obj == fd_sensorsmotorsgui->joystick) 
    {
      if ((display_state & BASE_TELEOPERATOR)!=0) 
	{
	  if (fl_get_button(fd_sensorsmotorsgui->back)==RELEASED)
	    joystick_y=0.5+0.5*fl_get_positioner_yvalue(fd_sensorsmotorsgui->joystick);
	  else 
	    joystick_y=0.5-0.5*fl_get_positioner_yvalue(fd_sensorsmotorsgui->joystick);
	  joystick_x=fl_get_positioner_xvalue(fd_sensorsmotorsgui->joystick);
	  fl_redraw_object(fd_sensorsmotorsgui->joystick);
	}
    }    
   else if (obj == fd_sensorsmotorsgui->back) 
    {
      if (fl_get_button(fd_sensorsmotorsgui->back)==RELEASED)
	joystick_y=0.5+0.5*fl_get_positioner_yvalue(fd_sensorsmotorsgui->joystick);
      else 
	joystick_y=0.5-0.5*fl_get_positioner_yvalue(fd_sensorsmotorsgui->joystick);
      joystick_x=fl_get_positioner_xvalue(fd_sensorsmotorsgui->joystick);
      fl_redraw_object(fd_sensorsmotorsgui->joystick);
    }    
  else if (obj == fd_sensorsmotorsgui->stop) 
    {
      fl_set_positioner_xvalue(fd_sensorsmotorsgui->joystick,0.5);
      fl_set_positioner_yvalue(fd_sensorsmotorsgui->joystick,0.);
      joystick_x=0.5;
      joystick_y=0.5;
    }     
  else if (obj == fd_sensorsmotorsgui->pantilt_joystick) 
    {
      if ((display_state & PANTILT_TELEOPERATOR)!=0) 
	{pt_joystick_y=fl_get_positioner_yvalue(fd_sensorsmotorsgui->pantilt_joystick);
	pt_joystick_x=fl_get_positioner_xvalue(fd_sensorsmotorsgui->pantilt_joystick);
	/*  fl_redraw_object(fd_sensorsmotorsgui->pantilt_joystick);*/
	}
    }    
  else if (obj == fd_sensorsmotorsgui->pantilt_origin) 
    {
      if ((MAX_PAN_ANGLE - MIN_PAN_ANGLE) > 0.05) 
	pt_joystick_x= 1.-(0.-MIN_PAN_ANGLE)/(MAX_PAN_ANGLE-MIN_PAN_ANGLE);
      if ((MAX_TILT_ANGLE - MIN_TILT_ANGLE) > 0.05) 
	pt_joystick_y= (0.-MIN_TILT_ANGLE)/(MAX_TILT_ANGLE-MIN_TILT_ANGLE);   
    }     
  else if (obj == fd_sensorsmotorsgui->pantilt_stop) 
    {
      /* current pantilt position as initial command, to avoid any movement */
      if ((MAX_PAN_ANGLE - MIN_PAN_ANGLE) > 0.05) 
	pt_joystick_x= 1.-(pan_angle-MIN_PAN_ANGLE)/(MAX_PAN_ANGLE-MIN_PAN_ANGLE);
      if ((MAX_TILT_ANGLE - MIN_TILT_ANGLE) > 0.05) 
	pt_joystick_y= (tilt_angle-MIN_TILT_ANGLE)/(MAX_TILT_ANGLE-MIN_TILT_ANGLE);   
    }
  else if (obj == fd_sensorsmotorsgui->ptspeed)
    {
      speed_coef = fl_get_slider_value(fd_sensorsmotorsgui->ptspeed);
      longitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
      latitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
    }
  
  
  /* modifies pantilt positioner to follow pantilt angles. 
     It tracks the pantilt movement. It should be at
     display_poll, but there it causes a weird display behavior. */
  if ((MAX_PAN_ANGLE - MIN_PAN_ANGLE) > 0.05) 
    dpan= 1.-(pan_angle-MIN_PAN_ANGLE)/(MAX_PAN_ANGLE-MIN_PAN_ANGLE);
  if ((MAX_TILT_ANGLE - MIN_TILT_ANGLE) > 0.05) 
    dtilt= (tilt_angle-MIN_TILT_ANGLE)/(MAX_TILT_ANGLE-MIN_TILT_ANGLE);   
  fl_set_positioner_xvalue(fd_sensorsmotorsgui->pantilt_joystick,dpan);
  fl_set_positioner_yvalue(fd_sensorsmotorsgui->pantilt_joystick,dtilt);
  /*fl_redraw_object(fd_mastergui->pantilt_joystick);*/

}


void sensorsmotorsgui_display() 
     /* Siempre hay una estructura grafica con todo lo que debe estar en pantalla. Permite el pintado incremental para los buffers de puntos, borran el incremento viejo y pintan solo el nuevo */
{
  int i,c,row,j,k;
  Tvoxel start,end;
  float r,lati,longi,dx,dy,dz;
  float matColors[4];
  float  Xp_sensor, Yp_sensor;

  /* resetea el buffer de color y el de profundidad */ 
  /*  glDrawBuffer(GL_BACK);*/
  /*  glClearColor(0.9,0.9,0.9,0.0);  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /** Virtual camera **/
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); 
  /* proyección perspectiva */
  /*gluPerspective(45.,(GLfloat)w/(GLfloat)h,1.0,100.0);
    glTranslatef(0,0,-15);*/
  /* proyección ortográfica 
     glOrtho(-5.0,5.0,-5.0,5.0,1.0,100.0);
     glTranslatef(0,0,-5);
  */
  /* intrinsic parameters + frustrum */
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
  mypioneer.posx=jde_robot[0]/100.;
  mypioneer.posy=jde_robot[1]/100.;
  mypioneer.posz=0.;
  mypioneer.foax=jde_robot[0]/100.;
  mypioneer.foay=jde_robot[1]/100.;
  mypioneer.foaz=10.;
  mypioneer.roll=jde_robot[2]*RADTODEG;
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
	Xp_sensor = us[k]; /* mm */
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
	  Xp_sensor = jde_laser[k]*cos(((float)k-90.)*DEGTORAD); 
	  Yp_sensor = jde_laser[k]*sin(((float)k-90.)*DEGTORAD);
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
  glXSwapBuffers(fl_display, fl_get_canvas_id(fd_sensorsmotorsgui->canvas));

  /* sample image display */
  {
    /* Pasa de la imagen capturada a la imagen para visualizar (sampleimage_buf), "cambiando" de formato adecuadamente */
    if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
      {for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	{ sampleimage_buf[i]= (unsigned char)tabla[(unsigned char)(samplesource[i*3])];}}
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
      {
	for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
	  { sampleimage_buf[i*2+1]=(0xf8&(samplesource[i*3+2]))+((0xe0&(samplesource[i*3+1]))>>5);
	  sampleimage_buf[i*2]=((0xf8&(samplesource[i*3]))>>3)+((0x1c&(samplesource[i*3+1]))<<3);
	  }
      }
    else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	     ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
      {for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	{ sampleimage_buf[i*4]=samplesource[i*3]; /* Blue Byte */
	sampleimage_buf[i*4+1]=samplesource[i*3+1]; /* Green Byte */
	sampleimage_buf[i*4+2]=samplesource[i*3+2]; /* Red Byte */
	sampleimage_buf[i*4+3]=0; /* dummy byte */  }
      }
  }
  
  /* color imageA display */
  if ((display_state&DISPLAY_COLORIMAGEA)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenA_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenA_buf[i]= (unsigned char)tabla[(unsigned char)(colorA[j*3])];
	  }
	}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++)
	    { 
	      c=i%(SIFNTSC_COLUMNS/2);
	      row=i/(SIFNTSC_COLUMNS/2);
	      j=2*row*SIFNTSC_COLUMNS+2*c;
	      imagenA_buf[i*2+1]=(0xf8&(colorA[j*3+2]))+((0xe0&(colorA[j*3+1]))>>5);
	      imagenA_buf[i*2]=((0xf8&(colorA[j*3]))>>3)+((0x1c&(colorA[j*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenA_buf[i*4]=colorA[j*3]; /* Blue Byte */
	    imagenA_buf[i*4+1]=colorA[j*3+1]; /* Green Byte */
	    imagenA_buf[i*4+2]=colorA[j*3+2]; /* Red Byte */
	    imagenA_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  
  /* color imageB display */
  if ((display_state&DISPLAY_COLORIMAGEB)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenB_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenB_buf[i]= (unsigned char)tabla[(unsigned char)(colorB[j*3])];
	  }}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++)
	    { 
	      c=i%(SIFNTSC_COLUMNS/2);
	      row=i/(SIFNTSC_COLUMNS/2);
	      j=2*row*SIFNTSC_COLUMNS+2*c;
	      imagenB_buf[i*2+1]=(0xf8&(colorB[j*3+2]))+((0xe0&(colorB[j*3+1]))>>5);
	      imagenB_buf[i*2]=((0xf8&(colorB[j*3]))>>3)+((0x1c&(colorB[j*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenB_buf[i*4]=colorB[j*3]; /* Blue Byte */
	    imagenB_buf[i*4+1]=colorB[j*3+1]; /* Green Byte */
	    imagenB_buf[i*4+2]=colorB[j*3+2]; /* Red Byte */
	    imagenB_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  /* color imageC display */
  if ((display_state&DISPLAY_COLORIMAGEC)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenC_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenC_buf[i]= (unsigned char)tabla[(unsigned char)(colorC[j*3])];
	  }}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++)
	    { 
	      c=i%(SIFNTSC_COLUMNS/2);
	      row=i/(SIFNTSC_COLUMNS/2);
	      j=2*row*SIFNTSC_COLUMNS+2*c;
	      imagenC_buf[i*2+1]=(0xf8&(colorC[j*3+2]))+((0xe0&(colorC[j*3+1]))>>5);
	      imagenC_buf[i*2]=((0xf8&(colorC[j*3]))>>3)+((0x1c&(colorC[j*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenC_buf[i*4]=colorC[j*3]; /* Blue Byte */
	    imagenC_buf[i*4+1]=colorC[j*3+1]; /* Green Byte */
	    imagenC_buf[i*4+2]=colorC[j*3+2]; /* Red Byte */
	    imagenC_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  /* color imageD display */
  if ((display_state&DISPLAY_COLORIMAGED)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenD_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenD_buf[i]= (unsigned char)tabla[(unsigned char)(colorD[j*3])];
	  }}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++)
	    { 
	      c=i%(SIFNTSC_COLUMNS/2);
	      row=i/(SIFNTSC_COLUMNS/2);
	      j=2*row*SIFNTSC_COLUMNS+2*c;
	      imagenD_buf[i*2+1]=(0xf8&(colorD[j*3+2]))+((0xe0&(colorD[j*3+1]))>>5);
	      imagenD_buf[i*2]=((0xf8&(colorD[j*3]))>>3)+((0x1c&(colorD[j*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS/4; i++) 
	  { 
	    c=i%(SIFNTSC_COLUMNS/2);
	    row=i/(SIFNTSC_COLUMNS/2);
	    j=2*row*SIFNTSC_COLUMNS+2*c;
	    imagenD_buf[i*4]=colorD[j*3]; /* Blue Byte */
	    imagenD_buf[i*4+1]=colorD[j*3+1]; /* Green Byte */
	    imagenD_buf[i*4+2]=colorD[j*3+2]; /* Red Byte */
	    imagenD_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  if ((display_state&DISPLAY_COLORIMAGEA)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenA,0,0,fd_sensorsmotorsgui->ventanaA->x+1, fd_sensorsmotorsgui->ventanaA->y+1, SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2);
    }
  
  if ((display_state&DISPLAY_COLORIMAGEB)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenB,0,0,fd_sensorsmotorsgui->ventanaB->x+1, fd_sensorsmotorsgui->ventanaB->y+1, SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2);
    }
  
  if ((display_state&DISPLAY_COLORIMAGEC)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenC,0,0,fd_sensorsmotorsgui->ventanaC->x+1, fd_sensorsmotorsgui->ventanaC->y+1, SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2);
    }
  
  if ((display_state&DISPLAY_COLORIMAGED)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenD,0,0,fd_sensorsmotorsgui->ventanaD->x+1, fd_sensorsmotorsgui->ventanaD->y+1, SIFNTSC_COLUMNS/2, SIFNTSC_ROWS/2);
    }
  
  /* Draw sample image onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,sampleimage,0,0,fd_sensorsmotorsgui->sampleimage->x+1, fd_sensorsmotorsgui->sampleimage->y+1, SIFNTSC_COLUMNS, SIFNTSC_ROWS);
 
}


void mastergui_suspend(void)
{
  if (mastergui_on==TRUE)
    {
      /* printf("mastergui suspend\n"); */
      mastergui_request=FALSE;
    }
}

void mastergui_resume(void)
{
  if (mastergui_on==FALSE)
    {
      /*  printf("mastergui resume\n");*/
      mastergui_request=TRUE;
    }
}

void mastergui_buttons(FL_OBJECT *obj)
{
  int i;

  if (obj == fd_mastergui->exit) jdeshutdown(0);
  else if (obj == fd_mastergui->hide) mastergui_suspend();
  else if (obj == fd_mastergui->vissonars)
    {
      if (fl_get_button(fd_mastergui->vissonars)==RELEASED)
	{display_state = display_state & ~DISPLAY_SONARS;
	if (((display_state & DISPLAY_SONARS)==0) &&
	    ((display_state & DISPLAY_LASER)==0) &&
	    ((display_state & DISPLAY_ROBOT)==0) &&
	    ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	    ((display_state & DISPLAY_COLORIMAGED)==0) &&
	    ((display_state & BASE_TELEOPERATOR)==0) &&
	    ((display_state & PANTILT_TELEOPERATOR)==0))
	  sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | DISPLAY_SONARS;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->sonars)
    {
      if (fl_get_button(fd_mastergui->sonars)==RELEASED)
	{
	sonars_suspend();
	/* if visualization is active, turn it off */
	fl_set_button(fd_mastergui->vissonars,RELEASED);
	if (display_state & DISPLAY_SONARS)
	  {
	    display_state = display_state & ~DISPLAY_SONARS;
	    if (((display_state & DISPLAY_SONARS)==0) &&
		((display_state & DISPLAY_LASER)==0) &&
		((display_state & DISPLAY_ROBOT)==0) &&
		((display_state & DISPLAY_PANTILTENCODERS)==0) &&
		((display_state & DISPLAY_COLORIMAGEA)==0) &&
		((display_state & DISPLAY_COLORIMAGEB)==0) &&
		((display_state & DISPLAY_COLORIMAGEC)==0) &&
		((display_state & DISPLAY_COLORIMAGED)==0) &&
		((display_state & BASE_TELEOPERATOR)==0) &&
		((display_state & PANTILT_TELEOPERATOR)==0))
	      sensorsmotorsgui_suspend();
	  }
	}
      else 
	sonars_resume();
    }
  else if (obj == fd_mastergui->vislaser)
    {
      if (fl_get_button(fd_mastergui->vislaser)==RELEASED)
	{display_state = display_state & ~DISPLAY_LASER;
	if (((display_state & DISPLAY_SONARS)==0) &&
	    ((display_state & DISPLAY_LASER)==0) &&
	    ((display_state & DISPLAY_ROBOT)==0) &&
	    ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	    ((display_state & DISPLAY_COLORIMAGED)==0) &&
	    ((display_state & BASE_TELEOPERATOR)==0) &&
	    ((display_state & PANTILT_TELEOPERATOR)==0))
	  sensorsmotorsgui_suspend();
	}
      else 
	{	
	  display_state=display_state | DISPLAY_LASER;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->laser)
    {
      if (fl_get_button(fd_mastergui->laser)==RELEASED)
	{laser_suspend();
	/* if visualization is active, turn it off */
	fl_set_button(fd_mastergui->vislaser,RELEASED);
	if (display_state & DISPLAY_LASER)
	  {display_state = display_state & ~DISPLAY_LASER;
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	  }
	}
      else 
	laser_resume();
    }
  else if (obj == fd_mastergui->visrobot)
    {
      if (fl_get_button(fd_mastergui->visrobot)==RELEASED)
	{display_state = display_state & ~DISPLAY_ROBOT;
	if (((display_state & DISPLAY_SONARS)==0) &&
	    ((display_state & DISPLAY_LASER)==0) &&
	    ((display_state & DISPLAY_ROBOT)==0) &&
	    ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	    ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	    ((display_state & DISPLAY_COLORIMAGED)==0) &&
	    ((display_state & BASE_TELEOPERATOR)==0) &&
	    ((display_state & PANTILT_TELEOPERATOR)==0))
	  sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | DISPLAY_ROBOT;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->robot)
    {
      if (fl_get_button(fd_mastergui->robot)==RELEASED)
	{ encoders_suspend();
	/* if visualization is active, turn it off */
	fl_set_button(fd_mastergui->visrobot,RELEASED);
	if (display_state & DISPLAY_ROBOT)
	  {display_state = display_state & ~DISPLAY_ROBOT;
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	  }
	}
      else 
	encoders_resume();
    }
  else if (obj == fd_mastergui->vispantiltencoders)
    {
      if (fl_get_button(fd_mastergui->vispantiltencoders)==RELEASED)
	{	
	  display_state = display_state & ~DISPLAY_PANTILTENCODERS;
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | DISPLAY_PANTILTENCODERS;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->pantilt_encoders)
    {
      if (fl_get_button(fd_mastergui->pantilt_encoders)==RELEASED)
	{pantiltencoders_suspend();
	/* if visualization is active, turn it off */
	fl_set_button(fd_mastergui->vispantiltencoders,RELEASED);
	if (display_state & DISPLAY_PANTILTENCODERS)
	  {display_state = display_state & ~DISPLAY_PANTILTENCODERS;
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	  }
	}
      else pantiltencoders_resume();
    }
  else if (obj == fd_mastergui->viscolorimageA)
    {
      if (fl_get_button(fd_mastergui->viscolorimageA)==RELEASED)
	{
	  display_state = display_state & ~DISPLAY_COLORIMAGEA;
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | DISPLAY_COLORIMAGEA;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->colorimageA)
    { 
      if (fl_get_button(fd_mastergui->colorimageA)==PUSHED)
	imageA_resume();
      else 
	{
	  imageA_suspend();
	  /* if visualization is active, turn it off */
	  fl_set_button(fd_mastergui->viscolorimageA,RELEASED);
	  if (display_state & DISPLAY_COLORIMAGEA)
	    {display_state = display_state & ~DISPLAY_PANTILTENCODERS;
	    if (((display_state & DISPLAY_SONARS)==0) &&
		((display_state & DISPLAY_LASER)==0) &&
		((display_state & DISPLAY_ROBOT)==0) &&
		((display_state & DISPLAY_PANTILTENCODERS)==0) &&
		((display_state & DISPLAY_COLORIMAGEA)==0) &&
		((display_state & DISPLAY_COLORIMAGEB)==0) &&
		((display_state & DISPLAY_COLORIMAGEC)==0) &&
		((display_state & DISPLAY_COLORIMAGED)==0) &&
		((display_state & BASE_TELEOPERATOR)==0) &&
		((display_state & PANTILT_TELEOPERATOR)==0))
	      sensorsmotorsgui_suspend();
	    }
	}
      fpsA=0;
    }
  else if (obj == fd_mastergui->viscolorimageB)
    {
      if (fl_get_button(fd_mastergui->viscolorimageB)==RELEASED)
         {
	   display_state = display_state & ~DISPLAY_COLORIMAGEB;
	   if (((display_state & DISPLAY_SONARS)==0) &&
	       ((display_state & DISPLAY_LASER)==0) &&
	       ((display_state & DISPLAY_ROBOT)==0) &&
	       ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	       ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	       ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	       ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	       ((display_state & DISPLAY_COLORIMAGED)==0) &&
	       ((display_state & BASE_TELEOPERATOR)==0) &&
	       ((display_state & PANTILT_TELEOPERATOR)==0))
	     sensorsmotorsgui_suspend();
	 }
      else 
	{
	  display_state=display_state | DISPLAY_COLORIMAGEB;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->colorimageB)
    {
      if (fl_get_button(fd_mastergui->colorimageB)==PUSHED)
	imageB_resume();
      else 
	{
	  imageB_suspend();
	  /* if visualization is active, turn it off */
	  fl_set_button(fd_mastergui->viscolorimageB,RELEASED);
	  if (display_state & DISPLAY_COLORIMAGEB)
	    {display_state = display_state & ~DISPLAY_PANTILTENCODERS;
	    if (((display_state & DISPLAY_SONARS)==0) &&
		((display_state & DISPLAY_LASER)==0) &&
		((display_state & DISPLAY_ROBOT)==0) &&
		((display_state & DISPLAY_PANTILTENCODERS)==0) &&
		((display_state & DISPLAY_COLORIMAGEA)==0) &&
		((display_state & DISPLAY_COLORIMAGEB)==0) &&
		((display_state & DISPLAY_COLORIMAGEC)==0) &&
		((display_state & DISPLAY_COLORIMAGED)==0) &&
		((display_state & BASE_TELEOPERATOR)==0) &&
		((display_state & PANTILT_TELEOPERATOR)==0))
	      sensorsmotorsgui_suspend();
	    }
	}
      fpsB=0;
    }
  else if (obj == fd_mastergui->viscolorimageC)
    {
      if (fl_get_button(fd_mastergui->viscolorimageC)==RELEASED)
	{
	  display_state = display_state & ~DISPLAY_COLORIMAGEC;
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | DISPLAY_COLORIMAGEC;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->colorimageC)
    { 
      if (fl_get_button(fd_mastergui->colorimageC)==PUSHED)
	imageC_resume();
      else 
	{
	  imageC_suspend();
	  /* if visualization is active, turn it off */
	  fl_set_button(fd_mastergui->viscolorimageC,RELEASED);
	  if (display_state & DISPLAY_COLORIMAGEC)
	    {display_state = display_state & ~DISPLAY_PANTILTENCODERS;
	    if (((display_state & DISPLAY_SONARS)==0) &&
		((display_state & DISPLAY_LASER)==0) &&
		((display_state & DISPLAY_ROBOT)==0) &&
		((display_state & DISPLAY_PANTILTENCODERS)==0) &&
		((display_state & DISPLAY_COLORIMAGEA)==0) &&
		((display_state & DISPLAY_COLORIMAGEB)==0) &&
		((display_state & DISPLAY_COLORIMAGEC)==0) &&
		((display_state & DISPLAY_COLORIMAGED)==0) &&
		((display_state & BASE_TELEOPERATOR)==0) &&
		((display_state & PANTILT_TELEOPERATOR)==0))
	      sensorsmotorsgui_suspend();
	    }
	}
      fpsC=0;
    }
  else if (obj == fd_mastergui->viscolorimageD)
    {
      if (fl_get_button(fd_mastergui->viscolorimageD)==RELEASED)
	{
	  display_state = display_state & ~DISPLAY_COLORIMAGED;
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | DISPLAY_COLORIMAGED;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->colorimageD)
    { 
      if (fl_get_button(fd_mastergui->colorimageD)==PUSHED)
	imageD_resume();
      else 
	{
	  imageD_suspend();
	  /* if visualization is active, turn it off */
	  fl_set_button(fd_mastergui->viscolorimageD,RELEASED);
	  if (display_state & DISPLAY_COLORIMAGED)
	    {display_state = display_state & ~DISPLAY_PANTILTENCODERS;
	    if (((display_state & DISPLAY_SONARS)==0) &&
		((display_state & DISPLAY_LASER)==0) &&
		((display_state & DISPLAY_ROBOT)==0) &&
		((display_state & DISPLAY_PANTILTENCODERS)==0) &&
		((display_state & DISPLAY_COLORIMAGEA)==0) &&
		((display_state & DISPLAY_COLORIMAGEB)==0) &&
		((display_state & DISPLAY_COLORIMAGEC)==0) &&
		((display_state & DISPLAY_COLORIMAGED)==0) &&
		((display_state & BASE_TELEOPERATOR)==0) &&
		((display_state & PANTILT_TELEOPERATOR)==0))
	      sensorsmotorsgui_suspend();
	    }
	}
      fpsD=0;
    }
  else if (obj == fd_mastergui->vismotors)
    {
      if (fl_get_button(fd_mastergui->vismotors)==RELEASED)
	{
	  display_state = display_state & ~BASE_TELEOPERATOR;
	  v=0.; w=0.; /*safety stop when disabling the teleoperator */
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | BASE_TELEOPERATOR;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->motors) 
    {
      if (fl_get_button(fd_mastergui->motors)==RELEASED)
	{
	  motors_suspend(); /* it makes a safety stop itself before suspending */ 
	  /* if visualization is active, turn it off */
	  fl_set_button(fd_mastergui->vismotors,RELEASED);
	  if (display_state & BASE_TELEOPERATOR)
	    {display_state = display_state & ~BASE_TELEOPERATOR;
	    if (((display_state & DISPLAY_SONARS)==0) &&
		((display_state & DISPLAY_LASER)==0) &&
		((display_state & DISPLAY_ROBOT)==0) &&
		((display_state & DISPLAY_PANTILTENCODERS)==0) &&
		((display_state & DISPLAY_COLORIMAGEA)==0) &&
		((display_state & DISPLAY_COLORIMAGEB)==0) &&
		((display_state & DISPLAY_COLORIMAGEC)==0) &&
		((display_state & DISPLAY_COLORIMAGED)==0) &&
		((display_state & BASE_TELEOPERATOR)==0) &&
		((display_state & PANTILT_TELEOPERATOR)==0))
	      sensorsmotorsgui_suspend();
	    }
	}
      else motors_resume();
    }
  else if (obj == fd_mastergui->vispantiltmotors)
    {
      if (fl_get_button(fd_mastergui->vispantiltmotors)==RELEASED)
	{
	  display_state = display_state & ~PANTILT_TELEOPERATOR;
	  /*safety stop when disabling the teleoperator */
	  /* current pantilt position as initial command, to avoid any movement */
	  if ((MAX_PAN_ANGLE - MIN_PAN_ANGLE) > 0.05) 
	    pt_joystick_x= 1.-(pan_angle-MIN_PAN_ANGLE)/(MAX_PAN_ANGLE-MIN_PAN_ANGLE);
	  if ((MAX_TILT_ANGLE - MIN_TILT_ANGLE) > 0.05) 
	    pt_joystick_y= (tilt_angle-MIN_TILT_ANGLE)/(MAX_TILT_ANGLE-MIN_TILT_ANGLE);   
	  if (((display_state & DISPLAY_SONARS)==0) &&
	      ((display_state & DISPLAY_LASER)==0) &&
	      ((display_state & DISPLAY_ROBOT)==0) &&
	      ((display_state & DISPLAY_PANTILTENCODERS)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEA)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEB)==0) &&
	      ((display_state & DISPLAY_COLORIMAGEC)==0) &&
	      ((display_state & DISPLAY_COLORIMAGED)==0) &&
	      ((display_state & BASE_TELEOPERATOR)==0) &&
	      ((display_state & PANTILT_TELEOPERATOR)==0))
	    sensorsmotorsgui_suspend();
	}
      else 
	{
	  display_state=display_state | PANTILT_TELEOPERATOR;
	  sensorsmotorsgui_resume();
	}
    }
  else if (obj == fd_mastergui->pantiltmotors) 
  {
      if (fl_get_button(fd_mastergui->pantiltmotors)==RELEASED)
	{
	  pantiltmotors_suspend();
	  /* if visualization is active, turn it off */
	  fl_set_button(fd_mastergui->vispantiltmotors,RELEASED);
	  if (display_state & PANTILT_TELEOPERATOR)
	    {display_state = display_state & ~PANTILT_TELEOPERATOR;
	    if (((display_state & DISPLAY_SONARS)==0) &&
		((display_state & DISPLAY_LASER)==0) &&
		((display_state & DISPLAY_ROBOT)==0) &&
		((display_state & DISPLAY_PANTILTENCODERS)==0) &&
		((display_state & DISPLAY_COLORIMAGEA)==0) &&
		((display_state & DISPLAY_COLORIMAGEB)==0) &&
		((display_state & DISPLAY_COLORIMAGEC)==0) &&
		((display_state & DISPLAY_COLORIMAGED)==0) &&
		((display_state & BASE_TELEOPERATOR)==0) &&
		((display_state & PANTILT_TELEOPERATOR)==0))
	      sensorsmotorsgui_suspend();
	    } 
	}
     else pantiltmotors_resume();
    }

  /* GUI entries for loaded schemas */
  for(i=0;i<MAX_LOADEDSCHEMAS;i++)
    {
      if (associated_ID[i]!=-1)
	{
	  if (obj == act[i]) 
	    {if (fl_get_button(act[i])==RELEASED) 
	      {(*all[associated_ID[i]].suspend)();
		if (fl_get_button(vis[i])==PUSHED)
		  {
		    fl_set_button(vis[i],RELEASED);
		    (*all[associated_ID[i]].guisuspend)();
		  }
	      }
	    else 
	      (*all[associated_ID[i]].resume)(GUIHUMAN,NULL,null_arbitration);
	    }
	  else if (obj == vis[i])
	    {if (fl_get_button(vis[i])==RELEASED)
		(*all[associated_ID[i]].guisuspend)();
	    else 
		(*all[associated_ID[i]].guiresume)();
	    }
	}
    }
}


void navigate(int schema,int *x,int *y)
{
  int i;

  /* print the names of all the children of "schema" */
  for (i=0;i<MAX_SCHEMAS;i++)
    {
      if (all[schema].children[i]==TRUE)
	{  
	  if (all[i].state==notready) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),40,sizeY,FL_BLUE,9,20,all[i].name);
	  else if (all[i].state==notready) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),40,sizeY,FL_BLUE,9,20,all[i].name);
	  else if (all[i].state==ready) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),40,sizeY,FL_GREEN,9,20,all[i].name);
	  else if (all[i].state==forced) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),40,sizeY,FL_RED,9,20,all[i].name);
	  else if (all[i].state==winner) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),40,sizeY,FL_RED,9,20,all[i].name);
	  
	  if ((*x+sizeX*strlen(all[i].name)) < (fd_mastergui->hierarchy->x + fd_mastergui->hierarchy->w))
	    (*x)+=sizeX*strlen(all[i].name);
	}
    }

  /* expand the winner children of "schema", hopefully there will be only one */
  for (i=0;i<MAX_SCHEMAS;i++)
    {
      if ((all[schema].children[i]==TRUE) &&
	  (all[i].state==winner))
	navigate(i,x,y);
    }
}

void mastergui_display()
{
  int i,haschanged,j;
  int xx,yy;
  char fpstext[80]="";

  sprintf(fpstext,"%.1f",fpsA);
  fl_set_object_label(fd_mastergui->frame_rateA,fpstext);
  
  sprintf(fpstext,"%.1f",fpsB);
  fl_set_object_label(fd_mastergui->frame_rateB,fpstext);
  
  sprintf(fpstext,"%.1f",fpsC);
  fl_set_object_label(fd_mastergui->frame_rateC,fpstext);
  
  sprintf(fpstext,"%.1f",fpsD);
  fl_set_object_label(fd_mastergui->frame_rateD,fpstext);
  
  sprintf(fpstext,"%.1f",fpssonars);
  fl_set_object_label(fd_mastergui->fpssonars,fpstext);
  
  sprintf(fpstext,"%.1f",fpslaser);
  fl_set_object_label(fd_mastergui->fpslaser,fpstext);
  
  sprintf(fpstext,"%.1f",fpsencoders);
  fl_set_object_label(fd_mastergui->fpsencoders,fpstext);
  
  sprintf(fpstext,"%.1f",fpspantiltencoders);
  fl_set_object_label(fd_mastergui->fpspantiltencoders,fpstext);
  
  sprintf(fpstext,"%.1f",fpspantiltmotors);
  fl_set_object_label(fd_mastergui->fpspantiltmotors,fpstext);
  
  sprintf(fpstext,"%.1f",fpsmotors);
  fl_set_object_label(fd_mastergui->fpsmotors,fpstext);
  
  sprintf(fpstext,"%.1f ips",fpsgui);
  fl_set_object_label(fd_mastergui->guifps,fpstext);  
  
  /* GUI entries for loaded schemas */
  for(i=0;i<MAX_LOADEDSCHEMAS; i++)
    {
      if (associated_ID[i]!=-1)
	{ 
	  fl_show_object(act[i]);
	  fl_show_object(vis[i]);
	  fl_show_object(fps[i]);
	  fl_set_object_label(act[i],all[associated_ID[i]].name);
	  if (all[associated_ID[i]].state==winner)
	    sprintf(fpstext,"%.1f",all[associated_ID[i]].fps);
	  else sprintf(fpstext," ");
	  fl_set_object_label(fps[i],fpstext);
	}
      else 
	{
	  fl_hide_object(act[i]);
	  fl_hide_object(vis[i]);
	  fl_hide_object(fps[i]);
	}
    }

  /* hierarchy oscilloscope */
  haschanged=FALSE;
  for(i=0;i<num_schemas;i++) 
    {
      if (all[i].state!=state_dpy[i]) 
	{haschanged=TRUE;
	break;
	}
    }

  if ((haschanged==TRUE) ||     /* the hierarchy has changed */
      (iteracion_display==0))   /* slow refresh of the complete master gui, needed because incremental refresh of the hierarchy misses window occlusions */
    {
      /* clear of the hierarchy "window" */
      fl_winset(hierarchy_win); 
      fl_rectbound(fd_mastergui->hierarchy->x-1,fd_mastergui->hierarchy->y-1,fd_mastergui->hierarchy->w,fd_mastergui->hierarchy->h,FL_COL1);         
      /*
      for(i=0;i<num_schemas;i++)
	{
	  state_dpy[i]=all[i].state;
	  if (all[i].state==slept)  
	    fl_drw_text(FL_ALIGN_LEFT,fd_mastergui->hierarchy->x+10,fd_mastergui->hierarchy->y+0+i*30,40,30,FL_BLACK,9,20,all[i].name);
	  else if (all[i].state==active) 
	    fl_drw_text(FL_ALIGN_LEFT,fd_mastergui->hierarchy->x+10,fd_mastergui->hierarchy->y+0+i*30,40,30,FL_BLUE,9,20,all[i].name);
	  else if (all[i].state==notready) 
	    fl_drw_text(FL_ALIGN_LEFT,fd_mastergui->hierarchy->x+10,fd_mastergui->hierarchy->y+0+i*30,40,30,FL_BLUE,9,20,all[i].name);
	  else if (all[i].state==ready) 
	    fl_drw_text(FL_ALIGN_LEFT,fd_mastergui->hierarchy->x+10,fd_mastergui->hierarchy->y+0+i*30,40,30,FL_GREEN,9,20,all[i].name);
	  else if (all[i].state==forced) 
	    fl_drw_text(FL_ALIGN_LEFT,fd_mastergui->hierarchy->x+10,fd_mastergui->hierarchy->y+0+i*30,40,30,FL_RED,9,20,all[i].name);
	  else if (all[i].state==winner) 
	    fl_drw_text(FL_ALIGN_LEFT,fd_mastergui->hierarchy->x+10,fd_mastergui->hierarchy->y+0+i*30,40,30,FL_RED,9,20,all[i].name);
	}
      */
  
      j=0; xx=fd_mastergui->hierarchy->x+5; yy=fd_mastergui->hierarchy->y+5;
      for(i=0;i<num_schemas;i++)
	{
	  state_dpy[i]=all[i].state;
	  if ((all[i].state!=slept) && 
	      ((all[i].father==(*all[i].id)) || (all[i].father==GUIHUMAN)))
	    {
	      /* the root of one hierarchy */
	      j++;
	      if (j!=1)
		{
		  if ((yy+5) < (fd_mastergui->hierarchy->y+fd_mastergui->hierarchy->h)) yy+=5;
		  fl_line(xx,yy,xx+fd_mastergui->hierarchy->w-15,yy,FL_BLACK);
		  if ((yy+5) < (fd_mastergui->hierarchy->y+fd_mastergui->hierarchy->h)) yy+=5;
		}
		
	      if (all[i].state==active) 
		fl_drw_text(FL_ALIGN_LEFT,xx,yy,40,sizeY,FL_BLUE,9,20,all[i].name);
	      else if (all[i].state==notready) 
		fl_drw_text(FL_ALIGN_LEFT,xx,yy,40,sizeY,FL_BLUE,9,20,all[i].name);
	      else if (all[i].state==ready) 
		fl_drw_text(FL_ALIGN_LEFT,xx,yy,40,sizeY,FL_GREEN,9,20,all[i].name);
	      else if (all[i].state==forced) 
		fl_drw_text(FL_ALIGN_LEFT,xx,yy,40,sizeY,FL_RED,9,20,all[i].name);
	      else if (all[i].state==winner) 
		fl_drw_text(FL_ALIGN_LEFT,xx,yy,40,sizeY,FL_RED,9,20,all[i].name);	      

	      if ((yy+sizeY) < (fd_mastergui->hierarchy->y+fd_mastergui->hierarchy->h)) yy+=sizeY;
	      navigate(i,&xx,&yy); 
	    }     
	}
    }
}


void jdegui_iteration()
{ 
  FL_OBJECT *obj; 
  double delta, deltapos;
  float speed_coef;
  int i;
  static int kmastergui=0;
  static int ksensorsmotorsgui=0;
  float r,lati,longi,guix,guiy,guiz;

  if (jdegui_debug) printf("jdegui iteration\n");
  kgui++;

  if (iteracion_display*jdegui_cycle>FORCED_REFRESH) 
    iteracion_display=0;
  else iteracion_display++;

  /* change of visualization state requests */
  if (mastergui_request!=mastergui_on)
    {
      if ((mastergui_request==FALSE) && (mastergui_on==TRUE))
	/* mastergui_suspend request */
	{
	  fl_hide_form(fd_mastergui->mastergui);
	  mastergui_on=FALSE;
	}
      else if ((mastergui_request==TRUE) && (mastergui_on==FALSE))
	/* mastergui_resume request */
	{
	  mastergui_on=TRUE;
	  if (kmastergui==0) /* not initialized */
	    {
	      kmastergui++;
	      fd_mastergui = create_form_mastergui();
	      /* tabla de asociacion guientry-esquema */
	      vis[0]=fd_mastergui->vis0;
	      vis[1]=fd_mastergui->vis1;
	      vis[2]=fd_mastergui->vis2;
	      vis[3]=fd_mastergui->vis3;
	      vis[4]=fd_mastergui->vis4;
	      vis[5]=fd_mastergui->vis5;
	      vis[6]=fd_mastergui->vis6;
	      vis[7]=fd_mastergui->vis7;
	      vis[8]=fd_mastergui->vis8;
	      vis[9]=fd_mastergui->vis9;
	      vis[10]=fd_mastergui->vis10;
	      vis[11]=fd_mastergui->vis11;
	      act[0]=fd_mastergui->act0;
	      act[1]=fd_mastergui->act1;
	      act[2]=fd_mastergui->act2;
	      act[3]=fd_mastergui->act3;
	      act[4]=fd_mastergui->act4;
	      act[5]=fd_mastergui->act5;
	      act[6]=fd_mastergui->act6;
	      act[7]=fd_mastergui->act7;
	      act[8]=fd_mastergui->act8;
	      act[9]=fd_mastergui->act9;
	      act[10]=fd_mastergui->act10;
	      act[11]=fd_mastergui->act11;
	      fps[0]=fd_mastergui->fps0;
	      fps[1]=fd_mastergui->fps1;
	      fps[2]=fd_mastergui->fps2;
	      fps[3]=fd_mastergui->fps3;
	      fps[4]=fd_mastergui->fps4;
	      fps[5]=fd_mastergui->fps5;
	      fps[6]=fd_mastergui->fps6;
	      fps[7]=fd_mastergui->fps7;
	      fps[8]=fd_mastergui->fps8;
	      fps[9]=fd_mastergui->fps9;
	      fps[10]=fd_mastergui->fps10;
	      fps[11]=fd_mastergui->fps11;
	      fl_set_form_position(fd_mastergui->mastergui,200,50);
	    }
	  fl_show_form(fd_mastergui->mastergui,FL_PLACE_POSITION,FL_FULLBORDER,"jde master gui");
	  hierarchy_win = FL_ObjWin(fd_mastergui->hierarchy);
	}
    }

  if (sensorsmotorsgui_request!=sensorsmotorsgui_on)
    {
      if (jdegui_debug) printf("sensorsmotorsgui_suspend request\n");
      if ((sensorsmotorsgui_request==FALSE) && (sensorsmotorsgui_on==TRUE))
	/* sensorsmotorsgui_suspend request */
	{
	  fl_hide_form(fd_sensorsmotorsgui->sensorsmotorsgui);	  
	  sensorsmotorsgui_on=FALSE;
	}
      if ((sensorsmotorsgui_request==TRUE) && (sensorsmotorsgui_on==FALSE))
	/* sensorsmotorsgui_resume request */
	{
	  sensorsmotorsgui_on=TRUE;
	  if (ksensorsmotorsgui==0) /* not initialized */
	    {
	      ksensorsmotorsgui++;
	      fd_sensorsmotorsgui = create_form_sensorsmotorsgui();
	      fl_set_form_position(fd_sensorsmotorsgui->sensorsmotorsgui,400,50);
	      fl_show_form(fd_sensorsmotorsgui->sensorsmotorsgui,FL_PLACE_POSITION,FL_FULLBORDER,"sensors and motors");
	      image_displaysetup(); /* Tiene que ir despues de la inicializacion de Forms, pues hace uso de informacion que la libreria rellena en tiempo de ejecucion al iniciarse */
	      fl_add_canvas_handler(fd_sensorsmotorsgui->canvas,Expose,InitOGL,0);
  	    }
	  else 
	    fl_show_form(fd_sensorsmotorsgui->sensorsmotorsgui,FL_PLACE_POSITION,FL_FULLBORDER,"sensors and motors");
	  
	  fl_set_slider_bounds(fd_sensorsmotorsgui->posX,MAXWORLD*10,-MAXWORLD*10);	     
	  fl_set_slider_bounds(fd_sensorsmotorsgui->posY,MAXWORLD*10,-MAXWORLD*10);
	  fl_set_slider_bounds(fd_sensorsmotorsgui->posZ,MAXWORLD*10,-MAXWORLD*10);
	  fl_set_positioner_xbounds(fd_sensorsmotorsgui->latlong,-180,180.);
	  fl_set_positioner_ybounds(fd_sensorsmotorsgui->latlong,-89.99,89.99); 
	  fl_set_slider_bounds(fd_sensorsmotorsgui->posR,MAXWORLD*10*2,0.);
	  fl_set_button(fd_sensorsmotorsgui->selectfoaRel,RELEASED);
	  fl_set_button(fd_sensorsmotorsgui->selectfoa,RELEASED);
	  fl_set_button(fd_sensorsmotorsgui->selectposition,PUSHED);
	  guix=(float) virtualcam.posx;
	  guiy=(float) virtualcam.posy;
	  guiz=(float) virtualcam.posz;
	  r=(float)sqrt((double)(guix*guix+guiy*guiy+guiz*guiz));
	  lati=(float)asin(guiz/r)*360./(2.*PI);
	  longi=(float)atan2((float)guiy,(float)guix)*360./(2.*PI);
	  fl_set_slider_value(fd_sensorsmotorsgui->posX,virtualcam.posx);
	  fl_set_slider_value(fd_sensorsmotorsgui->posY,virtualcam.posy);
	  fl_set_slider_value(fd_sensorsmotorsgui->posZ,virtualcam.posz);
	  fl_set_positioner_xvalue(fd_sensorsmotorsgui->latlong,(double) longi);
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->latlong,(double) lati);
	  fl_set_slider_value(fd_sensorsmotorsgui->posR,(double)r); 
	  if (track_robot==TRUE) fl_set_button(fd_sensorsmotorsgui->track_robot,PUSHED);
	  else fl_set_button(fd_sensorsmotorsgui->track_robot,RELEASED);
	  if (toggle==TRUE) fl_set_button(fd_sensorsmotorsgui->toggle,PUSHED);
	  else fl_set_button(fd_sensorsmotorsgui->toggle,RELEASED);
	  
	  sensorsmotorsgui_win = FL_ObjWin(fd_sensorsmotorsgui->ventanaA);
	  /* the window (sensorsmotorsgui_win) changes every time the form is hided and showed again. They need to be updated before displaying anything again */

	  fl_set_positioner_xvalue(fd_sensorsmotorsgui->joystick,0.5);
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->joystick,0.);
	  joystick_x=0.5;
	  joystick_y=0.5;	  
	  fl_set_slider_value(fd_sensorsmotorsgui->ptspeed,(double)(1.-latitude_speed/MAX_SPEED_PANTILT));
	}
    }


  /* buttons check (polling) */
  /* Puesto que el control no se cede al form, sino que se hace polling de sus botones pulsados, debemos proveer el enlace para los botones que no tengan callback asociada, en esta rutina de polling. OJO aquellos que tengan callback asociada jamas se veran con fl_check_forms, la libreria llamara automaticamente a su callback sin que fl_check_forms o fl_do_forms se enteren en absoluto.*/
  obj = fl_check_forms();
  if (mastergui_on==TRUE) mastergui_buttons(obj);
  if (sensorsmotorsgui_on==TRUE) sensorsmotorsgui_buttons(obj);
  /*
  for(i=0;i<num_schemas;i++)
    {
      if (all[i].gui==TRUE)
	(*all[i].guibuttons)(obj);
    }
  */
  for(i=0;i<num_buttonscallbacks;i++)
    {
      if (buttonscallbacks[i]!=NULL)
	(buttonscallbacks[i])(obj);
    }

  /* display iteration */
  if (mastergui_on==TRUE) mastergui_display();
  if (sensorsmotorsgui_on==TRUE)
    {
      sensorsmotorsgui_display(); 
      if ((display_state & BASE_TELEOPERATOR)!=0)
	{
	  /* ROTACION=ejeX: Ajusta a un % de joystick_maxRotVel. OJO no funcion lineal del desplazamiento visual, sino con el al cuadrado, para aplanarla en el origen y evitar cabeceos, conseguir suavidad en la teleoperacion */
	  
	  delta = (joystick_x-0.5)*2; /* entre +-1 */
	  deltapos = fabs(delta); /* Para que no moleste el signo de delta en el factor de la funcion de control */
	  if (delta<0) w = (float) joystick_maxRotVel*deltapos*deltapos*deltapos; 
	  else w = (float) -1.*joystick_maxRotVel*deltapos*deltapos*deltapos;
	  	  
	  /* TRASLACION=ejeY: Ajusta a un % de +-joystick_maxTranVel. OJO no funcion lineal del desplazamiento visual, sino con el a la cuarta, para aplanarla en el origen */
	  
	  delta = (joystick_y-0.5)*2; /* entre +-1 */
	  deltapos = fabs(delta);/* Para que no moleste el signo de delta en el factor de la funcion de control */
	  if (delta<0) v = (float) -1.*joystick_maxTranVel*deltapos*deltapos*deltapos;
	  else v = (float) joystick_maxTranVel*deltapos*deltapos*deltapos;
	}

      if ((display_state & PANTILT_TELEOPERATOR)!=0)      
	{
	  /* pt_joystick_x and pt_joystick_y fall in [0,1], the default limits from Xforms */  
	  latitude=MIN_TILT_ANGLE+pt_joystick_y*(MAX_TILT_ANGLE-MIN_TILT_ANGLE);
	  longitude=MAX_PAN_ANGLE-pt_joystick_x*(MAX_PAN_ANGLE-MIN_PAN_ANGLE);
	  
	  speed_coef = fl_get_slider_value(fd_sensorsmotorsgui->ptspeed);
	  longitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
	  latitude_speed=(1.-speed_coef)*MAX_SPEED_PANTILT;
	  /*printf("JDEGUI: longitude speed %.2f, latitude speed %.2f\n",longitude_speed,latitude_speed);*/
	}
    }
 
  for(i=0;i<num_displaycallbacks;i++)
    {
      if (displaycallbacks[i]!=NULL)
	(displaycallbacks[i])();
    }

}

void *jdegui_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      pthread_mutex_lock(&(jdegui_mymutex));
      if (jdegui_state==slept) 
	{
	  /*printf("jdegui: off\n");*/
	  pthread_cond_wait(&(jdegui_condition),&(jdegui_mymutex));
	  /*printf("jdegui: on\n");*/
	  pthread_mutex_unlock(&(jdegui_mymutex));
	}
      else 
	{
	  pthread_mutex_unlock(&(jdegui_mymutex));
	  gettimeofday(&a,NULL);
	  jdegui_iteration();
	  gettimeofday(&b,NULL);  
	  diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;

	  next = jdegui_cycle*1000-diff-10000; 
	  /* discounts 10ms taken by calling usleep itself */
	  if (next>0) usleep(jdegui_cycle*1000-diff);
	  else { 
	    /* don't bother with this message, just display as fast as you can */
	    /* printf("jdegui: time interval violated\n"); */
	  }
	}
    }
}


void jdegui_startup()
{
  int myargc=1,i=0;
  char **myargv;
  char *aa;
  char a[]="myjde";
 
  virtualcam.posx=-150;
  virtualcam.posy=-150.;
  virtualcam.posz=150.;
  virtualcam.foax=0.;
  virtualcam.foay=0.;
  virtualcam.foaz=0.;
  virtualcam.roll=0.;

  /* prepara el display */
  aa=a;
  myargv=&aa;
  display= fl_initialize(&myargc,myargv,"JDE",0,0);
  screen = DefaultScreen(display);
  samplesource=colorA; 

  for(i=0;i<MAX_SCHEMAS;i++) state_dpy[i]=slept;

  jdegui_state=slept;
  pthread_mutex_init(&jdegui_mymutex,PTHREAD_MUTEX_TIMED_NP);
  pthread_cond_init(&jdegui_condition,NULL);

  pthread_mutex_lock(&(jdegui_mymutex));
  /* printf("jdegui thread started up\n");*/
  jdegui_state=slept;
  pthread_create(&(jdegui_mythread),NULL,jdegui_thread,NULL);
  pthread_mutex_unlock(&(jdegui_mymutex));
}

void jdegui_resume()
{
  int i;

  pthread_mutex_lock(&(jdegui_mymutex));
  if (jdegui_debug) printf("jdegui thread on\n");
  jdegui_state=active;
  for(i=0;i<MAX_SCHEMAS;i++) state_dpy[i]=slept;
  if (mastergui_on==TRUE) mastergui_resume(); 
  if (sensorsmotorsgui_on==TRUE) sensorsmotorsgui_resume();
  pthread_cond_signal(&(jdegui_condition));
  pthread_mutex_unlock(&(jdegui_mymutex));
}

void jdegui_suspend()
{
  pthread_mutex_lock(&(jdegui_mymutex));
  if (jdegui_debug) printf("jdegui thread off\n");
  jdegui_state=slept;
  if (mastergui_on==TRUE) mastergui_suspend(); 
  if (sensorsmotorsgui_on==TRUE) sensorsmotorsgui_suspend();
  pthread_mutex_unlock(&(jdegui_mymutex));
}


void jdegui_close()
{
  /*
  pthread_mutex_lock(&(jdegui_mymutex));
  jdegui_state=slept;
  if (mastergui_on==TRUE) mastergui_suspend(); 
  if (sensorsmotorsgui_on==TRUE) sensorsmotorsgui_suspend();  
  pthread_mutex_unlock(&(jdegui_mymutex));
  sleep(2);
  fl_free_form(fd_sensorsmotorsgui->sensorsmotorsgui);
  fl_free_form(fd_mastergui->mastergui);
  */

  free(imagenA_buf);
  free(imagenB_buf);
  free(imagenC_buf);
  free(imagenD_buf);  
}

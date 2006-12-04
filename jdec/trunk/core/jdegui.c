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

/* jdegui has the same schema API, but it is not an schema, and so it is not stored at all[]. It is just a service thread. */
pthread_mutex_t jdegui_mymutex;
pthread_cond_t jdegui_condition;
pthread_t jdegui_mythread;
int jdegui_debug;
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
#define FORCED_REFRESH 5000 /* ms */ 
/*Every forced_refresh the display is drawn from scratch. If it is too small it will cause flickering with grid display. No merece la pena una hebra de "display_lento" solo para repintar completamente la pantalla. */

#define joystick_maxRotVel 30 /* deg/sec */
#define joystick_maxTranVel 500 /* mm/sec */

float joystick_x, joystick_y;
float pt_joystick_x, pt_joystick_y;
float mouse_x, mouse_y;
int mouse_new=0;
int back=0;

int canvas_mouse_button_pressed=0;
int ventanaA_mouse_button_pressed=0;
int ventanaB_mouse_button_pressed=0;
int ventanaC_mouse_button_pressed=0;
int ventanaD_mouse_button_pressed=0;
int mouse_button=0;
int robot_mouse_motion=0;

FL_Coord x_canvas,y_canvas,old_x_canvas,old_y_canvas;
float diff_x,diff_y,diff_w;

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
FD_sensorsmotorsgui *fd_sensorsmotorsgui;
GC sensorsmotorsgui_gc;
Window  sensorsmotorsgui_win; 
Window  canvas_win;
int vmode;
XImage *imagenA,*imagenB,*imagenC,*imagenD; 
char *imagenA_buf, *imagenB_buf, *imagenC_buf, *imagenD_buf; /* puntero a memoria para la imagen a visualizar en el servidor X. No compartida con el servidor */
long int tabla[256]; 
/* tabla con la traduccion de niveles de gris a numero de pixel en Pseudocolor-8bpp. Cada numero de pixel apunta al valor adecuado del ColorMap, con el color adecuado instalado */
int pixel8bpp_rojo, pixel8bpp_blanco, pixel8bpp_amarillo;
float   escala, jde_width, jde_height;

float odometrico[5];


unsigned long display_state;
int visual_refresh=FALSE;
int track_robot=FALSE;
int iteracion_display=0;

#define EGOMAX NUM_SONARS+5
XPoint ego[EGOMAX];
float last_heading; /* ultima orientacion visualizada del robot */
int numego=0;
int visual_delete_ego=FALSE;

XPoint laser_dpy[NUM_LASER];
int visual_delete_laser=FALSE;

XPoint us_dpy[NUM_SONARS*2];
int visual_delete_us=FALSE;

/* to display the hierarchy */
int state_dpy[MAX_SCHEMAS];
int sizeY=20;
int sizeX=20;

#define RANGO_MAX 20000. /* en mm */
#define RANGO_MIN 500. /* en mm */ 
#define RANGO_INICIAL 4000. /* en mm */
float rango=(float)RANGO_INICIAL; /* Rango de visualizacion en milimetros */

char fpstext[80]="";


const char *rangoenmetros(FL_OBJECT *ob, double value, int prec)
{
static char buf[32];

sprintf(buf,"%.1f",value/1000.);
return buf;
}

int xy2canvas(Tvoxel point, XPoint* grafico)
     /* return -1 if point falls outside the canvas */
{
float xi, yi;

xi = (point.x * odometrico[3] - point.y * odometrico[4] + odometrico[0])*escala;
yi = (point.x * odometrico[4] + point.y * odometrico[3] + odometrico[1])*escala;
/* Con esto cambiamos al sistema de referencia de visualizacion, centrado en algun punto xy y con alguna orientacion definidos por odometrico. Ahora cambiamos de ese sistema al del display, donde siempre hay un desplazamiento a la esquina sup. izda. y que las y se cuentan para abajo. */

grafico->x = xi + jde_width/2;
grafico->y = -yi + jde_height/2;

 if ((grafico->x <0)||(grafico->x>jde_width)) return -1; 
 if ((grafico->y <0)||(grafico->y>jde_height)) return -1; 
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
    canvas_win= FL_ObjWin(fd_sensorsmotorsgui->micanvas);
    XGetWindowAttributes(display, sensorsmotorsgui_win, &win_attributes);  
    XMapWindow(display, sensorsmotorsgui_win);
    /*XSelectInput(display, sensorsmotorsgui_win, ButtonPress|StructureNotifyMask);*/   
    gc_values.graphics_exposures = False;
    sensorsmotorsgui_gc = XCreateGC(display, sensorsmotorsgui_win, GCGraphicsExposures, &gc_values);  
    
    /* Utilizan el Visual (=estructura de color) y el colormap con que este operando el programa principal con su Xforms. No crea un nuevo colormap, sino que modifica el que se estaba usando a traves de funciones de Xforms*/
    vmode= fl_get_vclass();
    if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
      {printf("jdegui: truecolor 16 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2);    
      imagenA = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2);    
      imagenB = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2);    
      imagenC = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2);    
      imagenD = XCreateImage(display,DefaultVisual(display,screen),win_attributes.depth, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      return win_attributes.depth;
      }
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24)) 
      { printf("jdegui: truecolor 24 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenA = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenB = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenC = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenD = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      return win_attributes.depth;
      }
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32)) 
      { printf("jdegui: truecolor 24 bpp\n");
      imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenA = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenB = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenC = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
      imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
      imagenD = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
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

	imagenA_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS);    
	imagenA = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
	imagenB_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS);    
	imagenB = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenB_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
	imagenC_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS);    
	imagenC = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenC_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
	imagenD_buf = (char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS);    
	imagenD = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenD_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);

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

/* callback function for button pressed inside the canvas object*/
int button_pressed_on_micanvas(FL_OBJECT *ob, Window win, int win_width, int win_height, XEvent *xev, void *user_data)
{
  
  unsigned int keymap;
  float ygraf, xgraf;

  /* in order to know the mouse button that created the event */
  mouse_button=xev->xkey.keycode;
  if(canvas_mouse_button_pressed==0){
    if((mouse_button==MOUSELEFT)||(mouse_button==MOUSERIGHT)){
	 
      /* a button has been pressed */
      canvas_mouse_button_pressed=1;
      
      /* getting mouse coordenates. win will be always the canvas window, because this callback has been defined only for that canvas */  
      fl_get_win_mouse(win,&x_canvas,&y_canvas,&keymap);
      old_x_canvas=x_canvas;
      old_y_canvas=y_canvas;
      
      /* from graphical coordinates to spatial ones */
      ygraf=((float) (jde_height/2-y_canvas))/escala;
      xgraf=((float) (x_canvas-jde_width/2))/escala;
      mouse_y=(ygraf-odometrico[1])*odometrico[3]+(-xgraf+odometrico[0])*odometrico[4];
      mouse_x=(ygraf-odometrico[1])*odometrico[4]+(xgraf-odometrico[0])*odometrico[3];
      mouse_new=1;
      
      /*printf("(%d,%d) Canvas: Click on (%.2f,%.2f)\n",x,y,mouse_x,mouse_y);
	printf("robot_x=%.2f robot_y=%.2f robot_theta=%.2f\n",jde_robot[0],jde_robot[1],jde_robot[2]);*/
      
    }else if(mouse_button==MOUSEWHEELUP){

      /* a button has been pressed */
      canvas_mouse_button_pressed=1;

      /* modifing scale */
      rango-=1000;
      if(rango<=RANGO_MIN) rango=RANGO_MIN;
      fl_set_slider_value(fd_sensorsmotorsgui->escala,rango);
      visual_refresh = TRUE; /* activa el flag que limpia el fondo de la pantalla y repinta todo */ 
      escala = jde_width /rango;

    }else if(mouse_button==MOUSEWHEELDOWN){

      /* a button has been pressed */
      canvas_mouse_button_pressed=1;

      /* modifing scale */
      rango+=1000;
      if(rango>=RANGO_MAX) rango=RANGO_MAX;
      fl_set_slider_value(fd_sensorsmotorsgui->escala,rango);
      visual_refresh = TRUE; /* activa el flag que limpia el fondo de la pantalla y repinta todo */ 
      escala = jde_width /rango;
    }
  }

  return 0;
}

/* callback function for mouse motion inside the canvas object*/
int mouse_motion_on_micanvas(FL_OBJECT *ob, Window win, int win_width, int win_height, XEvent *xev, void *user_data)
{  
 
 unsigned int keymap;

  if(canvas_mouse_button_pressed==1){

    /* getting mouse coordenates. win will be always the canvas window, because this callback has been defined only for that canvas */  
    fl_get_win_mouse(win,&x_canvas,&y_canvas,&keymap);

    if(mouse_button==MOUSELEFT){
      if ((fl_get_button(fd_mastergui->vismotors)==PUSHED)&&(fl_get_button(fd_mastergui->motors)==PUSHED)){
      

	float sqrt_value,acos_value,new_w_value,new_v_value;
	
	/* robot is being moved using the canvas */
	robot_mouse_motion=1;
	
	/* getting difference between old and new coordenates */
	diff_x=(x_canvas-old_x_canvas);
	diff_y=(old_y_canvas-y_canvas);
	
	sqrt_value=sqrt((diff_x*diff_x)+(diff_y*diff_y));
	if(diff_y>=0) acos_value=acos(diff_x/sqrt_value);
	else acos_value=2*PI-acos(diff_x/sqrt_value);
	diff_w=jde_robot[2]-acos_value;
	
	/* shortest way to the robot theta*/
	if(diff_w>0){
	  
	  if(diff_w>=2*PI-diff_w){
	    if(2*PI-diff_w<=PI*0.7) new_w_value=RADTODEG*(diff_w)*(0.3);
	    else new_w_value=2.;

	  }else{
	    if(diff_w<=PI*0.7) new_w_value=RADTODEG*(diff_w)*(-0.3);
	    else new_w_value=-2.;
	  }
	  	  
	}else if(diff_w<0){
	  
	  /* changing signus to diff_w */
	  diff_w=diff_w*(-1);
	  if(diff_w>=2*PI-diff_w){
	    if(2*PI-diff_w<=PI*0.7) new_w_value=RADTODEG*(diff_w)*(-0.3);
	    else new_w_value=-2.;

	  }else{
	    if(diff_w<=2*PI-diff_w) new_w_value=RADTODEG*(diff_w)*(0.3);
	    else new_w_value=2;
	  }
	  
	}else new_w_value=0.;

	/* setting new value for v*/
	if((diff_w>=PI/2)&&(2*PI-diff_w>=PI/2)) new_v_value=(-1)*sqrt_value;
	else new_v_value=sqrt_value;

	/* checking values */
	if(new_w_value<-30.0) new_w_value=-30.0;
	else if(new_w_value>30.0) new_w_value=30.0;
	if(new_v_value<-200.0) new_v_value=-200.0;
	else if(new_v_value>200.0) new_v_value=200.0;

	w=new_w_value;
	v=new_v_value;

	if(new_v_value>=0){
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->joystick,new_v_value/200.0);
	}else{
	  fl_set_positioner_yvalue(fd_sensorsmotorsgui->joystick,new_v_value/-200.0);
	}
	fl_set_positioner_xvalue(fd_sensorsmotorsgui->joystick,(-1)*new_w_value/30.0+0.5);
      }
      
    }else if(mouse_button==MOUSERIGHT){

      /* getting difference between old and new coordenates */
      diff_x=(old_x_canvas-x_canvas);
      diff_y=(y_canvas-old_y_canvas);
      old_x_canvas=x_canvas;
      old_y_canvas=y_canvas;

      /* changing odometric range */
      odometrico[0]+=rango*(diff_x)*(0.005);
      odometrico[1]+=rango*(diff_y)*(0.005);
      visual_refresh=TRUE;
    }
  }

  return 0;
}

/* callback function for button released inside the canvas object*/
int button_released_on_micanvas(FL_OBJECT *ob, Window win, int win_width, int win_height, XEvent *xev, void *user_data)
{
  
  if(canvas_mouse_button_pressed==1){

    if(mouse_button==MOUSELEFT){
      if ((fl_get_button(fd_sensorsmotorsgui->joystick)==PUSHED)&&(fl_get_button(fd_mastergui->motors)==PUSHED)){
	/* robot is being stopped */
	robot_mouse_motion=1;

	/* stopping robot */
	v=0.;
	w=0.;
	fl_set_positioner_xvalue(fd_sensorsmotorsgui->joystick,0.5);
	fl_set_positioner_yvalue(fd_sensorsmotorsgui->joystick,0.0);
      }
    }

    /* a button has been released */
    canvas_mouse_button_pressed=0;
  }

  return 0;
}

/* it will handle the callbacks produced by ventanaA freeobject */
int ventanaA_callback_handler(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){

  /* this is a example of a callback function for the freeobject 'ventanaA'. in this example we have used
     three events: FL_PUSH (make a click with a button mouse or mouse wheel), FL_MOUSE (move mouse inside
     the freeobject area), and FL_RELEASE (release the click made previously if any click was made).

     to learn more about freeobjects and check some other events, take a look at the next site:

     * http://www.public.iastate.edu/~xforms/node2.html (see Freeobjects section)

     for the example, we have only use MOUSELEFT button. you can use MOUSERIGHT, MOUSEMIDDLE or MOUSEWHEELUP and
     MOUSEWHEELDOWN in your own callback function.

     uncomment the 'printf' statements to see the result of the callbacks.
  */

  int x,y;
  
  if((event==FL_PUSH)&&(ventanaA_mouse_button_pressed==0)){

    /* a button has been pressed */
    ventanaA_mouse_button_pressed=1;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaA->x+1;
      y=my-fd_sensorsmotorsgui->ventanaA->y+1;

      /*printf("ventanaA: click on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_MOUSE)&&(ventanaA_mouse_button_pressed==1)){

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaA->x+1;
      y=my-fd_sensorsmotorsgui->ventanaA->y+1;
      
      /*printf("ventanaA: mouse motion on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_RELEASE)&&(ventanaA_mouse_button_pressed==1)){

    /* a button has been released */
    ventanaA_mouse_button_pressed=0;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaA->x+1;
      y=my-fd_sensorsmotorsgui->ventanaA->y+1;

      /*printf("ventanaA: release on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }
  }

  return 0;
}

/* it will handle the callbacks produced by ventanaB freeobject */
int ventanaB_callback_handler(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){

  int x,y;
  
  if((event==FL_PUSH)&&(ventanaB_mouse_button_pressed==0)){

    /* a button has been pressed */
    ventanaB_mouse_button_pressed=1;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaB->x+1;
      y=my-fd_sensorsmotorsgui->ventanaB->y+1;

      /*printf("ventanaB: click on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_MOUSE)&&(ventanaB_mouse_button_pressed==1)){

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaB->x+1;
      y=my-fd_sensorsmotorsgui->ventanaB->y+1;
      
      /*printf("ventanaB: mouse motion on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_RELEASE)&&(ventanaB_mouse_button_pressed==1)){

    /* a button has been released */
    ventanaB_mouse_button_pressed=0;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaB->x+1;
      y=my-fd_sensorsmotorsgui->ventanaB->y+1;

      /*printf("ventanaB: release on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }
  }

  return 0;
}

/* it will handle the callbacks produced by ventanaC freeobject */
int ventanaC_callback_handler(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){

  int x,y;
  
  if((event==FL_PUSH)&&(ventanaC_mouse_button_pressed==0)){

    /* a button has been pressed */
    ventanaC_mouse_button_pressed=1;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaC->x+1;
      y=my-fd_sensorsmotorsgui->ventanaC->y+1;

      /*printf("ventanaC: click on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_MOUSE)&&(ventanaC_mouse_button_pressed==1)){

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaC->x+1;
      y=my-fd_sensorsmotorsgui->ventanaC->y+1;
      
      /*printf("ventanaC: mouse motion on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_RELEASE)&&(ventanaC_mouse_button_pressed==1)){

    /* a button has been released */
    ventanaC_mouse_button_pressed=0;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaC->x+1;
      y=my-fd_sensorsmotorsgui->ventanaC->y+1;

      /*printf("ventanaC: release on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }
  }

  return 0;
}

/* it will handle the callbacks produced by ventanaD freeobject */
int ventanaD_callback_handler(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){

  int x,y;
  
  if((event==FL_PUSH)&&(ventanaD_mouse_button_pressed==0)){

    /* a button has been pressed */
    ventanaD_mouse_button_pressed=1;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaD->x+1;
      y=my-fd_sensorsmotorsgui->ventanaD->y+1;

      /*printf("ventanaD: click on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_MOUSE)&&(ventanaD_mouse_button_pressed==1)){

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaD->x+1;
      y=my-fd_sensorsmotorsgui->ventanaD->y+1;
      
      /*printf("ventanaD: mouse motion on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }

  }else if((event==FL_RELEASE)&&(ventanaD_mouse_button_pressed==1)){

    /* a button has been released */
    ventanaD_mouse_button_pressed=0;

    if(key==MOUSELEFT){

      /* getting coordenates */
      x=mx-fd_sensorsmotorsgui->ventanaD->x+1;
      y=my-fd_sensorsmotorsgui->ventanaD->y+1;

      /*printf("ventanaD: release on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
    }
  }

  return 0;
}

void sensorsmotorsgui_suspend(void)
{
  if (sensorsmotorsgui_on==TRUE)
    {
      /* printf("sensorsmotorsgui suspend\n"); */
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

  if (obj == fd_sensorsmotorsgui->hide) 
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
  else if (obj == fd_sensorsmotorsgui->escala)  
    {  rango=fl_get_slider_value(fd_sensorsmotorsgui->escala);
    visual_refresh = TRUE; /* activa el flag que limpia el fondo de la pantalla y repinta todo */ 
    escala = jde_width /rango;}
  else if (obj == fd_sensorsmotorsgui->track_robot) 
    {if (fl_get_button(obj)==PUSHED) track_robot=TRUE;
    else track_robot=FALSE;
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
  int i;
  Tvoxel kaka;
 
  fl_winset(canvas_win); 
  
  if ((track_robot==TRUE)&&
      ((fabs(jde_robot[0]+odometrico[0])>(rango/4.))||
       (fabs(jde_robot[1]+odometrico[1])>(rango/4.))))
    {odometrico[0]=-jde_robot[0];
    odometrico[1]=-jde_robot[1];
    visual_refresh = TRUE;
    if (jdegui_debug) printf("gui: robot tracking, display movement\n"); 
    }
  
  
  if (iteracion_display==0) visual_refresh=TRUE;
  /* slow refresh of the complete sensorsmotors gui, needed because incremental refresh misses 
   window occlusions */
  
  if (visual_refresh==TRUE)
    {
      if (jdegui_debug) printf(" TOTAL ");
      fl_rectbound(0,0,jde_width,jde_height,FL_WHITE);   
      XFlush(display);
      /*XSync(display,True);*/
    }
  
  
  /* VISUALIZACION de una instantanea ultrasonica */
  if ((((display_state&DISPLAY_SONARS)!=0)&&(visual_refresh==FALSE))
      || (visual_delete_us==TRUE))
    {  
      fl_set_foreground(sensorsmotorsgui_gc,FL_WHITE); 
      /* clean last sonars, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      for(i=0;i<NUM_SONARS*2;i+=2) XDrawLine(display,canvas_win,sensorsmotorsgui_gc,us_dpy[i].x,us_dpy[i].y,us_dpy[i+1].x,us_dpy[i+1].y);
      
    }
  
  if ((display_state&DISPLAY_SONARS)!=0){
    if (jdegui_debug) printf(" sonars ");
    for(i=0;i<NUM_SONARS;i++)
      {us2xy(i,0.,0.,&kaka); /* Da en el Tvoxel kaka las coordenadas del sensor, pues es distancia 0 */
      xy2canvas(kaka,&us_dpy[2*i]);
      us2xy(i,us[i],0.,&kaka);
      /*us2xy(i,200,0.,&kaka);
	if (i==6) us2xy(i,400,0.,&kaka);*/
      xy2canvas(kaka,&us_dpy[2*i+1]);
      }
    fl_set_foreground(sensorsmotorsgui_gc,FL_PALEGREEN);
    for(i=0;i<NUM_SONARS*2;i+=2) XDrawLine(display,canvas_win,sensorsmotorsgui_gc,us_dpy[i].x,us_dpy[i].y,us_dpy[i+1].x,us_dpy[i+1].y);
  }
  
  /* VISUALIZACION de una instantanea laser*/
  if ((((display_state&DISPLAY_LASER)!=0)&&(visual_refresh==FALSE))
      || (visual_delete_laser==TRUE))
    {  
      fl_set_foreground(sensorsmotorsgui_gc,FL_WHITE); 
      /* clean last laser, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      /*for(i=0;i<NUM_LASER;i++) XDrawPoint(display,canvas_win,sensorsmotorsgui_gc,laser_dpy[i].x,laser_dpy[i].y);*/
      XDrawPoints(display,canvas_win,sensorsmotorsgui_gc,laser_dpy,NUM_LASER,CoordModeOrigin);
    }
  
  if ((display_state&DISPLAY_LASER)!=0){
    if (jdegui_debug) printf(" laser ");
    for(i=0;i<NUM_LASER;i++)
      {
	laser2xy(i,jde_laser[i],&kaka);
	xy2canvas(kaka,&laser_dpy[i]);
      }
    fl_set_foreground(sensorsmotorsgui_gc,FL_BLUE);
    /*for(i=0;i<NUM_LASER;i++) XDrawPoint(display,canvas_win,sensorsmotorsgui_gc,laser_dpy[i].x,laser_dpy[i].y);*/
    XDrawPoints(display,canvas_win,sensorsmotorsgui_gc,laser_dpy,NUM_LASER,CoordModeOrigin);
  }
  
  
  
  
  /* VISUALIZACION: pintar o borrar de el PROPIO ROBOT.
     Siempre hay un repintado total. Esta es la ultima estructura que se se pinta, para que ninguna otra se solape encima */
  
  if ((((display_state&DISPLAY_ROBOT)!=0) &&(visual_refresh==FALSE))
      || (visual_delete_ego==TRUE))
    {  
      fl_set_foreground(sensorsmotorsgui_gc,FL_WHITE); 
      /* clean last robot, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      for(i=0;i<numego;i++) XDrawLine(display,canvas_win,sensorsmotorsgui_gc,ego[i].x,ego[i].y,ego[i+1].x,ego[i+1].y);
      
    }
  
  if ((display_state&DISPLAY_ROBOT)!=0){
    if (jdegui_debug) printf(" ego ");
    fl_set_foreground(sensorsmotorsgui_gc,FL_MAGENTA);
    /* relleno los nuevos */
    us2xy(15,0.,0.,&kaka);
    xy2canvas(kaka,&ego[0]);
    us2xy(3,0.,0.,&kaka);
    xy2canvas(kaka,&ego[1]);
    us2xy(4,0.,0.,&kaka);
    xy2canvas(kaka,&ego[2]);
    us2xy(8,0.,0.,&kaka);
    xy2canvas(kaka,&ego[3]);
    us2xy(15,0.,0.,&kaka);
    xy2canvas(kaka,&ego[EGOMAX-1]);
    for(i=0;i<NUM_SONARS;i++)
      {
	us2xy((15+i)%NUM_SONARS,0.,0.,&kaka); /* Da en el Tvoxel kaka las coordenadas del sensor, pues es distancia 0 */
	xy2canvas(kaka,&ego[i+4]);       
      }
    
    /* pinto los nuevos */
    numego=EGOMAX-1;
    for(i=0;i<numego;i++) XDrawLine(display,canvas_win,sensorsmotorsgui_gc,ego[i].x,ego[i].y,ego[i+1].x,ego[i+1].y);
  }
  
  if (jdegui_debug) printf("\n");
  
  
  /* color imageA display */
  if ((display_state&DISPLAY_COLORIMAGEA)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenA_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenA_buf[i]= (unsigned char)tabla[(unsigned char)(colorA[i*3])];}}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
	    { imagenA_buf[i*2+1]=(0xf8&(colorA[i*3+2]))+((0xe0&(colorA[i*3+1]))>>5);
	    imagenA_buf[i*2]=((0xf8&(colorA[i*3]))>>3)+((0x1c&(colorA[i*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenA_buf[i*4]=colorA[i*3]; /* Blue Byte */
	  imagenA_buf[i*4+1]=colorA[i*3+1]; /* Green Byte */
	  imagenA_buf[i*4+2]=colorA[i*3+2]; /* Red Byte */
	  imagenA_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  
  /* color imageB display */
  if ((display_state&DISPLAY_COLORIMAGEB)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenB_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenB_buf[i]= (unsigned char)tabla[(unsigned char)(colorB[i*3])];}}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
	    { imagenB_buf[i*2+1]=(0xf8&(colorB[i*3+2]))+((0xe0&(colorB[i*3+1]))>>5);
	    imagenB_buf[i*2]=((0xf8&(colorB[i*3]))>>3)+((0x1c&(colorB[i*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenB_buf[i*4]=colorB[i*3]; /* Blue Byte */
	  imagenB_buf[i*4+1]=colorB[i*3+1]; /* Green Byte */
	  imagenB_buf[i*4+2]=colorB[i*3+2]; /* Red Byte */
	  imagenB_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  /* color imageC display */
  if ((display_state&DISPLAY_COLORIMAGEC)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenC_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenC_buf[i]= (unsigned char)tabla[(unsigned char)(colorC[i*3])];}}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
	    { imagenC_buf[i*2+1]=(0xf8&(colorC[i*3+2]))+((0xe0&(colorC[i*3+1]))>>5);
	    imagenC_buf[i*2]=((0xf8&(colorC[i*3]))>>3)+((0x1c&(colorC[i*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenC_buf[i*4]=colorC[i*3]; /* Blue Byte */
	  imagenC_buf[i*4+1]=colorC[i*3+1]; /* Green Byte */
	  imagenC_buf[i*4+2]=colorC[i*3+2]; /* Red Byte */
	  imagenC_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  /* color imageD display */
  if ((display_state&DISPLAY_COLORIMAGED)!=0)
    {
      /* Pasa de la imagen capturada a la imagen para visualizar (imagenD_buf), "cambiando" de formato adecuadamente */
      if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenD_buf[i]= (unsigned char)tabla[(unsigned char)(colorD[i*3])];}}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
	    { imagenD_buf[i*2+1]=(0xf8&(colorD[i*3+2]))+((0xe0&(colorD[i*3+1]))>>5);
	    imagenD_buf[i*2]=((0xf8&(colorD[i*3]))>>3)+((0x1c&(colorD[i*3+1]))<<3);
	    }
	}
      else if (((vmode==TrueColor)&&(fl_state[vmode].depth==24)) ||
	       ((vmode==TrueColor)&&(fl_state[vmode].depth==32)))
	{for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
	  { imagenD_buf[i*4]=colorD[i*3]; /* Blue Byte */
	  imagenD_buf[i*4+1]=colorD[i*3+1]; /* Green Byte */
	  imagenD_buf[i*4+2]=colorD[i*3+2]; /* Red Byte */
	  imagenD_buf[i*4+3]=0; /* dummy byte */  }
	}
    }
  
  if ((display_state&DISPLAY_COLORIMAGEA)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenA,0,0,fd_sensorsmotorsgui->ventanaA->x+1, fd_sensorsmotorsgui->ventanaA->y+1,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
    }
  
  if ((display_state&DISPLAY_COLORIMAGEB)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenB,0,0,fd_sensorsmotorsgui->ventanaB->x+1, fd_sensorsmotorsgui->ventanaB->y+1,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
    }
  
  if ((display_state&DISPLAY_COLORIMAGEC)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenC,0,0,fd_sensorsmotorsgui->ventanaC->x+1, fd_sensorsmotorsgui->ventanaC->y+1,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
    }
  
  if ((display_state&DISPLAY_COLORIMAGED)!=0)
    {    /* Draw screen onto display */
      XPutImage(display,sensorsmotorsgui_win,sensorsmotorsgui_gc,imagenD,0,0,fd_sensorsmotorsgui->ventanaD->x+1, fd_sensorsmotorsgui->ventanaD->y+1,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
    }
  
  /* clear all flags. If they were set at the beginning, they have been already used in this iteration */
  visual_refresh=FALSE;
  visual_delete_us=FALSE; 
  visual_delete_laser=FALSE; 
  visual_delete_ego=FALSE;
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
	visual_delete_us=TRUE;
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
	    visual_delete_us=TRUE;
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
	visual_delete_laser=TRUE;
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
	  visual_delete_laser=TRUE;
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
	visual_delete_ego=TRUE;
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
	  visual_delete_ego=TRUE;
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
	      fl_show_form(fd_mastergui->mastergui,FL_PLACE_POSITION,FL_FULLBORDER,"jde master gui");
	    }
	  else 
	    /*fl_set_form_position(fd_mastergui->mastergui,200,50);*/
	    fl_show_form(fd_mastergui->mastergui,FL_PLACE_POSITION,FL_FULLBORDER,"jde master gui");
	}
    }

  if (sensorsmotorsgui_request!=sensorsmotorsgui_on)
    {
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
	      sensorsmotorsgui_win = FL_ObjWin(fd_sensorsmotorsgui->ventanaA);
	      canvas_win = FL_ObjWin(fd_sensorsmotorsgui->micanvas);
	      image_displaysetup(); /* Tiene que ir despues de la inicializacion de Forms, pues hace uso de informacion que la libreria rellena en tiempo de ejecucion al iniciarse */

	      /* canvas handlers */
	      fl_add_canvas_handler(fd_sensorsmotorsgui->micanvas,ButtonPress,button_pressed_on_micanvas,NULL);
	      fl_add_canvas_handler(fd_sensorsmotorsgui->micanvas,ButtonRelease,button_released_on_micanvas,NULL);
	      fl_add_canvas_handler(fd_sensorsmotorsgui->micanvas,MotionNotify,mouse_motion_on_micanvas,NULL);
	    }
	  else 
	    fl_show_form(fd_sensorsmotorsgui->sensorsmotorsgui,FL_PLACE_POSITION,FL_FULLBORDER,"sensors and motors");
	  
	  sensorsmotorsgui_win = FL_ObjWin(fd_sensorsmotorsgui->ventanaA);
	  canvas_win = FL_ObjWin(fd_sensorsmotorsgui->micanvas);
	  /* the windows (sensorsmotorsgui_win and canvas_win) change every time the form is hided and showed again. They need to be updated before displaying anything again */
	  
	  /* Empiezo con el canvas en blanco */
	  jde_width = fd_sensorsmotorsgui->micanvas->w;
	  jde_height = fd_sensorsmotorsgui->micanvas->h;
	  fl_winset(canvas_win); 
	  fl_rectbound(0,0,jde_width,jde_height,FL_WHITE);   
	  XFlush(display);
	  
	  track_robot=TRUE;
	  fl_set_button(fd_sensorsmotorsgui->track_robot,PUSHED);
	  
	  fl_set_slider_bounds(fd_sensorsmotorsgui->escala,RANGO_MAX,RANGO_MIN);
	  fl_set_slider_filter(fd_sensorsmotorsgui->escala,rangoenmetros); /* Para poner el valor del slider en metros en pantalla */
	  fl_set_slider_value(fd_sensorsmotorsgui->escala,RANGO_INICIAL);
	  escala = jde_width/rango;
	  
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
      if (((display_state & BASE_TELEOPERATOR)!=0)&&(canvas_mouse_button_pressed==RELEASED))
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
  /*
    for(i=0;i<num_schemas;i++)
    {
    if (all[i].gui==TRUE)
    (*all[i].guidisplay)();
    }
  */
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

  /* prepara el display */
  aa=a;
  myargv=&aa;
  display= fl_initialize(&myargc,myargv,"JDE",0,0);
  screen = DefaultScreen(display);
 
  /*  sensorsmotorsgui_resume(); */
  /* Coord del sistema odometrico respecto del visual */
  odometrico[0]=0.;
  odometrico[1]=0.;
  odometrico[2]=0.;
  odometrico[3]= cos(0.);
  odometrico[4]= sin(0.);

  for(i=0;i<MAX_SCHEMAS;i++) state_dpy[i]=slept;

  jdegui_debug=0;
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

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

#include <stdlib.h>
#include <forms.h>
#include <math.h>
#include <unistd.h>
#include <jde.h>
#include <graphics_xforms.h>
#include <pioneer.h>
#include <introrob.h>
#include <introrobgui.h>
#include <navegacion.h>

int introrob_id=0; 
int introrob_brothers[MAX_SCHEMAS];
arbitration introrob_callforarbitration;
int introrob_cycle=100; /* ms */

/*Gui declarations*/
Display *mydisplay;
int  *myscreen;


/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;



char imagenRGB[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];
char **mycolorA=NULL;
runFn colorArun;
stopFn colorAstop;

float laser[NUM_LASER];
int *mylaser=NULL;
runFn laserrun;
stopFn laserstop;

float robot[5];
float *myencoders=NULL;
runFn encodersrun;
stopFn encodersstop;

float v;
float w;
float *myv=NULL;
float *myw=NULL;
runFn motorsrun;
stopFn motorsstop;

enum introrobstates {teleoperated,yourcode};
int introrob_state;

FD_introrobgui *fd_introrobgui=NULL;
GC introrobgui_gc;
GC introrobgui_colorA_gc;
Window  introrob_canvas_win;
Window  introrob_colorA_win;
int introrob_visual_refresh=FALSE;
int introrob_iteracion_display=0;
int introrob_canvas_mouse_button_pressed=0;
int introrob_mouse_button=0;
int introrob_robot_mouse_motion=0;
FL_Coord introrob_x_canvas,introrob_y_canvas,old_introrob_x_canvas,old_introrob_y_canvas;
float introrob_mouse_x, introrob_mouse_y;
Tvoxel vfftarget,oldvfftarget;
int introrob_mouse_new=0;

#define DISPLAY_ROBOT 0x01UL
#define DISPLAY_COLORA 0x02UL
#define DISPLAY_LASER 0x04UL
#define DISPLAY_MOTORS 0x04UL
unsigned long introrob_display_state=0;

#define PUSHED 1
#define RELEASED 0 
#define FORCED_REFRESH 5000 /* ms */
/*Every forced_refresh the display is drawn from scratch. If it is too small it will cause flickering with grid display. No merece la pena una hebra de "display_lento" solo para repintar completamente la pantalla. */

float   introrob_escala, introrob_width, introrob_height;
int introrob_trackrobot=FALSE;
float introrob_odometrico[5];
#define RANGO_MAX 20000. /* en mm */
#define RANGO_MIN 500. /* en mm */ 
#define RANGO_INICIAL 4000. /* en mm */
float introrob_rango=(float)RANGO_INICIAL; /* Rango de visualizacion en milimetros */

static int vmode;
static XImage *imagenA;
static unsigned char *imagenA_buf;
static long int tabla[256]; 
/* tabla con la traduccion de niveles de gris a numero de pixel en Pseudocolor-8bpp. Cada numero de pixel apunta al valor adecuado del ColorMap, con el color adecuado instalado */


#define EGOMAX NUM_SONARS+5
XPoint introrob_ego[EGOMAX];
int numintrorob_ego=0;
int visual_delete_introrob_ego=FALSE;

XPoint introrob_laser_dpy[NUM_LASER];
int visual_delete_introrob_laser=FALSE;

#define joystick_maxRotVel 30 /* deg/sec */
#define joystick_maxTranVel 500 /* mm/sec */
float v_teleop, w_teleop;


int xy2introrobcanvas(Tvoxel point, XPoint* grafico)
     /* return -1 if point falls outside the canvas */
{
float xi, yi;

xi = (point.x * introrob_odometrico[3] - point.y * introrob_odometrico[4] + introrob_odometrico[0])*introrob_escala;
yi = (point.x * introrob_odometrico[4] + point.y * introrob_odometrico[3] + introrob_odometrico[1])*introrob_escala;
/* Con esto cambiamos al sistema de referencia de visualizacion, centrado en algun punto xy y con alguna orientacion definidos por odometrico. Ahora cambiamos de ese sistema al del display, donde siempre hay un desplazamiento a la esquina sup. izda. y que las y se cuentan para abajo. */

grafico->x = xi + introrob_width/2;
grafico->y = -yi + introrob_height/2;

 if ((grafico->x <0)||(grafico->x>introrob_width)) return -1; 
 if ((grafico->y <0)||(grafico->y>introrob_height)) return -1; 
 return 0;
}

int absolutas2relativas(Tvoxel in, Tvoxel *out)
/*  Calcula la posicion relativa respecto del robot de un punto absoluto. El robot se encuentra en robot[0], robot[1] con orientacion robot[2] respecto al sistema de referencia absoluto
*/ 
{
  if (out!=NULL && myencoders!=NULL){
     (*out).x = in.x*myencoders[3] + in.y*myencoders[4] -
           myencoders[0]*myencoders[3] - myencoders[1]*myencoders[4];
     (*out).y = in.y*myencoders[3] - in.x*myencoders[4] +
           myencoders[0]*myencoders[4] - myencoders[1]*myencoders[3];
     return 0;
  }
  return 1;
}

int relativas2absolutas(Tvoxel in, Tvoxel *out)
/*  Calcula la posicion absoluta de un punto expresado en el sistema de coordenadas solidario al robot. El robot se encuentra en robot[0], robot[1] con orientacion robot[2] respecto al sistema de referencia absoluto
*/ 
{
  if (out!=NULL && myencoders!=NULL){
     (*out).x = in.x*myencoders[3] - in.y*myencoders[4] + myencoders[0];
     (*out).y = in.y*myencoders[3] + in.x*myencoders[4] + myencoders[1];
     return 0;
  }
  return 1;
}

int pintaSegmento(Tvoxel a, Tvoxel b, int color)
  /* colores: FL_PALEGREEN */
   {
     XPoint aa,bb;

     fl_set_foreground(introrobgui_gc,color);
     xy2introrobcanvas(a,&aa);
     xy2introrobcanvas(b,&bb);
     XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,aa.x,aa.y,bb.x,bb.y);
     return 0;
   }

void introrob_iteration()
{
  int i;

  speedcounter(introrob_id);
  /* printf("introrob iteration %d\n",d++);*/

  if (myencoders!=NULL){
     for (i=0;i<5; i++)
       robot[i]= myencoders[i];
  }
  
  if (mycolorA!=NULL){ 
    for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) { 
      imagenRGB[i*3]=(*mycolorA)[i*3]; /* Blue Byte */
      imagenRGB[i*3+1]=(*mycolorA)[i*3+1]; /* Green Byte */
      imagenRGB[i*3+2]=(*mycolorA)[i*3+2]; /* Red Byte */
    }
  }

  if (mylaser!=NULL){
    for (i=0; i<NUM_LASER; i++)
      laser[i]= mylaser[i];
  }
  
  if (introrob_state==teleoperated){
     v=v_teleop;
     w=w_teleop;
  }
  else if (introrob_state==yourcode) yourcode_iteration();
  else {v=0;w=0;}

  /*Mover los valores de la variables de trabajo a las variables actuadoras
    del robot que han sido importadas*/
  if (myv!=NULL)
     *myv=v;
  if (myw!=NULL)
     *myw=w;

}


void introrob_stop()
{
  pthread_mutex_lock(&(all[introrob_id].mymutex));
  put_state(introrob_id,slept);

  if (mycolorA!=NULL){
     all[introrob_id].children[(*(int *)myimport("colorA","id"))]=FALSE;
     colorAstop();
   }
  if (mylaser!=NULL){
    all[introrob_id].children[(*(int *)myimport("laser","id"))]=FALSE;
    laserstop();
  }
  if (myencoders!=NULL){
    all[introrob_id].children[(*(int *)myimport("encoders","id"))]=FALSE;
    encodersstop();
  }
  if ((myv!=NULL)&&(myw!=NULL)){
    all[introrob_id].children[(*(int *)myimport("motors","id"))]=FALSE;
    motorsstop();
  }
  printf("introrob: off\n");
  pthread_mutex_unlock(&(all[introrob_id].mymutex));
}


void introrob_run(int father, int *brothers, arbitration fn)
{
  int i;
 
  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[introrob_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[introrob_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[introrob_id].children[i]=FALSE;

  all[introrob_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) introrob_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {introrob_brothers[i]=brothers[i];i++;}
    }
  introrob_callforarbitration=fn;
  put_state(introrob_id,notready);

  introrob_display_state=0;
  mycolorA=myimport("colorA", "colorA");
  colorArun=(runFn)myimport("colorA", "run");
  colorAstop=(stopFn)myimport("colorA", "stop");
  if (mycolorA==NULL)
    {printf("No image source found. Check your jde configuration file. I go on anyway\n");}
  else introrob_display_state = introrob_display_state | DISPLAY_COLORA;  

  mylaser=(int *)myimport("laser","laser");
  laserrun=(runFn)myimport("laser","run");
  laserstop=(stopFn)myimport("laser","stop");
  if (mylaser==NULL)
    {printf("No laser source found. Check your jde configuration file. I go on anyway\n");}
  else introrob_display_state = introrob_display_state | DISPLAY_LASER;

  myencoders=(float *)myimport("encoders","jde_robot");
  encodersrun=(runFn)myimport("encoders","run");
  encodersstop=(stopFn)myimport("encoders","stop");
  if (myencoders==NULL)
    {printf("No encoders source found. Check your jde configuration file. I go on anyway\n");}
  else introrob_display_state = introrob_display_state | DISPLAY_ROBOT;

  myv=(float *)myimport("motors","v");
  myw=(float *)myimport("motors","w");
  motorsrun=(runFn)myimport("motors","run");
  motorsstop=(stopFn)myimport("motors","stop");
  if ((myv==NULL)||(myw==NULL)) 
    {printf("No motors found. Check your jde configuration file. I go on anyway\n");}
  else introrob_display_state = introrob_display_state | DISPLAY_MOTORS;

  printf("introrob: on\n");
  pthread_cond_signal(&(all[introrob_id].condition));
  pthread_mutex_unlock(&(all[introrob_id].mymutex));
}

void *introrob_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      pthread_mutex_lock(&(all[introrob_id].mymutex));

      if (all[introrob_id].state==slept) 
	{
	  v=0; w=0;
	  pthread_cond_wait(&(all[introrob_id].condition),&(all[introrob_id].mymutex));
	  pthread_mutex_unlock(&(all[introrob_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[introrob_id].state==notready) put_state(introrob_id,ready);
	  else if (all[introrob_id].state==ready)	  /* check brothers and arbitrate. For now this is the only winner */
	    { 
	      put_state(introrob_id,winner);
	      v=0; w=0; 
	      v_teleop=0; w_teleop=0;
	      /* start the winner state from controlled motor values */ 
	      if (mycolorA!=NULL){
		all[introrob_id].children[(*(int *)myimport("colorA","id"))]=TRUE;
		colorArun(introrob_id,NULL,NULL);
	      }
	      if (mylaser!=NULL){
		all[introrob_id].children[(*(int *)myimport("laser","id"))]=TRUE;
		laserrun(introrob_id,NULL,NULL);
	      }
	      if (myencoders!=NULL){
		all[introrob_id].children[(*(int *)myimport("encoders","id"))]=TRUE;
		encodersrun(introrob_id,NULL,NULL);
	      }
	      if ((myv!=NULL)&&(myw!=NULL)){
		all[introrob_id].children[(*(int *)myimport("motors","id"))]=TRUE;
		motorsrun(introrob_id,NULL,NULL);
	      }
	    }	  
	  else if (all[introrob_id].state==winner);

	  if (all[introrob_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[introrob_id].mymutex));
	      gettimeofday(&a,NULL);
	      introrob_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = introrob_cycle*1000-diff-10000; 
	      if (next>0) 
		/* discounts 10ms taken by calling usleep itself */
		usleep(introrob_cycle*1000-diff);
	      else 
		/* just let this iteration go away. overhead time negligible */
		{printf("time interval violated: introrob\n"); 
		usleep(introrob_cycle*1000);
		}
	    }
	  else 
	    {
	      pthread_mutex_unlock(&(all[introrob_id].mymutex));
	      usleep(introrob_cycle*1000);
	    }
	}
    }
}


void introrob_terminate()
{
  if (fd_introrobgui!=NULL)
    {
      if (all[introrob_id].guistate==on){
        introrob_hide();
        all[introrob_id].guistate=off;
      }
    }
  introrob_stop();
  printf ("introrob terminated\n");
}

void introrob_init(char *configfile)
{
  pthread_mutex_lock(&(all[introrob_id].mymutex));
  myexport("introrob","id",&introrob_id);
  myexport("introrob","run",(void *) &introrob_run);
  myexport("introrob","stop",(void *) &introrob_stop);
  printf("introrob schema started up\n");
  put_state(introrob_id,slept);
  pthread_create(&(all[introrob_id].mythread),NULL,introrob_thread,NULL);

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
    if ((myscreen=(int *)myimport("graphics_xforms", "screen"))==NULL){
      fprintf (stderr, "introrob: I can't fetch screen from graphics_xforms\n");
      jdeshutdown(1);
    }
    if ((mydisplay=(Display *)myimport("graphics_xforms", "display"))==NULL){
      fprintf (stderr, "introrob: I can't fetch display from graphics_xforms\n");
      jdeshutdown(1);
    }
  }
  init_pioneer();
  pthread_mutex_unlock(&(all[introrob_id].mymutex));
  introrob_state=teleoperated;
}

void introrob_guibuttons(void *obj1)
{
  double delta, deltapos;
  FL_OBJECT *obj=(FL_OBJECT *)obj1;
 
  if (obj == fd_introrobgui->exit) jdeshutdown(0);
  else if (obj == fd_introrobgui->escala)  
    {  introrob_rango=fl_get_slider_value(fd_introrobgui->escala);
    introrob_visual_refresh = TRUE; /* activa el flag que limpia el fondo de la pantalla y repinta todo */ 
    introrob_escala = introrob_width /introrob_rango;}
  else if (obj == fd_introrobgui->track_robot) 
    {if (fl_get_button(obj)==PUSHED) introrob_trackrobot=TRUE;
    else introrob_trackrobot=FALSE;
    } 
  else if (obj== fd_introrobgui->center)
    /* Se mueve 10%un porcentaje del rango */
    {
      introrob_odometrico[0]+=introrob_rango*(fl_get_positioner_xvalue(fd_introrobgui->center)-0.5)*(-2.)*(0.1);
      introrob_odometrico[1]+=introrob_rango*(fl_get_positioner_yvalue(fd_introrobgui->center)-0.5)*(-2.)*(0.1);
      fl_set_positioner_xvalue(fd_introrobgui->center,0.5);
      fl_set_positioner_yvalue(fd_introrobgui->center,0.5);
      introrob_visual_refresh=TRUE;  }
  else if (obj == fd_introrobgui->joystick) 
    {
      /* ROTACION=ejeX: Ajusta a un % de joystick_maxRotVel. OJO no funcion lineal del desplazamiento visual, sino con el al cuadrado, para aplanarla en el origen y evitar cabeceos, conseguir suavidad en la teleoperacion */
      delta = (fl_get_positioner_xvalue(fd_introrobgui->joystick)-0.5)*2; /* entre +-1 */
      deltapos = fabs(delta); /* Para que no moleste el signo de delta en el factor de la funcion de control */
      if (delta<0) w_teleop = (float) joystick_maxRotVel*deltapos*deltapos*deltapos; 
      else w_teleop = (float) -1.*joystick_maxRotVel*deltapos*deltapos*deltapos;
      
      /* TRASLACION=ejeY: Ajusta a un % de +-joystick_maxTranVel. OJO no funcion lineal del desplazamiento visual, sino con el a la cuarta, para aplanarla en el origen */
      delta = fl_get_positioner_yvalue(fd_introrobgui->joystick); /* entre 0 y 1 */
      if (fl_get_button(fd_introrobgui->back)==PUSHED)
	v_teleop = (float) -1.*joystick_maxTranVel*delta*delta*delta;
      else 
	v_teleop = (float) joystick_maxTranVel*delta*delta*delta;
    }    
  else if (obj == fd_introrobgui->back) ;
  else if (obj == fd_introrobgui->stop) 
    {
      fl_set_positioner_xvalue(fd_introrobgui->joystick,0.5);
      fl_set_positioner_yvalue(fd_introrobgui->joystick,0.);
      v_teleop=0.;
      w_teleop=0.;
    } 
  else if (obj == fd_introrobgui->yourcode)
    {
      if (fl_get_button(fd_introrobgui->yourcode)==PUSHED) introrob_state=yourcode;
    }
  else if (obj == fd_introrobgui->teleoperated);
  {
    if (fl_get_button(fd_introrobgui->teleoperated)==PUSHED) introrob_state=teleoperated;
    }
}

void introrob_guidisplay()
{
  char text[80]="";
  static int it=0;
  Tvoxel aa,bb;
  static XPoint targetgraf;
  XPoint a,b;
  int i,c,row,j;

  fl_redraw_object(fd_introrobgui->joystick);
  /*fl_redraw_object(fd_introrobgui->escala);*/

  /* slow refresh of the complete introrob gui, needed because incremental refresh misses window occlusions */
  if (introrob_iteracion_display*introrob_cycle>FORCED_REFRESH) 
    {introrob_iteracion_display=0;
    introrob_visual_refresh=TRUE;
    }
  else introrob_iteracion_display++;

  it=it+1;
  sprintf(text,"%d",it);
  fl_set_object_label(fd_introrobgui->fps,text);

  fl_winset(introrob_canvas_win); 
  
  
  if ((introrob_trackrobot==TRUE)&&
      ((fabs(myencoders[0]+introrob_odometrico[0])>(introrob_rango/4.))||
       (fabs(myencoders[1]+introrob_odometrico[1])>(introrob_rango/4.))))
 {
    introrob_odometrico[0]=-myencoders[0];
    introrob_odometrico[1]=-myencoders[1];
    introrob_visual_refresh = TRUE;
 }
 
 
 if (introrob_visual_refresh==TRUE){
    fl_rectbound(0,0,introrob_width,introrob_height,FL_WHITE);
    XFlush(mydisplay);
 }

  /* the grid at the floor */
  fl_set_foreground(introrobgui_gc,FL_LEFT_BCOL); 
  for(i=0;i<31;i++)
    {
      aa.x=-15000.+(float)i*1000;
      aa.y=-15000.;
      bb.x=-15000.+(float)i*1000;
      bb.y=15000.;
      xy2introrobcanvas(aa,&a);
      xy2introrobcanvas(bb,&b);
      XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,a.x,a.y,b.x,b.y);
      aa.y=-15000.+(float)i*1000;
      aa.x=-15000.;
      bb.y=-15000.+(float)i*1000;
      bb.x=15000.;
      xy2introrobcanvas(aa,&a);
      xy2introrobcanvas(bb,&b);
      XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,a.x,a.y,b.x,b.y);
    }
  /* fl_set_foreground(introrobgui_gc,FL_RIGHT_BCOL); */
  
  
  /* VISUALIZACION de una instantanea laser*/
  if ((((introrob_display_state&DISPLAY_LASER)!=0)&&(introrob_visual_refresh==FALSE))
      || (visual_delete_introrob_laser==TRUE))
    {  
      fl_set_foreground(introrobgui_gc,FL_WHITE); 
      /* clean last laser, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      /*for(i=0;i<NUM_LASER;i++) XDrawPoint(display,introrob_canvas_win,introrobgui_gc,introrob_laser_dpy[i].x,introrob_laser_dpy[i].y);*/
      XDrawPoints(mydisplay,introrob_canvas_win,introrobgui_gc,introrob_laser_dpy,NUM_LASER,CoordModeOrigin);
    }
 
  if ((introrob_display_state&DISPLAY_LASER)!=0){
    /* check to avoid the segmentation fault in case GUI is activated before the schema imports mylaser */
    if (mylaser!=NULL){
    for(i=0;i<NUM_LASER;i++)
      {
	laser2xy(i,mylaser[i],&aa, myencoders);
	xy2introrobcanvas(aa,&introrob_laser_dpy[i]);
      }
    fl_set_foreground(introrobgui_gc,FL_BLUE);
    /*for(i=0;i<NUM_LASER;i++) XDrawPoint(display,introrob_canvas_win,introrobgui_gc,introrob_laser_dpy[i].x,introrob_laser_dpy[i].y);*/
    XDrawPoints(mydisplay,introrob_canvas_win,introrobgui_gc,introrob_laser_dpy,NUM_LASER,CoordModeOrigin);
  }
  }

  
  /* VISUALIZACION: pintar o borrar de el PROPIO ROBOT.
     Siempre hay un repintado total. Esta es la ultima estructura que se se pinta, para que ninguna otra se solape encima */
  
  if ((((introrob_display_state&DISPLAY_ROBOT)!=0) &&(introrob_visual_refresh==FALSE))
      || (visual_delete_introrob_ego==TRUE))
    {  
      fl_set_foreground(introrobgui_gc,FL_WHITE); 
      /* clean last robot, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      for(i=0;i<numintrorob_ego;i++) XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,introrob_ego[i].x,introrob_ego[i].y,introrob_ego[i+1].x,introrob_ego[i+1].y);
      
    }
  
  if ((introrob_display_state&DISPLAY_ROBOT)!=0){
    fl_set_foreground(introrobgui_gc,FL_MAGENTA);
    /* relleno los nuevos */
    us2xy(15,0.,0.,&aa, myencoders);
    xy2introrobcanvas(aa,&introrob_ego[0]);
    us2xy(3,0.,0.,&aa, myencoders);
    xy2introrobcanvas(aa,&introrob_ego[1]);
    us2xy(4,0.,0.,&aa, myencoders);
    xy2introrobcanvas(aa,&introrob_ego[2]);
    us2xy(8,0.,0.,&aa, myencoders);
    xy2introrobcanvas(aa,&introrob_ego[3]);
    us2xy(15,0.,0.,&aa, myencoders);
    xy2introrobcanvas(aa,&introrob_ego[EGOMAX-1]);
    for(i=0;i<NUM_SONARS;i++)
      {
	us2xy((15+i)%NUM_SONARS,0.,0.,&aa, myencoders); /* Da en el Tvoxel aa las coordenadas del sensor, pues es distancia 0 */
	xy2introrobcanvas(aa,&introrob_ego[i+4]);       
      }
    
    /* pinto los nuevos */
    numintrorob_ego=EGOMAX-1;
    for(i=0;i<numintrorob_ego;i++) XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,introrob_ego[i].x,introrob_ego[i].y,introrob_ego[i+1].x,introrob_ego[i+1].y);
  }

  /* visualization of colorA */
  if ((introrob_display_state&DISPLAY_COLORA)!=0){
    /* Pasa de la imagen capturada a la imagen para visualizar (imagenA_buf),
       "cambiando" de formato adecuadamente */
    if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)){
      for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){
	c=i%(SIFNTSC_COLUMNS);
	row=i/(SIFNTSC_COLUMNS);
	j=row*SIFNTSC_COLUMNS+c;
	if (mycolorA!=NULL)
	  imagenA_buf[i]= (unsigned char)tabla[(unsigned char)((*mycolorA)[j*3])];
	else
	  imagenA_buf[i]= (unsigned char)tabla[(unsigned char)(0)];
      }
    }
    else if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)){
      for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){ 
	c=i%(SIFNTSC_COLUMNS);
	row=i/SIFNTSC_COLUMNS;
	j=row*SIFNTSC_COLUMNS+c;
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
        for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){
	  c=i%SIFNTSC_COLUMNS;
	  row=i/SIFNTSC_COLUMNS;
	  j=row*SIFNTSC_COLUMNS+c;
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

    /* Draw *myscreen onto display */
       XPutImage(mydisplay,introrob_colorA_win,introrobgui_colorA_gc,imagenA,0,0,fd_introrobgui->ventanaA->x+1, fd_introrobgui->ventanaA->y+1, SIFNTSC_COLUMNS, SIFNTSC_ROWS);
    
  }
  
 
  /* visualization of VFF target */
  if ((introrob_state==yourcode)||(introrob_state==teleoperated))
    {
      if ((oldvfftarget.x!=vfftarget.x)||(oldvfftarget.y!=vfftarget.y))
	/* the target has changed, do its last position must be cleaned from the canvas */
	{
	  fl_set_foreground(introrobgui_gc,FL_WHITE);
	  XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,targetgraf.x-5,targetgraf.y,targetgraf.x+5,targetgraf.y);
	  XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,targetgraf.x,targetgraf.y-5,targetgraf.x,targetgraf.y+5);
	}
      fl_set_foreground(introrobgui_gc,FL_RED);
      xy2introrobcanvas(vfftarget,&targetgraf);
      XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,targetgraf.x-5,targetgraf.y,targetgraf.x+5,targetgraf.y);
      XDrawLine(mydisplay,introrob_canvas_win,introrobgui_gc,targetgraf.x,targetgraf.y-5,targetgraf.x,targetgraf.y+5);
    }
  

   /* clear all flags. If they were set at the beginning, they have been already used in this iteration */
  introrob_visual_refresh=FALSE;
  visual_delete_introrob_laser=FALSE; 
  visual_delete_introrob_ego=FALSE;

  visualizacion(); 
}


/* callback function for button pressed inside the canvas object*/
int introrob_button_pressed_on_micanvas(FL_OBJECT *ob, Window win, int win_width, int win_height, XEvent *xev, void *user_data)
{
  unsigned int keymap;
  float ygraf, xgraf;
  FL_Coord x,y;

  /* in order to know the mouse button that created the event */
  introrob_mouse_button=xev->xkey.keycode;
  if(introrob_canvas_mouse_button_pressed==0){
    if (introrob_mouse_button==MOUSELEFT)
      {
	/* getting mouse coordenates. win will be always the canvas window, because this callback has been defined only for that canvas */  
      fl_get_win_mouse(win,&x,&y,&keymap);
      /* from graphical coordinates to spatial ones */
      ygraf=((float) (introrob_height/2-y))/introrob_escala;
      xgraf=((float) (x-introrob_width/2))/introrob_escala;

      /* Target for VFF navigation getting mouse coordenates */
      oldvfftarget.x=vfftarget.x;
      oldvfftarget.y=vfftarget.y;
      vfftarget.y=(ygraf-introrob_odometrico[1])*introrob_odometrico[3]+(-xgraf+introrob_odometrico[0])*introrob_odometrico[4];
      vfftarget.x=(ygraf-introrob_odometrico[1])*introrob_odometrico[4]+(xgraf-introrob_odometrico[0])*introrob_odometrico[3];
      introrob_mouse_y=vfftarget.y;
      introrob_mouse_x=vfftarget.x;
      introrob_mouse_new=1;
      }
    else if ((introrob_mouse_button==MOUSERIGHT)||(introrob_mouse_button==MOUSEMIDDLE))
      {
	introrob_canvas_mouse_button_pressed=1;
	/* For canvas displacement on 2D world (mouseright) or teleoperation (mousemiddle) */
	fl_get_win_mouse(win,&introrob_x_canvas,&introrob_y_canvas,&keymap);
	old_introrob_x_canvas=introrob_x_canvas;
	old_introrob_y_canvas=introrob_y_canvas;
      }
    else if(introrob_mouse_button==MOUSEWHEELDOWN){
      /* a button has been pressed */
      introrob_canvas_mouse_button_pressed=1;

      /* modifing scale of the visualization window */
      introrob_rango-=1000;
      if(introrob_rango<=RANGO_MIN) introrob_rango=RANGO_MIN;
      fl_set_slider_value(fd_introrobgui->escala,introrob_rango);
      introrob_visual_refresh = TRUE; /* activa el flag que limpia el fondo de la pantalla y repinta todo */ 
      introrob_escala = introrob_width /introrob_rango;

    }else if(introrob_mouse_button==MOUSEWHEELUP){
      /* a button has been pressed */
      introrob_canvas_mouse_button_pressed=1;

      /* modifing scale of the visualization window */
      introrob_rango+=1000;
      if(introrob_rango>=RANGO_MAX) introrob_rango=RANGO_MAX;
      fl_set_slider_value(fd_introrobgui->escala,introrob_rango);
      introrob_visual_refresh = TRUE; /* activa el flag que limpia el fondo de la pantalla y repinta todo */ 
      introrob_escala = introrob_width /introrob_rango;
    }
  }

  return 0;
}

/* callback function for mouse motion inside the canvas object*/
int introrob_mouse_motion_on_micanvas(FL_OBJECT *ob, Window win, int win_width, int win_height, XEvent *xev, void *user_data)
{  
  float diff_x,diff_y,diff_w;
  unsigned int keymap;
  float sqrt_value,acos_value;

  if(introrob_canvas_mouse_button_pressed==1){

    /* getting mouse coordenates. win will be always the canvas window, because this callback has been defined only for that canvas */  
    fl_get_win_mouse(win,&introrob_x_canvas,&introrob_y_canvas,&keymap);

    if(introrob_mouse_button==MOUSEMIDDLE){
      if (introrob_state==teleoperated)
	{
	/* robot is being moved using the canvas */
	introrob_robot_mouse_motion=1;
	
	/* getting difference between old and new coordenates */
	diff_x=(introrob_x_canvas-old_introrob_x_canvas);
	diff_y=(old_introrob_y_canvas-introrob_y_canvas);
	
	sqrt_value=sqrt((diff_x*diff_x)+(diff_y*diff_y));
	if(diff_y>=0) acos_value=acos(diff_x/sqrt_value);
	else acos_value=2*PI-acos(diff_x/sqrt_value);
	diff_w=myencoders[2]-acos_value;
	
	/* shortest way to the robot theta*/
	if(diff_w>0){
	  if(diff_w>=2*PI-diff_w){
	    if(2*PI-diff_w<=PI*0.7) w_teleop=RADTODEG*(diff_w)*(0.3);
	    else w_teleop=2.;
	    
	  }else{
	    if(diff_w<=PI*0.7) w_teleop=RADTODEG*(diff_w)*(-0.3);
	    else w_teleop=-2.;
	  }
	}else if(diff_w<0){
	  /* changing signus to diff_w */
	  diff_w=diff_w*(-1);
	  if(diff_w>=2*PI-diff_w){
	    if(2*PI-diff_w<=PI*0.7) w_teleop=RADTODEG*(diff_w)*(-0.3);
	    else w_teleop=-2.;
	  }else{
	    if(diff_w<=2*PI-diff_w) w_teleop=RADTODEG*(diff_w)*(0.3);
	    else w_teleop=2;
	  }	  
	}else w_teleop=0.;
	if(w_teleop<-joystick_maxRotVel) w_teleop=-joystick_maxRotVel;
	else if(w_teleop>joystick_maxRotVel) w_teleop=joystick_maxRotVel;

	/* setting new value for v */
	if((diff_w>=PI/2)&&(2*PI-diff_w>=PI/2)) v_teleop=(-1)*sqrt_value;
	else v_teleop=sqrt_value;
	if(v_teleop<-joystick_maxTranVel) v_teleop=-joystick_maxTranVel;
	else if(v_teleop>joystick_maxTranVel) v_teleop=joystick_maxTranVel;

	/* updating the joystick at GUI */
	if(v_teleop>=0){
	  fl_set_positioner_yvalue(fd_introrobgui->joystick,powf(v_teleop/joystick_maxTranVel,1/3.));
	}else{
	  fl_set_positioner_yvalue(fd_introrobgui->joystick,powf(v_teleop/-joystick_maxTranVel,1/3.));
	}
	if(w_teleop>=0){
	  fl_set_positioner_xvalue(fd_introrobgui->joystick,(powf(w_teleop/joystick_maxRotVel,1/3.)/2.)+0.5);
	}else {
	  fl_set_positioner_xvalue(fd_introrobgui->joystick,(powf(-w_teleop/joystick_maxRotVel,1/3.)/2.)+0.5);
	}
      }
      
    }else if(introrob_mouse_button==MOUSERIGHT){
      /* getting difference between old and new coordenates */
      diff_x=(old_introrob_x_canvas-introrob_x_canvas);
      diff_y=(introrob_y_canvas-old_introrob_y_canvas);
      old_introrob_x_canvas=introrob_x_canvas;
      old_introrob_y_canvas=introrob_y_canvas;

      /* changing the visualization window position */
      introrob_odometrico[0]+=introrob_rango*(diff_x)*(0.005);
      introrob_odometrico[1]+=introrob_rango*(diff_y)*(0.005);
      introrob_visual_refresh=TRUE;
    }
  }

  return 0;
}

/* callback function for button released inside the canvas object*/
int introrob_button_released_on_micanvas(FL_OBJECT *ob, Window win, int win_width, int win_height, XEvent *xev, void *user_data)
{
  
  if(introrob_canvas_mouse_button_pressed==1){

    if(introrob_mouse_button==MOUSEMIDDLE){
      if (introrob_state==teleoperated){
	/* robot is being stopped */
	introrob_robot_mouse_motion=1;

	/* stopping robot */
	v_teleop=0.;
	w_teleop=0.;
	fl_set_positioner_xvalue(fd_introrobgui->joystick,0.5);
	fl_set_positioner_yvalue(fd_introrobgui->joystick,0.0);
      }
    }

    /* a button has been released */
    introrob_canvas_mouse_button_pressed=0;
  }

  return 0;
}


void introrob_hide_aux(void)
{
  v_teleop=0; w_teleop=0; 
  all[introrob_id].guistate=off;
  /* to make a safety stop when the robot is being teleoperated from GUI */
  mydelete_buttonscallback(introrob_guibuttons);
  mydelete_displaycallback(introrob_guidisplay);
  fl_hide_form(fd_introrobgui->introrobgui);
}

void introrob_hide(){
   static callback fn=NULL;

   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)introrob_hide_aux);
      }
   }
   else{
      fn ((gui_function)introrob_hide_aux);
   }
}

int myclose_form(FL_FORM *form, void *an_argument)
{
  introrob_hide();
  return FL_IGNORE;
}


void introrob_show_aux(void)
{
  static int k=0;
  XGCValues gc_values;
  XWindowAttributes win_attributes;
  XColor nuevocolor;
  int pixelNum, numCols;
  int allocated_colors=0, non_allocated_colors=0;

  all[introrob_id].guistate=on;
  if (k==0) /* not initialized */
    {
      k++;

      /* Coord del sistema odometrico respecto del visual */
      introrob_odometrico[0]=0.;
      introrob_odometrico[1]=0.;
      introrob_odometrico[2]=0.;
      introrob_odometrico[3]= cos(0.);
      introrob_odometrico[4]= sin(0.);

      fd_introrobgui = create_form_introrobgui();
      fl_set_form_position(fd_introrobgui->introrobgui,400,50);
      fl_show_form(fd_introrobgui->introrobgui,FL_PLACE_POSITION,FL_FULLBORDER,"introrob");
      fl_set_form_atclose(fd_introrobgui->introrobgui,myclose_form,0);

      introrob_canvas_win= FL_ObjWin(fd_introrobgui->micanvas);
      gc_values.graphics_exposures = False;
      introrobgui_gc = XCreateGC(mydisplay, introrob_canvas_win, GCGraphicsExposures, &gc_values);  


      /* Utilizan el Visual (=estructura de color) y el colormap con que este operando el programa principal con su Xforms. No crea un nuevo colormap, sino que modifica el que se estaba usando a traves de funciones de Xforms*/
 /* Tiene que ir despues de la inicializacion de Forms, pues hace uso de informacion que la libreria rellena en tiempo de ejecucion al iniciarse */
      introrob_colorA_win= FL_ObjWin(fd_introrobgui->ventanaA);
      XGetWindowAttributes(mydisplay, introrob_colorA_win, &win_attributes);
      XMapWindow(mydisplay, introrob_colorA_win);
      /*XSelectInput(display, introrob_colorA_win, ButtonPress|StructureNotifyMask);*/   
      gc_values.graphics_exposures = False;
      introrobgui_colorA_gc = XCreateGC(mydisplay, introrob_colorA_win, GCGraphicsExposures, &gc_values);

      vmode= fl_get_vclass();
      if ((vmode==TrueColor)&&(fl_state[vmode].depth==16)) 
	{printf("introrob: truecolor 16 bpp\n");
	  imagenA_buf = (unsigned char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*2);    
	  imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),win_attributes.depth, ZPixmap,0,(char *)imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
	}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24)) 
	{ printf("introrob: truecolor 24 bpp\n");
	  imagenA_buf = (unsigned char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
	  imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,(char *)imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
	}
      else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32)) 
	{ printf("introrob: truecolor 24 bpp\n");
	  imagenA_buf = (unsigned char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*4); 
	  imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,(char *)imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
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
	  printf("introrob: depth= %d\n", fl_state[vmode].depth); 
	  printf("introrob: colormap got %d colors, %d non_allocated colors\n",allocated_colors,non_allocated_colors);
	  
	  imagenA_buf = (unsigned char *) malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS);    
	  imagenA = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,(char *)imagenA_buf,SIFNTSC_COLUMNS, SIFNTSC_ROWS,8,0);
	}
      else 
	{
	  perror("Unsupported color mode in X server");exit(1);
	}


      /* canvas handlers */
      fl_add_canvas_handler(fd_introrobgui->micanvas,ButtonPress,introrob_button_pressed_on_micanvas,NULL);
      fl_add_canvas_handler(fd_introrobgui->micanvas,ButtonRelease,introrob_button_released_on_micanvas,NULL);
      fl_add_canvas_handler(fd_introrobgui->micanvas,MotionNotify,introrob_mouse_motion_on_micanvas,NULL);

      /* initial values */
      introrob_width = fd_introrobgui->micanvas->w;
      introrob_height = fd_introrobgui->micanvas->h;
      introrob_trackrobot=TRUE;
      fl_set_button(fd_introrobgui->track_robot,PUSHED);
      fl_set_slider_bounds(fd_introrobgui->escala,RANGO_MAX,RANGO_MIN);
      fl_set_slider_value(fd_introrobgui->escala,RANGO_INICIAL);
      introrob_escala = introrob_width/introrob_rango;
      introrob_state=teleoperated;
      fl_set_button(fd_introrobgui->teleoperated,PUSHED); 
   }
  else 
    {
      fl_show_form(fd_introrobgui->introrobgui,FL_PLACE_POSITION,FL_FULLBORDER,"introrob");
      introrob_canvas_win= FL_ObjWin(fd_introrobgui->micanvas);
      introrob_colorA_win = FL_ObjWin(fd_introrobgui->ventanaA);
  /* the window (introrob_colorA_win) changes every time the form is hided and showed again. They need to be updated before displaying anything again */
    }

  /* Empiezo con el canvas en blanco */
  introrob_width = fd_introrobgui->micanvas->w;
  introrob_height = fd_introrobgui->micanvas->h;
  fl_winset(introrob_canvas_win); 
  fl_rectbound(0,0,introrob_width,introrob_height,FL_WHITE);   
  /*  XFlush(display);*/
  
  fl_set_slider_value(fd_introrobgui->escala,introrob_rango);

  fl_set_positioner_xvalue(fd_introrobgui->joystick,0.5);
  fl_set_positioner_yvalue(fd_introrobgui->joystick,0.);
  v_teleop=0.;
  w_teleop=0.;

  fl_set_positioner_xvalue(fd_introrobgui->center,0.5);
  fl_set_positioner_yvalue(fd_introrobgui->center,0.5);

  fl_redraw_form(fd_introrobgui->introrobgui);

  myregister_buttonscallback(introrob_guibuttons);
  myregister_displaycallback(introrob_guidisplay);
}

void introrob_show(){
   static callback fn=NULL;

   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)introrob_show_aux);
      }
   }
   else{
      fn ((gui_function)introrob_show_aux);
   }
}

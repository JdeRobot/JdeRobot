#include "jde.h"
#include "jdegui.h"
#include "introrobgui.h"

#define DISPLAY_ROBOT 0x01UL
#define DISPLAY_SONARS 0x04UL
#define DISPLAY_LASER 0x08UL
#define BASE_TELEOPERATOR 0x100UL

int introrob_id=0; 
int introrob_brothers[MAX_SCHEMAS];
arbitration introrob_callforarbitration;
int introrob_cycle=100; /* ms */

FD_introrobgui *fd_introrobgui;
GC introrobgui_gc;
Window  canvas_win;
unsigned long display_state;
int visual_refresh=FALSE;
int iteracion_display=0;

#define PUSHED 1
#define RELEASED 0 
#define FORCED_REFRESH 5000 /* ms */
/*Every forced_refresh the display is drawn from scratch. If it is too small it will cause flickering with grid display. No merece la pena una hebra de "display_lento" solo para repintar completamente la pantalla. */

char fpstext[80]="";

float   escala, jde_width, jde_height;
int track_robot=FALSE;
float odometrico[5];
#define RANGO_MAX 20000. /* en mm */
#define RANGO_MIN 500. /* en mm */ 
#define RANGO_INICIAL 4000. /* en mm */
float rango=(float)RANGO_INICIAL; /* Rango de visualizacion en milimetros */

#define EGOMAX NUM_SONARS+5
XPoint ego[EGOMAX];
float last_heading; /* ultima orientacion visualizada del robot */
int numego=0;
int visual_delete_ego=FALSE;

XPoint laser_dpy[NUM_LASER];
int visual_delete_laser=FALSE;

XPoint us_dpy[NUM_SONARS*2];
int visual_delete_us=FALSE;

#define joystick_maxRotVel 30 /* deg/sec */
#define joystick_maxTranVel 500 /* mm/sec */
float joystick_x, joystick_y;

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


void introrob_iteration()
{  
  static int d=0;
  double delta, deltapos;

  speedcounter(introrob_id);
  /*printf("introrob iteration %d\n",d++);*/

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


void introrob_suspend()
{
  pthread_mutex_lock(&(all[introrob_id].mymutex));
  put_state(introrob_id,slept);
  printf("introrob: off\n");
  pthread_mutex_unlock(&(all[introrob_id].mymutex));
}


void introrob_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[introrob_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[introrob_id].mymutex));
  /* this schema resumes its execution with no children at all */
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
	  else all[introrob_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[introrob_id].state==ready) put_state(introrob_id,winner);


	  if (all[introrob_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[introrob_id].mymutex));
	      gettimeofday(&a,NULL);
	      introrob_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = introrob_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(introrob_cycle*1000-diff);
	      else 
		{printf("time interval violated: introrob\n"); usleep(introrob_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[introrob_id].mymutex));
	      usleep(introrob_cycle*1000);
	    }
	}
    }
}

void introrob_startup()
{
  pthread_mutex_lock(&(all[introrob_id].mymutex));
  printf("introrob schema started up\n");
  put_state(introrob_id,slept);
  pthread_create(&(all[introrob_id].mythread),NULL,introrob_thread,NULL);
  pthread_mutex_unlock(&(all[introrob_id].mymutex));
}

void introrob_guibuttons(FL_OBJECT *obj)
{
  if (obj == fd_introrobgui->exit) jdeshutdown(0);
  else if (obj == fd_introrobgui->escala)  
    {  rango=fl_get_slider_value(fd_introrobgui->escala);
    visual_refresh = TRUE; /* activa el flag que limpia el fondo de la pantalla y repinta todo */ 
    escala = jde_width /rango;}
  else if (obj == fd_introrobgui->track_robot) 
    {if (fl_get_button(obj)==PUSHED) track_robot=TRUE;
    else track_robot=FALSE;
    } 
  else if (obj== fd_introrobgui->center)
    /* Se mueve 10%un porcentaje del rango */
    {
      odometrico[0]+=rango*(fl_get_positioner_xvalue(fd_introrobgui->center)-0.5)*(-2.)*(0.1);
      odometrico[1]+=rango*(fl_get_positioner_yvalue(fd_introrobgui->center)-0.5)*(-2.)*(0.1);
      fl_set_positioner_xvalue(fd_introrobgui->center,0.5);
      fl_set_positioner_yvalue(fd_introrobgui->center,0.5);
      visual_refresh=TRUE;  }
  else if (obj == fd_introrobgui->joystick) 
    {
      if ((display_state & BASE_TELEOPERATOR)!=0) 
	{
	  if (fl_get_button(fd_introrobgui->back)==RELEASED)
	    joystick_y=0.5+0.5*fl_get_positioner_yvalue(fd_introrobgui->joystick);
	  else 
	    joystick_y=0.5-0.5*fl_get_positioner_yvalue(fd_introrobgui->joystick);
	  joystick_x=fl_get_positioner_xvalue(fd_introrobgui->joystick);
	  fl_redraw_object(fd_introrobgui->joystick);
	}
    }    
   else if (obj == fd_introrobgui->back) 
    {
      if (fl_get_button(fd_introrobgui->back)==RELEASED)
	joystick_y=0.5+0.5*fl_get_positioner_yvalue(fd_introrobgui->joystick);
      else 
	joystick_y=0.5-0.5*fl_get_positioner_yvalue(fd_introrobgui->joystick);
      joystick_x=fl_get_positioner_xvalue(fd_introrobgui->joystick);
      fl_redraw_object(fd_introrobgui->joystick);
    }    
  else if (obj == fd_introrobgui->stop) 
    {
      fl_set_positioner_xvalue(fd_introrobgui->joystick,0.5);
      fl_set_positioner_yvalue(fd_introrobgui->joystick,0.);
      joystick_x=0.5;
      joystick_y=0.5;
    }     
}

void introrob_guidisplay()
{
  char text[80]="";
  static float k=0;
  int i;
  Tvoxel kaka;

  /* slow refresh of the complete introrob gui, needed because incremental refresh misses window occlusions */
  if (iteracion_display*introrob_cycle>FORCED_REFRESH) 
    {iteracion_display=0;
    visual_refresh=TRUE;
    }
  else iteracion_display++;


  k=k+1.;
  sprintf(text,"%.1f",k);
  fl_set_object_label(fd_introrobgui->fps,text);

  fl_winset(canvas_win); 
  
  if ((track_robot==TRUE)&&
      ((fabs(jde_robot[0]+odometrico[0])>(rango/4.))||
       (fabs(jde_robot[1]+odometrico[1])>(rango/4.))))
    {odometrico[0]=-jde_robot[0];
    odometrico[1]=-jde_robot[1];
    visual_refresh = TRUE;
    }
    
 
  if (visual_refresh==TRUE)
    {
      fl_rectbound(0,0,jde_width,jde_height,FL_WHITE);   
      XFlush(display);
    }
  
  
  /* VISUALIZACION de una instantanea ultrasonica */
  if ((((display_state&DISPLAY_SONARS)!=0)&&(visual_refresh==FALSE))
      || (visual_delete_us==TRUE))
    {  
      fl_set_foreground(introrobgui_gc,FL_WHITE); 
      /* clean last sonars, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      for(i=0;i<NUM_SONARS*2;i+=2) XDrawLine(display,canvas_win,introrobgui_gc,us_dpy[i].x,us_dpy[i].y,us_dpy[i+1].x,us_dpy[i+1].y);
      
    }
  
  if ((display_state&DISPLAY_SONARS)!=0){
    for(i=0;i<NUM_SONARS;i++)
      {us2xy(i,0.,0.,&kaka); /* Da en el Tvoxel kaka las coordenadas del sensor, pues es distancia 0 */
      xy2canvas(kaka,&us_dpy[2*i]);
      us2xy(i,us[i],0.,&kaka);
      /*us2xy(i,200,0.,&kaka);
	if (i==6) us2xy(i,400,0.,&kaka);*/
      xy2canvas(kaka,&us_dpy[2*i+1]);
      }
    fl_set_foreground(introrobgui_gc,FL_PALEGREEN);
    for(i=0;i<NUM_SONARS*2;i+=2) XDrawLine(display,canvas_win,introrobgui_gc,us_dpy[i].x,us_dpy[i].y,us_dpy[i+1].x,us_dpy[i+1].y);
  }
  
  /* VISUALIZACION de una instantanea laser*/
  if ((((display_state&DISPLAY_LASER)!=0)&&(visual_refresh==FALSE))
      || (visual_delete_laser==TRUE))
    {  
      fl_set_foreground(introrobgui_gc,FL_WHITE); 
      /* clean last laser, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      /*for(i=0;i<NUM_LASER;i++) XDrawPoint(display,canvas_win,introrobgui_gc,laser_dpy[i].x,laser_dpy[i].y);*/
      XDrawPoints(display,canvas_win,introrobgui_gc,laser_dpy,NUM_LASER,CoordModeOrigin);
    }
  
  if ((display_state&DISPLAY_LASER)!=0){
    for(i=0;i<NUM_LASER;i++)
      {
	laser2xy(i,jde_laser[i],&kaka);
	xy2canvas(kaka,&laser_dpy[i]);
      }
    fl_set_foreground(introrobgui_gc,FL_BLUE);
    /*for(i=0;i<NUM_LASER;i++) XDrawPoint(display,canvas_win,introrobgui_gc,laser_dpy[i].x,laser_dpy[i].y);*/
    XDrawPoints(display,canvas_win,introrobgui_gc,laser_dpy,NUM_LASER,CoordModeOrigin);
  }
  
  
  /* VISUALIZACION: pintar o borrar de el PROPIO ROBOT.
     Siempre hay un repintado total. Esta es la ultima estructura que se se pinta, para que ninguna otra se solape encima */
  
  if ((((display_state&DISPLAY_ROBOT)!=0) &&(visual_refresh==FALSE))
      || (visual_delete_ego==TRUE))
    {  
      fl_set_foreground(introrobgui_gc,FL_WHITE); 
      /* clean last robot, but only if there wasn't a total refresh. In case of total refresh the white rectangle already cleaned all */
      for(i=0;i<numego;i++) XDrawLine(display,canvas_win,introrobgui_gc,ego[i].x,ego[i].y,ego[i+1].x,ego[i+1].y);
      
    }
  
  if ((display_state&DISPLAY_ROBOT)!=0){
    fl_set_foreground(introrobgui_gc,FL_MAGENTA);
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
    for(i=0;i<numego;i++) XDrawLine(display,canvas_win,introrobgui_gc,ego[i].x,ego[i].y,ego[i+1].x,ego[i+1].y);
  }

   /* clear all flags. If they were set at the beginning, they have been already used in this iteration */
  visual_refresh=FALSE;
  visual_delete_us=FALSE; 
  visual_delete_laser=FALSE; 
  visual_delete_ego=FALSE;
}


void introrob_guisuspend(void)
{
  delete_buttonscallback(introrob_guibuttons);
  delete_displaycallback(introrob_guidisplay);
  fl_hide_form(fd_introrobgui->introrobgui);
}

void introrob_guiresume(void)
{
  static int k=0;
  XGCValues gc_values;

  if (k==0) /* not initialized */
    {
      k++;

      /* Coord del sistema odometrico respecto del visual */
      odometrico[0]=0.;
      odometrico[1]=0.;
      odometrico[2]=0.;
      odometrico[3]= cos(0.);
      odometrico[4]= sin(0.);

      display_state = display_state | DISPLAY_LASER;
      display_state = display_state | DISPLAY_SONARS;
      display_state = display_state | DISPLAY_ROBOT;
      display_state = display_state | BASE_TELEOPERATOR;

      fd_introrobgui = create_form_introrobgui();
      fl_set_form_position(fd_introrobgui->introrobgui,400,50);
      fl_show_form(fd_introrobgui->introrobgui,FL_PLACE_POSITION,FL_FULLBORDER,"introrob");
      canvas_win= FL_ObjWin(fd_introrobgui->micanvas);
      gc_values.graphics_exposures = False;
      introrobgui_gc = XCreateGC(display, canvas_win, GCGraphicsExposures, &gc_values);  
    }
  else 
    {
      fl_show_form(fd_introrobgui->introrobgui,FL_PLACE_POSITION,FL_FULLBORDER,"introrob");
      canvas_win= FL_ObjWin(fd_introrobgui->micanvas);
    }

  /* Empiezo con el canvas en blanco */
  jde_width = fd_introrobgui->micanvas->w;
  jde_height = fd_introrobgui->micanvas->h;
  fl_winset(canvas_win); 
  fl_rectbound(0,0,jde_width,jde_height,FL_WHITE);   
  /*  XFlush(display);*/
  
  track_robot=TRUE;
  fl_set_button(fd_introrobgui->track_robot,PUSHED);
  
  fl_set_slider_bounds(fd_introrobgui->escala,RANGO_MAX,RANGO_MIN);
  fl_set_slider_filter(fd_introrobgui->escala,rangoenmetros); /* Para poner el valor del slider en metros en pantalla */
  fl_set_slider_value(fd_introrobgui->escala,RANGO_INICIAL);
  escala = jde_width/rango;

  fl_set_positioner_xvalue(fd_introrobgui->joystick,0.5);
  fl_set_positioner_yvalue(fd_introrobgui->joystick,0.);
  joystick_x=0.5;
  joystick_y=0.5;

  fl_set_positioner_xvalue(fd_introrobgui->center,0.5);
  fl_set_positioner_yvalue(fd_introrobgui->center,0.5);
	
  register_buttonscallback(introrob_guibuttons);
  register_displaycallback(introrob_guidisplay);
}

/*
 *  Copyright (C) 2006 Jos� Mar�a Ca�as Plaza 
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
 *  Authors : Jos� Mar�a Ca�as Plaza <jmplaza@gsyc.escet.urjc.es>
 */

#include <jde.h>
#include <forms.h>
#include "graphics_xforms.h"
#define v3f glVertex3f

#include <GL/gl.h>              
#include <GL/glx.h>
#include <GL/glu.h>

#include <forms.h>
#include <glcanvas.h>
#include "opengldemogui.h"

/*Gui declarations*/
Display *mydisplay;
int  *myscreen;

/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;

int opengldemo_id=0; 
int opengldemo_brothers[MAX_SCHEMAS];
arbitration opengldemo_callforarbitration;
int opengldemo_cycle=1000; /* ms */

FD_opengldemogui *fd_opengldemogui=NULL;

#define PI 3.141592654
#define MAXWORLD 30.
char rotar=1;     
/* ventana de visualizacion 3d */
float xcam,ycam,zcam,foax,foay,foaz;

void opengldemo_stop(){
if (fd_opengldemogui!=NULL)
    {
      if (all[opengldemo_id].guistate==on) 
	fl_hide_form(fd_opengldemogui->opengldemogui);
      fl_free_form(fd_opengldemogui->opengldemogui);
    }
  printf ("opengldemo close\n");
}

void opengldemo_iteration()
{  
  all[opengldemo_id].k++;
  /* printf("opengldemo iteration %d\n",d++);*/
}


void opengldemo_suspend()
{
  /* printf("opengldemo: cojo-suspend\n");*/
  pthread_mutex_lock(&(all[opengldemo_id].mymutex));
  put_state(opengldemo_id,slept);
  printf("opengldemo: off\n");
  pthread_mutex_unlock(&(all[opengldemo_id].mymutex));
  /*  printf("opengldemo: suelto-suspend\n");*/
}


void opengldemo_resume(int father, int *brothers, arbitration fn)
{
  int i; 
  
  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[opengldemo_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[opengldemo_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[opengldemo_id].children[i]=FALSE;
  all[opengldemo_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) opengldemo_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {opengldemo_brothers[i]=brothers[i];i++;}
    }
  opengldemo_callforarbitration=fn;
  put_state(opengldemo_id,notready);
  printf("opengldemo: on\n");
  pthread_cond_signal(&(all[opengldemo_id].condition));
  pthread_mutex_unlock(&(all[opengldemo_id].mymutex));
}

void *opengldemo_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      /* printf("opengldemo: iteration-cojo\n");*/
      pthread_mutex_lock(&(all[opengldemo_id].mymutex));

      if (all[opengldemo_id].state==slept) 
	{
	  pthread_cond_wait(&(all[opengldemo_id].condition),&(all[opengldemo_id].mymutex));
	  pthread_mutex_unlock(&(all[opengldemo_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[opengldemo_id].state==notready) put_state(opengldemo_id,ready);
	  else all[opengldemo_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[opengldemo_id].state==ready) put_state(opengldemo_id,winner);


	  if (all[opengldemo_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[opengldemo_id].mymutex));
	      /*printf("opengldemo: iteration-suelto2\n");*/

	      gettimeofday(&a,NULL);
	      opengldemo_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = opengldemo_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(opengldemo_cycle*1000-diff);
	      else 
		{printf("time interval violated: opengldemo\n"); usleep(opengldemo_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[opengldemo_id].mymutex));
	      /*printf("opengldemo: iteration-suelto3\n");*/
	      usleep(opengldemo_cycle*1000);
	    }
	}
    }
}

void opengldemo_init(){
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
}

void opengldemo_startup()
{
  pthread_mutex_lock(&(all[opengldemo_id].mymutex));
  printf("opengldemo schema started up\n");
  put_state(opengldemo_id,slept);
  pthread_create(&(all[opengldemo_id].mythread),NULL,opengldemo_thread,NULL);
  opengldemo_init();
  pthread_mutex_unlock(&(all[opengldemo_id].mymutex));
}



int InitOGL2(FL_OBJECT *ob, Window win,int w,int h, XEvent *xev, void *ud)
{
  /*glViewport(0,0,(GLint)w,(GLint)h);*/
  /*glDrawBuffer(GL_FRONT);*/
  glClearColor(0.0,0.0,0.0,0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0);
  return 0;
}



int InitOGL(FL_OBJECT *ob, Window win,int w,int h, XEvent *xev, void *ud)
{

  /* La primera parte de esta funcion inicializa OpenGL con los parametros
     que diran como se visualiza. */
  fl_activate_glcanvas(fd_opengldemogui->canvas);  
  glViewport(0,0,(GLint)w,(GLint)h);

  /* resetea el buffer de color y el de profundidad */ 
  glDrawBuffer(GL_BACK);
  glClearColor(1.,1.,1.,0.0);  
  glClearDepth(1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);      
  /*  
      glCullFace(GL_BACK);
      glEnable(GL_CULL_FACE);
      glScalef(2.0,2.0,2.0);
      glShadeModel(GL_SMOOTH);
  */

  /*
    glDrawBuffer(GL_FRONT);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
  */	
  return 0;
}



void rendering_cubo( void )
{
  /* las 6 caras del cubo */
   glColor3f( 1.0, 0.0, 0.0 );
   glBegin(GL_POLYGON);
   v3f( 1.0, 1.0, 1.0 );   v3f( 1.0, -1.0, 1.0 );
   v3f( 1.0, -1.0, -1.0 ); v3f( 1.0, 1.0, -1.0 );
   glEnd();

   glColor3f( 0.5, 0.0, 0.0 );
   glBegin(GL_POLYGON);
   v3f( -1.0, 1.0, 1.0 );   v3f( -1.0, 1.0, -1.0 );
   v3f( -1.0, -1.0, -1.0 ); v3f( -1.0, -1.0, 1.0 );
   glEnd();

   glColor3f( 0.0, 1.0, 0.0 );
   glBegin(GL_POLYGON);
   v3f(  1.0, 1.0,  1.0 ); v3f(  1.0, 1.0, -1.0 );
   v3f( -1.0, 1.0, -1.0 ); v3f( -1.0, 1.0,  1.0 );
   glEnd();

   glColor3f( 0.0, 0.5, 0.0 );
   glBegin(GL_POLYGON);
   v3f(  1.0, -1.0,  1.0 ); v3f( -1.0, -1.0,  1.0 );
   v3f( -1.0, -1.0, -1.0 ); v3f(  1.0, -1.0, -1.0 );
   glEnd();

   glColor3f( 0.0, 0.0, 1.0 );
   glBegin(GL_POLYGON);
   v3f(  1.0,  1.0,  1.0 ); v3f( -1.0,  1.0,  1.0 );
   v3f( -1.0, -1.0,  1.0 ); v3f(  1.0, -1.0,  1.0 );
   glEnd();

   glColor3f( 0.0, 0.0, 0.5 );
   glBegin(GL_POLYGON);
   v3f(  1.0, 1.0, -1.0 ); v3f(  1.0,-1.0, -1.0 );
   v3f( -1.0,-1.0, -1.0 ); v3f( -1.0, 1.0, -1.0 );
   glEnd();
}


void Rotar(FL_OBJECT *ob,long data)
{
        rotar=1;
}
void Detener(FL_OBJECT *ob,long data)
{
        rotar=0;
}

void PintaEscenaCubos()
{
 static int dd=0;

 int i;

  if(rotar)
    {     
      /* cada vez que se llama a esta funci�n rota un poquito */
      /* se va acumulando sobre la anterior que hubiera */

      /* cubo 1 */
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslatef(0.0,0.0,-10.0);
      glRotatef(1.0*dd,1.0,2.0,-3.0);
      rendering_cubo();                               
      
      /* cubo 2 */
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glTranslatef(0.0,4.0,-20.0);
      glRotatef(2.0*dd,1.0,2.0,-3.0);
      dd++;
      rendering_cubo();                               

      /* ejes */
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glColor3f( 0.7, 0., 0. );
      glBegin(GL_LINES);
      v3f( 0.0, 0.0, 0.0 );   
      v3f( 10.0, 0.0, 0.0 );
      glEnd();
      glBegin(GL_LINES);
      v3f( 0.0, 0.0, 0.0 );   
      v3f( 0.0, 10.0, 0.0 );
      glEnd();
      glBegin(GL_LINES);
      v3f( 0.0, 0.0, 0.0 );   
      v3f( 0.0, 0.0, 10.0 );
      glEnd();

      /* suelo */ 
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glColor3f( 0.4, 0.4, 0.4 );
      glBegin(GL_POLYGON);
      v3f( 10.0, -3.0, 0.0 );   
      v3f( 10.0, -3.0, 10.0 );
      v3f( 10.0, -3.0, -1.0 ); 
      v3f( 10.0, -3.0, -10.0 );
      glEnd();

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glColor3f( 0.4, 0.4, 0.4 );
      glBegin(GL_LINES);
      for(i=0;i<61;i++)
	{
	  v3f(-30.+(float)i,0.,-30.);
	  v3f(-30.+(float)i,0.,30.);
	  v3f(-30.,0.,-30.+(float)i);
	  v3f(30.,0.,-30.+(float)i);
	}
      glEnd();
    }
}

void opengldemo_guibuttons(void *obj1)
{
  float r,lati,longi;
  FL_OBJECT *obj=(FL_OBJECT *)obj1;

  if (obj==fd_opengldemogui->camOrigin) 
    { xcam=0.; ycam=0.; zcam=0.;
    fl_set_slider_value(fd_opengldemogui->camX,(double)xcam);
    fl_set_slider_value(fd_opengldemogui->camY,(double)ycam);
    fl_set_slider_value(fd_opengldemogui->camZ,(double)zcam);
    fl_set_positioner_xvalue(fd_opengldemogui->camlatlong,(double)0.);
    fl_set_positioner_yvalue(fd_opengldemogui->camlatlong,(double)90.);
    fl_set_slider_value(fd_opengldemogui->camR,(double)0.); 
    } 
  else if ((obj==fd_opengldemogui->camX) ||
	   (obj==fd_opengldemogui->camY) || 
	   (obj==fd_opengldemogui->camZ))
    {
      xcam=(float) fl_get_slider_value(fd_opengldemogui->camX);
      ycam=(float) fl_get_slider_value(fd_opengldemogui->camY);
      zcam=(float) fl_get_slider_value(fd_opengldemogui->camZ);

      r=(float)sqrt((double)(xcam*xcam+ycam*ycam+zcam*zcam));
      lati=(float)asin(zcam/r)*360./(2.*PI);
      longi=(float)atan2((float)ycam,(float)xcam)*360./(2.*PI);
      fl_set_positioner_xvalue(fd_opengldemogui->camlatlong,(double) longi);
      fl_set_positioner_yvalue(fd_opengldemogui->camlatlong,(double) lati);
      fl_set_slider_value(fd_opengldemogui->camR,(double)r); 
    }
  else if ((obj==fd_opengldemogui->camR) ||
	   (obj==fd_opengldemogui->camlatlong))
    {
      longi=2*PI*fl_get_positioner_xvalue(fd_opengldemogui->camlatlong)/360.;
      lati=2*PI*fl_get_positioner_yvalue(fd_opengldemogui->camlatlong)/360.;
      r=fl_get_slider_value(fd_opengldemogui->camR);
      xcam=r*cos(lati)*cos(longi);
      ycam=r*cos(lati)*sin(longi);
      zcam=r*sin(lati);
      fl_set_slider_value(fd_opengldemogui->camX,(double)xcam);
      fl_set_slider_value(fd_opengldemogui->camY,(double)ycam);
      fl_set_slider_value(fd_opengldemogui->camZ,(double)zcam);
    }
  else if (obj==fd_opengldemogui->foaOrigin) 
    { foax=0.; foay=0.; foaz=0.;}
  else if (obj==fd_opengldemogui->foaX)
    foax=(float) fl_get_slider_value(fd_opengldemogui->foaX);
  else if (obj==fd_opengldemogui->foaY)
    foay=(float) fl_get_slider_value(fd_opengldemogui->foaY);
  else if (obj==fd_opengldemogui->foaZ)
    foaz=(float) fl_get_slider_value(fd_opengldemogui->foaZ);




}

void opengldemo_guidisplay()
{
  fl_activate_glcanvas(fd_opengldemogui->canvas);
  /* Set the OpenGL state machine to the right context for this display */
  /* reset of the depth and color buffers */
  InitOGL(fd_opengldemogui->canvas, FL_ObjWin(fd_opengldemogui->canvas),fd_opengldemogui->canvas->w,fd_opengldemogui->canvas->h,NULL,NULL);
  /* proyecci�n perspectiva */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.,(GLfloat)640/(GLfloat)480,1.0,100.0);
  gluLookAt(xcam,ycam,zcam,foax,foay,foaz,0.,1.,0.);
  /* gluPerspective(45.,(GLfloat)w/(GLfloat)h,1.0,100.0);
     glTranslatef(0,0,-15); */
  /* glTranslatef(-xcam,-ycam,-zcam); 
     OJO que esta transformaci�n hay que "ponerle el menos"
  */
  /* proyecci�n ortogr�fica 
     glOrtho(-5.0,5.0,-5.0,5.0,1.0,100.0);
     glTranslatef(0,0,-5);
  */

  /* rendering */
  PintaEscenaCubos();

  /*
  glClearColor(0.0,0.0,0.0,0.0); 
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glColor3f(1.0,1.0,1.0);
  glBegin(GL_POLYGON);
  glVertex3f(0.25,0.25,0.0);
  glVertex3f(0.75,0.25,0.0);
  glVertex3f(0.75,0.75,0.0);
  glVertex3f(0.25,0.75,0.0);
  glEnd();
  glFlush();

  */
  glXSwapBuffers(fl_display, fl_get_canvas_id(fd_opengldemogui->canvas));
}


void opengldemo_guisuspend_aux(void)
{
  /*  all[opengldemo_id].gui=FALSE;*/
  mydelete_buttonscallback(opengldemo_guibuttons);
  mydelete_displaycallback(opengldemo_guidisplay);
  fl_hide_form(fd_opengldemogui->opengldemogui);
}

void opengldemo_guisuspend(void){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)opengldemo_guisuspend_aux);
      }
   }
   else{
      fn ((gui_function)opengldemo_guisuspend_aux);
   }
}

void opengldemo_guiresume_aux(void)
{
  static int k=0;

  /*  all[opengldemo_id].gui=TRUE;*/
  if (k==0) /* not initialized */
    {
      k++;
      fd_opengldemogui = create_form_opengldemogui();
      fl_add_canvas_handler(fd_opengldemogui->canvas,Expose,InitOGL,0);
	/*fl_add_canvas_handler(fd_opengldemogui->canvas2,Expose,InitOGL2,0);*/
      fl_set_form_position(fd_opengldemogui->opengldemogui,400,50);
      fl_set_slider_bounds(fd_opengldemogui->camX,MAXWORLD/2.,-MAXWORLD/2);
      fl_set_slider_value(fd_opengldemogui->camX,0.);
      fl_set_slider_bounds(fd_opengldemogui->camY,MAXWORLD/2.,-MAXWORLD/2);
      fl_set_slider_value(fd_opengldemogui->camY,0.);
      fl_set_slider_bounds(fd_opengldemogui->camZ,MAXWORLD/2.,-MAXWORLD/2);
      fl_set_slider_value(fd_opengldemogui->camZ,0.);
      fl_set_slider_bounds(fd_opengldemogui->foaX,MAXWORLD/2.,-MAXWORLD/2);
      fl_set_slider_value(fd_opengldemogui->foaX,0.);
      fl_set_slider_bounds(fd_opengldemogui->foaY,MAXWORLD/2.,-MAXWORLD/2);
      fl_set_slider_value(fd_opengldemogui->foaY,0.);
      fl_set_slider_bounds(fd_opengldemogui->foaZ,MAXWORLD/2.,-MAXWORLD/2);
      fl_set_slider_value(fd_opengldemogui->foaZ,0.);
      fl_set_positioner_xbounds(fd_opengldemogui->camlatlong,-180,180.);
      fl_set_positioner_xvalue(fd_opengldemogui->camlatlong,(double) 0);
      fl_set_positioner_ybounds(fd_opengldemogui->camlatlong,-89.99,89.99); 
      fl_set_positioner_yvalue(fd_opengldemogui->camlatlong,(double) 0);
      fl_set_slider_bounds(fd_opengldemogui->camR,2*MAXWORLD,0.);
      fl_set_slider_value(fd_opengldemogui->camR,(double)0.); 

    }
  myregister_buttonscallback(opengldemo_guibuttons);
  myregister_displaycallback(opengldemo_guidisplay);

  fl_show_form(fd_opengldemogui->opengldemogui,FL_PLACE_POSITION,FL_FULLBORDER,"opengldemo");

}

void opengldemo_guiresume(void){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)opengldemo_guiresume_aux);
      }
   }
   else{
      fn ((gui_function)opengldemo_guiresume_aux);
   }
}


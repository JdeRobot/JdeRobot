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
 *  Authors : José Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

#include "jde.h"
#include <forms.h>
#include "graphics_xforms.h"
#include "opflow.h"
#include "eyeoperator1gui.h"
#include "eyeoperator1.h"
#include <ipp.h>

#define EYEOPERATORver "eyeoperator1"

#define CUBO(a) ((a)*(a)*(a))
#define CUADRADO(a) ((a)*(a))

#define PI 3.14159265

int eyeoperator1_id=0;
int eyeoperator1_brothers[MAX_SCHEMAS];
arbitration eyeoperator1_callforarbitration;

FD_eyeoperator1gui *fd_eyeoperator1gui=NULL;

/* exported variables */
int eyeoperator1_cycle=50; /* ms */

/*Necesarias para las Xlib*/
GC eyeoperator1_gc;
Window eyeoperator1_window;
XImage *image;

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320


Display *mydisplay;
int  *myscreen;

/*Gui callbacks*/
registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

#ifdef GRABAR
/*Exportar para grabar*/
Ipp8u record1[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];
Ipp8u record2[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];
Ipp8u *record;
Ipp8u *record_work;
#endif

/*Imágenes para el gui*/
Ipp8u show_img[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];/*Para mostrar en el display*/
Ipp8u *img=NULL;/*Para que las utilice el gui y las pase al display*/
Ipp8u show_temp[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];


/*Parámetros*/
int th1=1;
int th2=2;
float min_mov=2.0;
int threshold1=4;
int threshold2=8;

double modulo=0.0;
double angulo=0.0;

/*Imported variables*/
char **mycolorA=NULL;
resumeFn mycolorAresume;
suspendFn mycolorAsuspend;

t_opflow **myopflow_img=NULL;
resumeFn myopflowresume;
suspendFn myopflowsuspend;
int *myopflowid=NULL;
Ipp8u **mask=NULL;
/*Robot*/
float *myw=NULL;
float *myv=NULL;
resumeFn mymotorsresume;
suspendFn mymotorssuspend;
int *mymotorsid=NULL;

/*Regiones de movimiento*/
typedef struct s_regiones{
   int x1;
   int x2;
   int y1;
   int y2;
}t_region;

t_region region;

/*fin regiones*/

int lineinimage(Ipp8u *img, int xa, int ya, int xb, int yb, FL_COLOR thiscolor){
   float L;
   int i,imax,r,g,b;
   int lastx,lasty,thisx,thisy,lastcount;
   int threshold=1;
   int Xmax,Xmin,Ymax,Ymin;
   int punto;

   Xmin=0; Xmax=SIFNTSC_COLUMNS-1; Ymin=0; Ymax=SIFNTSC_ROWS-1;
   /* In this image always graf coordinates x=horizontal, y=vertical, starting
   at the top left corner of the image. They can't reach 240 or 320, their are
   not valid values for the pixels.  */

   if (thiscolor==FL_BLACK) {r=0;g=0;b=0;}
   else if (thiscolor==FL_RED) {r=255;g=0;b=0;}
   else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;}
   else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;}
   else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
   else if (thiscolor==FL_GREEN) {r=0;g=255;b=0;}
   else {r=0;g=0;b=0;}

   /* first, check both points are inside the limits and draw them */
   /* draw both points */
   if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1)){
      img[(SIFNTSC_COLUMNS*ya+xa)*4+0]=b;
      img[(SIFNTSC_COLUMNS*ya+xa)*4+1]=g;
      img[(SIFNTSC_COLUMNS*ya+xa)*4+2]=r;
   }
   if ((xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)){
      img[(SIFNTSC_COLUMNS*yb+xb)*4+0]=b;
      img[(SIFNTSC_COLUMNS*yb+xb)*4+1]=g;
      img[(SIFNTSC_COLUMNS*yb+xb)*4+2]=r;
   }
   L=(float)sqrt((double)((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya)));
   imax=3*(int)L+1;
   lastx=xa; lasty=ya; lastcount=0;
   for(i=0;i<=imax;i++)
   {
      thisy=(int)((float)ya+(float)i/(float)imax*(float)(yb-ya));
      thisx=(int)((float)xa+(float)i/(float)imax*(float)(xb-xa));

      if ((thisy==lasty)&&(thisx==lastx)) lastcount++;
      else
      {
         if (lastcount>=threshold)
         { /* draw that point in the image */
            if ((lastx>=Xmin)&&(lastx<Xmax+1)&&(lasty>=Ymin)&&(lasty<Ymax+1)){
               punto=(SIFNTSC_COLUMNS*lasty+lastx)*4;
               img[punto]=b;
               img[punto+1]=g;
               img[punto+2]=r;
            }
         }
         lasty=thisy;
         lastx=thisx;
         lastcount=0;
      }
   }
   return 0;
}

int eyeoperator1gui_setupDisplay(void)
/* Inicializa las ventanas, la paleta de colores y memoria
       compartida para visualizacion*/
{
   int vmode;
   static XGCValues gc_values;

   gc_values.graphics_exposures = False;
   eyeoperator1_gc = XCreateGC(mydisplay,eyeoperator1_window, GCGraphicsExposures, &gc_values);
   
   vmode= fl_get_vclass();

   if ((vmode==TrueColor)&&(fl_state[vmode].depth==16))
   {
      printf("16 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),16,
                           ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
                           SIFNTSC_ROWS,8,0);
   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24))
   {
      printf("24 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24,
                           ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
                           SIFNTSC_ROWS,8,0);

   }
   else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32))
   {

      printf("32 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32,
                           ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
                           SIFNTSC_ROWS,8,0);
   }
   else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
   {
      printf("8 bits mode\n");
      /* Imagen principal */
      image = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8,
                           ZPixmap, 0, (char*)show_img, SIFNTSC_COLUMNS,
                           SIFNTSC_ROWS,8,0);
   }
   else
   {
      perror("Unsupported color mode in X server");
      jdeshutdown(1);
   }
   return 1;
}

void eyeoperator1_iteration(){

   int x, y, puntos;
   double compx, compy;
   
   speedcounter(eyeoperator1_id);

   if ((*myopflow_img)!=NULL){
      puntos=0;
      angulo=0.0;
      modulo=0.0;
      compx=0.0;
      compy=0.0;

      for (y=region.y1+2; y<region.y2-2; y++){
         for (x=region.x1+2; x<region.x2-2; x++){
            /*Se comprueba el flujo que hay para cada posición y se hace la media*/
            int valor=x+y*SIFNTSC_COLUMNS;
            if ((*myopflow_img)[valor].calc==1 &&
                  (*myopflow_img)[valor].hyp>min_mov)
            {
               puntos++;
               compx+=x-(*myopflow_img)[valor].dest.x;
               compy+=y-(*myopflow_img)[valor].dest.y;
            }
         }
      }
      if (puntos){
         compx/=puntos;
         compy/=puntos;
      }

      angulo=atan2( (double) compy, (double) compx );
      modulo=sqrt( compy*compy + compx*compx );

      if (compx>threshold1 && compx< threshold2){
         (*myw)=-CUADRADO(compx-threshold1);
      }
      else if ((-compx)>threshold1 && (-compx)< threshold2){
         (*myw)=CUADRADO(compx+threshold1);
      }
      else if (compx>=threshold2){
         /*Avanza cuadráticamente con el flujo*/
         (*myw)=-(CUADRADO(compx-threshold1)*1.1/*0.6*/);
      }
      else if (-compx>=threshold2){
         /*Avanza cuadráticamente con el flujo*/
         (*myw)=CUADRADO(compx+threshold1)*1.1;
      }
      else{
         (*myw)=0;
      }

      if (compy>threshold1 && compy< threshold2){
         (*myv)=CUBO(compy-threshold1);
      }
      else if ((-compy)>threshold1 && (-compy)< threshold2){
         (*myv)=CUBO(compy+threshold1);
      }
      else if (compy>=threshold2){
         /*Avanza cúbicamente con el flujo*/
         (*myv)=CUBO(compy-threshold1)*5;
      }
      else if (-compy>=threshold2){
         /*Avanza cúbicamente con el flujo*/
         (*myv)=CUBO(compy+threshold1)*5;
      }
      else{
         (*myv)=0;
      }
   }
}


void eyeoperator1_suspend()
{
  /* printf("eyeoperator1: cojo-suspend\n");*/
  pthread_mutex_lock(&(all[eyeoperator1_id].mymutex));
  put_state(eyeoperator1_id,slept);
  myopflowsuspend();
  mymotorssuspend();
  printf("eyeoperator1: off\n");
  pthread_mutex_unlock(&(all[eyeoperator1_id].mymutex));
  /*  printf("eyeoperator1: suelto-suspend\n");*/
}


void eyeoperator1_resume(int father, int *brothers, arbitration fn){
   int i;

   /* update the father incorporating this schema as one of its children */
   if (/*father!=GUIHUMAN*/father>=0){
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[eyeoperator1_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
   }

   pthread_mutex_lock(&(all[eyeoperator1_id].mymutex));
   /* this schema resumes its execution with no children at all */
   for(i=0;i<MAX_SCHEMAS;i++) all[eyeoperator1_id].children[i]=FALSE;
   all[eyeoperator1_id].father=father;
   if (brothers!=NULL){
      for(i=0;i<MAX_SCHEMAS;i++) eyeoperator1_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {eyeoperator1_brothers[i]=brothers[i];i++;}
   }
   eyeoperator1_callforarbitration=fn;
   put_state(eyeoperator1_id,notready);

   myopflow_img=(t_opflow **)myimport ("opflow", "opflow_img");
   myopflowresume=(resumeFn)myimport("opflow","resume");
   myopflowsuspend=(suspendFn)myimport("opflow","suspend");
   myopflowid=(int *)myimport ("opflow", "id");
   mask=(Ipp8u **)myimport("opflow", "mask");
  
   mymotorsresume=(resumeFn)myimport("motors", "resume");
   mymotorssuspend=(suspendFn)myimport("motors", "suspend");
   myv=(float *)myimport("motors","v");
   myw=(float *)myimport("motors","w");
   mymotorsid=(int *)myimport("motors", "id");
  
   if (myv==NULL || myw==NULL || mask==NULL|| myopflow_img==NULL){
      fprintf (stderr, "I can't import one or more needed varibles, please");
      fprintf (stderr, "load all needed schemas (motors, colorA, opflow)\n");
      jdeshutdown(1);
   }

   printf("eyeoperator1: on\n");
   pthread_cond_signal(&(all[eyeoperator1_id].condition));
   pthread_mutex_unlock(&(all[eyeoperator1_id].mymutex));
   {
      int y,x;
      for (y=0;y<SIFNTSC_ROWS;y++){
         for (x=0; x<SIFNTSC_COLUMNS; x++){
            i=(x+y*SIFNTSC_COLUMNS)*3;
            if (x>=(region.x1-10) && x<=(region.x2+10) && y>=(region.y1-10) && y<=(region.y2+10)){
               (*mask)[i]=UCHAR_MAX;
               (*mask)[i+1]=UCHAR_MAX;
               (*mask)[i+2]=UCHAR_MAX;
            }
            else{
               (*mask)[i]=0;
               (*mask)[i+1]=0;
               (*mask)[i+2]=0;
            }
         }
      }
   }
}


void *eyeoperator1_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[eyeoperator1_id].mymutex));

      if (all[eyeoperator1_id].state==slept)
      {
         pthread_cond_wait(&(all[eyeoperator1_id].condition),&(all[eyeoperator1_id].mymutex));
         pthread_mutex_unlock(&(all[eyeoperator1_id].mymutex));
      }
      else
      {
         /* check preconditions. For now, preconditions are always satisfied*/
         if (all[eyeoperator1_id].state==notready)
            put_state(eyeoperator1_id,ready);
         /* check brothers and arbitrate. For now this is the only winner */
         if (all[eyeoperator1_id].state==ready){
	    
            pthread_mutex_unlock(&(all[eyeoperator1_id].mymutex));
            myopflowresume (eyeoperator1_id,NULL,NULL);
            mymotorsresume (eyeoperator1_id,NULL,NULL);
            
            pthread_mutex_lock(&(all[eyeoperator1_id].mymutex));
            if (myopflowid!=NULL)
               all[eyeoperator1_id].children[*myopflowid]=TRUE;
            if (mymotorsid!=NULL)
               all[eyeoperator1_id].children[*mymotorsid]=TRUE;
            put_state(eyeoperator1_id,winner);
            gettimeofday(&a,NULL);
            aa=a.tv_sec*1000000+a.tv_usec;
            n=0;
         }

         if (all[eyeoperator1_id].state==winner)
            /* I'm the winner and must execute my iteration */
         {
            pthread_mutex_unlock(&(all[eyeoperator1_id].mymutex));
            /*      gettimeofday(&a,NULL);*/
            n++;
            eyeoperator1_iteration();
            gettimeofday(&b,NULL);
            bb=b.tv_sec*1000000+b.tv_usec;
            next=aa+(n+1)*(long)eyeoperator1_cycle*1000-bb;

	      
            if (next>5000)
            {
               usleep(next-5000);
               /* discounts 5ms taken by calling usleep itself, on average */
            }
            else  ;
         }
         else
            /* just let this iteration go away. overhead time negligible */
         {
            pthread_mutex_unlock(&(all[eyeoperator1_id].mymutex));
            usleep(eyeoperator1_cycle*1000);
         }
      }
   }
}

void eyeoperator1_init(){
   if (myregister_displaycallback==NULL){
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

void eyeoperator1_startup(char *configfile)
{
   int i;
   pthread_mutex_lock(&(all[eyeoperator1_id].mymutex));
   printf("eyeoperator1 schema started up\n");
   myexport("eyeoperator1","cycle",&eyeoperator1_cycle);
   myexport("eyeoperator1","resume",(void *)eyeoperator1_resume);
   myexport("eyeoperator1","suspend",(void *)eyeoperator1_suspend);
   myexport("eyeoperator1","id", &eyeoperator1_id);
#ifdef GRABAR
   printf ("Image to record is being exported, it's name is \"record\"\n");
   record=record1;
   record_work=record2;
   myexport("eyeoperator1", "record", &record);
#endif
   put_state(eyeoperator1_id,slept);
   for (i=0; i<SIFNTSC_COLUMNS*SIFNTSC_ROWS; i++){
      show_img[i*4+3]=UCHAR_MAX;
      show_temp[i*4+3]=UCHAR_MAX;
   }
   region.x1=70;
   region.x2=250;
   region.y1=50;
   region.y2=190;

   
   pthread_create(&(all[eyeoperator1_id].mythread),NULL,eyeoperator1_thread,
		    NULL);
   eyeoperator1_init();
   pthread_mutex_unlock(&(all[eyeoperator1_id].mymutex));
}

void eyeoperator1_stop()
{
  if (fd_eyeoperator1gui!=NULL)
    {
      if (all[eyeoperator1_id].guistate==on) 
	fl_hide_form(fd_eyeoperator1gui->eyeoperator1gui);
      fl_free_form(fd_eyeoperator1gui->eyeoperator1gui);
    }
  printf ("eyeoperator1 close\n");
}

void eyeoperator1_guidisplay()
{
   static IppiSize imgtam;

   imgtam.height=SIFNTSC_ROWS;
   imgtam.width=SIFNTSC_COLUMNS;

   if (mycolorA!=NULL){
      ippiCopy_8u_C3AC4R ((Ipp8u *)*mycolorA, SIFNTSC_COLUMNS*3,
                           (Ipp8u*)show_temp, SIFNTSC_COLUMNS*4, imgtam);

      /*Pintar región en el display*/
      {
         int x1=region.x1;
         int x2=region.x2;
         int y1=region.y1;
         int y2=region.y2;

         lineinimage(show_temp, x1, y1, x1, y2, FL_GREEN);
         lineinimage(show_temp, x2, y1, x2, y2, FL_GREEN);
         lineinimage(show_temp, x2, y1, x1, y1, FL_GREEN);
         lineinimage(show_temp, x2, y2, x1, y2, FL_GREEN);

      }

      /*Ahora pintar el flujo medio*/
      {
         int x2,y2;
         int x1=(region.x1+region.x2)/2;
         int y1=(region.y1+region.y2)/2;;
         /*Darle longitud al segmento que se pinta*/
         x2 = (int) (x1 - 2 * modulo * cos(angulo));
         y2 = (int) (y1 - 2 * modulo * sin(angulo));
         /*Dibujar la línea principal de la flecha*/
         lineinimage(show_temp, x1, y1, x2, y2,FL_RED);
         lineinimage(show_temp, x1, y1, x2, y1, FL_BLUE);
         lineinimage(show_temp, x1, y1, x1, y2, FL_BLUE);
         /*Ahora las puntas de las flechas*/
         x1 = (int) (x2 + (modulo/4) * cos(angulo + PI / 4));
         y1 = (int) (y2 + (modulo/4) * sin(angulo + PI / 4));
         lineinimage(show_temp, x1, y1, x2, y2, FL_RED);
         x1 = (int) (x2 + (modulo/4) * cos(angulo - PI / 4));
         y1 = (int) (y2 + (modulo/4) * sin(angulo - PI / 4));
         lineinimage(show_temp, x1, y1, x2, y2, FL_RED);
      }

      ippiMirror_8u_C4R (show_temp, SIFNTSC_COLUMNS*4, show_img,
                         SIFNTSC_COLUMNS*4, imgtam, ippAxsVertical);

#ifdef GRABAR
      ippiCopy_8u_C4R ((Ipp8u *)show_img, SIFNTSC_COLUMNS*4, (Ipp8u*)record_work,
                        SIFNTSC_COLUMNS*4, imgtam);
      {
         Ipp8u *aux;
         aux=record;
         record=record_work;
         record_work=aux;
      }
#endif
      XPutImage(mydisplay,eyeoperator1_window,eyeoperator1_gc,image,0,0,
                fd_eyeoperator1gui->img->x,fd_eyeoperator1gui->img->y,
                SIFNTSC_COLUMNS, SIFNTSC_ROWS);
   }
}


void eyeoperator1_guisuspend_aux(void)
{
  mydelete_displaycallback(eyeoperator1_guidisplay);
  mycolorAsuspend();
  fl_hide_form(fd_eyeoperator1gui->eyeoperator1gui);
}

void eyeoperator1_guisuspend(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)eyeoperator1_guisuspend_aux);
      }
   }
   else{
      fn ((gui_function)eyeoperator1_guisuspend_aux);
   }
}

void eyeoperator1_guiresume_aux(void)
{
  static int k=0;

  if (k==0){ /* not initialized */
     k++;
     fd_eyeoperator1gui = create_form_eyeoperator1gui();
     fl_set_form_position(fd_eyeoperator1gui->eyeoperator1gui,400,50);
     fl_show_form(fd_eyeoperator1gui->eyeoperator1gui,FL_PLACE_POSITION,
		  FL_FULLBORDER,EYEOPERATORver);
     eyeoperator1_window = FL_ObjWin(fd_eyeoperator1gui->img);
     eyeoperator1gui_setupDisplay();
     mycolorA=(char **) myimport ("colorA", "colorA");
     mycolorAresume=(resumeFn)myimport ("colorA", "resume");
     mycolorAsuspend=(suspendFn)myimport("colorA","suspend");
  }
  else{
     fl_show_form(fd_eyeoperator1gui->eyeoperator1gui,FL_PLACE_POSITION,
		  FL_FULLBORDER,EYEOPERATORver);
     eyeoperator1_window = FL_ObjWin(fd_eyeoperator1gui->img);
  }

  mycolorAresume(eyeoperator1_id,NULL,NULL); /*Arrancar colorA*/

  myregister_displaycallback(eyeoperator1_guidisplay);
}

void eyeoperator1_guiresume(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)eyeoperator1_guiresume_aux);
      }
   }
   else{
      fn ((gui_function)eyeoperator1_guiresume_aux);
   }
}


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
#include "eyeoperator4gui.h"
#include "eyeoperator4.h"
#include <ipp.h>

#define EYEOPERATORver "eyeoperator4 4.0"

#define CUBO(a) ((a)*(a)*(a))
#define CUADRADO(a) ((a)*(a))

#define PI 3.14159265

int eyeoperator4_id=0;
int eyeoperator4_brothers[MAX_SCHEMAS];
arbitration eyeoperator4_callforarbitration;

FD_eyeoperator4gui *fd_eyeoperator4gui;

/* exported variables */
int eyeoperator4_cycle=50; /* ms */

/*Necesarias para las Xlib*/
GC eyeoperator4_gc;
Window eyeoperator4_window;
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

#define max_color 6
#define PUSHED 1
#define RELEASED 0

/*Botones del gui*/
float min_mov=2.0;
int threshold1=5, threshold2=9;

double modulo[4];
double angulo[4];

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

t_region region[4]; /*URLD  (Arriba, derecha, izquierda, abajo)*/

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

int eyeoperator4gui_setupDisplay(void)
/* Inicializa las ventanas, la paleta de colores y memoria
       compartida para visualizacion*/
{
   int vmode;
   static XGCValues gc_values;

   gc_values.graphics_exposures = False;
   eyeoperator4_gc = XCreateGC(mydisplay,eyeoperator4_window, GCGraphicsExposures, &gc_values);
   
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

void eyeoperator4_iteration(){

   int x, y, puntos, i;
   int sumaw, sumav;
   
   speedcounter(eyeoperator4_id);

   if ((*myopflow_img)!=NULL){
      for (i=0; i<4; i++){
         puntos=0;
         angulo[i]=0.0;
         modulo[i]=0.0;

         for (y=region[i].y1+2; y<region[i].y2-2; y++){
            for (x=region[i].x1+2; x<region[i].x2-2; x++){
               /*Se comprueba el flujo que hay para cada posición y se hace la media*/
               int valor=x+y*SIFNTSC_COLUMNS;
               if ((*myopflow_img)[valor].calc==1 &&
                     (*myopflow_img)[valor].hyp>min_mov)
               {
                  puntos++;
                  angulo[i]+=(*myopflow_img)[valor].angle;
                  modulo[i]+=(*myopflow_img)[valor].hyp;
               }
            }
         }
         if (puntos){
            angulo[i]/=puntos;
            modulo[i]/=puntos;
            modulo[i]=sqrt(modulo[i]);
         }
      }
      sumaw=0;
      sumav=0;

      if (modulo[2]>threshold1){
         sumaw=modulo[2];
      }
      if (modulo[1]>threshold1){
         sumaw-=modulo[1];
      }

      if (sumaw>threshold1 && sumaw< threshold2){
         (*myw)=CUADRADO(sumaw-threshold1);
      }
      else if ((-sumaw)>threshold1 && (-sumaw)< threshold2){
         (*myw)=-CUADRADO(sumaw+threshold1);
      }
      else if (sumaw>=threshold2){
         /*Avanza cuadráticamente con el flujo*/
         (*myw)=CUADRADO(sumaw-threshold1)*1.1/*0.6*/;
      }
      else if (-sumaw>=threshold2){
         /*Avanza cuadráticamente con el flujo*/
         (*myw)=-CUADRADO(sumaw+threshold1)*1.1;
      }
      else{
         (*myw)=0;
      }

      if (modulo[0]>threshold1){
         sumav=modulo[0];
      }
      if (modulo[3]>threshold1){
         sumav-=modulo[3];
      }
      
      if (sumav>threshold1 && sumav< threshold2){
         (*myv)=CUBO(sumav-threshold1);
      }
      else if ((-sumav)>threshold1 && (-sumav)< threshold2){
         (*myv)=CUBO(sumav+threshold1);
      }
      else if (sumav>=threshold2){
         (*myv)=(CUBO(sumav-threshold1)*5);
      }
      else if (-sumav>=threshold2){
         (*myv)=CUBO(sumav+threshold1)*5;
      }
      else{
         (*myv)=0;
      }
   }
}


void eyeoperator4_suspend()
{
  /* printf("eyeoperator4: cojo-suspend\n");*/
  pthread_mutex_lock(&(all[eyeoperator4_id].mymutex));
  put_state(eyeoperator4_id,slept);
  myopflowsuspend();
  mymotorssuspend();
  printf("eyeoperator4: off\n");
  pthread_mutex_unlock(&(all[eyeoperator4_id].mymutex));
  /*  printf("eyeoperator4: suelto-suspend\n");*/
}


void eyeoperator4_resume(int father, int *brothers, arbitration fn){
   int i;

   /* update the father incorporating this schema as one of its children */
   if (/*father!=GUIHUMAN*/father>=0){
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[eyeoperator4_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
   }

   pthread_mutex_lock(&(all[eyeoperator4_id].mymutex));
   /* this schema resumes its execution with no children at all */
   for(i=0;i<MAX_SCHEMAS;i++) all[eyeoperator4_id].children[i]=FALSE;
   all[eyeoperator4_id].father=father;
   if (brothers!=NULL){
      for(i=0;i<MAX_SCHEMAS;i++) eyeoperator4_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {eyeoperator4_brothers[i]=brothers[i];i++;}
   }
   eyeoperator4_callforarbitration=fn;
   put_state(eyeoperator4_id,notready);

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

   printf("eyeoperator4: on\n");
   pthread_cond_signal(&(all[eyeoperator4_id].condition));
   pthread_mutex_unlock(&(all[eyeoperator4_id].mymutex));
   {
      int y,x;
      for (y=0;y<SIFNTSC_ROWS;y++){
         for (x=0; x<SIFNTSC_COLUMNS; x++){
            i=(x+y*SIFNTSC_COLUMNS)*3;
            if (x>=region[0].x1 && x<=region[0].x2 &&
                y>=region[0].y1 && y<=region[0].y2)
            {
               (*mask)[i]=UCHAR_MAX;
               (*mask)[i+1]=UCHAR_MAX;
               (*mask)[i+2]=UCHAR_MAX;
            }
            else if (x>=region[1].x1 && x<=region[1].x2 &&
                     y>=region[1].y1 && y<=region[1].y2)
            {
               (*mask)[i]=UCHAR_MAX;
               (*mask)[i+1]=UCHAR_MAX;
               (*mask)[i+2]=UCHAR_MAX;
            }
            else if (x>=region[2].x1 && x<=region[2].x2 &&
                     y>=region[2].y1 && y<=region[2].y2)
            {
               (*mask)[i]=UCHAR_MAX;
               (*mask)[i+1]=UCHAR_MAX;
               (*mask)[i+2]=UCHAR_MAX;
            }
            else if (x>=region[3].x1 && x<=region[3].x2 &&
                     y>=region[3].y1 && y<=region[3].y2)
            {
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


void *eyeoperator4_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[eyeoperator4_id].mymutex));

      if (all[eyeoperator4_id].state==slept)
      {
         pthread_cond_wait(&(all[eyeoperator4_id].condition),&(all[eyeoperator4_id].mymutex));
         pthread_mutex_unlock(&(all[eyeoperator4_id].mymutex));
      }
      else
      {
         /* check preconditions. For now, preconditions are always satisfied*/
         if (all[eyeoperator4_id].state==notready)
            put_state(eyeoperator4_id,ready);
         /* check brothers and arbitrate. For now this is the only winner */
         if (all[eyeoperator4_id].state==ready){
	    
            pthread_mutex_unlock(&(all[eyeoperator4_id].mymutex));
            myopflowresume (eyeoperator4_id,NULL,NULL);
            mymotorsresume (eyeoperator4_id,NULL,NULL);
            
            pthread_mutex_lock(&(all[eyeoperator4_id].mymutex));
            if (myopflowid!=NULL)
               all[eyeoperator4_id].children[*myopflowid]=TRUE;
            if (mymotorsid!=NULL)
               all[eyeoperator4_id].children[*mymotorsid]=TRUE;
            put_state(eyeoperator4_id,winner);
            gettimeofday(&a,NULL);
            aa=a.tv_sec*1000000+a.tv_usec;
            n=0;
         }

         if (all[eyeoperator4_id].state==winner)
            /* I'm the winner and must execute my iteration */
         {
            pthread_mutex_unlock(&(all[eyeoperator4_id].mymutex));
            /*      gettimeofday(&a,NULL);*/
            n++;
            eyeoperator4_iteration();
            gettimeofday(&b,NULL);
            bb=b.tv_sec*1000000+b.tv_usec;
            next=aa+(n+1)*(long)eyeoperator4_cycle*1000-bb;

	      
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
            pthread_mutex_unlock(&(all[eyeoperator4_id].mymutex));
            usleep(eyeoperator4_cycle*1000);
         }
      }
   }
}

void eyeoperator4_init(){
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

void eyeoperator4_startup(char *configfile)
{
   int i;
   pthread_mutex_lock(&(all[eyeoperator4_id].mymutex));
   printf("eyeoperator4 schema started up\n");
   myexport("eyeoperator4","cycle",&eyeoperator4_cycle);
   myexport("eyeoperator4","resume",(void *)eyeoperator4_resume);
   myexport("eyeoperator4","suspend",(void *)eyeoperator4_suspend);
   myexport("eyeoperator4","id", &eyeoperator4_id);
#ifdef GRABAR
   printf ("Image to record is being exported, it's name is \"record\"\n");
   record=record1;
   record_work=record2;
   myexport("eyeoperator4", "record", &record);
#endif
   put_state(eyeoperator4_id,slept);
   for (i=0; i<SIFNTSC_COLUMNS*SIFNTSC_ROWS; i++){
      show_img[i*4+3]=UCHAR_MAX;
      show_temp[i*4+3]=UCHAR_MAX;
   }
   /*Arriba*/
   region[0].x1=90;
   region[0].x2=230;
   region[0].y1=10;
   region[0].y2=70;
   /*Derecha (mirror)*/
   region[1].x1=10;
   region[1].x2=90;
   region[1].y1=80;
   region[1].y2=160;
   /*Izquierda (mirror)*/
   region[2].x1=230;
   region[2].x2=310;
   region[2].y1=80;
   region[2].y2=160;
   /*Abajo*/
   region[3].x1=90;
   region[3].x2=230;
   region[3].y1=170;
   region[3].y2=230;
   
   pthread_create(&(all[eyeoperator4_id].mythread),NULL,eyeoperator4_thread,
		    NULL);
   eyeoperator4_init();
   pthread_mutex_unlock(&(all[eyeoperator4_id].mymutex));
}

void eyeoperator4_stop()
{
  if (fd_eyeoperator4gui!=NULL)
    {
      if (all[eyeoperator4_id].guistate==on) 
	fl_hide_form(fd_eyeoperator4gui->eyeoperator4gui);
      fl_free_form(fd_eyeoperator4gui->eyeoperator4gui);
    }
  printf ("eyeoperator4 close\n");
}

void eyeoperator4_guidisplay()
{
   static IppiSize imgtam;
   int i;

   imgtam.height=SIFNTSC_ROWS;
   imgtam.width=SIFNTSC_COLUMNS;

   if (mycolorA!=NULL){
      ippiCopy_8u_C3AC4R ((Ipp8u *)*mycolorA, SIFNTSC_COLUMNS*3,
                           (Ipp8u*)show_temp, SIFNTSC_COLUMNS*4, imgtam);

      /*Pintar región en el display*/
      for (i=0; i<4; i++){
         int x1=region[i].x1;
         int x2=region[i].x2;
         int y1=region[i].y1;
         int y2=region[i].y2;

         lineinimage(show_temp, x1, y1, x1, y2, FL_GREEN);
         lineinimage(show_temp, x2, y1, x2, y2, FL_GREEN);
         lineinimage(show_temp, x2, y1, x1, y1, FL_GREEN);
         lineinimage(show_temp, x2, y2, x1, y2, FL_GREEN);
         /*Ahora pintar el flujo medio*/
         {
            int x1=(region[i].x1+region[i].x2)/2;
            int y1=(region[i].y1+region[i].y2)/2;;
            int x2,y2;
            /*Darle longitud al segmento que se pinta*/
            x2 = (int) (x1 - 2 * modulo[i] * cos(angulo[i]));
            y2 = (int) (y1 - 2 * modulo[i] * sin(angulo[i]));
            /*Dibujar la línea principal de la flecha*/
            lineinimage(show_temp, x1, y1, x2, y2,FL_RED);
//             lineinimage(show_temp, x1, y1, x2, y1, FL_BLUE);
//             lineinimage(show_temp, x1, y1, x1, y2, FL_BLUE);
            /*Ahora las puntas de las flechas*/
            x1 = (int) (x2 + (modulo[i]/4) * cos(angulo[i] + PI / 4));
            y1 = (int) (y2 + (modulo[i]/4) * sin(angulo[i] + PI / 4));
            lineinimage(show_temp, x1, y1, x2, y2, FL_RED);
            x1 = (int) (x2 + (modulo[i]/4) * cos(angulo[i] - PI / 4));
            y1 = (int) (y2 + (modulo[i]/4) * sin(angulo[i] - PI / 4));
            lineinimage(show_temp, x1, y1, x2, y2, FL_RED);
         }
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
      XPutImage(mydisplay,eyeoperator4_window,eyeoperator4_gc,image,0,0,
                fd_eyeoperator4gui->img->x,fd_eyeoperator4gui->img->y,
                SIFNTSC_COLUMNS, SIFNTSC_ROWS);
   }
}


void eyeoperator4_guisuspend_aux(void)
{
  mydelete_displaycallback(eyeoperator4_guidisplay);
  mycolorAsuspend();
  fl_hide_form(fd_eyeoperator4gui->eyeoperator4gui);
}

void eyeoperator4_guisuspend(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)eyeoperator4_guisuspend_aux);
      }
   }
   else{
      fn ((gui_function)eyeoperator4_guisuspend_aux);
   }
}

void eyeoperator4_guiresume_aux(void)
{
  static int k=0;

  if (k==0){ /* not initialized */
     k++;
     fd_eyeoperator4gui = create_form_eyeoperator4gui();
     fl_set_form_position(fd_eyeoperator4gui->eyeoperator4gui,400,50);
     fl_show_form(fd_eyeoperator4gui->eyeoperator4gui,FL_PLACE_POSITION,
		  FL_FULLBORDER,EYEOPERATORver);
     eyeoperator4_window = FL_ObjWin(fd_eyeoperator4gui->img);
     eyeoperator4gui_setupDisplay();
     mycolorA=(char **) myimport ("colorA", "colorA");
     mycolorAresume=(resumeFn)myimport ("colorA", "resume");
     mycolorAsuspend=(suspendFn)myimport("colorA","suspend");
  }
  else{
     fl_show_form(fd_eyeoperator4gui->eyeoperator4gui,FL_PLACE_POSITION,
		  FL_FULLBORDER,EYEOPERATORver);
     eyeoperator4_window = FL_ObjWin(fd_eyeoperator4gui->img);
  }

  mycolorAresume(eyeoperator4_id,NULL,NULL); /*Arrancar colorA*/
  

  myregister_displaycallback(eyeoperator4_guidisplay);
}

void eyeoperator4_guiresume(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)eyeoperator4_guiresume_aux);
      }
   }
   else{
      fn ((gui_function)eyeoperator4_guiresume_aux);
   }
}


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
 *  
 */

#include <jde.h>
#include <forms.h>
#include "hsvtunergui.h"
#include <math.h>
#include <colorspaces.h>
#include "graphics_xforms.h"

#define HSVtunerVer "hsvtuner 2.8"

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

Display *mydisplay;
int  *myscreen;

/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;

int hsvtuner_id=0;
int hsvtuner_brothers[MAX_SCHEMAS];
arbitration hsvtuner_callforarbitration;
int hsvtuner_cycle=100; /* ms */

FD_hsvtunergui *fd_hsvtunergui=NULL;

/* Necesarias para las Xlib */
GC hsvtuner_gc;
Window hsvtuner_win; /* image window */
XImage *imagenOrig;
XImage *hsifiltrada;
XImage *histograma;

#define PI 3.141592654
/* this memory must exist even with the hsvtunergui turned on */
char imagenOrig_buf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];
char hsifiltrada_buf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];
#define SMAX SIFNTSC_COLUMNS /*320*/
char disco_buf[SMAX*SMAX*4];
char histograma_buf[SMAX*SMAX*4];
int masc[SMAX*SMAX];

/*Para los botones*/
#define PUSHED 1
#define RELEASED 0

/*Variables que guardan las coordenadas del quesito*/
#define DEGTORAD     (3.14159264 / 180.0)
#define RADTODEG     (180.0 /3.14159264)
#define centro_x 160
#define centro_y 160
#define radio_hsi_map 160.0
int x_pulsada,y_pulsada,xsoltada,ysoltada;
int x_max=200;
int y_max=200;
int x_min=centro_x;
int y_min=centro_y;

int xtrapecio1;
int xtrapecio2;
int ytrapecio1;
int ytrapecio2;

int xquesito1;
int xquesito2;
int xquesito3;
int xquesito4;

int yquesito1;
int yquesito2;
int yquesito3;
int yquesito4;

int pulsada=0;
/*Fin quesito*/

int toblack=0;
int primera =0;

double s_min, s_max, h_min, h_max, v_min, v_max;
int hsimap_threshold;

char **mycolorA;
resumeFn colorAresume;
suspendFn colorAsuspend;

void draw_hsvmap(char *buffer, int size){
   int i,j,ind; 
   float x, y, H, S, scale;
   double r,g,b;
   unsigned char R,G,B;

   for(j=0; j < size; j++){  
      for(i=0; i < size; i++){
	 x = (2.0*i)/size - 1.0;
	 y = 1.0 - (2.0*j)/size;
	 H = atan2(y,x);
	 if (H>=2*PI){
	    H-=2*PI;
	 }
	 if (H<0){
	    H+=2*PI;
	 }
	 H = RADTODEG*H;
	 S = sqrt(y*y + x*x);
	 ind = (size*j + i)*4;
	 if (S<1.)
	   {
	     hsv2rgb(H,S,0.7,&r,&g,&b);
	     scale = 255.0;
	     R = (unsigned char) (scale * r);
	     G = (unsigned char) (scale * g);
	     B = (unsigned char) (scale * b);
	   }
	 else
	   { R=175; G=175; B=175;}

	 buffer[ind]   = B; /* Blue */
	 buffer[ind+1] = G; /* Green */
	 buffer[ind+2] = R; /* Red */
      }
   }
}
/*Funciones para dibujar los quesitos*/
int drawcircle(char *img, int xcentro, int ycentro, int radio, FL_COLOR thiscolor){

   int r,g,b;
   int x1,y1;
   float i;

   /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, their are not valid values for the pixels.  */

   if (thiscolor==FL_BLACK) {r=0;g=0;b=0;}
   else if (thiscolor==FL_RED) {r=255;g=0;b=0;} 
   else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;} 
   else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;} 
   else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
   else if (thiscolor==FL_DEEPPINK) {r=213;g=85;b=178; }   
   else if (thiscolor==FL_WHITE) {r=255;g=255;b=255;}
   else {r=0;g=0;b=0;}
   
   for (i=0.;i<=360;i=i+0.1){
      x1=cos(i*DEGTORAD)*radio+xcentro;
      y1=sin(i*DEGTORAD)*radio+ycentro;
      fflush (NULL);
      img[(y1*SIFNTSC_COLUMNS+x1)*4]=b;
      img[(y1*SIFNTSC_COLUMNS+x1)*4+1]=g;
      img[(y1*SIFNTSC_COLUMNS+x1)*4+2]=r;
   }
   return 0; 
}

int lineinimage(char *img, int xa, int ya, int xb, int yb, FL_COLOR thiscolor){
   float L;
   int i,imax,r,g,b;
   int lastx,lasty,thisx,thisy,lastcount;
   int threshold=1;
   int Xmax,Xmin,Ymax,Ymin;

   Xmin=0; Xmax=SIFNTSC_COLUMNS-1; Ymin=0; Ymax=SIFNTSC_COLUMNS-1;
   /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, their are not valid values for the pixels.  */

   if (thiscolor==FL_BLACK) {r=0;g=0;b=0;}
   else if (thiscolor==FL_RED) {r=255;g=0;b=0;} 
   else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;} 
   else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;} 
   else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
   else if (thiscolor==FL_DEEPPINK) {r=213;g=85;b=178;}
   else if (thiscolor==FL_WHITE) {r=255;g=255;b=255;}   
   else {r=0;g=0;b=0;}

   /* first, check both points are inside the limits and draw them */
   if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1) &&
	(xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)) 
   {
      /* draw both points */
		 
      img[(SMAX*ya+xa)*4+0]=b;
      img[(SMAX*ya+xa)*4+1]=g;
      img[(SMAX*ya+xa)*4+2]=r;
      img[(SMAX*yb+xb)*4+0]=b;
      img[(SMAX*yb+xb)*4+1]=g;
      img[(SMAX*yb+xb)*4+2]=r;
		 
      L=(float)sqrt((double)((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya)));
      imax=4*(int)L+1;
      /* if (debug==1) printf("xa=%d ya=%d xb=%d yb=%d L=%.f imax=%d\n",xa,ya,xb,yb,L,imax);  */
      lastx=xa; lasty=xb; lastcount=0;
      for(i=0;i<=imax;i++)
      {
	 thisy=(int)((float)ya+(float)i/(float)imax*(float)(yb-ya));
	 thisx=(int)((float)xa+(float)i/(float)imax*(float)(xb-xa));
	 if ((thisy==lasty)&&(thisx==lastx)) lastcount++;
	 else 
	 { 
	    if (lastcount>=threshold)
	    { /* draw that point in the image */
	       img[(SMAX*lasty+lastx)*4+0]=b;
	       img[(SMAX*lasty+lastx)*4+1]=g;
	       img[(SMAX*lasty+lastx)*4+2]=r;
	    }
	    lasty=thisy; 
	    lastx=thisx; 
	    lastcount=0;
	 }
      }
      return 0; 
   }
   else return -1;
}

int drawarc(char *img, int xcentro, int ycentro, int radio, int x1, int y1, int x2, int y2, FL_COLOR thiscolor)
{
	
   int r,g,b;
   int x,y;
   float i,imax;

   /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, their are not valid values for the pixels.  */
   if ((x1==x2)&&(y1==y2)){
      drawcircle(img, xcentro, ycentro, radio, thiscolor);
   }else{
      if (thiscolor==FL_BLACK) {r=0;g=0;b=0;}
      else if (thiscolor==FL_RED) {r=255;g=0;b=0;} 
      else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;} 
      else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;} 
      else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
      else if (thiscolor==FL_DEEPPINK) {r=213;g=85;b=178; }   
      else if (thiscolor==FL_WHITE) {r=255;g=255;b=255;}
      else {r=0;g=0;b=0;}
      x1=x1-xcentro;
      y1=ycentro-y1;
      x2=x2-xcentro;
      y2=ycentro-y2;
	   
      if (x1==0){	
	 if (y1<0){							
	    i=3*3.1416/2.;
	 }else{
	    i=3.1416/2.;
	 }
      }else{
	 if (y1==0){
	    if (x1<0){
	       i=3.1416;
	    }else{
	       i=0.;
	    }
	 }else{
	    if (x1>0){
	       i=atan((float)y1/(float)x1);
	    }else{
	       i=atan((float)y1/(float)x1)+3.1416;
	    }
	 }
      }

      i=i*RADTODEG;
	   
      if (x2==0){	
	 if (y2<0){							
	    imax=3*3.1416/2.;
	 }else{
	    imax=3.1416/2.;
	 }
      }else{
	 if (y2==0){
	    if (x2<0){
	       imax=3.1416;
	    }else{
	       imax=0.;
	    }
	 }else{
	    if (x2>0){
	       imax=atan((float)y2/(float)x2);
	    }else{
	       imax=atan((float)y2/(float)x2)+3.1416;
	    }
	 }
      }
      imax=imax*RADTODEG;
      if (imax<i){
	 imax=imax+360;
      }
      for (;i<=imax;i=i+0.1){
	 x=(cos(i*DEGTORAD)*radio+xcentro);
	 y=(ycentro-sin(i*DEGTORAD)*radio);

	 img[(y*SMAX+x)*4]=b;
	 img[(y*SMAX+x)*4+1]=g;
	 img[(y*SMAX+x)*4+2]=r;
      }
   }
   return 0; 
}


void drawcheese (char *img,int x_centro,int y_centro, double h_max, double h_min, double s_max, double s_min, FL_COLOR thiscolor){
   int x1,y1,x2,y2;
   s_max=s_max*radio_hsi_map;	
   s_min=s_min*radio_hsi_map;

   x1=(cos(h_max)*s_min+x_centro);
   y1=(y_centro-sin(h_max)*s_min);
   x2=(cos(h_max)*s_max+x_centro);
   y2=(y_centro-sin(h_max)*s_max);

   lineinimage(img,x1,y1,x2,y2,thiscolor);

   x1=(cos(h_min)*s_min+x_centro);
   y1=(y_centro-sin(h_min)*s_min);
   x2=(cos(h_min)*s_max+x_centro);
   y2=(y_centro-sin(h_min)*s_max);

   lineinimage(img,x1,y1,x2,y2,thiscolor);

   x1=(cos(h_min)*s_max+x_centro);
   y1=(y_centro-sin(h_min)*s_max);
   x2=(cos(h_max)*s_max+x_centro);
   y2=(y_centro-sin(h_max)*s_max);

   drawarc(img,x_centro,y_centro,s_max,x1,y1,x2,y2,thiscolor);	

   x1=(cos(h_min)*s_min+x_centro);
   y1=(y_centro-sin(h_min)*s_min);
   x2=(cos(h_max)*s_min+x_centro);
   y2=(y_centro-sin(h_max)*s_min);

   drawarc(img,x_centro,y_centro,s_min,x1,y1,x2,y2,thiscolor);

}

/*Fin funciones quesito*/

int hsvtunergui_setupDisplay(void) 
     /* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/ 
{
  int vmode;
  XGCValues gc_values;

  gc_values.graphics_exposures = False;
  hsvtuner_gc = XCreateGC(mydisplay,hsvtuner_win, GCGraphicsExposures, &gc_values);
   
  vmode= fl_get_vclass();

  if ((vmode==TrueColor)&&(fl_state[vmode].depth==16))
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),16, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),16, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/* Mapa HSI */
		histograma = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),16, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
	}
	else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24))
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/* Mapa HSI */
		histograma = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),24, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
 	}
	else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32))
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/* Mapa HSI */
		histograma = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),32, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
	}
	else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)) 
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
		
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
		
		/* Mapa HSI */
		histograma = XCreateImage(mydisplay, DefaultVisual(mydisplay,*myscreen),8, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
	}
  else 
  {
		perror("Unsupported color mode in X server");exit(1);
  }
	return 1;
}



void hsvtuner_iteration()
{  
  int i;
  double r,g,b,V,H,S;
  unsigned int X, Y;
  double x,y;
  struct HSV* myHSV;

  all[hsvtuner_id].k++;
  /*  printf("hsvtuner iteration %d\n",d++);*/
  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
    { imagenOrig_buf[i*4]=(*mycolorA)[i*3]; /* Blue Byte */
      imagenOrig_buf[i*4+1]=(*mycolorA)[i*3+1]; /* Green Byte */
      imagenOrig_buf[i*4+2]=(*mycolorA)[i*3+2]; /* Red Byte */
      imagenOrig_buf[i*4+3]=UCHAR_MAX; /* dummy byte */
    }

  for(i=0; i<SMAX*SMAX; i++)
    masc[i]=0;

  for(i=0;i< SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
    {
      /* Modo XLib, 4 bytes por pixel */
      r = (float)(unsigned int)(unsigned char)imagenOrig_buf[i*4+2];
      g = (float)(unsigned int)(unsigned char)imagenOrig_buf[i*4+1];
      b = (float)(unsigned int)(unsigned char)imagenOrig_buf[i*4];

      myHSV = (struct HSV*) RGB2HSV_getHSV( (int)r,(int)g,(int)b);
      H=myHSV->H;
      S=myHSV->S;
      V=myHSV->V;

      /*El resultado de la libreria es  en 
	grados, y la gui se expresa en radianes para la H*/
      H=H*DEGTORAD;
            
      if((V<=v_max)&&(V>=v_min)&&
	  (S >= s_min) && (S <= s_max) && 
	 (((H >= h_min) && (H <= h_max) && (h_min <= h_max)) ||
	  ((H >= h_min) && (H <= 2*PI) && (h_min > h_max)) ||
	  ((H <= h_max) && (H >= 0.) && (h_min > h_max))))
	{
	  /* pasa el filtro */
	  hsifiltrada_buf[i*4+0]=imagenOrig_buf[i*4+0];
	  hsifiltrada_buf[i*4+1]=imagenOrig_buf[i*4+1];
	  hsifiltrada_buf[i*4+2]=imagenOrig_buf[i*4+2];
	  hsifiltrada_buf[i*4+3]=UCHAR_MAX;
	}
      else {
	 /*No pasa el filtro*/
	 if (toblack){
	    /*Pasar a negro*/
	    hsifiltrada_buf[i*4+0] = (unsigned char) 0;
	    hsifiltrada_buf[i*4+1] = (unsigned char) 0;
	    hsifiltrada_buf[i*4+2] = (unsigned char) 0;
	    hsifiltrada_buf[i*4+3] = UCHAR_MAX;
	 }
	 else{
	    /* En vez de a negro lo pasamos a BW, oscurecido para que se vea mejor (180/255 sobre 1) */
	    hsifiltrada_buf[i*4+0] = (unsigned char) (V*180/255);
	    hsifiltrada_buf[i*4+1] = (unsigned char) (V*180/255);
	    hsifiltrada_buf[i*4+2] = (unsigned char) (V*180/255);
	    hsifiltrada_buf[i*4+3] = UCHAR_MAX;
	 }
	}

      /*Apunto los valores en la máscara HSV (histograma negativo) */
	x = S*cos(-H-(3.1416/2.)); /* El rojo (H=0)está a la derecha */
	y = S*sin(-H-(3.1416/2.));

	X = (x + 1.0)*SMAX/2.0;
	Y = (1.0 - y)*SMAX/2.0;

	masc[X*SMAX + Y]++;
    }
 
  /* Dibujamos el histograma negativo */
  memcpy(histograma_buf, disco_buf, SMAX*SMAX*4);
  for (i=0;i<SMAX*SMAX; i++){
    if (masc[i] >= hsimap_threshold)
      {
      histograma_buf[i*4+0] = (char)255; 
      histograma_buf[i*4+1] = (char)255; 
      histograma_buf[i*4+2] = (char)255;
      }
    histograma_buf[i*4+3] = UCHAR_MAX;
    }
    /*dibujamos los quesitos*/
}


void hsvtuner_suspend()
{
  pthread_mutex_lock(&(all[hsvtuner_id].mymutex));
  put_state(hsvtuner_id,slept);
  printf("hsvtuner: off\n");
  pthread_mutex_unlock(&(all[hsvtuner_id].mymutex));
}


void hsvtuner_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[hsvtuner_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[hsvtuner_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[hsvtuner_id].children[i]=FALSE;
  all[hsvtuner_id].father=father;
  if (brothers!=NULL)
   {
     for(i=0;i<MAX_SCHEMAS;i++) hsvtuner_brothers[i]=-1;
     i=0;
     while(brothers[i]!=-1) {hsvtuner_brothers[i]=brothers[i];i++;}
   }

  /* Importamos colorA and launch the colorA child schema */
  mycolorA=myimport ("colorA", "colorA");
  colorAresume=myimport("colorA", "resume");
  colorAsuspend=myimport("colorA", "suspend");  
  if (colorAresume!=NULL)
	colorAresume(hsvtuner_id, NULL, NULL);

  hsvtuner_callforarbitration=fn;
  put_state(hsvtuner_id,notready);
  printf("hsvtuner: on\n");  
    
  RGB2HSV_init();
  RGB2HSV_createTable();

  pthread_cond_signal(&(all[hsvtuner_id].condition));
  pthread_mutex_unlock(&(all[hsvtuner_id].mymutex));
}

void *hsvtuner_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      pthread_mutex_lock(&(all[hsvtuner_id].mymutex));

      if (all[hsvtuner_id].state==slept) 
	{
	  //v=0; w=0;
	  pthread_cond_wait(&(all[hsvtuner_id].condition),&(all[hsvtuner_id].mymutex));
	  pthread_mutex_unlock(&(all[hsvtuner_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[hsvtuner_id].state==notready) put_state(hsvtuner_id,ready);
	  else all[hsvtuner_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[hsvtuner_id].state==ready) put_state(hsvtuner_id,winner);


	  if (all[hsvtuner_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[hsvtuner_id].mymutex));
	      gettimeofday(&a,NULL);
	      hsvtuner_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = hsvtuner_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(hsvtuner_cycle*1000-diff);
	      else 
		{printf("time interval violated: hsvtuner\n"); usleep(hsvtuner_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[hsvtuner_id].mymutex));
	      usleep(hsvtuner_cycle*1000);
	    }
	}
    }
}

void hsvtuner_init(){
   if ((mydisplay= (Display *)myimport("graphics_xforms", "display"))==NULL){
      fprintf (stderr, "hsvtuner: I can't fetch display from graphics_xforms\n");
      jdeshutdown(1);
   }
   if ((myscreen= (int *)myimport("graphics_xforms", "screen"))==NULL){
      fprintf (stderr, "hsvtuner: I can't fetch screen from graphics_xforms\n");
      jdeshutdown(1);
   }

   if (myregister_buttonscallback==NULL){
      if ((myregister_buttonscallback=(registerbuttons)myimport ("graphics_xforms", "register_buttonscallback"))==NULL){
         printf ("hsvtuner: I can't fetch register_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_buttonscallback=(deletebuttons)myimport ("graphics_xforms", "delete_buttonscallback"))==NULL){
         printf ("hsvtuner: I can't fetch delete_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_xforms", "register_displaycallback"))==NULL){
         printf ("hsvtuner: I can't fetch register_displaycallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_xforms", "delete_displaycallback"))==NULL){
         jdeshutdown(1);
         printf ("hsvtuner: I can't fetch delete_displaycallback from graphics_xforms\n");
      }
   }
}

void hsvtuner_stop()
{

  /*
  pthread_mutex_lock(&(all[hsvtuner_id].mymutex));
  hsvtuner_suspend();  
  pthread_mutex_unlock(&(all[hsvtuner_id].mymutex));
  sleep(2);
  */
  if (fd_hsvtunergui!=NULL)
    {
      if (all[hsvtuner_id].guistate==on) 
	fl_hide_form(fd_hsvtunergui->hsvtunergui);
      fl_free_form(fd_hsvtunergui->hsvtunergui);
    }
  printf ("hsvtuner close\n");
}


void hsvtuner_startup()
{
  draw_hsvmap(disco_buf, SMAX);
  v_min=0.; v_max=255.;
  h_min=0.; h_max=1.;
  s_min=0.2; s_max=0.4;
  hsimap_threshold=20;  

  pthread_mutex_lock(&(all[hsvtuner_id].mymutex));
  myexport("hsvtuner","id",&hsvtuner_id);
  myexport("hsvtuner","resume",(void *) &hsvtuner_resume);
  myexport("hsvtuner","suspend",(void *) &hsvtuner_suspend);
  
  printf("hsvtuner schema started up\n");
  put_state(hsvtuner_id,slept);
  hsvtuner_init();
  pthread_create(&(all[hsvtuner_id].mythread),NULL,hsvtuner_thread,NULL);
  pthread_mutex_unlock(&(all[hsvtuner_id].mymutex));
}


void hsvtuner_guibuttons(void *obj2)
{ 
   double aux;
   FL_OBJECT *obj=(FL_OBJECT *)obj2;
   
  if (obj == fd_hsvtunergui->Hmax){
       h_max=fl_get_slider_value(fd_hsvtunergui->Hmax);
       if(h_max>2*PI){
	  h_max=h_max-2*PI;
       }
       if(h_max<0){
	  h_max=h_max+2*PI;
       }
       if (h_max==h_min){
	  fl_set_slider_value(fd_hsvtunergui->Smin,0.0);
	  s_min=0.0;
       }
    }	
  else if (obj == fd_hsvtunergui->Hmin){  
     h_min=fl_get_slider_value(fd_hsvtunergui->Hmin);
     if(h_min>2*PI){
	h_min=h_min-2*PI;
     }
     if(h_max<0){
	h_min=h_min+2*PI;
     }
     if (h_max==h_min){
	fl_set_slider_value(fd_hsvtunergui->Smin,0.0);
	s_min=0.0;
     }
    }	
  
  else if (obj == fd_hsvtunergui->Smax){
    s_max=fl_get_slider_value(fd_hsvtunergui->Smax);
    if (s_max<=s_min){
       aux=s_min;
       s_min=s_max;
       s_max=aux;
       fl_set_slider_value(fd_hsvtunergui->Smax,s_max);
       fl_set_slider_value(fd_hsvtunergui->Smin,s_min);
    }
  }	
  
  else if (obj == fd_hsvtunergui->Smin)
    {
      s_min=fl_get_slider_value(fd_hsvtunergui->Smin);
      if (s_max<=s_min){
	 aux=s_min;
	 s_min=s_max;
	 s_max=aux;
	 fl_set_slider_value(fd_hsvtunergui->Smax,s_max);
	 fl_set_slider_value(fd_hsvtunergui->Smin,s_min);
      }
    }	
  
  else if (obj == fd_hsvtunergui->Vmax)
    {v_max=fl_get_slider_value(fd_hsvtunergui->Vmax);
      if (v_max<=v_min){
      aux=v_min;
      v_min=v_max;
      v_max=aux;
      fl_set_slider_value(fd_hsvtunergui->Vmax,v_max);
      fl_set_slider_value(fd_hsvtunergui->Vmin,v_min);
      }
    }	
  
  else if (obj == fd_hsvtunergui->Vmin)
    {v_min=fl_get_slider_value(fd_hsvtunergui->Vmin);
    if (v_max<=v_min){
      aux=v_min;
      v_min=v_max;
      v_max=aux;
      fl_set_slider_value(fd_hsvtunergui->Vmax,v_max);
      fl_set_slider_value(fd_hsvtunergui->Vmin,v_min);
      }
    }
  
  else if (obj ==fd_hsvtunergui->w_slider)
    {
      hsimap_threshold=(int)fl_get_slider_value(fd_hsvtunergui->w_slider);
      /*printf("hsimap_threshold %d\n",hsimap_threshold);*/
  }
  if (obj ==fd_hsvtunergui->toblack){
     if (fl_get_button(fd_hsvtunergui->toblack)==RELEASED){
	toblack=0;
     }
     else 
	toblack=1;
  }
}

void hsvtuner_guidisplay()
{
   drawcheese(histograma_buf,centro_x,centro_y,h_max,h_min,s_max,s_min,FL_PALEGREEN);
  
  XPutImage(mydisplay,hsvtuner_win,hsvtuner_gc,imagenOrig,0,0,fd_hsvtunergui->oculo_orig->x, fd_hsvtunergui->oculo_orig->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
  
  XPutImage(mydisplay,hsvtuner_win,hsvtuner_gc,hsifiltrada,0,0,fd_hsvtunergui->oculo_modif->x, fd_hsvtunergui->oculo_modif->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
 
  XPutImage(mydisplay,hsvtuner_win,hsvtuner_gc,histograma,0,0,fd_hsvtunergui->histograma->x, fd_hsvtunergui->histograma->y, SMAX, SMAX);
}


void hsvtuner_guisuspend_aux(void)
{
   mydelete_buttonscallback(hsvtuner_guibuttons);
   mydelete_displaycallback(hsvtuner_guidisplay);
   fl_hide_form(fd_hsvtunergui->hsvtunergui);
}

void hsvtuner_guisuspend(void)
{
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)hsvtuner_guisuspend_aux);
      }
   }
   else{
      fn ((gui_function)hsvtuner_guisuspend_aux);
   }
}

void hsvtuner_guiresume_aux(void)
{
  static int k=0;

  if (k==0) /* not initialized */
    {
      k++;
      fd_hsvtunergui = create_form_hsvtunergui();
      fl_set_form_position(fd_hsvtunergui->hsvtunergui,400,50);
      fl_show_form(fd_hsvtunergui->hsvtunergui,FL_PLACE_POSITION,FL_FULLBORDER,HSVtunerVer);
      hsvtuner_win= FL_ObjWin(fd_hsvtunergui->oculo_orig);
      hsvtunergui_setupDisplay();
    }
  else 
    {
      fl_show_form(fd_hsvtunergui->hsvtunergui,FL_PLACE_POSITION,FL_FULLBORDER,HSVtunerVer);
      hsvtuner_win= FL_ObjWin(fd_hsvtunergui->oculo_orig);
    }

  myregister_buttonscallback(hsvtuner_guibuttons);
  myregister_displaycallback(hsvtuner_guidisplay);

  fl_set_slider_bounds(fd_hsvtunergui->w_slider,100,1);
  fl_set_slider_value(fd_hsvtunergui->w_slider,hsimap_threshold);
  fl_set_slider_bounds(fd_hsvtunergui->Vmin,255,0);
  fl_set_slider_value(fd_hsvtunergui->Vmin,v_min);
  fl_set_slider_bounds(fd_hsvtunergui->Vmax,255,0);
  fl_set_slider_value(fd_hsvtunergui->Vmax,v_max);
  fl_set_slider_bounds(fd_hsvtunergui->Hmin,2.*PI,0);
  fl_set_slider_value(fd_hsvtunergui->Hmin,h_min);
  fl_set_slider_bounds(fd_hsvtunergui->Hmax,2.*PI,0);
  fl_set_slider_value(fd_hsvtunergui->Hmax,h_max);
  fl_set_slider_bounds(fd_hsvtunergui->Smin,1,0);
  fl_set_slider_value(fd_hsvtunergui->Smin,s_min);
  fl_set_slider_bounds(fd_hsvtunergui->Smax,1,0);
  fl_set_slider_value(fd_hsvtunergui->Smax,s_max);
}

void hsvtuner_guiresume(void)
{
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)hsvtuner_guiresume_aux);
      }
   }
   else{
      fn ((gui_function)hsvtuner_guiresume_aux);
   }
}

int handle2 (FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my, int key, void *xev){
   int x_matriz,y_matriz;
   float r,g,b;
   double H,S,I;
   struct HSV* myHSV;

   if (event==FL_PUSH){
      if ((my>=10) && (my<=250) && (mx>=430) && (mx<=750)){			
	 x_matriz=mx-430;
	 y_matriz=my-10;
	 primera=1;
	 r=(float)(unsigned int)(unsigned char)imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4+2];
	 g=(float)(unsigned int)(unsigned char)imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4+1];
	 b=(float)(unsigned int)(unsigned char)imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4];
	 //printf("R,G,B %1f %1f %1f \n",r,g,b);
	 myHSV = (struct HSV*) RGB2HSV_getHSV( (int)r,(int)g,(int)b);
	 H=myHSV->H;
	 S=myHSV->S;
	 I=myHSV->V;

	 /*El resultado de la libreria es  en 
	  grados, y la gui se expresa en radianes para la H*/
	 H=H*DEGTORAD;

	 h_max=H+.2;
	 h_min=H-.2;
	 if (h_max>2*3.1416){
	    h_max=h_max-2*3.1416;
	 }
	 if (h_min<0){
	    h_min=h_min+2*3.1416;
	 }
	 s_max=S+.1;
	 s_min=S-.1;
	 if (s_max>1.){
	    s_max=1.;
	 }
	 if (s_min<0){
	    s_min=0;
	 }
	 v_max=I+50.;
	 v_min=I-50.;
	 if (v_max>255.){
	    v_max=255.;
	 }
	 if (v_min<0.){
	    v_min=0.;
	 }
	 fl_set_slider_value(fd_hsvtunergui->Hmax,h_max);
	 fl_set_slider_value(fd_hsvtunergui->Hmin,h_min);
	 fl_set_slider_value(fd_hsvtunergui->Smax,s_max);
	 fl_set_slider_value(fd_hsvtunergui->Smin,s_min);
	 fl_set_slider_value(fd_hsvtunergui->Vmax,v_max);
	 fl_set_slider_value(fd_hsvtunergui->Vmin,v_min);
      }

		
   }
   return 0;
}
	
int handle (FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my, int key, void *xev){
   if(event==FL_DBLCLICK){
      /*printf("He hecho doble click\n");*/
      x_pulsada=mx-10;y_pulsada=my-10;
      x_pulsada=x_pulsada-centro_x;
      y_pulsada=centro_y-y_pulsada;
      if (x_pulsada==0){	
	 if (y_pulsada<0){	
	    h_max=3.1416/2.;
	 }else{
	    h_max=3.*3.1416/2.;
	 }
      }else{
	 if (y_pulsada==0){
	    if (x_pulsada<0){
	       h_max=3.1416;
	    }else{
	       h_max=0.;
	    }
	 }else{
	    if (x_pulsada>0){
	       h_max=atan((float)(y_pulsada)/(float)x_pulsada);
	       if(y_pulsada<0){
		  h_max=h_max+2*3.1416;
	       }
	    }else{
	       h_max=atan((float)y_pulsada/(float)x_pulsada)+3.1416;
	    }
	 }
      }
      h_max=h_max+.2;
      h_min=h_max-.4;
      if (h_max>(2.*3.1416)){
	 h_max=h_max-(2.*3.1416);
      }
      if (h_min<0){
	 h_min=h_min+(2.*3.1416);
      }
      s_max=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
      s_max=s_max+.1;
      s_min=s_max-.2;
      if (s_max>1.){
	 s_max=1.;
      }
      if (s_min<0.){
	 s_min=0.;
      }
      /*CAMBIO LOS VALORES DE LOS SLIDERS*/
      fl_set_slider_value(fd_hsvtunergui->Hmax,h_max);
      fl_set_slider_value(fd_hsvtunergui->Hmin,h_min);
      fl_set_slider_value(fd_hsvtunergui->Smax,s_max);
      fl_set_slider_value(fd_hsvtunergui->Smin,s_min);
   }
   xquesito1=cos(h_max)*s_max*radio_hsi_map+centro_x;
   yquesito1=centro_y-sin(h_max)*s_max*radio_hsi_map;
	
   xquesito2=cos(h_min)*s_max*radio_hsi_map+centro_x;
   yquesito2=centro_y-sin(h_min)*s_max*radio_hsi_map;
	
   xquesito3=cos(h_max)*s_min*radio_hsi_map+centro_x;
   yquesito3=centro_y-sin(h_max)*s_min*radio_hsi_map;
	
   xquesito4=cos(h_min)*s_min*radio_hsi_map+centro_x;
   yquesito4=centro_y-sin(h_min)*s_min*radio_hsi_map;

   if (event==FL_PUSH){
      /*printf("H_max:%f,H_min:%f,S_max:%f,S_min:%f\n",h_max,h_min,s_max,s_min);*/
      if (((mx-10)<=xquesito1+5) && ((mx-10)>=xquesito1-5)){
	 if (((my-10)<=yquesito1+5) && ((my-10)>=yquesito1-5)){
	    x_pulsada=mx-10;y_pulsada=my-10;
	    pulsada=1;
	 }
      }
      if (((mx-10)<=xquesito2+5) && ((mx-10)>=xquesito2-5)){
	 if (((my-10)<=yquesito2+5) && ((my-10)>=yquesito2-5)){
	    x_pulsada=mx-10;y_pulsada=my-10;
	    pulsada=2;
	 }
      }
      if (((mx-10)<=xquesito3+5) && ((mx-10)>=xquesito3-5)){
	 if (((my-10)<=yquesito3+5) && ((my-10)>=yquesito3-5)){
	    x_pulsada=mx-10;y_pulsada=my-10;
	    pulsada=3;
	 }
      }
      if (((mx-10)<=xquesito4+5) && ((mx-10)>=xquesito4-5)){
	 if (((my-10)<=yquesito4+5) && ((my-10)>=yquesito4-5)){
	    x_pulsada=mx-10;y_pulsada=my-10;
	    pulsada=4;
	 }
      }
      if (h_max==h_min){			
	 x_pulsada=mx-10;
	 y_pulsada=my-10;
	 x_pulsada=x_pulsada-centro_x;
	 y_pulsada=centro_y-y_pulsada;
	 if ((float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map<=1){
	    s_max=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
	    pulsada=0;
	    s_min=0.;
	    fl_set_slider_value(fd_hsvtunergui->Smax,s_max);
	    fl_set_slider_value(fd_hsvtunergui->Smin,s_min);
	 }
      }
		
   }
   if (event==FL_MOUSE){
      if (pulsada>0){
	 xsoltada=mx-10;ysoltada=my-10;
	 if (sqrt((xsoltada-centro_x)*(xsoltada-centro_x)+(ysoltada-centro_y)*(ysoltada-centro_y))<=radio_hsi_map){
	    xsoltada=xsoltada-centro_x;
	    ysoltada=centro_y-ysoltada;
	    switch(pulsada){
	       case 1:					
		  s_max=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
		  if (xsoltada==0){	
		     if (ysoltada<0){
			h_max=3*3.1416/2.;
		     }else{
			h_max=3.1416/2.;
		     }
		  }else{
		     if (ysoltada==0){
			if (xsoltada<0){
			   h_max=3.1416;
			}else{
			   h_max=0.;
			}
		     }else{
			if (xsoltada>0){
			   h_max=atan((float)ysoltada/(float)xsoltada);
			   if(ysoltada<0){
			      h_max=h_max+2*3.1416;
			   }
			}else{
			   h_max=atan((float)ysoltada/(float)xsoltada)+3.1416;
			}
		     }
		  }
		  break;
	       case 2:
		  s_max=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
		  if (xsoltada==0){	
		     if (ysoltada<0){
			h_min=3*3.1416/2.;
		     }else{
			h_min=3.1416/2.;
		     }
		  }else{
		     if (ysoltada==0){
			if (xsoltada<0){
			   h_min=3.1416;
			}else{
			   h_min=0.;
			}
		     }else{
			if (xsoltada>0){
			   h_min=atan((float)ysoltada/(float)xsoltada);
			   if(ysoltada<0){
			      h_min=h_min+2*3.1416;
			   }
			}else{
			   h_min=atan((float)ysoltada/(float)xsoltada)+3.1416;
			}
		     }
		  }
		  break;
	       case 3:
		  s_min=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;	
		  if (xsoltada==0){	
		     if (ysoltada<0){
			h_max=3*3.1416/2.;
		     }else{
			h_max=3.1416/2.;
		     }
		  }else{
		     if (ysoltada==0){
			if (xsoltada<0){
			   h_max=3.1416;
			}else{
			   h_max=0.;
			}
		     }else{
			if (xsoltada>0){
			   h_max=atan((float)ysoltada/(float)xsoltada);
			   if(ysoltada<0){
			      h_max=h_max+2*3.1416;
			   }
			}else{
			   h_max=atan((float)ysoltada/(float)xsoltada)+3.1416;
			}
		     }
		  }

		  break;
	       case 4:	
		  s_min=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
		  if (xsoltada==0){
		     if (ysoltada<0){
			h_min=3*3.1416/2.;
		     }else{
			h_min=3.1416/2.;
		     }
		  }else{
		     if (ysoltada==0){
			if (xsoltada<0){
			   h_min=3.1416;
			}else{
			   h_min=0.;
			}
		     }else{
			if (xsoltada>0){
			   h_min=atan((float)ysoltada/(float)xsoltada);
			   if(ysoltada<0){
			      h_min=h_min+2*3.1416;
			   }
			}else{
			   h_min=atan((float)ysoltada/(float)xsoltada)+3.1416;
			}
		     }
		  }
		  break;
	       default:
		  break;
	    }
	    fl_set_slider_value(fd_hsvtunergui->Hmax,h_max);
	    fl_set_slider_value(fd_hsvtunergui->Hmin,h_min);
	    fl_set_slider_value(fd_hsvtunergui->Smax,s_max);
	    fl_set_slider_value(fd_hsvtunergui->Smin,s_min);
	 }
      }
   }
   if (event==FL_RELEASE){
      pulsada=0;
   }
   return 0;
}

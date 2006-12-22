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

#include <jde.h>
#include <jdegui.h>
#include "hsitunergui.h"
#include <math.h>

#define HSItunerVER "HSItuner 2.7"

int hsituner_id=0;
int hsituner_brothers[MAX_SCHEMAS];
arbitration hsituner_callforarbitration;
int hsituner_cycle=150; /* ms */

FD_hsitunergui *fd_hsitunergui;

/* Necesarias para las Xlib */
GC hsituner_gc;
Window hsituner_win; /* image window */
XImage *imagenOrig;
XImage *hsifiltrada;
XImage *histograma;

#define PI 3.141592654
/* this memory must exist even with the hsitunergui turned on */
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

double s_min, s_max, h_min, h_max, i_min, i_max;
int hsimap_threshold;

/*Conversor a HSI*/
void rgb2hsi (double r, double g, double b, double *H, double *S, double *I){
   double a, n, d;

   *I = (r + b + g) / 3.0;

   /* El minimo*/
   if ((r <= g) && (r <= b)){
      a = r;
   }
   else if ((g <= r) && (g <= b)){
      a = g;
   }
   else{
      a = b;
   }

   if ((r + b + g) == 0){
      *S = 1.0;
   }
   else{
      *S = 1.0 - (3.0 / (r + b + g)) * a;
   }

   n = .5 * ((r - g) + (r - b));
   d = sqrt ((r - g) * (r - g) + (r - b) * (g - b));
   if ((d == 0) || (*S == 1) || (*S == 0)){
      /* En estos casos *H no tiene sentido*/
      *H = 0.0;
   }
   else{
			*H = acos (n / d);/* Falta medio círculo*/
   }
   if (b < g){
      /* Círculo completo*/
      *H = (2*PI) - *H;
   }
}

void hsi2rgb (double H, double S, double I, double *r, double *g, double *b)
{
   /*Pasa de hsi a rgb. Todos entre 0 y 1, execpto H, que esta entre 0 y 2*pi */

   if (S <= 1)
   {
      /* Hay que pasar el punto HS a el espacio RGB */
      if (H <= 2.0 * PI / 3.0)
      {
	 *b = (1.0 - S) / 3.0;
	 *r = (1.0 + ((S * cos (H)) / (cos (PI / 3.0 - H)))) / 3.0;
	 *g = 1.0 - (*b + *r);
      }
      else if (H <= 4.0 * PI / 3.0)
      {
	 H = H - 2 * PI / 3.0;
	 *r = (1.0 - S) / 3.0;
	 *g = (1.0 + ((S * cos (H)) / (cos (PI / 3.0 - H)))) / 3.0;
	 *b = 1.0 - (*r + *g);
      }
      else if (H <= 2.0 * PI)
      {
	 H = H - 4.0 * PI / 3.0;
	 *g = (1.0 - S) / 3.0;
	 *b = (1.0 + ((S * cos (H)) / (cos (PI / 3.0 - H)))) / 3.0;
	 *r = 1.0 - (*g + *b);
      }
   }
   else
   {			/* Puntos fuera del circulo negros */
      *r = 0;
      *b = 0;
      *g = 0;
   }
}

void draw_hsimap(char *buffer, int size){
   int i,j,ind; 
   float x, y, H, S, scale;
   double r,g,b;
   unsigned char R,G,B;

   for(j=0; j < size; j++){  
      for(i=0; i < size; i++){
	 x = (2.0*i)/size - 1.0;
	 y = 1.0 - (2.0*j)/size;
	 if ((x >= -1e-7) && (x <= 1e-7)) {
	    if (y > 0){
	       H = PI/2.0;
	    } else{
	       H = 3.0*PI/2.0;
	    }
	 } else { /*x != 0 */
	    if (x < 0){
	       H = atan(y/x) + PI;
	    } else {
	       H = atan (y/x);
	    }
	 }
	 H=-H;
	 if (H>2*PI){
	    H-=2*PI;
	 }
	 if (H<0){
	    H+=2*PI;
	 }
	
	 S = sqrt(y*y + x*x);
	 ind = (size*j + i)*4; 
	
	 hsi2rgb (H,S,0,&r,&g,&b);
	 scale = 255.0;
	 R = (unsigned char) (scale * r);
	 G = (unsigned char) (scale * g);
	 B = (unsigned char) (scale * b);
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

int hsitunergui_setupDisplay(void) 
     /* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/ 
{
  int vmode;
  XGCValues gc_values;

  gc_values.graphics_exposures = False;
  hsituner_gc = XCreateGC(display,hsituner_win, GCGraphicsExposures, &gc_values); 
   
  vmode= fl_get_vclass();

  if ((vmode==TrueColor)&&(fl_state[vmode].depth==16))
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(display,DefaultVisual(display,screen),16, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(display, DefaultVisual(display,screen),16, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/* Mapa HSI */
		histograma = XCreateImage(display, DefaultVisual(display,screen),16, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
	}
	else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24))
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(display,DefaultVisual(display,screen),24, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(display, DefaultVisual(display,screen),24, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/* Mapa HSI */
		histograma = XCreateImage(display, DefaultVisual(display,screen),24, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
 	}
	else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32))
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(display,DefaultVisual(display,screen),32, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(display, DefaultVisual(display,screen),32, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	
		/* Mapa HSI */
		histograma = XCreateImage(display, DefaultVisual(display,screen),32, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
	}
	else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8)) 
	{
		/* Imagen principal */
		imagenOrig = XCreateImage(display,DefaultVisual(display,screen),8, ZPixmap,0,imagenOrig_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
		
		/*Imagen filtrada */
		hsifiltrada = XCreateImage(display, DefaultVisual(display,screen),8, ZPixmap,0,hsifiltrada_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
		
		/* Mapa HSI */
		histograma = XCreateImage(display, DefaultVisual(display,screen),8, ZPixmap,0,histograma_buf,SMAX,SMAX,8,0);
	}
  else 
  {
		perror("Unsupported color mode in X server");exit(1);
  }
	return 1;
}



void hsituner_iteration()
{  
  int i;
  double r,g,b,I,H,S;
  unsigned int X, Y;
  double x,y;

  all[hsituner_id].k++;
  /*  printf("hsituner iteration %d\n",d++);*/
  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++) 
    { imagenOrig_buf[i*4]=colorA[i*3]; /* Blue Byte */
    imagenOrig_buf[i*4+1]=colorA[i*3+1]; /* Green Byte */
    imagenOrig_buf[i*4+2]=colorA[i*3+2]; /* Red Byte */
    imagenOrig_buf[i*4+3]=0; /* dummy byte */  
    }
 
  for(i=0; i<SMAX*SMAX; i++)
    masc[i]=0;

  for(i=0;i< SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
    {
      /* Modo XLib, 4 bytes por pixel */
      r = (float)(unsigned int)(unsigned char)imagenOrig_buf[i*4+2]/255.0;
      g = (float)(unsigned int)(unsigned char)imagenOrig_buf[i*4+1]/255.0;
      b = (float)(unsigned int)(unsigned char)imagenOrig_buf[i*4]/255.0;
      rgb2hsi (r, g, b, &H, &S, &I);
    
      if((I<=i_max/255.0)&&(I>=i_min/255.0)&&
	  (S >= s_min) && (S <= s_max) && 
	 (((H >= h_min) && (H <= h_max) && (h_min < h_max)) ||
	  ((H >= h_min) && (H <= 2*PI) && (h_min > h_max)) ||
	  ((H <= h_max) && (H >= 0.) && (h_min > h_max))))
	{
	  /* pasa el filtro */
	  hsifiltrada_buf[i*4+0]=imagenOrig_buf[i*4+0];
	  hsifiltrada_buf[i*4+1]=imagenOrig_buf[i*4+1];
	  hsifiltrada_buf[i*4+2]=imagenOrig_buf[i*4+2];
	  hsifiltrada_buf[i*4+3]=0;
	}
      else {
	 /*No pasa el filtro*/
	 if (toblack){
	    /*Pasar a negro*/
	    hsifiltrada_buf[i*4+0] = (unsigned char) 0;
	    hsifiltrada_buf[i*4+1] = (unsigned char) 0;
	    hsifiltrada_buf[i*4+2] = (unsigned char) 0;
	    hsifiltrada_buf[i*4+3] = 0;
	 }
	 else{
	    /* En vez de a negro lo pasamos a BW, oscurecido para que se vea mejor (180/255 sobre 1) */
	    hsifiltrada_buf[i*4+0] = (unsigned char) (180.0*I);
	    hsifiltrada_buf[i*4+1] = (unsigned char) (180.0*I);
	    hsifiltrada_buf[i*4+2] = (unsigned char) (180.0*I);
	    hsifiltrada_buf[i*4+3] = 0;
	 }
	}

      /*Apunto los valores en la máscara HSI (histograma negativo) */
	x = S*cos(-H-(3.1416/2.)); /* El rojo (H=0)está a la derecha */
	y = S*sin(-H-(3.1416/2.));

	X = (x + 1.0)*SMAX/2.0;
	Y = (1.0 - y)*SMAX/2.0;

	masc[X*SMAX + Y]++;
    }
 
  /* Dibujamos el histograma negativo */
  memcpy(histograma_buf, disco_buf, SMAX*SMAX*4);
  for (i=0;i<=SMAX*SMAX; i++){
    if (masc[i] >= hsimap_threshold)
      {
      histograma_buf[i*4+0] = (char)255; 
      histograma_buf[i*4+1] = (char)255; 
      histograma_buf[i*4+2] = (char)255; 
      }
    }
    /*dibujamos los quesitos*/
}


void hsituner_suspend()
{
  pthread_mutex_lock(&(all[hsituner_id].mymutex));
  put_state(hsituner_id,slept);
  printf("hsituner: off\n");
  pthread_mutex_unlock(&(all[hsituner_id].mymutex));
}


void hsituner_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[hsituner_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[hsituner_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[hsituner_id].children[i]=FALSE;
  all[hsituner_id].father=father;
  if (brothers!=NULL)
   {
     for(i=0;i<MAX_SCHEMAS;i++) hsituner_brothers[i]=-1;
     i=0;
     while(brothers[i]!=-1) {hsituner_brothers[i]=brothers[i];i++;}
   }
  hsituner_callforarbitration=fn;
  put_state(hsituner_id,notready);
  printf("hsituner: on\n");
  pthread_cond_signal(&(all[hsituner_id].condition));
  pthread_mutex_unlock(&(all[hsituner_id].mymutex));
}

void *hsituner_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      pthread_mutex_lock(&(all[hsituner_id].mymutex));

      if (all[hsituner_id].state==slept) 
	{
	  v=0; w=0;
	  pthread_cond_wait(&(all[hsituner_id].condition),&(all[hsituner_id].mymutex));
	  pthread_mutex_unlock(&(all[hsituner_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[hsituner_id].state==notready) put_state(hsituner_id,ready);
	  else all[hsituner_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[hsituner_id].state==ready) put_state(hsituner_id,winner);


	  if (all[hsituner_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[hsituner_id].mymutex));
	      gettimeofday(&a,NULL);
	      hsituner_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = hsituner_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(hsituner_cycle*1000-diff);
	      else 
		{printf("time interval violated: hsituner\n"); usleep(hsituner_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[hsituner_id].mymutex));
	      usleep(hsituner_cycle*1000);
	    }
	}
    }
}

void hsituner_startup()
{
  draw_hsimap(disco_buf, SMAX);
  i_min=0.; i_max=255.;
  h_min=0.; h_max=1.;
  s_min=0.2; s_max=0.4;
  hsimap_threshold=20;  

  pthread_mutex_lock(&(all[hsituner_id].mymutex));
  printf("hsituner schema started up\n");
  put_state(hsituner_id,slept);
  pthread_create(&(all[hsituner_id].mythread),NULL,hsituner_thread,NULL);
  pthread_mutex_unlock(&(all[hsituner_id].mymutex));
}


void hsituner_guibuttons(FL_OBJECT *obj)
{ 
   double aux;
   
  if (obj == fd_hsitunergui->Hmax){
       h_max=fl_get_slider_value(fd_hsitunergui->Hmax);
       if(h_max>2*PI){
	  h_max=h_max-2*PI;
       }
       if(h_max<0){
	  h_max=h_max+2*PI;
       }
       if (h_max==h_min){
	  fl_set_slider_value(fd_hsitunergui->Smin,0.0);
	  s_min=0.0;
       }
    }	
  else if (obj == fd_hsitunergui->Hmin){  
     h_min=fl_get_slider_value(fd_hsitunergui->Hmin);
     if(h_min>2*PI){
	h_min=h_min-2*PI;
     }
     if(h_max<0){
	h_min=h_min+2*PI;
     }
     if (h_max==h_min){
	fl_set_slider_value(fd_hsitunergui->Smin,0.0);
	s_min=0.0;
     }
    }	
  
  else if (obj == fd_hsitunergui->Smax){
    s_max=fl_get_slider_value(fd_hsitunergui->Smax);
    if (s_max<=s_min){
       aux=s_min;
       s_min=s_max;
       s_max=aux;
       fl_set_slider_value(fd_hsitunergui->Smax,s_max);
       fl_set_slider_value(fd_hsitunergui->Smin,s_min);
    }
  }	
  
  else if (obj == fd_hsitunergui->Smin)
    {
      s_min=fl_get_slider_value(fd_hsitunergui->Smin);
      if (s_max<=s_min){
	 aux=s_min;
	 s_min=s_max;
	 s_max=aux;
	 fl_set_slider_value(fd_hsitunergui->Smax,s_max);
	 fl_set_slider_value(fd_hsitunergui->Smin,s_min);
      }
    }	
  
  else if (obj == fd_hsitunergui->Imax)
    {i_max=fl_get_slider_value(fd_hsitunergui->Imax);
      if (i_max<=i_min){
      aux=i_min;
      i_min=i_max;
      i_max=aux;
      fl_set_slider_value(fd_hsitunergui->Imax,i_max);
      fl_set_slider_value(fd_hsitunergui->Imin,i_min);
      }
    }	
  
  else if (obj == fd_hsitunergui->Imin)
    {i_min=fl_get_slider_value(fd_hsitunergui->Imin);
    if (i_max<=i_min){
      aux=i_min;
      i_min=i_max;
      i_max=aux;
      fl_set_slider_value(fd_hsitunergui->Imax,i_max);
      fl_set_slider_value(fd_hsitunergui->Imin,i_min);
      }
    }
  
  else if (obj ==fd_hsitunergui->w_slider)
    {
      hsimap_threshold=(int)fl_get_slider_value(fd_hsitunergui->w_slider);
      /*printf("hsimap_threshold %d\n",hsimap_threshold);*/
  }
  if (obj ==fd_hsitunergui->toblack){
     if (fl_get_button(fd_hsitunergui->toblack)==RELEASED){
	toblack=0;
     }
     else 
	toblack=1;
  }
}

void hsituner_guidisplay()
{
   drawcheese(histograma_buf,centro_x,centro_y,h_max,h_min,s_max,s_min,FL_PALEGREEN);
  
  XPutImage(display,hsituner_win,hsituner_gc,imagenOrig,0,0,fd_hsitunergui->oculo_orig->x, fd_hsitunergui->oculo_orig->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
  
  XPutImage(display,hsituner_win,hsituner_gc,hsifiltrada,0,0,fd_hsitunergui->oculo_modif->x, fd_hsitunergui->oculo_modif->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
 
  XPutImage(display,hsituner_win,hsituner_gc,histograma,0,0,fd_hsitunergui->histograma->x, fd_hsitunergui->histograma->y, SMAX, SMAX);
}


void hsituner_guisuspend(void)
{
   delete_buttonscallback(hsituner_guibuttons);
   delete_displaycallback(hsituner_guidisplay);
   fl_hide_form(fd_hsitunergui->hsitunergui);
}

void hsituner_guiresume(void)
{
  static int k=0;

  if (k==0) /* not initialized */
    {
      k++;
      fd_hsitunergui = create_form_hsitunergui();
      fl_set_form_position(fd_hsitunergui->hsitunergui,400,50);
      fl_show_form(fd_hsitunergui->hsitunergui,FL_PLACE_POSITION,FL_FULLBORDER,HSItunerVER);
      hsituner_win= FL_ObjWin(fd_hsitunergui->oculo_orig);
      hsitunergui_setupDisplay();
    }
  else 
    {
      fl_show_form(fd_hsitunergui->hsitunergui,FL_PLACE_POSITION,FL_FULLBORDER,HSItunerVER);
      hsituner_win= FL_ObjWin(fd_hsitunergui->oculo_orig);
    }

  register_buttonscallback(hsituner_guibuttons);
  register_displaycallback(hsituner_guidisplay);

  fl_set_slider_bounds(fd_hsitunergui->w_slider,100,1);
  fl_set_slider_value(fd_hsitunergui->w_slider,hsimap_threshold);
  fl_set_slider_bounds(fd_hsitunergui->Imin,0,255);
  fl_set_slider_value(fd_hsitunergui->Imin,i_min);
  fl_set_slider_bounds(fd_hsitunergui->Imax,0,255);
  fl_set_slider_value(fd_hsitunergui->Imax,i_max);
  fl_set_slider_bounds(fd_hsitunergui->Hmin,2.*PI,0);
  fl_set_slider_value(fd_hsitunergui->Hmin,h_min);
  fl_set_slider_bounds(fd_hsitunergui->Hmax,2.*PI,0);
  fl_set_slider_value(fd_hsitunergui->Hmax,h_max);
  fl_set_slider_bounds(fd_hsitunergui->Smin,1,0);
  fl_set_slider_value(fd_hsitunergui->Smin,s_min);
  fl_set_slider_bounds(fd_hsitunergui->Smax,1,0);
  fl_set_slider_value(fd_hsitunergui->Smax,s_max);
}

int handle2 (FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my, int key, void *xev){
   int x_matriz,y_matriz;
   unsigned char r,g,b;
   double H,S,I;
   if (event==FL_PUSH){
      if ((my>=10) && (my<=250) && (mx>=430) && (mx<=750)){			
	 x_matriz=mx-430;
	 y_matriz=my-10;
	 r=imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4+2];
	 g=imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4+1];
	 b=imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4];
	 /*printf("r:%d,g:%d,b:%d\n",r,g,b);*/
	 rgb2hsi(r/255.0,g/255.0,b/255.0,&H,&S,&I);
	 /*printf("H:%1f,S:%1f,I:%1f\n",H,S,I);*/
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
	 I=I*255;
	 i_max=I+50.;
	 i_min=I-50.;
	 if (i_max>255.){
	    i_max=255.;
	 }
	 if (i_min<0.){
	    i_min=0.;
	 }
	 fl_set_slider_value(fd_hsitunergui->Hmax,h_max);
	 fl_set_slider_value(fd_hsitunergui->Hmin,h_min);
	 fl_set_slider_value(fd_hsitunergui->Smax,s_max);
	 fl_set_slider_value(fd_hsitunergui->Smin,s_min);
	 fl_set_slider_value(fd_hsitunergui->Imax,i_max);
	 fl_set_slider_value(fd_hsitunergui->Imin,i_min);
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
      fl_set_slider_value(fd_hsitunergui->Hmax,h_max);
      fl_set_slider_value(fd_hsitunergui->Hmin,h_min);
      fl_set_slider_value(fd_hsitunergui->Smax,s_max);
      fl_set_slider_value(fd_hsitunergui->Smin,s_min);
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
	    fl_set_slider_value(fd_hsitunergui->Smax,s_max);
	    fl_set_slider_value(fd_hsitunergui->Smin,s_min);
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
	    fl_set_slider_value(fd_hsitunergui->Hmax,h_max);
	    fl_set_slider_value(fd_hsitunergui->Hmin,h_min);
	    fl_set_slider_value(fd_hsitunergui->Smax,s_max);
	    fl_set_slider_value(fd_hsitunergui->Smin,s_min);
	 }
      }
   }
   if (event==FL_RELEASE){
      pulsada=0;
   }
   return 0;
}

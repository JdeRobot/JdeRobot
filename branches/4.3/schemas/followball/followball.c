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
 *            Roberto Calvo Palomino <rocapal@gsyc.escet.urjc.es>
 *            Jose Antonio Santos Cadena <santoscadenas@gmail.com>
 *
 */

#include <jde.h>
#include <forms.h>
#include "followballgui.h"
#include "followball.h"
#include <math.h>
#include <colorspaces.h>
#include "graphics_xforms.h"

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

#define FollowballVER  	"Followball - 2.0.0" 

#define D(x...)                  //printf(x)

#define MIN_PIXELES 400          /* Numero minimo de pixeles filtrados */
#define MIN_LINEAS 40            /* Numero minimo de lineas horizontales filtradas */
#define MIN_DISTANCIA 40         /* CTE Minima distancia para mover la PANTILT */

#define CTE_PTU 1.9              /* CTE que nos convierte de distancia a unidades PANTILT */
#define ENCOD_TO_DEG (3.086/60.) /* CTE que nos pasa de unidades PANTILT a grados */
#define DEG_TO_ENCOD (60./3.086) /* CTE que nos pasa de grados a unidades PANTILT */

/*Gui variables*/
Display *mydisplay;
int  *myscreen;

/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;

int last_movement;               /* Indica el ultimo movimiento que ha realizado la pantilt */
struct dfilter data_filter;

int followball_id=0;
int followball_brothers[MAX_SCHEMAS];
arbitration followball_callforarbitration;
int followball_cycle=30;         /* ms */

FD_followballgui *fd_followballgui=NULL;

/* Necesarias para las Xlib */
GC followball_gc;
Window followball_win;           /* image window */
XImage *imagenOrig;
XImage *hsifiltrada;
XImage *histograma;

/* Defines if the visualization of histograma is activated */
int isActivatedShowHistogram=0;

/* Defines if the visualization of ImageHSI is activated */
int isActivatedShowImageHSI=0;


#define PI 3.141592654
/* this memory must exist even with the followballgui turned on */
char imagenOrig_buf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];
char hsifiltrada_buf[SIFNTSC_COLUMNS*SIFNTSC_ROWS*4];
#define SMAX SIFNTSC_COLUMNS     /*320*/
char disco_buf[SMAX*SMAX*4];
char histograma_buf[SMAX*SMAX*4];
int masc[SMAX*SMAX];

float *mypan_angle=NULL, *mytilt_angle=NULL;  /* degs */
float *mylongitude=NULL; /* degs, pan angle */
float *mylatitude=NULL; /* degs, tilt angle */
float *mylongitude_speed=NULL;
float *mylatitude_speed=NULL;
float *max_pan=NULL;
float *min_pan=NULL;
float *max_tilt=NULL;
float *min_tilt=NULL;

resumeFn ptmotorsresume, ptencodersresume;
suspendFn ptmotorssuspend, ptencoderssuspend;

char** mycolorA;
resumeFn colorAresume;
suspendFn colorAsuspend;

/* Para los botones */
#define PUSHED 1
#define RELEASED 0

/* Variables que guardan las coordenadas del quesito */
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
  {                              /* Puntos fuera del circulo negros */
    *r = 0;
    *b = 0;
    *g = 0;
  }
}


void draw_hsimap(char *buffer, int size)
{
  int i,j,ind;
  float x, y, H, S, scale;
  double r,g,b;
  unsigned char R,G,B;

  for(j=0; j < size; j++)
  {
    for(i=0; i < size; i++)
    {
      x = (2.0*i)/size - 1.0;
      y = 1.0 - (2.0*j)/size;
      if ((x >= -1e-7) && (x <= 1e-7))
      {
        if (y > 0)
        {
          H = PI/2.0;
        }
        else
        {
          H = 3.0*PI/2.0;
        }
      }                          /*x != 0 */
      else
      {
        if (x < 0)
        {
          H = atan(y/x) + PI;
        }
        else
        {
          H = atan (y/x);
        }
      }
      H=-H;
      if (H>2*PI)
      {
        H-=2*PI;
      }
      if (H<0)
      {
        H+=2*PI;
      }

      S = sqrt(y*y + x*x);
      ind = (size*j + i)*4;

      hsi2rgb (H,S,0,&r,&g,&b);
      scale = 255.0;
      R = (unsigned char) (scale * r);
      G = (unsigned char) (scale * g);
      B = (unsigned char) (scale * b);
      buffer[ind]   = B;         /* Blue */
      buffer[ind+1] = G;         /* Green */
      buffer[ind+2] = R;         /* Red */
    }
  }
}


/*Funciones para dibujar los quesitos*/
int drawcircle(char *img, int xcentro, int ycentro, int radio, FL_COLOR thiscolor)
{

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

  for (i=0.;i<=360;i=i+0.1)
  {
    x1=cos(i*DEGTORAD)*radio+xcentro;
    y1=sin(i*DEGTORAD)*radio+ycentro;
    fflush (NULL);
    img[(y1*SIFNTSC_COLUMNS+x1)*4]=b;
    img[(y1*SIFNTSC_COLUMNS+x1)*4+1]=g;
    img[(y1*SIFNTSC_COLUMNS+x1)*4+2]=r;
  }
  return 0;
}


int lineinimage(char *img, int xa, int ya, int xb, int yb, FL_COLOR thiscolor)
{
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
        {                        /* draw that point in the image */
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
  if ((x1==x2)&&(y1==y2))
  {
    drawcircle(img, xcentro, ycentro, radio, thiscolor);
  }
  else
  {
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

    if (x1==0)
    {
      if (y1<0)
      {
        i=3*3.1416/2.;
      }
      else
      {
        i=3.1416/2.;
      }
    }
    else
    {
      if (y1==0)
      {
        if (x1<0)
        {
          i=3.1416;
        }
        else
        {
          i=0.;
        }
      }
      else
      {
        if (x1>0)
        {
          i=atan((float)y1/(float)x1);
        }
        else
        {
          i=atan((float)y1/(float)x1)+3.1416;
        }
      }
    }

    i=i*RADTODEG;

    if (x2==0)
    {
      if (y2<0)
      {
        imax=3*3.1416/2.;
      }
      else
      {
        imax=3.1416/2.;
      }
    }
    else
    {
      if (y2==0)
      {
        if (x2<0)
        {
          imax=3.1416;
        }
        else
        {
          imax=0.;
        }
      }
      else
      {
        if (x2>0)
        {
          imax=atan((float)y2/(float)x2);
        }
        else
        {
          imax=atan((float)y2/(float)x2)+3.1416;
        }
      }
    }
    imax=imax*RADTODEG;
    if (imax<i)
    {
      imax=imax+360;
    }
    for (;i<=imax;i=i+0.1)
    {
      x=(cos(i*DEGTORAD)*radio+xcentro);
      y=(ycentro-sin(i*DEGTORAD)*radio);

      img[(y*SMAX+x)*4]=b;
      img[(y*SMAX+x)*4+1]=g;
      img[(y*SMAX+x)*4+2]=r;
    }
  }
  return 0;
}


void drawcheese (char *img,int x_centro,int y_centro, double h_max, double h_min, double s_max, double s_min, FL_COLOR thiscolor)
{
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

int followballgui_setupDisplay(void)
/* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/
{
  int vmode;
  XGCValues gc_values;

  gc_values.graphics_exposures = False;
  followball_gc = XCreateGC(mydisplay,followball_win, GCGraphicsExposures, &gc_values);

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


void pantilt_iteration()
{

#define VEL_MAX_PAN 2500.0*ENCOD_TO_DEG
#define VEL_MIN_PAN 350.0*ENCOD_TO_DEG
#define POS_MIN_PAN 40.0
#define POS_MAX_PAN 160.0

#define VEL_MAX_TILT 1000.0*ENCOD_TO_DEG
#define VEL_MIN_TILT 300.0*ENCOD_TO_DEG
#define POS_MIN_TILT 40.0
#define POS_MAX_TILT 120.0

  float angulo_x, angulo_y;

  D ("Pantilt Iteration, pixeles=%d(%d) - distancia=%d(%d)\n",data_filter.pixeles,MIN_PIXELES,data_filter.distancia,MIN_DISTANCIA);
  if (data_filter.pixeles > MIN_PIXELES)
  {

                                 /* banda muerta */
    if (data_filter.distancia > MIN_DISTANCIA)
    {

      /* Calculamos la distancia que se tiene que mover la PAN-TILT */

                                 //  CTE_PTU);
      angulo_x = ENCOD_TO_DEG * ((int) abs(data_filter.x) );
                                 //  CTE_PTU);
      angulo_y = ENCOD_TO_DEG * ((int) abs(data_filter.y) );

      *mylongitude_speed = VEL_MAX_PAN-((POS_MAX_PAN-(abs(data_filter.x)))/((POS_MAX_PAN-POS_MIN_PAN)/(VEL_MAX_PAN-VEL_MIN_PAN)));
      *mylatitude_speed = VEL_MAX_TILT-((POS_MAX_TILT-(abs(data_filter.y)))/((POS_MAX_TILT-POS_MIN_TILT)/(VEL_MAX_TILT-VEL_MIN_TILT)));

      //*mylongitude_speed = 1000*ENCOD_TO_DEG;
      //*mylatitude_speed = 300*ENCOD_TO_DEG;

      switch (data_filter.cuadrante)
      {

        /* 

          -----------------------
          |          |          |
          |          |          |
          |    4     |    3     |
          |          |          |
          |--------- |--------- |
          |          |          |
          |          |          |
          |    2     |     1    |
          |          |          |
          -----------------------

        */
        case 1:                  /* Si esta en el 1 -> Mover hacia abajo y derecha */
          D("OBJETIVO: Primer Cuadrante\n");
          *mylongitude = *min_pan;
          *mylatitude =  *min_tilt;
          last_movement=right;
          break;

        case 2:                  /* Si esta en el 2 -> Mover hacia abajo e izquierda */
          D("OBJETIVO: Segundo Cuadrante\n");
          *mylongitude = *max_pan;
          *mylatitude = *min_tilt;
          last_movement=left;
          break;
        case 3:                  /* Si esta en el 3 -> Mover hacia arriba e derecha */
          D("OBJETIVO: Tercer Cuadrante\n");
          *mylongitude = *min_pan;
          *mylatitude = *max_tilt;
          last_movement=right;
          break;

        case 4:                  /*  Si esta en el 4 -> Mover hacia arriba e izquierda */
          D("OBJETIVO: Cuarto Cuadrante\n");
          *mylongitude = *max_pan;
          *mylatitude = *max_tilt;
          last_movement=left;
          break;

        default:
          D("Pantilt Iteration: Cuadrante erroneo\n");
      }

      D("Pantilt Iteration: longitude=%f - latitude=%f\n",*mylongitude,*mylatitude);
    }
    else
    {
      /* Paramos los ejes ya que estamos en zona de la banda muerta */
      D("Pantilt Iteration: Estamos en zona muerta ... parar ejes\n");
      *mylongitude_speed = 0.0;
      *mylatitude_speed = 0.0;
    }
  }
}


void busqueda_iteration()
{
  
  *mylongitude_speed=1200*ENCOD_TO_DEG;
  *mylatitude_speed = 0.0;

  /* Buscamos hacia la derecha */
  if (last_movement==right)
  {
    D("[DERECHA]Busqueda Iteration: pan_angle=%f (MAX=%f) , tilt_angle=%f\n",*mypan_angle, MAX_PAN_ANGLE ,*mytilt_angle);
    if ( *mypan_angle > *min_pan )
    {
      *mylongitude = *min_pan;
    }
    else
    {
      last_movement=left;
    }

  }
  /* Buscamos hacia la izquierda */
  else
  {
    D("[IZQUIERDA]Busqueda Iteration: pan_angle=%f (MAX=%f) , tilt_angle=%f\n",*mypan_angle, MAX_PAN_ANGLE ,*mytilt_angle);
    if ( *mypan_angle < *max_pan )
    {
      *mylongitude = *max_pan;
    }
    else
    {
      last_movement=right;
    }
  }

}


int sqr (int num)
{
  return num*num;
}


void followball_iteration()
{
  int i;
  double r,g,b,I,H,S;
  unsigned int X, Y;
  double x,y;
  struct HSV* myHSV;

  int pixeles=0, pasa_filtro=0, num_lineas=0;
  int pixel_x=1, pixel_y=1;

  data_filter.x=0;
  data_filter.y=0;
  data_filter.distancia=0;

  all[followball_id].k++;

  /*  printf("followball iteration %d\n",d++);*/

  /*
  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
  {                              // Blue Byte 
    imagenOrig_buf[i*4]=mycolorA[i*3];
                                 // Green Byte 
    imagenOrig_buf[i*4+1]=mycolorA[i*3+1];
                                 // Red Byte 
    imagenOrig_buf[i*4+2]=mycolorA[i*3+2];
    imagenOrig_buf[i*4+3]=0;     // dummy byte 
  }
  */
  
  for(i=0; i<SMAX*SMAX; i++)
    masc[i]=0;

  for(i=0;i< SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
  {

    r = (float)(unsigned int)(unsigned char)(*mycolorA)[i*3+2]; //imagenOrig_buf[i*4+2];
    g = (float)(unsigned int)(unsigned char)(*mycolorA)[i*3+1]; //imagenOrig_buf[i*4+1];
    b = (float)(unsigned int)(unsigned char)(*mycolorA)[i*3];   //imagenOrig_buf[i*4];

    myHSV = (struct HSV*) RGB2HSV_getHSV( (int)r,(int)g,(int)b);
    H=myHSV->H;
    S=myHSV->S;
    I=myHSV->V;

    if((I<=i_max)&&(I>=i_min)&&
      (S >= s_min) && (S <= s_max) &&
      (H >= h_min) && (H <= h_max) )
    {
      /* pasa el filtro */
      if (isActivatedShowImageHSI)
      {
        hsifiltrada_buf[i*4+0]=(*mycolorA)[i*3]; //imagenOrig_buf[i*4+0];
        hsifiltrada_buf[i*4+1]=(*mycolorA)[i*3+1]; //imagenOrig_buf[i*4+1];
        hsifiltrada_buf[i*4+2]=(*mycolorA)[i*3+2]; //imagenOrig_buf[i*4+2];
        hsifiltrada_buf[i*4+3]=0;
      }

      pixeles++;
      data_filter.x += pixel_x;
      data_filter.y += pixel_y;
      pasa_filtro=1;

    }
    else
    {
      if (isActivatedShowImageHSI)
      {
        /*No pasa el filtro*/
        if (toblack)
        {
          /*Pasar a negro*/
          hsifiltrada_buf[i*4+0] = (unsigned char) 0;
          hsifiltrada_buf[i*4+1] = (unsigned char) 0;
          hsifiltrada_buf[i*4+2] = (unsigned char) 0;
          hsifiltrada_buf[i*4+3] = 0;
        }
        else
        {
          /* En vez de a negro lo pasamos a BW, oscurecido para que se vea mejor (180/255 sobre 1) */
          hsifiltrada_buf[i*4+0] = (unsigned char) (180.0*I);
          hsifiltrada_buf[i*4+1] = (unsigned char) (180.0*I);
          hsifiltrada_buf[i*4+2] = (unsigned char) (180.0*I);
          hsifiltrada_buf[i*4+3] = 0;
        }
      }
    }

    /* Hacemos una conversion para simular que estamos sobre un array dimensional y saber en que pixel (x,y) estamos */
    if (pixel_x==320)
    {
      pixel_x=1;
      pixel_y++;
      if (pasa_filtro==1)
        num_lineas++;
      pasa_filtro=0;

    } else
    pixel_x++;

    if (isActivatedShowHistogram)
    {
      /*Apunto los valores en la mscara HSI (histograma negativo) */
      x = S*cos(-H-(3.1416/2.)); /* El rojo (H=0)est a la derecha */
      y = S*sin(-H-(3.1416/2.));

      X = (x + 1.0)*SMAX/2.0;
      Y = (1.0 - y)*SMAX/2.0;

      masc[X*SMAX + Y]++;
    }

  }                              // FOR

  /* Dibujamos el histograma negativo */

  if (isActivatedShowHistogram)
  {
    memcpy(histograma_buf, disco_buf, SMAX*SMAX*4);
    for (i=0;i<=SMAX*SMAX; i++)
    {
      if (masc[i] >= hsimap_threshold)
      {
        histograma_buf[i*4+0] = (char)255;
        histograma_buf[i*4+1] = (char)255;
        histograma_buf[i*4+2] = (char)255;
      }
    }
  }

  data_filter.pixeles = pixeles;
  data_filter.lineas = num_lineas;
  if (data_filter.pixeles>MIN_PIXELES)
  {

    /* hallamos la media de los puntos */
    data_filter.x = data_filter.x / data_filter.pixeles;
    data_filter.y = data_filter.y / data_filter.pixeles;

    /* hallamos las coordenadas reales para el centro (160,120)   */
    data_filter.x = data_filter.x - 160;
    data_filter.y = data_filter.y - 120;

    /* hallamos la distancia entre el centro y el punto medio */
    data_filter.distancia = sqrt((sqr(data_filter.x)+sqr(data_filter.y)));

    /* miramos en que cuadrante se encuentra */
    if (data_filter.x > 0)
      if (data_filter.y > 0)
        data_filter.cuadrante=1;
    else
      data_filter.cuadrante=3;
    else
    if (data_filter.y > 0)
      data_filter.cuadrante=2;
    else
      data_filter.cuadrante=4;

    if ((data_filter.x==0) && (data_filter.y==0))
      data_filter.cuadrante=0;

    pantilt_iteration();

  } else
  {
    busqueda_iteration();
  }

}


void followball_suspend()
{
  pthread_mutex_lock(&(all[followball_id].mymutex));
  put_state(followball_id,slept);

  *mylongitude_speed = 0.0;
  *mylatitude_speed = 0.0;
  
  //ptmotorssuspend();
  //ptencoderssuspend();
  //colorAsuspend();
	 

  RGB2HSV_destroyTable();

  printf("followball: off\n");
  pthread_mutex_unlock(&(all[followball_id].mymutex));
}


void followball_resume(int father, int *brothers, arbitration fn)
{
  int i;


  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN)
  {
    pthread_mutex_lock(&(all[father].mymutex));
    all[father].children[followball_id]=TRUE;
    pthread_mutex_unlock(&(all[father].mymutex));
  }

  pthread_mutex_lock(&(all[followball_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[followball_id].children[i]=FALSE;
  all[followball_id].father=father;
  if (brothers!=NULL)
  {
    for(i=0;i<MAX_SCHEMAS;i++) followball_brothers[i]=-1;
    i=0;
    while(brothers[i]!=-1) {followball_brothers[i]=brothers[i];i++;}
  }

  /* importamos motors de pantilt */
  
  mylongitude=myimport("ptmotors", "longitude");
  mylatitude=myimport ("ptmotors", "latitude");
  mylongitude_speed=myimport("ptmotors", "longitude_speed");
  mylatitude_speed=myimport("ptmotors","latitude_speed");
//  ptmotorsresume=myimport("ptmotors","resume");
//  ptmotorssuspend=myimport("ptmotors","suspend");
  
  //if (ptmotorsresume!=NULL)
  //   ptmotorsresume(followball_id, NULL, NULL);
  
  max_pan=myimport("ptmotors", "max_longitude");
  max_tilt=myimport("ptmotors", "max_latitude");
  min_pan=myimport("ptmotors", "min_longitude");
  min_tilt=myimport("ptmotors", "min_latitude");

  /* importamos encoders de pantilt */

  mypan_angle=myimport("ptencoders", "pan_angle");
  mytilt_angle=myimport("ptencoders", "tilt_angle");
  
  //ptencodersresume=myimport("ptencoders", "resume");
  //ptencoderssuspend=myimport("ptencoders", "suspend");
  //if (ptencodersresume!=NULL)
  //    ptencodersresume(followball_id, NULL, NULL);
 


  
  /* Importamos colorA */
  mycolorA=myimport ("colorA", "colorA");
  colorAresume=myimport("colorA", "resume");
  colorAsuspend=myimport("colorA", "suspend");
  
  if (colorAresume!=NULL)
	colorAresume(followball_id, NULL, NULL);
           
  
  followball_callforarbitration=fn;
  put_state(followball_id,notready);
  printf("followball: on\n");
    

  RGB2HSV_init();
  RGB2HSV_createTable();

  pthread_cond_signal(&(all[followball_id].condition));
  pthread_mutex_unlock(&(all[followball_id].mymutex));
}


void *followball_thread(void *not_used)
{
  struct timeval a,b;
  long diff, next;

  for(;;)
  {

    pthread_mutex_lock(&(all[followball_id].mymutex));

    if (all[followball_id].state==slept)
    {

      pthread_cond_wait(&(all[followball_id].condition),&(all[followball_id].mymutex));
      pthread_mutex_unlock(&(all[followball_id].mymutex));
    }
    else
    {
      /* check preconditions. For now, preconditions are always satisfied*/
      if (all[followball_id].state==notready) put_state(followball_id,ready);
      else all[followball_id].state=ready;
      /* check brothers and arbitrate. For now this is the only winner */
      if (all[followball_id].state==ready) put_state(followball_id,winner);

      if (all[followball_id].state==winner)
        /* I'm the winner and must execute my iteration */
      {
        pthread_mutex_unlock(&(all[followball_id].mymutex));
        gettimeofday(&a,NULL);
        followball_iteration();
        gettimeofday(&b,NULL);

        diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
        next = followball_cycle*1000-diff-10000;
        /* discounts 10ms taken by calling usleep itself */
        if (next>0) usleep(followball_cycle*1000-diff);
        else
        {
          printf("time interval violated: followball\n");
          usleep(followball_cycle*1000);
        }
      }
      else
        /* just let this iteration go away. overhead time negligible */
      {
        pthread_mutex_unlock(&(all[followball_id].mymutex));
        usleep(followball_cycle*1000);
      }
    }
  }
}

void followball_init(){
   if ((mydisplay= (Display *)myimport("graphics_xforms", "display"))==NULL){
      fprintf (stderr, "oplfow: I can't fetch display from graphics_xforms\n");
      jdeshutdown(1);
   }
   if ((myscreen= (int *)myimport("graphics_xforms", "screen"))==NULL){
      fprintf (stderr, "oplfow: I can't fetch screen from graphics_xforms\n");
      jdeshutdown(1);
   }

   if (myregister_buttonscallback==NULL){
      if ((myregister_buttonscallback=(registerbuttons)myimport ("graphics_xforms", "register_buttonscallback"))==NULL){
         printf ("opflow: I can't fetch register_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_buttonscallback=(deletebuttons)myimport ("graphics_xforms", "delete_buttonscallback"))==NULL){
         printf ("opflow: I can't fetch delete_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_xforms", "register_displaycallback"))==NULL){
         printf ("opflow: I can't fetch register_displaycallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_xforms", "delete_displaycallback"))==NULL){
         jdeshutdown(1);
         printf ("ofplow: I can't fetch delete_displaycallback from graphics_xforms\n");
      }
   }
}

void followball_stop()
{
  if (fd_followballgui!=NULL)
    {
      if (all[followball_id].guistate==on) 
	fl_hide_form(fd_followballgui->followballgui); 
      fl_free_form(fd_followballgui->followballgui);
    }
  printf ("followball close\n");
}

void followball_startup()
{
  printf("followball_startup\n"); 
  draw_hsimap(disco_buf, SMAX);
  i_min=120.0; i_max=255.0;
  h_min=277.5; h_max=252.5;
  s_min=0.25; s_max=0.56;

  hsimap_threshold=20;

  pthread_mutex_lock(&(all[followball_id].mymutex));
  printf("followball schema started up\n");

  put_state(followball_id,slept);

  followball_init();
  
  pthread_create(&(all[followball_id].mythread),NULL,followball_thread,NULL);
  pthread_mutex_unlock(&(all[followball_id].mymutex));
}


void followball_guibuttons(void *obj2)
{
  double aux;
  FL_OBJECT *obj=(FL_OBJECT *)obj2;
  
  if (obj == fd_followballgui-> btActiveHist )
  {
    if ( fl_get_button(fd_followballgui->btActiveHist)==PUSHED )
      isActivatedShowHistogram=0;
    else
      isActivatedShowHistogram=0;
  }
  else if (obj == fd_followballgui-> btActiveHSI )
  {
    if ( fl_get_button(fd_followballgui->btActiveHSI)==PUSHED )
      isActivatedShowImageHSI=1;
    else
      isActivatedShowImageHSI=0;
  }
  else if (obj == fd_followballgui->Hmax)
  {
    h_max=fl_get_slider_value(fd_followballgui->Hmax);
    if(h_max>360.0)
    {
      h_max=h_max-360.0;
    }
    if(h_max<0)
    {
      h_max=h_max+360.0;
    }
    if (h_max==h_min)
    {
      fl_set_slider_value(fd_followballgui->Smin,0.0);
      s_min=0.0;
    }
  }
  else if (obj == fd_followballgui->Hmin)
  {
    h_min=fl_get_slider_value(fd_followballgui->Hmin);
    if(h_min>360.0)
    {
      h_min=h_min-360.0;
    }
    if(h_max<0)
    {
      h_min=h_min+360.0;
    }
    if (h_max==h_min)
    {
      fl_set_slider_value(fd_followballgui->Smin,0.0);
      s_min=0.0;
    }
  }

  else if (obj == fd_followballgui->Smax)
  {
    s_max=fl_get_slider_value(fd_followballgui->Smax);
    if (s_max<=s_min)
    {
      aux=s_min;
      s_min=s_max;
      s_max=aux;
      fl_set_slider_value(fd_followballgui->Smax,s_max);
      fl_set_slider_value(fd_followballgui->Smin,s_min);
    }
  }

  else if (obj == fd_followballgui->Smin)
  {
    s_min=fl_get_slider_value(fd_followballgui->Smin);
    if (s_max<=s_min)
    {
      aux=s_min;
      s_min=s_max;
      s_max=aux;
      fl_set_slider_value(fd_followballgui->Smax,s_max);
      fl_set_slider_value(fd_followballgui->Smin,s_min);
    }
  }

  else if (obj == fd_followballgui->Imax)
  {
    i_max=fl_get_slider_value(fd_followballgui->Imax);
    if (i_max<=i_min)
    {
      aux=i_min;
      i_min=i_max;
      i_max=aux;
      fl_set_slider_value(fd_followballgui->Imax,i_max);
      fl_set_slider_value(fd_followballgui->Imin,i_min);
    }
  }

  else if (obj == fd_followballgui->Imin)
  {
    i_min=fl_get_slider_value(fd_followballgui->Imin);
    if (i_max<=i_min)
    {
      aux=i_min;
      i_min=i_max;
      i_max=aux;
      fl_set_slider_value(fd_followballgui->Imax,i_max);
      fl_set_slider_value(fd_followballgui->Imin,i_min);
    }
  }

  else if (obj ==fd_followballgui->w_slider)
  {
    hsimap_threshold=(int)fl_get_slider_value(fd_followballgui->w_slider);
    /*printf("hsimap_threshold %d\n",hsimap_threshold);*/
  }
  if (obj ==fd_followballgui->toblack)
  {
    if (fl_get_button(fd_followballgui->toblack)==RELEASED)
    {
      toblack=0;
    }
    else
      toblack=1;
  }
}


void followball_guidisplay()
{
  int vmode,i;
  
  vmode= fl_get_vclass();
  
  if ( (vmode==TrueColor)&&(fl_state[vmode].depth==24 || fl_state[vmode].depth==32) )
  {
	  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++)
	  {					 
		  imagenOrig_buf[i*4]= (*mycolorA)[3*i];
		  imagenOrig_buf[i*4+1]= (*mycolorA)[3*i+1];
		  imagenOrig_buf[i*4+2]= (*mycolorA)[3*i+2];
		  imagenOrig_buf[i*4+3]=0; 
	  }
		
  }
  drawcheese(histograma_buf,centro_x,centro_y,h_max,h_min,s_max,s_min,FL_PALEGREEN);

  XPutImage(mydisplay,followball_win,followball_gc,imagenOrig,0,0,fd_followballgui->oculo_orig->x, fd_followballgui->oculo_orig->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);

  XPutImage(mydisplay,followball_win,followball_gc,hsifiltrada,0,0,fd_followballgui->oculo_modif->x, fd_followballgui->oculo_modif->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);

  XPutImage(mydisplay,followball_win,followball_gc,histograma,0,0,fd_followballgui->histograma->x, fd_followballgui->histograma->y, SMAX, SMAX);
}


void followball_guisuspend_aux(void)
{
  mydelete_buttonscallback(followball_guibuttons);
  mydelete_displaycallback(followball_guidisplay);
  fl_hide_form(fd_followballgui->followballgui);
}

void followball_guisuspend(void){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)followball_guisuspend_aux);
      }
   }
   else{
      fn ((gui_function)followball_guisuspend_aux);
   }
}


void followball_guiresume_aux(void)
{
  static int k=0;

  if (k==0)                      /* not initialized */
  {
    k++;
    fd_followballgui = create_form_followballgui();
    fl_set_form_position(fd_followballgui->followballgui,400,50);
    fl_show_form(fd_followballgui->followballgui,FL_PLACE_POSITION,FL_FULLBORDER,FollowballVER);
    followball_win= FL_ObjWin(fd_followballgui->oculo_orig);
    followballgui_setupDisplay();
  }
  else
  {
    fl_show_form(fd_followballgui->followballgui,FL_PLACE_POSITION,FL_FULLBORDER,FollowballVER);
    followball_win= FL_ObjWin(fd_followballgui->oculo_orig);
  }
  

  myregister_buttonscallback(followball_guibuttons);
  myregister_displaycallback(followball_guidisplay);

  /* HSV Values for pink ball */
  i_min=120.0; i_max=255.0;
  h_min=277.5; h_max=252.5;
  s_min=0.25; s_max=0.56;

  /* HSI Values for red ball */
  /*
  i_min=49.0; i_max=150.3;
  h_min=240.0; h_max=280.0;
  s_min=0.43; s_max=0.80;
  */
  fl_set_slider_bounds(fd_followballgui->w_slider,100,1);
  fl_set_slider_value(fd_followballgui->w_slider,hsimap_threshold);
  fl_set_slider_bounds(fd_followballgui->Imin,0,255);
  fl_set_slider_value(fd_followballgui->Imin,i_min);
  fl_set_slider_bounds(fd_followballgui->Imax,0,255);
  fl_set_slider_value(fd_followballgui->Imax,i_max);
  fl_set_slider_bounds(fd_followballgui->Hmin,360,0);
  fl_set_slider_value(fd_followballgui->Hmin,h_min);
  fl_set_slider_bounds(fd_followballgui->Hmax,360,0);
  fl_set_slider_value(fd_followballgui->Hmax,h_max);
  fl_set_slider_bounds(fd_followballgui->Smin,1,0);
  fl_set_slider_value(fd_followballgui->Smin,s_min);
  fl_set_slider_bounds(fd_followballgui->Smax,1,0);
  fl_set_slider_value(fd_followballgui->Smax,s_max);
}

void followball_guiresume(void){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)followball_guiresume_aux);
      }
   }
   else{
      fn ((gui_function)followball_guiresume_aux);
   }
}

int handle2 (FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my, int key, void *xev)
{
  int x_matriz,y_matriz;
  unsigned char r,g,b;
  double H,S,I;
  struct HSV* myHSV;

  //if (!isActivatedShowHistogram)
  // 	return 0;

  if (event==FL_PUSH)
  {
    if ((my>=10) && (my<=250) && (mx>=430) && (mx<=750))
    {
      x_matriz=mx-430;
      y_matriz=my-10;
      r=imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4+2];
      g=imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4+1];
      b=imagenOrig_buf[(SIFNTSC_COLUMNS*y_matriz+x_matriz)*4];

      //rgb2hsi2(r/255.0,g/255.0,b/255.0,&H,&S,&I);

      myHSV = (struct HSV*) RGB2HSV_getHSV((int)r,(int)g,(int)b);
      H=myHSV->H;
      S=myHSV->S;
      I=myHSV->V;

	  //printf("(%d,%d,%d) = (%.1f,%.1f,%.1f) \n",r,g,b,H,S,I);

      h_max=H+20.0;
      h_min=H-20.0;
      if (h_max>360.0)
      {
        h_max=h_max-360.0;
      }
      if (h_min<0)
      {
        h_min=h_min+360.0;
      }

      s_max=S+.1;
      s_min=S-.1;
      if (s_max>1.)
      {
        s_max=1.;
      }
      if (s_min<0)
      {
        s_min=0;
      }

      i_max=I+50.;
      i_min=I-50.;
      if (i_max>255.)
      {
        i_max=255.;
      }
      if (i_min<0.)
      {
        i_min=0.;
      }
      fl_set_slider_value(fd_followballgui->Hmax,h_max);
      fl_set_slider_value(fd_followballgui->Hmin,h_min);
      fl_set_slider_value(fd_followballgui->Smax,s_max);
      fl_set_slider_value(fd_followballgui->Smin,s_min);
      fl_set_slider_value(fd_followballgui->Imax,i_max);
      fl_set_slider_value(fd_followballgui->Imin,i_min);
    }

  }
  return 0;
}


int handle (FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my, int key, void *xev)
{

  if (!isActivatedShowHistogram)
    return 0;

  if(event==FL_DBLCLICK)
  {
    
    x_pulsada=mx-10;y_pulsada=my-10;
    x_pulsada=x_pulsada-centro_x;
    y_pulsada=centro_y-y_pulsada;
    if (x_pulsada==0)
    {
      if (y_pulsada<0)
      {
        h_max=3.1416/2.;
      }
      else
      {
        h_max=3.*3.1416/2.;
      }
    }
    else
    {
      if (y_pulsada==0)
      {
        if (x_pulsada<0)
        {
          h_max=3.1416;
        }
        else
        {
          h_max=0.;
        }
      }
      else
      {
        if (x_pulsada>0)
        {
          h_max=atan((float)(y_pulsada)/(float)x_pulsada);
          if(y_pulsada<0)
          {
            h_max=h_max+2*3.1416;
          }
        }
        else
        {
          h_max=atan((float)y_pulsada/(float)x_pulsada)+3.1416;
        }
      }
    }
    h_max=h_max+20.0;
    h_min=h_max-40.0;
    if (h_max>360.0)
    {
      h_max=h_max-360.0;
    }
    if (h_min<0)
    {
      h_min=h_min+360.0;
    }
    s_max=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
    s_max=s_max+.1;
    s_min=s_max-.2;
    if (s_max>1.)
    {
      s_max=1.;
    }
    if (s_min<0.)
    {
      s_min=0.;
    }
    /*CAMBIO LOS VALORES DE LOS SLIDERS*/
    fl_set_slider_value(fd_followballgui->Hmax,h_max);
    fl_set_slider_value(fd_followballgui->Hmin,h_min);
    fl_set_slider_value(fd_followballgui->Smax,s_max);
    fl_set_slider_value(fd_followballgui->Smin,s_min);
  }
  xquesito1=cos(h_max)*s_max*radio_hsi_map+centro_x;
  yquesito1=centro_y-sin(h_max)*s_max*radio_hsi_map;

  xquesito2=cos(h_min)*s_max*radio_hsi_map+centro_x;
  yquesito2=centro_y-sin(h_min)*s_max*radio_hsi_map;

  xquesito3=cos(h_max)*s_min*radio_hsi_map+centro_x;
  yquesito3=centro_y-sin(h_max)*s_min*radio_hsi_map;

  xquesito4=cos(h_min)*s_min*radio_hsi_map+centro_x;
  yquesito4=centro_y-sin(h_min)*s_min*radio_hsi_map;

  if (event==FL_PUSH)
  {
    /*printf("H_max:%f,H_min:%f,S_max:%f,S_min:%f\n",h_max,h_min,s_max,s_min);*/
    if (((mx-10)<=xquesito1+5) && ((mx-10)>=xquesito1-5))
    {
      if (((my-10)<=yquesito1+5) && ((my-10)>=yquesito1-5))
      {
        x_pulsada=mx-10;y_pulsada=my-10;
        pulsada=1;
      }
    }
    if (((mx-10)<=xquesito2+5) && ((mx-10)>=xquesito2-5))
    {
      if (((my-10)<=yquesito2+5) && ((my-10)>=yquesito2-5))
      {
        x_pulsada=mx-10;y_pulsada=my-10;
        pulsada=2;
      }
    }
    if (((mx-10)<=xquesito3+5) && ((mx-10)>=xquesito3-5))
    {
      if (((my-10)<=yquesito3+5) && ((my-10)>=yquesito3-5))
      {
        x_pulsada=mx-10;y_pulsada=my-10;
        pulsada=3;
      }
    }
    if (((mx-10)<=xquesito4+5) && ((mx-10)>=xquesito4-5))
    {
      if (((my-10)<=yquesito4+5) && ((my-10)>=yquesito4-5))
      {
        x_pulsada=mx-10;y_pulsada=my-10;
        pulsada=4;
      }
    }
    if (h_max==h_min)
    {
      x_pulsada=mx-10;
      y_pulsada=my-10;
      x_pulsada=x_pulsada-centro_x;
      y_pulsada=centro_y-y_pulsada;
      if ((float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map<=1)
      {
        s_max=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
        pulsada=0;
        s_min=0.;
        fl_set_slider_value(fd_followballgui->Smax,s_max);
        fl_set_slider_value(fd_followballgui->Smin,s_min);
      }
    }

  }
  if (event==FL_MOUSE)
  {
    if (pulsada>0)
    {
      xsoltada=mx-10;ysoltada=my-10;
      if (sqrt((xsoltada-centro_x)*(xsoltada-centro_x)+(ysoltada-centro_y)*(ysoltada-centro_y))<=radio_hsi_map)
      {
        xsoltada=xsoltada-centro_x;
        ysoltada=centro_y-ysoltada;
        switch(pulsada)
        {
          case 1:
            s_max=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
            if (xsoltada==0)
            {
              if (ysoltada<0)
              {
                h_max=3*3.1416/2.;
              }
              else
              {
                h_max=3.1416/2.;
              }
            }
            else
            {
              if (ysoltada==0)
              {
                if (xsoltada<0)
                {
                  h_max=3.1416;
                }
                else
                {
                  h_max=0.;
                }
              }
              else
              {
                if (xsoltada>0)
                {
                  h_max=atan((float)ysoltada/(float)xsoltada);
                  if(ysoltada<0)
                  {
                    h_max=h_max+2*3.1416;
                  }
                }
                else
                {
                  h_max=atan((float)ysoltada/(float)xsoltada)+3.1416;
                }
              }
            }
            break;
          case 2:
            s_max=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
            if (xsoltada==0)
            {
              if (ysoltada<0)
              {
                h_min=3*3.1416/2.;
              }
              else
              {
                h_min=3.1416/2.;
              }
            }
            else
            {
              if (ysoltada==0)
              {
                if (xsoltada<0)
                {
                  h_min=3.1416;
                }
                else
                {
                  h_min=0.;
                }
              }
              else
              {
                if (xsoltada>0)
                {
                  h_min=atan((float)ysoltada/(float)xsoltada);
                  if(ysoltada<0)
                  {
                    h_min=h_min+2*3.1416;
                  }
                }
                else
                {
                  h_min=atan((float)ysoltada/(float)xsoltada)+3.1416;
                }
              }
            }
            break;
          case 3:
            s_min=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
            if (xsoltada==0)
            {
              if (ysoltada<0)
              {
                h_max=3*3.1416/2.;
              }
              else
              {
                h_max=3.1416/2.;
              }
            }
            else
            {
              if (ysoltada==0)
              {
                if (xsoltada<0)
                {
                  h_max=3.1416;
                }
                else
                {
                  h_max=0.;
                }
              }
              else
              {
                if (xsoltada>0)
                {
                  h_max=atan((float)ysoltada/(float)xsoltada);
                  if(ysoltada<0)
                  {
                    h_max=h_max+2*3.1416;
                  }
                }
                else
                {
                  h_max=atan((float)ysoltada/(float)xsoltada)+3.1416;
                }
              }
            }

            break;
          case 4:
            s_min=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
            if (xsoltada==0)
            {
              if (ysoltada<0)
              {
                h_min=3*3.1416/2.;
              }
              else
              {
                h_min=3.1416/2.;
              }
            }
            else
            {
              if (ysoltada==0)
              {
                if (xsoltada<0)
                {
                  h_min=3.1416;
                }
                else
                {
                  h_min=0.;
                }
              }
              else
              {
                if (xsoltada>0)
                {
                  h_min=atan((float)ysoltada/(float)xsoltada);
                  if(ysoltada<0)
                  {
                    h_min=h_min+2*3.1416;
                  }
                }
                else
                {
                  h_min=atan((float)ysoltada/(float)xsoltada)+3.1416;
                }
              }
            }
            break;
          default:
            break;
        }
        fl_set_slider_value(fd_followballgui->Hmax,h_max);
        fl_set_slider_value(fd_followballgui->Hmin,h_min);
        fl_set_slider_value(fd_followballgui->Smax,s_max);
        fl_set_slider_value(fd_followballgui->Smin,s_min);
      }
    }
  }
  if (event==FL_RELEASE)
  {
    pulsada=0;
  }
  return 0;
}

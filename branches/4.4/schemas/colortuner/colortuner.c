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
 *  	      Pablo Miangolarra Tejada <p.miangolabra@alumnos.urjc.es>
 */

#include <jde.h>
#include "colortuner.h"
#include <graphics_gtk.h>
#include <colorspaces.h>
#include <interfaces/varcolor.h>

#include <sys/time.h>
#include <time.h>

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <math.h>

#define colortunerVer "colortuner 0.1"
#define PI 3.141592654

enum COLOR {
  		RED, 
		WHEAT, 
		PALEGREEN, 
		BLUE,
		DEEPPINK,
		WHITE,
		BLACK}; 

int colortuner_id=0;
int colortuner_brothers[MAX_SCHEMAS];
arbitration colortuner_callforarbitration;

#define MAX_COLOR 8
/*Imported variables*/
Varcolor *myAA;
Varcolor *myBB;
Varcolor *myCC;
Varcolor *myDD;

int* width[MAX_COLOR];
int* height[MAX_COLOR];
char** mycolor[MAX_COLOR];
runFn myrun[MAX_COLOR];
stopFn mystop[MAX_COLOR];
char *mensajes[MAX_COLOR]={"Mostrando colorA","Mostrando colorB","Mostrando colorC","Mostrando colorD","Mostrando varcolorA","Mostrando varcolorB","Mostrando varcolorC","Mostrando varcolorD"};

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Global variables*/
char *image;
char *image_aux;
/*char *image_double_buff;*/

int image_selected=-1;
pthread_mutex_t main_mutex;

int contexto_pila;
int radio_original	=1;
int radio_rgb 		= 0;
int radio_hsv 		= 0;
int radio_yuv 		= 0;
float 	threshold 	= 20.0;

/*Option Parameters HSV*/
float hmax = 6.0;
float hmin = 0.0;
float smax = 0.9;
float smin = 0.1;
float vmax = 255.0;
float vmin = 0.0;

/*Option Parameters RGB*/
float rmax = 255.0;
float rmin = 0.0;
float gmax = 255.0;
float gmin = 0.0;
float bmax = 255.0;
float bmin = 0.0;

/*Option Parameters YUV*/
float ymax = 1.0;
float ymin = 0.0;
float umax = 0.2;
float umin = -0.2;
float vmax2 = 0.3;
float vmin2 = -0.3;

/*Exported variables */
int colortuner_cycle=100; /* ms */

/*GUI Variables*/
pthread_t hilo_gtk;
GladeXML *xml; /*Fichero xml*/
GtkWidget *win; /*Ventana*/

/*HSV space Variables*/
#define SMAX 320
char 	*image_hsv_space;
int 	hsvspace_loaded=0;
char 	disco_buf[SMAX*SMAX*3];
char 	histograma_buf[SMAX*SMAX*3];
int 	masc[SMAX*SMAX];

/*YUV space Variables*/
char 	*image_yuv_space;
int 	yuvspace_loaded=0;
char 	square_buf[SMAX*SMAX*3];
char 	histograma_yuv_buf[SMAX*SMAX*3];
int 	masc_yuv[SMAX*SMAX];

/*Para los botones*/
#define PUSHED 1
#define RELEASED 0

/*HSV cheese coordenates*/
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

int xquesito1;
int xquesito2;
int xquesito3;
int xquesito4;

int yquesito1;
int yquesito2;
int yquesito3;
int yquesito4;

int pulsada=0;

/*end HSV cheese variables*/

/*YUV square coordenates*/
int xsquare1;
int xsquare2;
int xsquare3;
int xsquare4;

int ysquare1;
int ysquare2;
int ysquare3;
int ysquare4;

/*end YUV square variables*/

void draw_hsvmap(char *img, int size){
   int i,j,ind; 
   float x, y, H, S, scale;
   double r,g,b;
   unsigned char R,G,B;
   int grey = 175;

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
	 
	 if (S<1.)
	   {
	     hsv2rgb(H,S,0.7,&r,&g,&b);
	     scale = 255.0;
	     R = (unsigned char) (scale * r);
	     G = (unsigned char) (scale * g);
	     B = (unsigned char) (scale * b);
	   }
	 else
	   {R=(unsigned char) grey; G= (unsigned char) grey; B= (unsigned char) grey;}
	 ind = (size*j + i)*3;

	 //printf("Valores R: %u G: %u B: %u ind: %d H: %2f S: %2f \n",R,G,B,ind,H,S);

	 img[ind]   = R; /* Blue */
	 img[ind+1] = G; /* Green */
	 img[ind+2] = B; /* Red */
      }
   }
}
/*Draw hsv cheese functions*/
int drawcircle(char *img, int xcentro, int ycentro, int radio, int thiscolor){

   int r,g,b;
   int x1,y1;
   float i;

   /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, they are not valid values for the pixels.  */

   if (thiscolor==BLACK) {r=0;g=0;b=0;}
   else if (thiscolor==RED) {r=255;g=0;b=0;} 
   else if (thiscolor==BLUE) {r=0;g=0;b=255;} 
   else if (thiscolor==PALEGREEN) {r=113;g=198;b=113;} 
   else if (thiscolor==WHEAT) {r=255;g=231;b=155;}
   else if (thiscolor==DEEPPINK) {r=213;g=85;b=178; }   
   else if (thiscolor==WHITE) {r=255;g=255;b=255;}
   else {r=0;g=0;b=0;}
   
   for (i=0.;i<=360;i=i+0.1){
      x1=cos(i*DEGTORAD)*radio+xcentro;
      y1=sin(i*DEGTORAD)*radio+ycentro;
      fflush (NULL);
      img[(y1*SMAX+x1)*3]=b;
      img[(y1*SMAX+x1)*3+1]=g;
      img[(y1*SMAX+x1)*3+2]=r;
   }
   return 0; 
}

int lineinimage(char *img, int xa, int ya, int xb, int yb, int thiscolor){
   float L;
   int i,imax,r,g,b;
   int lastx,lasty,thisx,thisy,lastcount;
   int threshold_line=1;
   int Xmax,Xmin,Ymax,Ymin;

   if (xa > SMAX-1){
	xa = SMAX-1;
   }
   if (xb > SMAX-1){
	xb = SMAX-1;
   }
   if (ya > SMAX-1){
	ya = SMAX-1;
   }
   if (yb > SMAX-1){
	yb = SMAX-1;
   }

   Xmin=0; Xmax=SMAX-1; Ymin=0; Ymax=SMAX-1;
   /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, their are not valid values for the pixels.  */

   if (thiscolor==BLACK) {r=0;g=0;b=0;}
   else if (thiscolor==RED) {r=255;g=0;b=0;} 
   else if (thiscolor==BLUE) {r=0;g=0;b=255;} 
   else if (thiscolor==PALEGREEN) {r=113;g=198;b=113;} 
   else if (thiscolor==WHEAT) {r=255;g=231;b=155;}
   else if (thiscolor==DEEPPINK) {r=213;g=85;b=178;}
   else if (thiscolor==WHITE) {r=255;g=255;b=255;}   
   else {r=0;g=0;b=0;}

   /* first, check both points are inside the limits and draw them */
   if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1) &&
	(xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)) 
   {
      /* draw both points */
		 
      img[(SMAX*ya+xa)*3+0]=b;
      img[(SMAX*ya+xa)*3+1]=g;
      img[(SMAX*ya+xa)*3+2]=r;

      img[(SMAX*yb+xb)*3+0]=b;
      img[(SMAX*yb+xb)*3+1]=g;
      img[(SMAX*yb+xb)*3+2]=r;
		 
      L=(float)sqrt((double)((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya)));
      imax=3*(int)L+1;
      /* if (debug==1) printf("xa=%d ya=%d xb=%d yb=%d L=%.f imax=%d\n",xa,ya,xb,yb,L,imax);  */
      lastx=xa; lasty=xb; lastcount=0;
      for(i=0;i<=imax;i++)
      {
	 thisy=(int)((float)ya+(float)i/(float)imax*(float)(yb-ya));
	 thisx=(int)((float)xa+(float)i/(float)imax*(float)(xb-xa));
	 if ((thisy==lasty)&&(thisx==lastx)) lastcount++;
	 else 
	 { 
	    if (lastcount>=threshold_line)
	    { /* draw that point in the image */
	       img[(SMAX*lasty+lastx)*3+0]=b;
	       img[(SMAX*lasty+lastx)*3+1]=g;
	       img[(SMAX*lasty+lastx)*3+2]=r;
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

int drawarc(char *img, int xcentro, int ycentro, int radio, int x1, int y1, int x2, int y2, int thiscolor)
{
	
   int r,g,b;
   int x,y;
   float i,imax;

   /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image. They can't reach 240 or 320, their are not valid values for the pixels.  */
   if ((x1==x2)&&(y1==y2)){
      drawcircle(img, xcentro, ycentro, radio, thiscolor);
   }else{
      if (thiscolor==BLACK) {r=0;g=0;b=0;}
      else if (thiscolor==RED) {r=255;g=0;b=0;} 
      else if (thiscolor==BLUE) {r=0;g=0;b=255;} 
      else if (thiscolor==PALEGREEN) {r=113;g=198;b=113;} 
      else if (thiscolor==WHEAT) {r=255;g=231;b=155;}
      else if (thiscolor==DEEPPINK) {r=213;g=85;b=178; }   
      else if (thiscolor==WHITE) {r=255;g=255;b=255;}
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

	 img[(y*SMAX+x)*3]=b;
	 img[(y*SMAX+x)*3+1]=g;
	 img[(y*SMAX+x)*3+2]=r;
      }
   }
   return 0; 
}


void drawcheese (char *img,int x_centro,int y_centro, double h_max, double h_min, double s_max, double s_min, int thiscolor){
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

/**********************************
	 End hsv cheese functions
************************************/

void draw_yuvmap(char *img, int size){
   int i,j,ind,ysize; 
   float scale;
   double r,g,b,U,V;
   unsigned char R,G,B;

   ysize = (size * 0.436*2)/(0.615*2);

   for(j=0; j < size; j++){
      for(i=0; i < size; i++){

	 U = 0.436 - (((size -i)*0.436*2.0)/size);
	 V = (((size-j)*0.615*2.0)/size) - 0.615;

	 ind = (size*j + i)*3;

	 yuv2rgb((ymax+ymin)/2,U,V,&r,&g,&b);

	 scale = 255.0;

	 R = (unsigned char) (unsigned int) (float) (scale * r);
	 G = (unsigned char) (unsigned int) (float) (scale * g);
	 B = (unsigned char) (unsigned int) (float) (scale * b);
	 
	 //printf("Valores R: %u G: %u B: %u\n",R,G,B);
	 img[ind]   = R; /* Blue */
	 img[ind+1] = G; /* Green */
	 img[ind+2] = B; /* Red */
      }
   }
}

/*Draw yuv square functions*/

void drawsquare (char *img,double u_max, double u_min, double v_max, double v_min, int thiscolor){
   int x1,y1,x2,y2,ysize;

   u_max =  u_max + 0.436;
   u_min =  u_min + 0.436;
   v_max = 0.615 - v_max;
   v_min = 0.615 - v_min;

   x1=(u_min*SMAX)/(0.436*2);
   y1=(v_max*SMAX)/(0.615*2);
   x2=(u_max*SMAX)/(0.436*2);
   y2=(v_max*SMAX)/(0.615*2);

   lineinimage(img,x1,y1,x2,y2,thiscolor);

   x1=(u_max*SMAX)/(0.436*2);
   y1=(v_max*SMAX)/(0.615*2);
   x2=(u_max*SMAX)/(0.436*2);
   y2=(v_min*SMAX)/(0.615*2);

   lineinimage(img,x1,y1,x2,y2,thiscolor);

   x1=(u_max*SMAX)/(0.436*2);
   y1=(v_min*SMAX)/(0.615*2);
   x2=(u_min*SMAX)/(0.436*2);
   y2=(v_min*SMAX)/(0.615*2);

   lineinimage(img,x1,y1,x2,y2,thiscolor);

   x1=(u_min*SMAX)/(0.436*2);
   y1=(v_min*SMAX)/(0.615*2);
   x2=(u_min*SMAX)/(0.436*2);
   y2=(v_max*SMAX)/(0.615*2);

   lineinimage(img,x1,y1,x2,y2,thiscolor);
}

/**********************************
	 End yuv square functions
************************************/

/*Callbacks*/
gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gdk_threads_leave();
   colortuner_hide();
   gdk_threads_enter();
   return TRUE;
}

void on_img_sel_changed(GtkComboBoxEntry *img_sel, gpointer user_data){
   /*Hay que comprobar el valor que tiene*/
   char *valor;
   valor=(char *)gtk_combo_box_get_active_text((GtkComboBox *)img_sel);
   /*Show box with images*/
   gtk_widget_show(glade_xml_get_widget(xml, "imagesbox"));
   /*Importar los valores oportunos y modificar la selección*/
   printf (valor);
   printf("\n");
   if (strcmp(valor,"colorA")==0){
      if (image_selected!=0){
         width[0]=myimport("colorA","width");
         height[0]=myimport("colorA","height");
         mycolor[0]=myimport("colorA","colorA");
         myrun[0]=(runFn)myimport("colorA","run");
         mystop[0]=(stopFn)myimport("colorA","stop");
         cambiar_imagen(0);
      }
   }
   else if (strcmp(valor,"colorB")==0){
      if (image_selected!=1){
         width[1]=myimport("colorB","width");
         height[1]=myimport("colorB","height");
         mycolor[1]=myimport("colorB","colorB");
         myrun[1]=(runFn)myimport("colorB","run");
         mystop[1]=(stopFn)myimport("colorB","stop");
         cambiar_imagen(1);
      }
   }
   else if (strcmp(valor,"colorC")==0){
      if (image_selected!=2){
         width[2]=myimport("colorC","width");
         height[2]=myimport("colorC","height");
         mycolor[2]=myimport("colorC","colorC");
         myrun[2]=(runFn)myimport("colorC","run");
         mystop[2]=(stopFn)myimport("colorC","stop");
         cambiar_imagen(2);
      }
   }
   else if (strcmp(valor,"colorD")==0){
      if (image_selected!=3){
         width[3]=myimport("colorD","width");
         height[3]=myimport("colorD","height");
         mycolor[3]=myimport("colorD","colorD");
         myrun[3]=(runFn)myimport("colorD","run");
         mystop[3]=(stopFn)myimport("colorD","stop");
         cambiar_imagen(3);
      }
   }
   else if (strcmp(valor,"varcolorA")==0){
      if (image_selected!=4){
	myAA=(Varcolor *)myimport("varcolorA","varcolorA");
	width[4]=&((*myAA).width);
	height[4]=&((*myAA).height);
	mycolor[4]=&((*myAA).img);
	myrun[4]=(runFn)myimport("varcolorA","run");
	mystop[4]=(stopFn)myimport("varcolorA","stop");
	cambiar_imagen(4);
      }
   }
   else if (strcmp(valor,"varcolorB")==0){
      if (image_selected!=5){
	myBB=(Varcolor *)myimport("varcolorB","varcolorB");
	width[5]=&((*myBB).width);
	height[5]=&((*myBB).height);
	mycolor[5]=&((*myBB).img);
	myrun[5]=(runFn)myimport("varcolorB","run");
	mystop[5]=(stopFn)myimport("varcolorB","stop");
	cambiar_imagen(5);
      }
   }
   else if (strcmp(valor,"varcolorC")==0){
      if (image_selected!=6){
	myCC=(Varcolor *)myimport("varcolorC","varcolorC");
	width[6]=&((*myCC).width);
	height[6]=&((*myCC).height);
	mycolor[6]=&((*myCC).img);
	myrun[6]=(runFn)myimport("varcolorC","run");
	mystop[6]=(stopFn)myimport("varcolorC","stop");
	cambiar_imagen(6);
      }
   }
   else if (strcmp(valor,"varcolorD")==0){
      if (image_selected!=7){
	myDD=(Varcolor *)myimport("varcolorD","varcolorD");
	width[7]=&((*myDD).width);
	height[7]=&((*myDD).height);
	mycolor[7]=&((*myDD).img);
	myrun[7]=(runFn)myimport("varcolorD","run");
	mystop[7]=(stopFn)myimport("varcolorD","stop");
	cambiar_imagen(7);
      }
   }
}


void on_active_original_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if(radio_original){
		radio_original=0;
	}else{ 
		radio_original=1;
	}
}
void on_active_rgb_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if(radio_rgb){
		gtk_widget_hide(glade_xml_get_widget(xml, "tableRGB"));
		radio_rgb=0;
	}else{ 
		gtk_widget_show(glade_xml_get_widget(xml, "tableRGB"));
		radio_rgb=1;
	}
}

void  on_active_hsv_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if(radio_hsv){
		gtk_widget_hide(glade_xml_get_widget(xml, "tableHSV"));
		gtk_widget_hide(glade_xml_get_widget(xml, "hsv_image_space"));
		radio_hsv=0;
	}else{ 
		gtk_widget_show(glade_xml_get_widget(xml, "tableHSV"));
		gtk_widget_show(glade_xml_get_widget(xml, "hsv_image_space"));
		radio_hsv=1;
	}
}

void  on_active_yub_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
	if(radio_yuv){
		gtk_widget_hide(glade_xml_get_widget(xml, "tableYUV"));
		gtk_widget_hide(glade_xml_get_widget(xml, "yuv_image_space"));
		radio_yuv=0;
	}else{ 
		gtk_widget_show(glade_xml_get_widget(xml, "tableYUV"));
		gtk_widget_show(glade_xml_get_widget(xml, "yuv_image_space"));
		radio_yuv=1;
	}
}

//***********************************************************************************
//
//			Functions for YUV and HSV maps interaction
//
//***********************************************************************************

void on_hsv_image_space_button_press_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
{

	int coordx = event->x;
	int coordy = event->y;

	if (event->button == 3){
		/*printf("Right mouse button clicked\n");*/
	      x_pulsada=coordx;y_pulsada=coordy;
	      x_pulsada=x_pulsada-centro_x;
	      y_pulsada=centro_y-y_pulsada;

	      if (x_pulsada==0){	
		 if (y_pulsada<0){	
		    hmax=3.1416/2.;
		 }else{
		    hmax=3.*3.1416/2.;
		 }
	      }else{
		 if (y_pulsada==0){
		    if (x_pulsada<0){
		       hmax=3.1416;
		    }else{
		       hmax=0.;
		    }
		 }else{
		    if (x_pulsada>0){
		       hmax=atan((float)(y_pulsada)/(float)x_pulsada);
		       if(y_pulsada<0){
			  hmax=hmax+2*3.1416;
		       }
		    }else{
		       hmax=atan((float)y_pulsada/(float)x_pulsada)+3.1416;
		    }
		 }
	      }
	      hmax=hmax+.2;
	      hmin=hmax-.4;
	      if (hmax>(2.*3.1416)){
		 hmax=hmax-(2.*3.1416);
	      }
	      if (hmin<0){
		 hmin=hmin+(2.*3.1416);
	      }
	      smax=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
	      smax=smax+.1;
	      smin=smax-.2;
	      if (smax>1.){
		 smax=1.;
	      }
	      if (smin<0.){
		 smin=0.;
	      }
	      /*Change sliders values*/

		gtk_range_set_value(glade_xml_get_widget(xml, "hmax"),hmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "hmin"),hmin);
		gtk_range_set_value(glade_xml_get_widget(xml, "smax"),smax);
		gtk_range_set_value(glade_xml_get_widget(xml, "smin"),smin);
	}
	   xquesito1=cos(hmax)*smax*radio_hsi_map+centro_x;
	   yquesito1=centro_y-sin(hmax)*smax*radio_hsi_map;
	
	   xquesito2=cos(hmin)*smax*radio_hsi_map+centro_x;
	   yquesito2=centro_y-sin(hmin)*smax*radio_hsi_map;
	
	   xquesito3=cos(hmax)*smin*radio_hsi_map+centro_x;
	   yquesito3=centro_y-sin(hmax)*smin*radio_hsi_map;
	
	   xquesito4=cos(hmin)*smin*radio_hsi_map+centro_x;
	   yquesito4=centro_y-sin(hmin)*smin*radio_hsi_map;
	     
	if (event->button == 1){
	      if (((coordx)<=xquesito1+5) && ((coordx)>=xquesito1-5)){
		 if (((coordy)<=yquesito1+5) && ((coordy)>=yquesito1-5)){
		    x_pulsada=coordx;y_pulsada=coordy;
		    pulsada=1;
		 }
	      }
	      if (((coordx)<=xquesito2+5) && ((coordx)>=xquesito2-5)){
		 if (((coordy)<=yquesito2+5) && ((coordy)>=yquesito2-5)){
		    x_pulsada=coordx;y_pulsada=coordy;
		    pulsada=2;
		 }
	      }
	      if (((coordx)<=xquesito3+5) && ((coordx)>=xquesito3-5)){
		 if (((coordy)<=yquesito3+5) && ((coordy)>=yquesito3-5)){
		    x_pulsada=coordx;y_pulsada=coordy;
		    pulsada=3;
		 }
	      }
	      if (((coordx)<=xquesito4+5) && ((coordx)>=xquesito4-5)){
		 if (((coordy)<=yquesito4+5) && ((coordy)>=yquesito4-5)){
		    x_pulsada=coordx;y_pulsada=coordy;
		    pulsada=4;
		 }
	      }
	      if (hmax==hmin){			
		 x_pulsada=coordx;
		 y_pulsada=coordy;
		 x_pulsada=x_pulsada-centro_x;
		 y_pulsada=centro_y-y_pulsada;
		 if ((float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map<=1){
		    smax=(float)sqrt((x_pulsada*x_pulsada)+(y_pulsada*y_pulsada))/radio_hsi_map;
		    pulsada=0;
		    smin=0.;
		    gtk_range_set_value(glade_xml_get_widget(xml, "smax"),smax);
		    gtk_range_set_value(glade_xml_get_widget(xml, "smin"),smin);
		 }
	      }
	}
}

void on_hsv_image_space_motion_notify_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
{
	int coordx = event->x;
	int coordy = event->y;

	if (pulsada>0){
		 xsoltada=coordx;ysoltada=coordy;
		 if (sqrt((xsoltada-centro_x)*(xsoltada-centro_x)+(ysoltada-centro_y)*(ysoltada-centro_y))<=radio_hsi_map){
		    xsoltada=xsoltada-centro_x;
		    ysoltada=centro_y-ysoltada;
		    switch(pulsada){
		       case 1:					
			  smax=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
			  if (xsoltada==0){	
			     if (ysoltada<0){
				hmax=3*3.1416/2.;
			     }else{
				hmax=3.1416/2.;
			     }
			  }else{
			     if (ysoltada==0){
				if (xsoltada<0){
				   hmax=3.1416;
				}else{
				   hmax=0.;
				}
			     }else{
				if (xsoltada>0){
				   hmax=atan((float)ysoltada/(float)xsoltada);
				   if(ysoltada<0){
				      hmax=hmax+2*3.1416;
				   }
				}else{
				   hmax=atan((float)ysoltada/(float)xsoltada)+3.1416;
				}
			     }
			  }
			  break;
		       case 2:
			  smax=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
			  if (xsoltada==0){	
			     if (ysoltada<0){
				hmin=3*3.1416/2.;
			     }else{
				hmin=3.1416/2.;
			     }
			  }else{
			     if (ysoltada==0){
				if (xsoltada<0){
				   hmin=3.1416;
				}else{
				   hmin=0.;
				}
			     }else{
				if (xsoltada>0){
				   hmin=atan((float)ysoltada/(float)xsoltada);
				   if(ysoltada<0){
				      hmin=hmin+2*3.1416;
				   }
				}else{
				   hmin=atan((float)ysoltada/(float)xsoltada)+3.1416;
				}
			     }
			  }
			  break;
		       case 3:
			  smin=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;	
			  if (xsoltada==0){	
			     if (ysoltada<0){
				hmax=3*3.1416/2.;
			     }else{
				hmax=3.1416/2.;
			     }
			  }else{
			     if (ysoltada==0){
				if (xsoltada<0){
				   hmax=3.1416;
				}else{
				   hmax=0.;
				}
			     }else{
				if (xsoltada>0){
				   hmax=atan((float)ysoltada/(float)xsoltada);
				   if(ysoltada<0){
				      hmax=hmax+2*3.1416;
				   }
				}else{
				   hmax=atan((float)ysoltada/(float)xsoltada)+3.1416;
				}
			     }
			  }

			  break;
		       case 4:	
			  smin=(float)sqrt((xsoltada*xsoltada)+(ysoltada*ysoltada))/radio_hsi_map;
			  if (xsoltada==0){
			     if (ysoltada<0){
				hmin=3*3.1416/2.;
			     }else{
				hmin=3.1416/2.;
			     }
			  }else{
			     if (ysoltada==0){
				if (xsoltada<0){
				   hmin=3.1416;
				}else{
				   hmin=0.;
				}
			     }else{
				if (xsoltada>0){
				   hmin=atan((float)ysoltada/(float)xsoltada);
				   if(ysoltada<0){
				      hmin=hmin+2*3.1416;
				   }
				}else{
				   hmin=atan((float)ysoltada/(float)xsoltada)+3.1416;
				}
			     }
			  }
			  break;
		       default:
			  break;
		    }
		    	gtk_range_set_value(glade_xml_get_widget(xml, "hmax"),hmax);
			gtk_range_set_value(glade_xml_get_widget(xml, "hmin"),hmin);
			gtk_range_set_value(glade_xml_get_widget(xml, "smax"),smax);
			gtk_range_set_value(glade_xml_get_widget(xml, "smin"),smin);
		 }
	      }
}

void on_hsv_image_space_button_release_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
{
	/*Stop drawing cheese*/
	pulsada=0;
}

void on_yuv_image_space_button_press_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
{
	int coordx = event->x;
	int coordy = event->y ;
	float u1,u2,v1,v2;
	float U,V;


	if (event->button == 3){
		/*printf("Right mouse button clicked\n");*/
	        x_pulsada=coordx;
	        y_pulsada=coordy;

	        x_pulsada = SMAX - x_pulsada;
		y_pulsada = SMAX - y_pulsada; 

	      	U =  0.436 - ((x_pulsada*0.436*2.0)/SMAX);
		V = ((y_pulsada*0.615*2.0)/SMAX) - 0.615;

	        umax=U+0.1;
		umin=U-0.1;
		 if (umax>0.436){
		    umax=0.436;
		 }
		 if (umin<-0.436){
		    umin=-0.436;
		 }

		 vmax2=V+0.1;
		 vmin2=V-0.1;
		 if (vmax2>0.615){
		    vmax2=0.615;
		 }
		 if (vmin2<-0.615){
		    vmin2=-0.615;
		 }
	      	/*Change sliders values*/

		gtk_range_set_value(glade_xml_get_widget(xml, "umax"),umax);
		gtk_range_set_value(glade_xml_get_widget(xml, "umin"),umin);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmax2"),vmax2);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmin2"),vmin2);
	}

	if ((coordx < SMAX)&&(coordy <SMAX)){
		
		u1 =  umax + 0.436;	
		u2 =  umin + 0.436;
		v1 = 0.615 - vmax2;
		v2 = 0.615 - vmin2;
		
		xsquare1=(u2*SMAX)/(0.436*2);
		ysquare1=(v1*SMAX)/(0.615*2);
		xsquare2=(u1*SMAX)/(0.436*2);
		ysquare2=(v1*SMAX)/(0.615*2);
		xsquare3=(u1*SMAX)/(0.436*2);
		ysquare3=(v2*SMAX)/(0.615*2);
		xsquare4=(u2*SMAX)/(0.436*2);
		ysquare4=(v2*SMAX)/(0.615*2);
		 
		if (event->button == 1){    
		      if (((coordx)<=xsquare1+5) && ((coordx)>=xsquare1-5)){
			 if (((coordy)<=ysquare1+5) && ((coordy)>=ysquare1-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=1;
			 }
		      }
		      if (((coordx)<=xsquare2+5) && ((coordx)>=xsquare2-5)){
			 if (((coordy)<=ysquare2+5) && ((coordy)>=ysquare2-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=2;
			 }
		      }
		      if (((coordx)<=xsquare3+5) && ((coordx)>=xsquare3-5)){
			 if (((coordy)<=ysquare3+5) && ((coordy)>=ysquare3-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=3;
			 }
		      }
		      if (((coordx)<=xsquare4+5) && ((coordx)>=xsquare4-5)){
			 if (((coordy)<=ysquare4+5) && ((coordy)>=ysquare4-5)){
			    x_pulsada=coordx;y_pulsada=coordy;
			    pulsada=4;
			 }
		      }
		}
	}
}

void on_yuv_image_space_motion_notify_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
{
	int coordx = event->x;
	int coordy = event->y;


	if ((pulsada>0)&&(coordx < SMAX)&&(coordy <SMAX)){
	
		xsoltada=coordx;
		ysoltada=coordy;

		xsoltada = SMAX - xsoltada;
		ysoltada = SMAX - ysoltada; 
		switch(pulsada){
		       case 1:		
			  umin=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmax2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       case 2:
			  umax=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmax2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       case 3:
			  umax=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmin2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       case 4:
			  umin=  0.436 - ((xsoltada*0.436*2.0)/SMAX);
			  vmin2 = ((ysoltada*0.615*2.0)/SMAX) - 0.615;
			  break;
		       default:
			  break;
		    }
		    	gtk_range_set_value(glade_xml_get_widget(xml, "umax"),umax);
			gtk_range_set_value(glade_xml_get_widget(xml, "umin"),umin);
			gtk_range_set_value(glade_xml_get_widget(xml, "vmax2"),vmax2);
			gtk_range_set_value(glade_xml_get_widget(xml, "vmin2"),vmin2);
	      }
}

void on_yuv_image_space_button_release_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
{
	/*Stop drawing square*/
	pulsada=0;
}

void on_eventbox1_button_press_event (GtkWidget *event_box,GdkEventButton *event, gpointer data)
{
	struct HSV* auxHSV;
	struct YUV* auxYUV;
	double r,g,b;
	double H,S,I,Y,U,V,R,G,B;
	int i;

		i = (width[image_selected][0]*event->y) + event->x;	
		r = (float)(unsigned int)(unsigned char) image[i*3];
		g = (float)(unsigned int)(unsigned char) image[i*3+1];
		b = (float)(unsigned int)(unsigned char) image[i*3+2];

	if(radio_hsv) {

		auxHSV = (struct HSV*) RGB2HSV_getHSV((int)r,(int)g,(int)b);

		H = (auxHSV->H*PI/180.0);
		S = auxHSV->S;
		I = auxHSV->V;

		 hmax=H+.2;
		 hmin=H-.2;
		 if (hmax>2*3.1416){
		    hmax=hmax-2*3.1416;
		 }
		 if (hmin<0){
		    hmin=hmin+2*3.1416;
		 }
		 smax=S+.1;
		 smin=S-.1;
		 if (smax>1.){
		    smax=1.;
		 }
		 if (smin<0){
		    smin=0;
		 }
		 vmax=I+50.;
		 vmin=I-50.;
		 if (vmax>255.){
		    vmax=255.;
		 }
		 if (vmin<0.){
		    vmin=0.;
		 }

		gtk_range_set_value(glade_xml_get_widget(xml, "hmax"),hmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "hmin"),hmin);
		gtk_range_set_value(glade_xml_get_widget(xml, "smax"),smax);
		gtk_range_set_value(glade_xml_get_widget(xml, "smin"),smin);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmax"),vmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmin"),vmin);

		if(smin > smax) {
			smax = smin;
			gtk_range_set_value(glade_xml_get_widget(xml, "smax"),smax);
		} 

		if(vmin > vmax) {
			vmax = vmin;
			gtk_range_set_value(glade_xml_get_widget(xml, "vmax"),vmax);
		}
	}

	if(radio_rgb) {
		
		 R = r;
		 G = g;
		 B = b;

		 rmax=R+20.0;
		 rmin=R-20.0;
		 if (rmax>255.0){
		    rmax=255.0;
		 }
		 if (rmin<0.0){
		    rmin=0.0;
		 }
		 gmax=G+20.0;
		 gmin=G-20.0;
		 if (gmax>255.0){
		    gmax=255.0;
		 }
		 if (gmin<0.0){
		    gmin=0.0;
		 }
		 bmax=B+20;
		 bmin=B-20;
		 if (bmax>255.0){
		    bmax=255.0;
		 }
		 if (bmin<0.0){
		    bmin=0.0;
		 }

		gtk_range_set_value(glade_xml_get_widget(xml, "rmax"),rmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "rmin"),rmin);
		gtk_range_set_value(glade_xml_get_widget(xml, "gmax"),gmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "gmin"),gmin);
		gtk_range_set_value(glade_xml_get_widget(xml, "bmax"),bmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "bmin"),bmin);

		if(rmin > rmax) {
			rmax = rmin;
			gtk_range_set_value(glade_xml_get_widget(xml, "rmax"),rmax);
		} 

		if(gmin > gmax) {
			gmax = gmin;
			gtk_range_set_value(glade_xml_get_widget(xml, "gmax"),gmax);
		} 

		if(bmin > bmax) {
			bmax = bmin;
			gtk_range_set_value(glade_xml_get_widget(xml, "bmax"),bmax);
		}
	}

	if(radio_yuv) {

		auxYUV = (struct YUV*) RGB2YUV_getYUV((int)r,(int)g,(int)b);
		
		Y = auxYUV->Y;
		U = auxYUV->U;
		V = auxYUV->V;

		ymax=Y+.2;
		 ymin=Y-.2;
		 if (ymax>1){
		    ymax=1.0;
		 }
		 if (ymin<0){
		    ymin=0.0;
		 }
		 umax=U+0.1;
		 umin=U-0.1;
		 if (umax>0.436){
		    umax=0.436;
		 }
		 if (umin<-0.436){
		    umin=-0.436;
		 }
		 vmax2=V+0.1;
		 vmin2=V-0.1;
		 if (vmax2>0.615){
		    vmax2=0.615;
		 }
		 if (vmin2<-0.615){
		    vmin2=-0.615;
		 }

		gtk_range_set_value(glade_xml_get_widget(xml, "ymax"),ymax);
		gtk_range_set_value(glade_xml_get_widget(xml, "ymin"),ymin);
		gtk_range_set_value(glade_xml_get_widget(xml, "umax"),umax);
		gtk_range_set_value(glade_xml_get_widget(xml, "umin"),umin);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmax2"),vmax2);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmin2"),vmin2);

		if(ymin > ymax) {
			ymax = ymin;
			gtk_range_set_value(glade_xml_get_widget(xml, "ymax"),ymax);
		}

		if(umin > umax) {
			umax = umin;
			gtk_range_set_value(glade_xml_get_widget(xml, "umax"),umax);
		} 

		if(vmin2 > vmax2) {
			vmax2 = vmin2;
			gtk_range_set_value(glade_xml_get_widget(xml, "vmax2"),vmax2);
		}
	}
}

//*************************************************
//	End functions for YUV and HSV maps interaction
//
//*************************************************

void cambiar_imagen(int i){
   if ((width[i]!=NULL) && (height[i]!=NULL) && (mycolor[i]!=NULL) &&
        (myrun[i]!=NULL) && (mystop[i]!=NULL))
   {
      pthread_mutex_lock(&main_mutex);
      if (image_selected!=-1){
         mystop[image_selected]();
         free(image);
	 free(image_aux);
      }

      image_selected=i;
      myrun[image_selected](colortuner_id,NULL,NULL);

      image=(char *)malloc(width[image_selected][0]*height[image_selected][0]*3);
      image_aux=(char *)malloc(width[image_selected][0]*height[image_selected][0]*3);

	//printf("Tamaño de la imagen: %d %d\n",width[image_selected][0],height[image_selected][0]);

      {
         GdkPixbuf *imgBuff;
         	GtkImage *img=(GtkImage *)glade_xml_get_widget(xml, "image");

        	imgBuff = gdk_pixbuf_new_from_data((unsigned char *)image,
                                             GDK_COLORSPACE_RGB,0,8,
                                             width[image_selected][0],height[image_selected][0],
                                             width[image_selected][0]*3,NULL,NULL);

         	gtk_image_clear(img);
         	gtk_image_set_from_pixbuf(img, imgBuff);
         	gtk_widget_queue_draw(GTK_WIDGET(img));

	GdkPixbuf *imgBuff_aux;
	 	GtkImage *img_aux=(GtkImage *)glade_xml_get_widget(xml, "image_aux");
	 	imgBuff_aux = gdk_pixbuf_new_from_data((unsigned char *)image_aux,
                                             GDK_COLORSPACE_RGB,0,8,
                                             width[image_selected][0],height[image_selected][0],
                                             width[image_selected][0]*3,NULL,NULL);
		gtk_image_clear(img_aux);
	 	gtk_image_set_from_pixbuf(img_aux, imgBuff_aux);
		gtk_widget_queue_draw(GTK_WIDGET(img_aux));

		gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
                             contexto_pila,
                             mensajes[i]);
         	gtk_window_resize (GTK_WINDOW(win),1,1);
	
      }
      pthread_mutex_unlock(&main_mutex);
   }
}

void load_hsv_space(void){

	image_hsv_space=(char *)malloc(SMAX*SMAX*3);

	{
		GdkPixbuf *hsvBuff;
		GtkImage *img_hsv_space=(GtkImage *)glade_xml_get_widget(xml, "HSVspace");

		hsvBuff = gdk_pixbuf_new_from_data((unsigned char *)image_hsv_space,
		                                     GDK_COLORSPACE_RGB,0,8,
		                                     SMAX,SMAX,
		                                     SMAX*3,NULL,NULL);

		 gtk_image_clear(img_hsv_space);
		 gtk_image_set_from_pixbuf(img_hsv_space, hsvBuff);
		 gtk_widget_queue_draw(GTK_WIDGET(img_hsv_space));
			
		draw_hsvmap(image_hsv_space, SMAX);
	}

}

void HSV() {

	struct HSV* myHSV;
	double r,g,b,x,y,H,S,V;
	int i;
	unsigned int X,Y;

	if (hsvspace_loaded == 0){
		load_hsv_space();
		hsvspace_loaded = 1;
	}

	for(i=0; i<SMAX*SMAX; i++)
		masc[i]=0;

	for (i=0;i<width[image_selected][0]*height[image_selected][0]; i++){
			
		r = (float)(unsigned int)(unsigned char) image[i*3];
		g = (float)(unsigned int)(unsigned char) image[i*3+1];
		b = (float)(unsigned int)(unsigned char) image[i*3+2];

		myHSV = (struct HSV*) RGB2HSV_getHSV((int)r,(int)g,(int)b);

		H = myHSV->H;
		S = myHSV->S;
		V = myHSV->V;

		H=H*DEGTORAD;

		if(((V<=vmax)&&(V>=vmin)&& (S >= smin) && (S <= smax) && 
	 		(((H >= hmin) && (H <= hmax) 	&& (hmin <= hmax))||
	  		((H >= hmin) && (H <= 2*PI) 	&& (hmin > hmax)) ||
	  		((H <= hmax) && (H >= 0.) 	&& (hmin > hmax))))) {
		/*Pixel get trought: Original color*/
			image_aux[i*3] = image[i*3];
			image_aux[i*3+1] = image[i*3+1];
			image_aux[i*3+2] = image[i*3+2];
		}else{
	   	/*Pixel does not get trought: Grey Scale */
			image_aux[i*3] = (unsigned char) (myHSV->V*100/255);
			image_aux[i*3+1] = (unsigned char) (myHSV->V*100/255);
			image_aux[i*3+2] = (unsigned char) (myHSV->V*100/255);
		}

		/*Save values in HSV mask*/
		x = S*cos(-H-(3.1416/2.)); /* El rojo (H=0)está a la derecha */
		y = S*sin(-H-(3.1416/2.));

		X = (x + 1.0)*SMAX/2.0;
		Y = (1.0 - y)*SMAX/2.0;

		masc[X*SMAX + Y]++;

	}

	memcpy(image_hsv_space, disco_buf, SMAX*SMAX*3);

	for (i=0;i<SMAX*SMAX; i++){
		if (masc[i] >= threshold)
	      	{
	     		image_hsv_space[i*3] = (char)255; 
	      		image_hsv_space[i*3+1] = (char)255; 
	      		image_hsv_space[i*3+2] = (char)255;
	      	}
	}
}

void RGB() {

	double r,g,b;
	int i;

	for (i=0;i<width[image_selected][0]*height[image_selected][0]; i++){
			
		r = (float)(unsigned int)(unsigned char) image[i*3];
		g = (float)(unsigned int)(unsigned char) image[i*3+1];
		b = (float)(unsigned int)(unsigned char) image[i*3+2];

		if((g <= gmax) && (g >= gmin) && (b <= bmax) && (b >= bmin)&&(r<= rmax) && (r >= rmin)) {
		/*Pixel get trought: Original color*/
			image_aux[i*3] = image[i*3];
			image_aux[i*3+1] = image[i*3+1];
			image_aux[i*3+2] = image[i*3+2];
		}  else {
	   	/*Pixel does not get trought: Grey Scale */
			image_aux[i*3] = (unsigned char) (b*100/255);
			image_aux[i*3+1] = (unsigned char) (b*100/255);
			image_aux[i*3+2] = (unsigned char) (b*100/255);
		}
	}
	
}

void load_yuv_space(void){

	int i,yuv_image_rows;
	
	yuv_image_rows = (SMAX * 0.436*2)/(0.615*2);
	image_yuv_space=(char *)malloc(SMAX*SMAX*3);

	{
		GdkPixbuf *yuvBuff;
		 	GtkImage *img_yuv_space=(GtkImage *)glade_xml_get_widget(xml, "YUVspace");

			yuvBuff = gdk_pixbuf_new_from_data((unsigned char *)image_yuv_space,
		                                     GDK_COLORSPACE_RGB,0,8,
		                                     SMAX,SMAX,SMAX*3,NULL,NULL);

		 	gtk_image_clear(img_yuv_space);
		 	gtk_image_set_from_pixbuf(img_yuv_space, yuvBuff);
		 	gtk_widget_queue_draw(GTK_WIDGET(img_yuv_space));
			
			draw_yuvmap(image_yuv_space, SMAX);
	}
}

void YUV() {
	
	struct YUV* myYUV;
	double r,g,b,x,y,Y,U,V;
	int i,valuex,valuey;

	if (yuvspace_loaded == 0){
		load_yuv_space();
		yuvspace_loaded = 1;
	}

	for(i=0; i<SMAX*SMAX; i++)
		masc_yuv[i]=0;
	
	for (i=0;i<width[image_selected][0]*height[image_selected][0]; i++){
			
		r = (float)(unsigned int)(unsigned char) image[i*3];
		g = (float)(unsigned int)(unsigned char) image[i*3+1];
		b = (float)(unsigned int)(unsigned char) image[i*3+2];

			
		myYUV = (struct YUV*) RGB2YUV_getYUV((int)r,(int)g,(int)b);

		Y = myYUV->Y;
		U = myYUV->U;
		V = myYUV->V;

		if(((U <= umax) && (U >= umin) && (V <= vmax2) && (V >= vmin2)&& (Y <= ymax) && (Y >= ymin))) {
		/*Pixel get trought: Original color*/
			image_aux[i*3] = image[i*3];
			image_aux[i*3+1] = image[i*3+1];
			image_aux[i*3+2] = image[i*3+2];
		}  else {
	   	/*Pixel does not get trought: Grey Scale */
			image_aux[i*3] = (unsigned char) ((int)b*100/255);
			image_aux[i*3+1] = (unsigned char) ((int)b*100/255);
			image_aux[i*3+2] = (unsigned char) ((int)b*100/255);
		}
		
		/*Save values in HSV mask*/
		
		x = ((U + 0.436)*SMAX) / (0.436*2);
		y = ((0.615 - V)*SMAX) / (0.615*2);

		valuex = (int)(float)x;
		valuey = (int)(float)y;	

		masc_yuv[valuey*SMAX + valuex]++;
	}
	memcpy(image_yuv_space, square_buf, SMAX*SMAX*3);
	for (i=0;i<SMAX*SMAX; i++){
		if (masc_yuv[i] >= threshold)
	      	{
	     		image_yuv_space[i*3] = (char)255; 
	      		image_yuv_space[i*3+1] = (char)255; 
	      		image_yuv_space[i*3+2] = (char)255;
	      	}
	}
}

void colortuner_iteration(){
   	int i;
   
   speedcounter(colortuner_id);
   pthread_mutex_lock(&main_mutex);

   if (image_selected!=-1){
	for (i=0;i<width[image_selected][0]*height[image_selected][0]; i++){
        	image[i*3]=(*mycolor[image_selected])[i*3+2];
        	image[i*3+1]=(*mycolor[image_selected])[i*3+1];
        	image[i*3+2]=(*mycolor[image_selected])[i*3];

		if(radio_original) {
			image_aux[i*3]=image[i*3];
			image_aux[i*3+1]=image[i*3+1];
			image_aux[i*3+2]=image[i*3+2];
		}
	}

	if(radio_rgb){

		RGB();

	}else if(radio_hsv){

		HSV();

	}else if (radio_yuv){

		YUV();
	}
   }
   pthread_mutex_unlock(&main_mutex);
}


/*Import symbols*/
void colortuner_imports(){
 if (myregister_displaycallback==NULL){
    if ((myregister_displaycallback=
	 (registerdisplay)myimport ("graphics_gtk", "register_displaycallback"))==NULL)
      {
	printf ("I can't fetch register_displaycallback from graphics_gtk\n");
	jdeshutdown(1);
      }
    if ((mydelete_displaycallback=
	 (deletedisplay)myimport ("graphics_gtk", "delete_displaycallback"))
	==NULL)
      {
	printf ("I can't fetch delete_displaycallback from graphics_gtk\n");
	jdeshutdown(1);
      }
  }
}

/*Export symbols*/
void colortuner_exports(){
   myexport("colortuner", "id", &colortuner_id);
   myexport("colortuner","cycle",&colortuner_cycle);
   myexport("colortuner","run",(void *)colortuner_run);
   myexport("colortuner","stop",(void *)colortuner_stop);
}


void colortuner_terminate(){
  /*  colortuner_hide();*/
  colortuner_stop();
  pthread_mutex_lock(&main_mutex);
  if (image_selected!=-1){
    mystop[image_selected]();
    free(image);
    free(image_aux);
    image = NULL;
    image_aux = NULL;
  }
  pthread_mutex_unlock(&main_mutex);
  printf ("colortuner terminated\n");
}

void colortuner_stop()
{
  pthread_mutex_lock(&(all[colortuner_id].mymutex));
  put_state(colortuner_id,slept);
  printf("colortuner: off\n");
  pthread_mutex_unlock(&(all[colortuner_id].mymutex));
}


void colortuner_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[colortuner_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[colortuner_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[colortuner_id].children[i]=FALSE;
  all[colortuner_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) colortuner_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {colortuner_brothers[i]=brothers[i];i++;}
    }
  colortuner_callforarbitration=fn;
  put_state(colortuner_id,notready);
  printf("colortuner: on\n");
  colortuner_imports();

  pthread_cond_signal(&(all[colortuner_id].condition));
  pthread_mutex_unlock(&(all[colortuner_id].mymutex));
}

void *colortuner_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;
   static kk=0;

   for(;;)
   {
      pthread_mutex_lock(&(all[colortuner_id].mymutex));
      if (all[colortuner_id].state==slept)
      {
	 pthread_cond_wait(&(all[colortuner_id].condition),&(all[colortuner_id].mymutex));
	 pthread_mutex_unlock(&(all[colortuner_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[colortuner_id].state==notready)
	    put_state(colortuner_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[colortuner_id].state==ready)
	 {put_state(colortuner_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[colortuner_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[colortuner_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    colortuner_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)colortuner_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[colortuner_id].mymutex));
	    usleep(colortuner_cycle*1000);
	 }
      }
   }
}

void colortuner_init()
{

  RGB2HSV_init();
  RGB2HSV_createTable();

  RGB2YUV_init();
  RGB2YUV_createTable();

  /* initial values for color space figures */
  draw_hsvmap(disco_buf, SMAX);
  draw_yuvmap(square_buf, SMAX); 

  pthread_mutex_lock(&(all[colortuner_id].mymutex));
  printf("colortuner schema started up\n");
  colortuner_exports();
  put_state(colortuner_id,slept);
  pthread_create(&(all[colortuner_id].mythread),NULL,colortuner_thread,NULL);
  pthread_mutex_unlock(&(all[colortuner_id].mymutex));

}

void colortuner_guidisplay(){
   pthread_mutex_lock(&main_mutex);
   if (image_selected!=-1){
      GtkImage *img = GTK_IMAGE(glade_xml_get_widget(xml, "image"));
      GtkImage *img_aux = GTK_IMAGE(glade_xml_get_widget(xml, "image_aux"));

      gdk_threads_enter();
      gtk_widget_queue_draw(GTK_WIDGET(img));
      gtk_widget_queue_draw(GTK_WIDGET(img_aux));

      threshold = gtk_range_get_value(glade_xml_get_widget(xml, "threshold"));


		if(radio_hsv) {
			GtkImage *img_hsv_space = GTK_IMAGE(glade_xml_get_widget(xml, "HSVspace"));
			gtk_widget_queue_draw(GTK_WIDGET(img_hsv_space));
			
			if (hsvspace_loaded == 1){
				drawcheese(image_hsv_space,centro_x,centro_y,
						(double)hmax,(double)hmin,
						(double)smax,(double)smin,
						BLACK);
			}
			hmax = gtk_range_get_value(glade_xml_get_widget(xml, "hmax"));
			hmin = gtk_range_get_value(glade_xml_get_widget(xml, "hmin"));
			smax = gtk_range_get_value(glade_xml_get_widget(xml, "smax"));
			smin = gtk_range_get_value(glade_xml_get_widget(xml, "smin"));
			vmax = gtk_range_get_value(glade_xml_get_widget(xml, "vmax"));
			vmin = gtk_range_get_value(glade_xml_get_widget(xml, "vmin"));


			if(smin > smax) {
				smax = smin;
				gtk_range_set_value(glade_xml_get_widget(xml, "smax"),smax);
			} 

			if(vmin > vmax) {
				vmax = vmin;
				gtk_range_set_value(glade_xml_get_widget(xml, "vmax"),vmax);
			}
		}

		if(radio_rgb) {
			rmax = gtk_range_get_value(glade_xml_get_widget(xml, "rmax"));
			rmin = gtk_range_get_value(glade_xml_get_widget(xml, "rmin"));
			gmax = gtk_range_get_value(glade_xml_get_widget(xml, "gmax"));
			gmin = gtk_range_get_value(glade_xml_get_widget(xml, "gmin"));
			bmax = gtk_range_get_value(glade_xml_get_widget(xml, "bmax"));
			bmin = gtk_range_get_value(glade_xml_get_widget(xml, "bmin"));

			if(rmin > rmax) {
				rmax = rmin;
				gtk_range_set_value(glade_xml_get_widget(xml, "rmax"),rmax);
			} 

			if(gmin > gmax) {
				gmax = gmin;
				gtk_range_set_value(glade_xml_get_widget(xml, "gmax"),gmax);
			} 

			if(bmin > bmax) {
				bmax = bmin;
				gtk_range_set_value(glade_xml_get_widget(xml, "bmax"),bmax);
			}
		}
		if(radio_yuv) {
			GtkImage *img_yuv_space = GTK_IMAGE(glade_xml_get_widget(xml, "YUVspace"));
			gtk_widget_queue_draw(GTK_WIDGET(img_yuv_space));

			if (yuvspace_loaded == 1){
				drawsquare (image_yuv_space,
						(double)umax,(double)umin,
						(double)vmax2,(double)vmin2,
						BLACK);
				/*Depends of Y' value*/
				draw_yuvmap(square_buf, SMAX); 
			}

			ymax = gtk_range_get_value(glade_xml_get_widget(xml, "ymax"));
			ymin = gtk_range_get_value(glade_xml_get_widget(xml, "ymin"));
			umax = gtk_range_get_value(glade_xml_get_widget(xml, "umax"));
			umin = gtk_range_get_value(glade_xml_get_widget(xml, "umin"));
			vmax2 = gtk_range_get_value(glade_xml_get_widget(xml, "vmax2"));
			vmin2 = gtk_range_get_value(glade_xml_get_widget(xml, "vmin2"));

			if(ymin > ymax) {
				ymax = ymin;
				gtk_range_set_value(glade_xml_get_widget(xml, "ymax"),ymax);
			} 

			if(umin > umax) {
				umax = umin;
				gtk_range_set_value(glade_xml_get_widget(xml, "umax"),umax);
			} 

			if(vmin2 > vmax2) {
				vmax2 = vmin2;
				gtk_range_set_value(glade_xml_get_widget(xml, "vmax2"),vmax2);
			}
		}

      gtk_window_resize (GTK_WINDOW(win),1,1);
      gdk_threads_leave();
   }
   pthread_mutex_unlock(&main_mutex);
}


void colortuner_hide(void){
   mydelete_displaycallback(colortuner_guidisplay);
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   all[colortuner_id].guistate=pending_off;
}

void colortuner_show(void){
   static int cargado=0;
   static pthread_mutex_t colortuner_gui_mutex;

   pthread_mutex_lock(&colortuner_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&colortuner_gui_mutex);

      /*Load window from xml file .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("colortuner.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window");

      /*Connect callbacks*/
      {
         GtkComboBoxEntry *sel_ent;

         sel_ent=(GtkComboBoxEntry *)glade_xml_get_widget(xml, "img_sel");
         g_signal_connect(G_OBJECT(sel_ent), "changed",
                          G_CALLBACK(on_img_sel_changed), NULL);
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
	 g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "eventbox1")), "button_press_event",
                          G_CALLBACK(on_eventbox1_button_press_event), image);

	 /*Callbacks for interfaces color spaces*/
	 g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "yuv_image_space")), "button_press_event",
                          G_CALLBACK(on_yuv_image_space_button_press_event), image_yuv_space);
	 g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "hsv_image_space")), "button_press_event",
                          G_CALLBACK(on_hsv_image_space_button_press_event), image_hsv_space);
	 g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "yuv_image_space")), "button_release_event",
                          G_CALLBACK(on_yuv_image_space_button_release_event), image_yuv_space);
	 g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "hsv_image_space")), "button_release_event",
                          G_CALLBACK(on_hsv_image_space_button_release_event), image_hsv_space);
	 g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "yuv_image_space")), "motion_notify_event",
                          G_CALLBACK(on_yuv_image_space_motion_notify_event), image_yuv_space);
	 g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "hsv_image_space")), "motion_notify_event",
                          G_CALLBACK(on_hsv_image_space_motion_notify_event), image_hsv_space);

         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "radio_original")),
                          "toggled", G_CALLBACK(on_active_original_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "radio_rgb")),
                          "toggled", G_CALLBACK(on_active_rgb_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "radio_hsv")),
                          "toggled", G_CALLBACK(on_active_hsv_toggled), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "radio_yuv")),
                          "toggled", G_CALLBACK(on_active_yub_toggled), NULL);

     	
      }
		/*Set values for HSV sliders*/
		gtk_range_set_value(glade_xml_get_widget(xml, "hmax"),hmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "hmin"),hmin);
		gtk_range_set_value(glade_xml_get_widget(xml, "smax"),smax);
		gtk_range_set_value(glade_xml_get_widget(xml, "smin"),smin);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmax"),vmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmin"),vmin);

		/*Set values for RGB sliders*/
		gtk_range_set_value(glade_xml_get_widget(xml, "rmax"),rmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "rmin"),rmin);
		gtk_range_set_value(glade_xml_get_widget(xml, "gmax"),gmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "gmin"),gmin);
		gtk_range_set_value(glade_xml_get_widget(xml, "bmax"),bmax);
		gtk_range_set_value(glade_xml_get_widget(xml, "bmin"),bmin);

		/*Set values for YUV sliders*/
		gtk_range_set_value(glade_xml_get_widget(xml, "ymax"),ymax);
		gtk_range_set_value(glade_xml_get_widget(xml, "ymin"),ymin);
		gtk_range_set_value(glade_xml_get_widget(xml, "umax"),umax);
		gtk_range_set_value(glade_xml_get_widget(xml, "umin"),umin);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmax2"),vmax2);
		gtk_range_set_value(glade_xml_get_widget(xml, "vmin2"),vmin2);

		/*Hide frames until they are displayed*/
		gtk_widget_hide(glade_xml_get_widget(xml, "tableRGB"));
		gtk_widget_hide(glade_xml_get_widget(xml, "tableHSV"));
		gtk_widget_hide(glade_xml_get_widget(xml, "tableYUV"));

		/*Hide spaces interfaces until they are displayed*/
		gtk_widget_hide(glade_xml_get_widget(xml, "yuv_image_space"));
		gtk_widget_hide(glade_xml_get_widget(xml, "hsv_image_space"));
		gtk_range_set_value(glade_xml_get_widget(xml, "threshold"),threshold);

		/*Hide images until they are displayed*/
		gtk_widget_hide(glade_xml_get_widget(xml, "imagesbox"));

      if (win==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      else{
         gtk_widget_show(win);
         gtk_widget_queue_draw(GTK_WIDGET(win));
      }
      contexto_pila=gtk_statusbar_get_context_id ((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
            "contexto general");
      gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "barra_estado"),
                         contexto_pila,
                         "No hay fuente de imagen seleccionada");
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&colortuner_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(colortuner_guidisplay);
   all[colortuner_id].guistate=pending_on;
}



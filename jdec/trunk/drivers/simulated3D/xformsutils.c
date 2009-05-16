#include <math.h>
#include <jde.h>
#include <pioneer.h>
#include "xformsutils.h"
#define INVERSE_MODE 1
#define NON_INVERSE_MODE 0

/* xformsutils 0.1 created by Antonio Pineda */
/* hsi table values */
double hsi[256*1024*3];

/* int draws an image on an FL_IMAGE XForms object */
int lineinimage(char *img, int xa, int ya, int xb, int yb, FL_COLOR thiscolor,int columns,int rows)
{
  
  float L;
  int i,imax,r,g,b;
  int lastx,lasty,thisx,thisy,lastcount;
  int threshold=1;
  int Xmax,Xmin,Ymax,Ymin;
  
  Xmin=0; Xmax=columns-1; Ymin=0; Ymax=rows-1;

  /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image.
     They can't reach 240 or 320, their are not valid values for the pixels. */
  
  if (thiscolor==FL_BLACK) {r=0;g=0;b=0;}
  else if (thiscolor==FL_RED) {r=255;g=0;b=0;} 
  else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;} 
  else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;} 
  else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
  else if (thiscolor==FL_DEEPPINK) {r=213;g=85;b=178; }   
  else {r=0;g=0;b=0;}
  
  /* first, check both points are inside the limits and draw them */
  if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1) &&
      (xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)){
    /* draw both points */
    
    img[(columns*ya+xa)*3+0]=b;
    img[(columns*ya+xa)*3+1]=g;
    img[(columns*ya+xa)*3+2]=r;
    img[(columns*yb+xb)*3+0]=b;
    img[(columns*yb+xb)*3+1]=g;
    img[(columns*yb+xb)*3+2]=r;
    
    L=(float)sqrt((double)((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya)));
    imax=3*(int)L+1;
    lastx=xa; lasty=xb; lastcount=0;
    for(i=0;i<=imax;i++){
      thisy=(int)((float)ya+(float)i/(float)imax*(float)(yb-ya));
      thisx=(int)((float)xa+(float)i/(float)imax*(float)(xb-xa));
      if ((thisy==lasty)&&(thisx==lastx)) lastcount++;
      else{ 
	if (lastcount>=threshold){ /* draw that point in the image */
	  img[(columns*lasty+lastx)*3+0]=b;
	  img[(columns*lasty+lastx)*3+1]=g;
	  img[(columns*lasty+lastx)*3+2]=r;
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

/* fills all the buffer positions with an input color */
void reset_buffer(char *buffer, double R, double G, double B,int columns,int rows)
{
  int i;
  for(i=0;i<columns*rows*3;i=i+3){
    buffer[i]=(char)B;
    buffer[i+1]=(char)G;
    buffer[i+2]=(char)R;
  }
}

/* it draws a white rectangular area in an image using the array coordenates given: top left_point bottom left_point...*/
void drawimage_area(char *buffer,int tleft_point, int bleft_point, int tright_point, int bright_point)
{

  int i,currentRow,currentColumn;
  int lower_y=(ARRAY_TO_GRAPHIC_ROW(tleft_point)>ARRAY_TO_GRAPHIC_ROW(bleft_point)?ARRAY_TO_GRAPHIC_ROW(bleft_point):ARRAY_TO_GRAPHIC_ROW(tleft_point));
  int lower_x=(ARRAY_TO_GRAPHIC_COL(tleft_point)>ARRAY_TO_GRAPHIC_COL(tright_point)?ARRAY_TO_GRAPHIC_COL(tright_point):ARRAY_TO_GRAPHIC_COL(tleft_point));
  int greater_y=(ARRAY_TO_GRAPHIC_ROW(tleft_point)>ARRAY_TO_GRAPHIC_ROW(bleft_point)?ARRAY_TO_GRAPHIC_ROW(tleft_point):ARRAY_TO_GRAPHIC_ROW(bleft_point));
  int greater_x=(ARRAY_TO_GRAPHIC_COL(tleft_point)>ARRAY_TO_GRAPHIC_COL(tright_point)?ARRAY_TO_GRAPHIC_COL(tleft_point):ARRAY_TO_GRAPHIC_COL(tright_point));
  
  /* horizontal lines */
  for(i=tleft_point;i<=tright_point;i++){
    buffer[i]=(char) 255;
  }
  for(i=bleft_point;i<=bright_point;i++){
    buffer[i]=(char) 255;
  }
  
  /* vertical lines*/
  for(i=tleft_point;i<=bright_point;i++){
    currentRow=ARRAY_TO_GRAPHIC_ROW(i);
    if((currentRow>=lower_y)&&(currentRow<=greater_y)){
      
      /* inside rows area */
      currentColumn=ARRAY_TO_GRAPHIC_COL(i);
      if((currentColumn==lower_x)||(currentColumn==greater_x)){
	buffer[i]=(char) 255;
      }
    }
  }  
}

/* it builds the hsi table. this function must be call if you want to transform from RGB TO HSI */
void build_hsi_table()
{
  int i,j,k,c;
  double r,g,b,h_hsi,s_hsi,i_hsi, valor_H,min;

  for(i=0;i<64;i++){
    for(j=0;j<64;j++){
      for(k=0;k<64;k++){
	c=((0x3f&i)<<12)+((0x3f&j)<<6)+((0x3f&k));
	r=(double)(i<<2);
	g=(double)(j<<2);
	b=(double)(k<<2);
	
	if (((r-g)*(r-g)+(r-b)*(g-b))<=0){h_hsi = -1;}
	else {h_hsi=acos(((r-g)+(r-b))/(2.*sqrt((r-g)*(r-g)+(r-b)*(g-b))));}
	if (g<b) h_hsi=6.2832-h_hsi;
	
	if ((r+g+b) == 0.){s_hsi=1.;i_hsi=0.;}
	else{
	  min=r;
	  if (g<min) {min=g;}
	  if (b<min) {min=b;}
	  s_hsi= 1.-(3./(r+g+b))*min;
	  i_hsi=(1./3.)*(r+g+b);
	}

	valor_H=h_hsi/DEGTORAD;/*pasamos a grados el resultado de la componente H*/
	hsi[c*3+0]=valor_H;
	hsi[c*3+1]=s_hsi;
	hsi[c*3+2]=i_hsi;
      }
    }
  }
}

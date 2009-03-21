/*
 *
 *  Copyright (C) 1997-2008 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.es>, Antonio Pineda <apineda@gsyc.es>
 *
 */

#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <forms.h>
#include <jde.h>
#include <graphics_xforms.h>
#include <progeo.h>
#include <extrinsicsgui.h>
#include <extrinsics.h>


int extrinsics_cycle=100; /* ms */
int extrinsics_id=0; 
int extrinsics_brothers[MAX_SCHEMAS];
arbitration extrinsics_callforarbitration;

/* array de camaras */
TPinHoleCamera mycameras[2];
int mycam=0; /* camera to be calibrated */
int vcam=1; /* virtual camera to look at the world */
char cameraINfile[256];
char cameraOUTfile[256];

/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;

/* imported images */
char **myColorA;
runFn mycolorArun;
stopFn mycolorAstop;


Display *mydisplay;
int *myscreen;
FD_extrinsicsgui *fd_extrinsicsgui;
/* buffer de imagen */
char mybuffer[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3];
char myvirtual_buffer[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3];
int flip_images = 0;
int fixedFOA = 0;

int aux_point_x, aux_point_y;
int mouse_pressed=0; /*it hasn't been pushed yet*/
int point1,point2,point3,point4;
int lower_x,lower_y,greater_x,greater_y,aux_point_x, aux_point_y;

/* X Image variables */
GC extrinsics_gc;
Window extrinsics_win;
XImage *extrinsics_imagen;
XImage *virtual_extrinsics_imagen;
XGCValues gc_values;
char image_buf[SIFNTSC_ROWS*SIFNTSC_COLUMNS*4];
char virtual_image_buf[SIFNTSC_ROWS*SIFNTSC_COLUMNS*4];

#define MAX_LINES_IN_ROOM 250
HPoint3D room[MAX_LINES_IN_ROOM*2]; /* mm */
int room_lines=0;
#define MAX_Z 30000. /* mm */
#define ROOM_MAX_X 79250.
#define ROOM_MIN_X -79250.
#define ROOM_MAX_Y 47890.
#define ROOM_MIN_Y -47890.
#define ROOM_MAX_Z 30000.
#define ROOM_MIN_Z -30000.
#define FDIST_MAX 750.
#define FDIST_MIN 1.
float maxX=0,minX=0,maxY=0,minY=0,maxZ=0,minZ=0;
float maxR=0., minR=0.;
float maxfoaX=0, minfoaX=0., maxfoaY=0, minfoaY=0., maxfoaZ=0., minfoaZ=0.;

#define SLOTS 3
#define DELTA 333 /* mm */
HPoint3D gridYZ[SLOTS*2*2], gridXZ[SLOTS*2*2], gridXY[SLOTS*2*2];


void mydebug()
{
 double u11,u12,u13,u14,u21,u22,u23,u24,u31,u32,u33,u34,u41,u42,u43,u44;
 double a=0.;
 double ri11,ri12,ri13,ri14,ri21,ri22,ri23,ri24,ri31,ri32,ri33,ri34,ri41,ri42,ri43,ri44;
 double ti11,ti12,ti13,ti14,ti21,ti22,ti23,ti24,ti31,ti32,ti33,ti34,ti41,ti42,ti43,ti44;
 double i11,i12,i13,i14,i21,i22,i23,i24,i31,i32,i33,i34,i41,i42,i43,i44;
 float rab31,rab32,rab33;
 float r;

 /* Rotation matrix is orthogonal: its inverse matrix is the transpose*/ 
 ri11=(double)mycameras[mycam].rt11;
 ri12=(double)mycameras[mycam].rt21;
 ri13=(double)mycameras[mycam].rt31;
 ri14=0.;
 ri21=(double)mycameras[mycam].rt12;
 ri22=(double)mycameras[mycam].rt22;
 ri23=(double)mycameras[mycam].rt32;
 ri24=0.;
 ri31=(double)mycameras[mycam].rt13;
 ri32=(double)mycameras[mycam].rt23;
 ri33=(double)mycameras[mycam].rt33;
 ri34=0.;
 ri41=0.;
 ri42=0.;
 ri43=0.;
 ri44=1.;
 
 ti11=1;
 ti12=0;
 ti13=0;
 ti14=(double)mycameras[mycam].position.X;
 ti21=0;
 ti22=1;
 ti23=0;
 ti24=(double)mycameras[mycam].position.Y;
 ti31=0;
 ti32=0;
 ti33=1;
 ti34=(double)mycameras[mycam].position.Z;
 ti41=0;
 ti42=0;
 ti43=0;
 ti44=(double)mycameras[mycam].position.H;
 
 i11=ti11*ri11+ti12*ri21+ti13*ri31+ti14*ri41;
 i12=ti11*ri12+ti12*ri22+ti13*ri32+ti14*ri42;
 i13=ti11*ri13+ti12*ri23+ti13*ri33+ti14*ri43;
 i14=ti11*ri14+ti12*ri24+ti13*ri34+ti14*ri44;
 i21=ti21*ri11+ti22*ri21+ti23*ri31+ti24*ri41;
 i22=ti21*ri12+ti22*ri22+ti23*ri32+ti24*ri42;
 i23=ti21*ri13+ti22*ri23+ti23*ri33+ti24*ri43;
 i24=ti21*ri14+ti22*ri24+ti23*ri34+ti24*ri44;
 i31=ti31*ri11+ti32*ri21+ti33*ri31+ti34*ri41;
 i32=ti31*ri12+ti32*ri22+ti33*ri32+ti34*ri42;
 i33=ti31*ri13+ti32*ri23+ti33*ri33+ti34*ri43;
 i34=ti31*ri14+ti32*ri24+ti33*ri34+ti34*ri44;
 i41=ti41*ri11+ti42*ri21+ti43*ri31+ti44*ri41;
 i42=ti41*ri12+ti42*ri22+ti43*ri32+ti44*ri42;
 i43=ti41*ri13+ti42*ri23+ti43*ri33+ti44*ri43;
 i44=ti41*ri14+ti42*ri24+ti43*ri34+ti44*ri44;
 
 a=mycameras[mycam].rt11*mycameras[mycam].rt11+
   mycameras[mycam].rt12*mycameras[mycam].rt12+
   mycameras[mycam].rt13*mycameras[mycam].rt13;
 printf("RT:modulo fila1: %f\n",a);
 a=mycameras[mycam].rt21*mycameras[mycam].rt21+
   mycameras[mycam].rt22*mycameras[mycam].rt22+
   mycameras[mycam].rt23*mycameras[mycam].rt23;
 printf("RT:modulo fila2: %f\n",a);
 a=mycameras[mycam].rt31*mycameras[mycam].rt31+
   mycameras[mycam].rt32*mycameras[mycam].rt32+
   mycameras[mycam].rt33*mycameras[mycam].rt33;
 printf("RT:modulo fila3: %f\n",a);
 a=mycameras[mycam].rt11*mycameras[mycam].rt21+
   mycameras[mycam].rt12*mycameras[mycam].rt22+
   mycameras[mycam].rt13*mycameras[mycam].rt23;
 printf("RT:(independencia) fila1*fila2: %f\n",a);
 a=mycameras[mycam].rt11*mycameras[mycam].rt31+
   mycameras[mycam].rt12*mycameras[mycam].rt32+
   mycameras[mycam].rt13*mycameras[mycam].rt33;
 printf("RT:(independencia) fila1*fila3: %f\n",a);
 a=mycameras[mycam].rt21*mycameras[mycam].rt31+
   mycameras[mycam].rt22*mycameras[mycam].rt32+
   mycameras[mycam].rt23*mycameras[mycam].rt33;
 printf("RT:(independencia) fila2*fila3: %f\n",a);
 
 a=mycameras[mycam].rt11*mycameras[mycam].rt11+
   mycameras[mycam].rt21*mycameras[mycam].rt21+
   mycameras[mycam].rt31*mycameras[mycam].rt31;
 printf("RTinversa: modulo fila1: %f\n",a);
 a=mycameras[mycam].rt12*mycameras[mycam].rt12+
   mycameras[mycam].rt22*mycameras[mycam].rt22+
   mycameras[mycam].rt32*mycameras[mycam].rt32;
 printf("RTinversa: modulo fila2: %f\n",a);
 a=mycameras[mycam].rt13*mycameras[mycam].rt13+
   mycameras[mycam].rt23*mycameras[mycam].rt23+
   mycameras[mycam].rt33*mycameras[mycam].rt33;
 printf("RTinversa: modulo fila3: %f\n",a);
 a=mycameras[mycam].rt11*mycameras[mycam].rt12+
   mycameras[mycam].rt21*mycameras[mycam].rt22+
   mycameras[mycam].rt31*mycameras[mycam].rt32;
 printf("RTinversa:(independencia) fila1*fila2: %f\n",a);
 a=mycameras[mycam].rt11*mycameras[mycam].rt13+
   mycameras[mycam].rt21*mycameras[mycam].rt23+
   mycameras[mycam].rt31*mycameras[mycam].rt33;
 printf("RTinversa:(independencia) fila1*fila3: %f\n",a);
 a=mycameras[mycam].rt12*mycameras[mycam].rt13+
   mycameras[mycam].rt22*mycameras[mycam].rt23+
   mycameras[mycam].rt32*mycameras[mycam].rt33;
 printf("RTinversa:(independencia) fila2*fila3: %f\n\n",a);
 
 u11=i11*mycameras[mycam].rt11+i12*mycameras[mycam].rt21+i13*mycameras[mycam].rt31+i14*mycameras[mycam].rt41;
 u12=i11*mycameras[mycam].rt12+i12*mycameras[mycam].rt22+i13*mycameras[mycam].rt32+i14*mycameras[mycam].rt42;
 u13=i11*mycameras[mycam].rt13+i12*mycameras[mycam].rt23+i13*mycameras[mycam].rt33+i14*mycameras[mycam].rt43;
 u14=i11*mycameras[mycam].rt14+i12*mycameras[mycam].rt24+i13*mycameras[mycam].rt34+i14*mycameras[mycam].rt44;
 u21=i21*mycameras[mycam].rt11+i22*mycameras[mycam].rt21+i23*mycameras[mycam].rt31+i24*mycameras[mycam].rt41;
 u22=i21*mycameras[mycam].rt12+i22*mycameras[mycam].rt22+i23*mycameras[mycam].rt32+i24*mycameras[mycam].rt42;
 u23=i21*mycameras[mycam].rt13+i22*mycameras[mycam].rt23+i23*mycameras[mycam].rt33+i24*mycameras[mycam].rt43;
 u24=i21*mycameras[mycam].rt14+i22*mycameras[mycam].rt24+i23*mycameras[mycam].rt34+i24*mycameras[mycam].rt44;
 u31=i31*mycameras[mycam].rt11+i32*mycameras[mycam].rt21+i33*mycameras[mycam].rt31+i34*mycameras[mycam].rt41;
 u32=i31*mycameras[mycam].rt12+i32*mycameras[mycam].rt22+i33*mycameras[mycam].rt32+i34*mycameras[mycam].rt42;
 u33=i31*mycameras[mycam].rt13+i32*mycameras[mycam].rt23+i33*mycameras[mycam].rt33+i34*mycameras[mycam].rt43;
 u34=i31*mycameras[mycam].rt14+i32*mycameras[mycam].rt24+i33*mycameras[mycam].rt34+i34*mycameras[mycam].rt44;
 u41=i41*mycameras[mycam].rt11+i42*mycameras[mycam].rt21+i43*mycameras[mycam].rt31+i44*mycameras[mycam].rt41;
 u42=i41*mycameras[mycam].rt12+i42*mycameras[mycam].rt22+i43*mycameras[mycam].rt32+i44*mycameras[mycam].rt42;
 u43=i41*mycameras[mycam].rt13+i42*mycameras[mycam].rt23+i43*mycameras[mycam].rt33+i44*mycameras[mycam].rt43;
 u44=i41*mycameras[mycam].rt14+i42*mycameras[mycam].rt24+i43*mycameras[mycam].rt34+i44*mycameras[mycam].rt44;
 
 printf(" RT por su 'inversa':\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n\n",
	u11,u12,u13,u14,u21,u22,u23,u24,u31,u32,u33,u34,u41,u42,u43,u44);

 printf(" RT :\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n\n",
	mycameras[mycam].rt11,mycameras[mycam].rt12,mycameras[mycam].rt13,mycameras[mycam].rt14,
	mycameras[mycam].rt21,mycameras[mycam].rt22,mycameras[mycam].rt23,mycameras[mycam].rt24,
	mycameras[mycam].rt31,mycameras[mycam].rt32,mycameras[mycam].rt33,mycameras[mycam].rt34,
	mycameras[mycam].rt41,mycameras[mycam].rt42,mycameras[mycam].rt43,mycameras[mycam].rt44);
 
  /* Orientation model: focus of attention + roll */
  rab31=mycameras[mycam].foa.X-mycameras[mycam].position.X;
  rab32=mycameras[mycam].foa.Y-mycameras[mycam].position.Y; 
  rab33=mycameras[mycam].foa.Z-mycameras[mycam].position.Z;
  printf(" foa-pos: %f %f %f\n",rab31,rab32,rab33);
  r=(float)sqrt((double)(rab31*rab31+rab32*rab32+rab33*rab33)); 
  rab31=rab31/r; rab32=rab32/r; rab33=rab33/r;
  printf(" normalizado: %f %f %f (r=%f)\n\n",rab31,rab32,rab33,r);

}


void relativas2absolutas(HPoint3D rel, HPoint3D *abs)
{
  double ri11,ri12,ri13,ri14,ri21,ri22,ri23,ri24,ri31,ri32,ri33,ri34,ri41,ri42,ri43,ri44;
  double ti11,ti12,ti13,ti14,ti21,ti22,ti23,ti24,ti31,ti32,ti33,ti34,ti41,ti42,ti43,ti44;
  double i11,i12,i13,i14,i21,i22,i23,i24,i31,i32,i33,i34,i41,i42,i43,i44;

  /* Rotation matrix is orthogonal: its inverse matrix is the transpose*/ 
  ri11=(double)mycameras[mycam].rt11;
  ri12=(double)mycameras[mycam].rt21;
  ri13=(double)mycameras[mycam].rt31;
  ri14=0.;
  ri21=(double)mycameras[mycam].rt12;
  ri22=(double)mycameras[mycam].rt22;
  ri23=(double)mycameras[mycam].rt32;
  ri24=0.;
  ri31=(double)mycameras[mycam].rt13;
  ri32=(double)mycameras[mycam].rt23;
  ri33=(double)mycameras[mycam].rt33;
  ri34=0.;
  ri41=0.;
  ri42=0.;
  ri43=0.;
  ri44=1.;
  
  ti11=1;
  ti12=0;
  ti13=0;
  ti14=(double)mycameras[mycam].position.X;
  ti21=0;
  ti22=1;
  ti23=0;
  ti24=(double)mycameras[mycam].position.Y;
  ti31=0;
  ti32=0;
  ti33=1;
  ti34=(double)mycameras[mycam].position.Z;
  ti41=0;
  ti42=0;
  ti43=0;
  ti44=(double)mycameras[mycam].position.H;

  i11=ti11*ri11+ti12*ri21+ti13*ri31+ti14*ri41;
  i12=ti11*ri12+ti12*ri22+ti13*ri32+ti14*ri42;
  i13=ti11*ri13+ti12*ri23+ti13*ri33+ti14*ri43;
  i14=ti11*ri14+ti12*ri24+ti13*ri34+ti14*ri44;
  i21=ti21*ri11+ti22*ri21+ti23*ri31+ti24*ri41;
  i22=ti21*ri12+ti22*ri22+ti23*ri32+ti24*ri42;
  i23=ti21*ri13+ti22*ri23+ti23*ri33+ti24*ri43;
  i24=ti21*ri14+ti22*ri24+ti23*ri34+ti24*ri44;
  i31=ti31*ri11+ti32*ri21+ti33*ri31+ti34*ri41;
  i32=ti31*ri12+ti32*ri22+ti33*ri32+ti34*ri42;
  i33=ti31*ri13+ti32*ri23+ti33*ri33+ti34*ri43;
  i34=ti31*ri14+ti32*ri24+ti33*ri34+ti34*ri44;
  i41=ti41*ri11+ti42*ri21+ti43*ri31+ti44*ri41;
  i42=ti41*ri12+ti42*ri22+ti43*ri32+ti44*ri42;
  i43=ti41*ri13+ti42*ri23+ti43*ri33+ti44*ri43;
  i44=ti41*ri14+ti42*ri24+ti43*ri34+ti44*ri44;
  
  (*abs).X=i11*(double)rel.X+i12*(double)rel.Y+i13*(double)rel.Z+i14*(double)rel.H;
  (*abs).Y=i21*(double)rel.X+i22*(double)rel.Y+i23*(double)rel.Z+i24*(double)rel.H;
  (*abs).Z=i31*(double)rel.X+i32*(double)rel.Y+i33*(double)rel.Z+i34*(double)rel.H;
  (*abs).H=i41*(double)rel.X+i42*(double)rel.Y+i43*(double)rel.Z+i44*(double)rel.H;
 
  if ((*abs).H!=0.)
    {
      (*abs).X=(float)((double)(*abs).X/(double)(*abs).H);
      (*abs).Y=(float)((double)(*abs).Y/(double)(*abs).H);
      (*abs).Z=(float)((double)(*abs).Z/(double)(*abs).H);
      (*abs).H=(float)((double)(*abs).H/(double)(*abs).H);
    }
  /*
  printf("TI4: X=%f Y=%f Z=%f H=%f\n",ti14,ti24,ti34,ti44);
  printf("REL: X=%f Y=%f Z=%f H=%f\n",rel.X,rel.Y,rel.Z,rel.H);
  printf("ABS: X=%f Y=%f Z=%f H=%f\n",(*abs).X,(*abs).Y,(*abs).Z,(*abs).H);
  printf("AUX: X=%f Y=%f Z=%f H=%f\n",aux.X,aux.Y,aux.Z,aux.H);
  */
}


void absolutas2relativas(HPoint3D abs, HPoint3D *rel)
{
 
  (*rel).X=mycameras[mycam].rt11*abs.X+mycameras[mycam].rt12*abs.Y+mycameras[mycam].rt13*abs.Z+mycameras[mycam].rt14*abs.H;
  (*rel).Y=mycameras[mycam].rt21*abs.X+mycameras[mycam].rt22*abs.Y+mycameras[mycam].rt23*abs.Z+mycameras[mycam].rt24*abs.H;
  (*rel).Z=mycameras[mycam].rt31*abs.X+mycameras[mycam].rt32*abs.Y+mycameras[mycam].rt33*abs.Z+mycameras[mycam].rt34*abs.H;
  (*rel).H=mycameras[mycam].rt41*abs.X+mycameras[mycam].rt42*abs.Y+mycameras[mycam].rt43*abs.Z+mycameras[mycam].rt44*abs.H;
  
  if ((*rel).H!=0.)
    {
      (*rel).X=(*rel).X/(*rel).H;
      (*rel).Y=(*rel).Y/(*rel).H;
      (*rel).Z=(*rel).Z/(*rel).H;
      (*rel).H=(*rel).H/(*rel).H;
    }
}


void extrinsics_iteration(){  
  /* nothing to do here. The functionality of this schema is "event-based" programmed, in the guidisplay and guibuttons functions */
}


/* creates a file with the tunned calibration of camera */
void save_cam(char *fich_sal)
{
  FILE *salida=fopen(fich_sal,"w");
  if (salida == NULL){fprintf(stderr,"Error: Can't write file '%s'\n",fich_sal); exit(-1);}
  
  fprintf(salida,"#extrinsics, position\n");
  fprintf(salida,"positionX %f\n",mycameras[mycam].position.X);
  fprintf(salida,"positionY %f\n",mycameras[mycam].position.Y);
  fprintf(salida,"positionZ %f\n",mycameras[mycam].position.Z);
  fprintf(salida,"positionH %f\n",mycameras[mycam].position.H);
  fprintf(salida,"\n #extrinsics, orientation\n");
  fprintf(salida,"FOApositionX %f\n",mycameras[mycam].foa.X);
  fprintf(salida,"FOApositionY %f\n",mycameras[mycam].foa.Y);
  fprintf(salida,"FOApositionZ %f\n",mycameras[mycam].foa.Z);
  fprintf(salida,"FOApositionH %f\n",mycameras[mycam].foa.H);
  fprintf(salida,"roll %f\n",mycameras[mycam].roll);
  fprintf(salida,"\n #intrinsics\n");
  fprintf(salida,"fx %f\n",mycameras[mycam].fdistx);
  fprintf(salida,"fy %f\n",mycameras[mycam].fdisty);
  fprintf(salida,"skew %f\n",mycameras[mycam].skew);
  fprintf(salida,"u0 %f\n",mycameras[mycam].u0);
  fprintf(salida,"v0 %f\n",mycameras[mycam].v0);
  fprintf(salida,"columns %d\n",mycameras[mycam].columns);
  fprintf(salida,"rows %d\n",mycameras[mycam].rows);
  fclose(salida);
}


/* gets the calibration of the camera from a file */
int load_cam_line(FILE *myfile)
{
 #define limit 256		
  char word1[limit],word2[limit];
  int i=0;
  char buffer_file[limit];   

  buffer_file[0]=fgetc(myfile);
  if (feof(myfile)) return EOF;
  if (buffer_file[0]==(char)255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  /* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
  while((buffer_file[i]!='\n') && 
	(buffer_file[i] != (char)255) &&  
	(i<limit-1) ) {
    buffer_file[++i]=fgetc(myfile);
  }
  
  if (i >= limit-1) { 
    printf("%s...\n", buffer_file); 
    printf ("Line too long in config file!\n"); 
    exit(-1);
  }
  buffer_file[++i]='\0';


  if (sscanf(buffer_file,"%s",word1)!=1) return 0; 
  /* return EOF; empty line*/
  else {
    if (strcmp(word1,"positionX")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].position.X=(float)atof(word2);
    }
    else if (strcmp(word1,"positionY")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].position.Y=(float)atof(word2);
    }
    else if (strcmp(word1,"positionZ")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].position.Z=(float)atof(word2);
    }
    else if (strcmp(word1,"positionH")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].position.H=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionX")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].foa.X=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionY")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].foa.Y=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionZ")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].foa.Z=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionH")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].foa.H=(float)atof(word2);
    }
    else if (strcmp(word1,"roll")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].roll=(float)atof(word2);
    }
    else if (strcmp(word1,"f")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].fdistx=(float)atof(word2);
      mycameras[mycam].fdisty=(float)atof(word2);
     }
    else if (strcmp(word1,"fx")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].fdistx=(float)atof(word2);
    }
    else if (strcmp(word1,"fy")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].fdisty=(float)atof(word2);
     }
    else if (strcmp(word1,"skew")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].skew=(float)atof(word2);
     }
    else if (strcmp(word1,"u0")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].u0=(float)atof(word2);
    }
    else if (strcmp(word1,"v0")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].v0=(float)atof(word2);
    } 
    else if (strcmp(word1,"columns")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].columns=(int)atof(word2);
    } 
    else if (strcmp(word1,"rows")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      mycameras[mycam].rows=(int)atof(word2);
    } 
  }
 return 1;
}

/* gets the calibration of the camera from a file */
void load_cam(char *fich_in)
{
  FILE *entrada;
  int i;

  entrada=fopen(fich_in,"r");
   if(entrada==NULL){
     printf("extrinsics: camera input calibration file %s does not exits\n",fich_in);
   }else{
     do{i=load_cam_line(entrada);}while(i!=EOF);
   } 
  fclose(entrada);
  update_camera_matrix(&mycameras[mycam]);
  /*mydebug();*/
}

/**
 * It reads a single line from config file, parses it and do the right thing.
 * @param myfile The config file descriptor.
 * @returns EOF in detects end of such file. Otherwise returns 0.
 */
int load_worldline(FILE *myfile)
{
  #define limit 256		
  char word1[limit],word2[limit],word3[limit],word4[limit],word5[limit];
  char word6[limit],word7[limit],word8[limit];
  char word[limit];
  int i=0;
  char buffer_file[limit];   

  buffer_file[0]=fgetc(myfile);
  if (feof(myfile)) return EOF;
  if (buffer_file[0]==(char)255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  /* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
  while((buffer_file[i]!='\n') && 
	(buffer_file[i] != (char)255) &&  
	(i<limit-1) ) {
    buffer_file[++i]=fgetc(myfile);
  }
  
  if (i >= limit-1) { 
    printf("%s...\n", buffer_file); 
    printf ("Line too long in config file!\n"); 
    exit(-1);
  }
  buffer_file[++i]='\0';


  if (sscanf(buffer_file,"%s",word)!=1) return 0; 
  /* return EOF; empty line*/
  else {
     if(strcmp(word,"worldline")==0){
	sscanf(buffer_file,"%s %s %s %s %s %s %s %s %s",word,word1,word2,word3,word4,word5,word6,word7,word8);
	room[room_lines*2+0].X=(float)atof(word1); room[room_lines*2+0].Y=(float)atof(word2); room[room_lines*2+0].Z=(float)atof(word3); room[room_lines*2+0].H=(float)atof(word4);
	room[room_lines*2+1].X=(float)atof(word5); room[room_lines*2+1].Y=(float)atof(word6); room[room_lines*2+1].Z=(float)atof(word7); room[room_lines*2+1].H=(float)atof(word8);
	room_lines++;
     }
     else if (strcmp(word,"cameraINfile")==0){
       sscanf(buffer_file,"%s %s",word,cameraINfile);
     }
     else if (strcmp(word,"cameraOUTfile")==0){
       sscanf(buffer_file,"%s %s",word,cameraOUTfile);
     }
     else if (strcmp(word,"maxX")==0){
     }
     else if (strcmp(word,"minX")==0){
     }
     else if (strcmp(word,"maxY")==0){
     }
     else if (strcmp(word,"minY")==0){
     }
     else if (strcmp(word,"maxZ")==0){
     }
     else if (strcmp(word,"minZ")==0){
     }
  }
  return 1;
}


/*Al terminar/cerrar el esquema*/
void extrinsics_terminate(){
 if (fd_extrinsicsgui!=NULL)
    {
      if (all[extrinsics_id].guistate==on) 
	fl_hide_form(fd_extrinsicsgui->extrinsicsgui);
      fl_free_form(fd_extrinsicsgui->extrinsicsgui);
    }
  printf ("extrinsics terminate\n");
}


void extrinsics_stop()
{
  /* printf("extrinsics: cojo-stop\n");*/
  pthread_mutex_lock(&(all[extrinsics_id].mymutex));
  put_state(extrinsics_id,slept);
  printf("extrinsics: off\n");
  pthread_mutex_unlock(&(all[extrinsics_id].mymutex));
  /*  printf("extrinsics: suelto-stop\n");*/
}


void extrinsics_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[extrinsics_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[extrinsics_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[extrinsics_id].children[i]=FALSE;
  all[extrinsics_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) extrinsics_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {extrinsics_brothers[i]=brothers[i];i++;}
    }
  extrinsics_callforarbitration=fn;
  put_state(extrinsics_id,notready);
  printf("extrinsics: on\n");
  pthread_cond_signal(&(all[extrinsics_id].condition));
  pthread_mutex_unlock(&(all[extrinsics_id].mymutex));

  /*Importar símbolos from images */
  myColorA=(char **)myimport("colorA","colorA");
  if (myColorA==NULL) printf("extrinsics: Warning colorA symbol not resolved\n");
  mycolorArun=(runFn)myimport("colorA","run");
  mycolorAstop=(stopFn)myimport("colorA","stop");
}

void *extrinsics_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[extrinsics_id].mymutex));

      if (all[extrinsics_id].state==slept)
      {
	 pthread_cond_wait(&(all[extrinsics_id].condition),&(all[extrinsics_id].mymutex));
	 pthread_mutex_unlock(&(all[extrinsics_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[extrinsics_id].state==notready)
	    put_state(extrinsics_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[extrinsics_id].state==ready)
	 {put_state(extrinsics_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[extrinsics_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[extrinsics_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    extrinsics_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)extrinsics_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[extrinsics_id].mymutex));
	    usleep(extrinsics_cycle*1000);
	 }
      }
   }
}


void extrinsics_init(char *configfile)
{
  int i=0;
  FILE *myconfig;

  pthread_mutex_lock(&(all[extrinsics_id].mymutex));
  printf("extrinsics schema started up\n");

  /*Exportar símbolos*/ 
  myexport("extrinsics","cycle",&extrinsics_cycle);
  myexport("extrinsics","run",(void *)extrinsics_run);
  myexport("extrinsics","stop",(void *)extrinsics_stop);

  put_state(extrinsics_id,slept);
  pthread_create(&(all[extrinsics_id].mythread),NULL,extrinsics_thread,NULL);
  pthread_mutex_unlock(&(all[extrinsics_id].mymutex));

  /* registering screen and display variables */
   if ((myscreen=(int *)myimport("graphics_xforms", "screen"))==NULL){
     fprintf (stderr, "extrinsics: I can't fetch screen from graphics_xforms\n");
     jdeshutdown(1);
   }
   if ((mydisplay=(Display *)myimport("graphics_xforms", "display"))==NULL){
     fprintf (stderr, "extrinsics: I can't fetch display from graphics_xforms\n");
     jdeshutdown(1);
   }

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

   /* initializes the 3D world */
   myconfig=fopen(configfile,"r");
   if(myconfig==NULL){
     printf("extrinsics: configuration file %s does not exits\n",configfile);
   }else{
     printf("extrinsics: configuration file %s\n",configfile);
     do{i=load_worldline(myconfig);}while(i!=EOF);
     fclose(myconfig);

     /* default values for camera parameters */
     mycameras[mycam].columns = SIFNTSC_COLUMNS;
     mycameras[mycam].rows = SIFNTSC_ROWS;
     mycameras[mycam].v0 = SIFNTSC_COLUMNS/2;
     mycameras[mycam].u0 = SIFNTSC_ROWS/2;
     mycameras[mycam].skew = 0.;
     /* load real info for the camera */
     load_cam(cameraINfile);

     /* for the undo operation be ready from the very beginning */
     save_cam(cameraOUTfile); 
   }
    
  /* info for virtual camera */
  mycameras[vcam].position.X = 14777.3;
  mycameras[vcam].position.Y = 1906.8;
  mycameras[vcam].position.Z = 6110.7;
  mycameras[vcam].position.H = 1.;
  mycameras[vcam].foa.X = 0.;
  mycameras[vcam].foa.Y = 1975.0;
  mycameras[vcam].foa.Z = -225.0;
  mycameras[vcam].foa.H = 1.;
  mycameras[vcam].fdistx = (float)ISIGHT_PINHOLE_FDIST;
  mycameras[vcam].fdisty = (float)ISIGHT_PINHOLE_FDIST;
  mycameras[vcam].u0 = (float)ISIGHT_PINHOLE_U0;
  mycameras[vcam].v0 = (float)ISIGHT_PINHOLE_V0;
  mycameras[vcam].roll = 0.0;
  mycameras[vcam].skew = 0.0; 
  mycameras[vcam].columns = SIFNTSC_COLUMNS;
  mycameras[vcam].rows = SIFNTSC_ROWS;
  update_camera_matrix(&mycameras[vcam]); 
}



void set_sliders_position()
{
  /* setting info from selected camera to sliders */
  fl_set_slider_value(fd_extrinsicsgui->x_cam_slider,(double)mycameras[mycam].position.X);
  fl_set_slider_value(fd_extrinsicsgui->y_cam_slider,(double)mycameras[mycam].position.Y);
  fl_set_slider_value(fd_extrinsicsgui->z_cam_slider,(double)mycameras[mycam].position.Z);

  /* orientation: foa + roll */
  fl_set_slider_value(fd_extrinsicsgui->X_cam_slider,(double)mycameras[mycam].foa.X);
  fl_set_slider_value(fd_extrinsicsgui->Y_cam_slider,(double)mycameras[mycam].foa.Y);
  fl_set_slider_value(fd_extrinsicsgui->Z_cam_slider,(double)mycameras[mycam].foa.Z);
  fl_set_slider_value(fd_extrinsicsgui->roll_slider,(double)((mycameras[mycam].roll*360)/(2*PI)));

  /* intrinsics */
  fl_set_slider_value(fd_extrinsicsgui->focus_slider,(double)mycameras[mycam].fdisty);
  fl_set_slider_value(fd_extrinsicsgui->u0,(double)mycameras[mycam].u0);
  fl_set_slider_value(fd_extrinsicsgui->v0,(double)mycameras[mycam].v0);
}


void extrinsics_guibuttons(void *obj){
  float a,b,rxy,lati,longi,r;
  float px,py,pz,inc_lati,inc_longi;
  float deltaX,deltaY,deltaZ;

  /* now we check every object to see if it has changed */
  if (obj == fd_extrinsicsgui->exit_button){ /* EXIT BUTTON */
    jdeshutdown(0);

  }else if (obj == fd_extrinsicsgui->flip_button){ /* FLIP BUTTON */
    if (fl_get_button(fd_extrinsicsgui->flip_button)==RELEASED){
      flip_images = 0;
    }else{
      flip_images = 1;
    }
  }
  else if (obj == fd_extrinsicsgui->fixedFOA){ /* FixedFOA BUTTON */
    if (fl_get_button(fd_extrinsicsgui->fixedFOA)==RELEASED){
      fixedFOA = 0;
    }else{
      fixedFOA = 1;
    }
    
  }else if (obj == fd_extrinsicsgui->save){ /* EXPORT BUTTON */
      save_cam(cameraOUTfile);
  }else  if (obj == fd_extrinsicsgui->reload){ /* RELOAD BUTTON */
      load_cam(cameraINfile);
  }else if (obj == fd_extrinsicsgui->undo){ /* UNDO BUTTON */
      load_cam(cameraOUTfile);

  }else if((obj == fd_extrinsicsgui->x_cam_slider)||(obj == fd_extrinsicsgui->y_cam_slider)||(obj == fd_extrinsicsgui->z_cam_slider)){
    /* CAMERA POSITION SLIDERS */
    deltaX=(float) fl_get_slider_value(fd_extrinsicsgui->x_cam_slider)-mycameras[mycam].position.X;
    deltaY=(float) fl_get_slider_value(fd_extrinsicsgui->y_cam_slider)-mycameras[mycam].position.Y;
    deltaZ=(float) fl_get_slider_value(fd_extrinsicsgui->z_cam_slider)-mycameras[mycam].position.Z;
    mycameras[mycam].position.X+=deltaX;
    mycameras[mycam].position.Y+=deltaY;
    mycameras[mycam].position.Z+=deltaZ;
    mycameras[mycam].position.H=1.;
    if (fixedFOA==0)
      {
	mycameras[mycam].foa.X+=deltaX;
	mycameras[mycam].foa.Y+=deltaY;
	mycameras[mycam].foa.Z+=deltaZ;
      }
    update_camera_matrix(&mycameras[mycam]);
    /*mydebug();*/

  }else if((obj == fd_extrinsicsgui->X_cam_slider)||(obj == fd_extrinsicsgui->Y_cam_slider)||(obj == fd_extrinsicsgui->Z_cam_slider)){
    /* FOA position sliders */
    mycameras[mycam].foa.X=(float)fl_get_slider_value(fd_extrinsicsgui->X_cam_slider);
    mycameras[mycam].foa.Y=(float)fl_get_slider_value(fd_extrinsicsgui->Y_cam_slider);
    mycameras[mycam].foa.Z=(float)fl_get_slider_value(fd_extrinsicsgui->Z_cam_slider);
    mycameras[mycam].foa.H=1.;
    update_camera_matrix(&mycameras[mycam]);
    /*mydebug();*/
    
  }else if(obj == fd_extrinsicsgui->roll_slider){ /* ROLL SLIDER */
    mycameras[mycam].roll=(2.*PI*(float) fl_get_slider_value(fd_extrinsicsgui->roll_slider))/360;
    update_camera_matrix(&mycameras[mycam]); 
    /*mydebug();*/

  }else if(obj == fd_extrinsicsgui->cam_joystick){

    a=fl_get_positioner_xvalue(fd_extrinsicsgui->cam_joystick);
    if ((a<0.2)&&(a>-0.2)) a=0.;
    a=a*a*a*3;
    inc_longi=2*PI*a/360.;
    b=fl_get_positioner_yvalue(fd_extrinsicsgui->cam_joystick);
    if ((b<0.2)&&(b>-0.2)) b=0.;
    b=b*b*b*3;
    inc_lati=2*PI*b/360.;

    /* new foa location in latitude+longitude relative to cartesian absolute axis at the current camera location. */
    /* they are added to the current latitude+longitude relative to the same axis (increments)*/
    px=mycameras[mycam].foa.X-mycameras[mycam].position.X;
    py=mycameras[mycam].foa.Y-mycameras[mycam].position.Y;
    pz=mycameras[mycam].foa.Z-mycameras[mycam].position.Z;
    rxy=(float)sqrt((double)(px*px+py*py)); 
    longi=(float)atan2((double)py,(double)px);
    lati=(float)atan2((double)pz,(double)rxy);

    longi=longi+inc_longi;
    while (longi>PI) longi=longi-2*PI;
    while (longi<-PI) longi=longi+2*PI;
    lati=lati+inc_lati;
    if (lati>PI/2) lati=PI/2;
    if (lati<-PI/2) lati=-PI/2;

    r=4000.; /* new foa distance to the current camera location */
    pz=r*sin(lati);
    r=r*cos(lati);
    px=r*cos(longi);
    py=r*sin(longi);
    mycameras[mycam].foa.X=px+mycameras[mycam].position.X;
    mycameras[mycam].foa.Y=py+mycameras[mycam].position.Y;
    mycameras[mycam].foa.Z=pz+mycameras[mycam].position.Z;
    update_camera_matrix(&mycameras[mycam]);
  
    fl_set_slider_value(fd_extrinsicsgui->X_cam_slider,(double)mycameras[mycam].foa.X);
    fl_set_slider_value(fd_extrinsicsgui->Y_cam_slider,(double)mycameras[mycam].foa.Y);
    fl_set_slider_value(fd_extrinsicsgui->Z_cam_slider,(double)mycameras[mycam].foa.Z);
    fl_set_positioner_xvalue(fd_extrinsicsgui->cam_joystick,(double) 0.);
    fl_set_positioner_yvalue(fd_extrinsicsgui->cam_joystick,(double) 0.);
  
  }else if(obj == fd_extrinsicsgui->focus_slider){ /* FOCUS DISTANCE SLIDER */

    mycameras[mycam].fdisty=(float) fl_get_slider_value(fd_extrinsicsgui->focus_slider);
    mycameras[mycam].fdistx=(float) fl_get_slider_value(fd_extrinsicsgui->focus_slider);
    update_camera_matrix(&mycameras[mycam]);
    /*mydebug();*/

  }else if(obj == fd_extrinsicsgui->u0){ 

    mycameras[mycam].u0=(float) fl_get_slider_value(fd_extrinsicsgui->u0);
    update_camera_matrix(&mycameras[mycam]);
    /*mydebug();*/

  }else if(obj == fd_extrinsicsgui->v0){

    mycameras[mycam].v0=(float) fl_get_slider_value(fd_extrinsicsgui->v0);
    update_camera_matrix(&mycameras[mycam]);
    /*mydebug();*/
  }
}



int drawline(HPoint2D p1, HPoint2D p2, FL_COLOR thiscolor, TPinHoleCamera camera, char *img)
/* it takes care of important features */
/* before/behind the focal plane, inside/outside the image frame */
{  
  int xa,ya,xb,yb;
  HPoint2D gooda,goodb;
  float L;
  int i,imax,r,g,b;
  int lastx,lasty,thisx,thisy,lastcount;
  int threshold=1; 
  int Xmax,Xmin,Ymax,Ymin;
  
  if(displayline(p1,p2,&gooda,&goodb,camera)==1)
    {
      xa=(int)gooda.y;
      ya=camera.rows-1-(int)gooda.x;
      xb=(int)goodb.y;
      yb=camera.rows-1-(int)goodb.x;

      Xmin=0; Xmax=camera.columns-1; Ymin=0; Ymax=camera.rows-1;
      
      /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image.
	 They can't reach 240 or 320, their are not valid values for the pixels. */
      
      if (thiscolor==FL_BLACK) {r=0;g=0;b=0;}
      else if (thiscolor==FL_RED) {r=255;g=0;b=0;} 
      else if (thiscolor==FL_GREEN) {r=0;g=255;b=0;} 
      else if (thiscolor==FL_BLUE) {r=0;g=0;b=255;} 
      else if (thiscolor==FL_PALEGREEN) {r=113;g=198;b=113;} 
      else if (thiscolor==FL_WHEAT) {r=255;g=231;b=155;}
      else if (thiscolor==FL_DEEPPINK) {r=213;g=85;b=178; }   
      else {r=0;g=0;b=0;}
      
      /* first, check both points are inside the limits and draw them */
      if ((xa>=Xmin) && (xa<Xmax+1) && (ya>=Ymin) && (ya<Ymax+1) &&
	  (xb>=Xmin) && (xb<Xmax+1) && (yb>=Ymin) && (yb<Ymax+1)){
	/* draw both points */
	
	img[(camera.columns*ya+xa)*3+0]=b;
	img[(camera.columns*ya+xa)*3+1]=g;
	img[(camera.columns*ya+xa)*3+2]=r;
	img[(camera.columns*yb+xb)*3+0]=b;
	img[(camera.columns*yb+xb)*3+1]=g;
	img[(camera.columns*yb+xb)*3+2]=r;
	
	L=(float)sqrt((double)((xb-xa)*(xb-xa)+(yb-ya)*(yb-ya)));
	imax=3*(int)L+1;
	lastx=xa; lasty=xb; lastcount=0;
	for(i=0;i<=imax;i++){
	  thisy=(int)((float)ya+(float)i/(float)imax*(float)(yb-ya));
	  thisx=(int)((float)xa+(float)i/(float)imax*(float)(xb-xa));
	  if ((thisy==lasty)&&(thisx==lastx)) lastcount++;
	  else{ 
	    if (lastcount>=threshold){ /* draw that point in the image */
	      img[(camera.columns*lasty+lastx)*3+0]=b;
	      img[(camera.columns*lasty+lastx)*3+1]=g;
	      img[(camera.columns*lasty+lastx)*3+2]=r;
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
  else return -1;
}

void extrinsics_guidisplay(){
  int i,fila,columna;
  HPoint2D a,b;
  HPoint3D a3A,a3B,a3C,a3D;
  HPoint3D H1,H2;
  HPoint3D Xaxis,Yaxis,Zaxis;
  HPoint3D XaxisRel,YaxisRel,ZaxisRel;
  int cross[17]; /* cross for the optical center */
  float rx,ry,rz,sx,sy,sz,r1x,r1y,r1z,r2x,r2y,r2z,s1x,s1y,s1z;
  float ax,ay,az,bx,by,bz,cx,cy,cz,dx,dy,dz,ex,ey,ez;
  float d,l;

  set_sliders_position();
  fl_redraw_object(fd_extrinsicsgui->cam_joystick);

  if(mycam == 0){
    if(myColorA!=NULL){
      bcopy(*myColorA,mybuffer,SIFNTSC_ROWS*SIFNTSC_COLUMNS*3);
    }else{
      printf("extrinsics: warning myColorA is NULL!\n");
    }
  }

  /* check if flip mode is active */
  if(flip_images == 1){
    char buffer_flipped[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3];
    for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){
      buffer_flipped[i*3]=mybuffer[(SIFNTSC_COLUMNS*SIFNTSC_ROWS-1-i)*3]; /* Blue Byte */
      buffer_flipped[i*3+1]=mybuffer[(SIFNTSC_COLUMNS*SIFNTSC_ROWS-1-i)*3+1]; /* Green Byte */
      buffer_flipped[i*3+2]=mybuffer[(SIFNTSC_COLUMNS*SIFNTSC_ROWS-1-i)*3+2]; /* Red Byte */
    }
    bcopy(buffer_flipped,mybuffer,SIFNTSC_ROWS*SIFNTSC_COLUMNS*3);
  }
  
  /* room in camera image */
  for(i=0;i<room_lines;i++){
    project(room[i*2+0],&a,mycameras[mycam]);
    project(room[i*2+1],&b,mycameras[mycam]);
    drawline(a,b,FL_RED,mycameras[mycam],mybuffer);
  }
  /* cross at the optical center */
  fila=SIFNTSC_ROWS-1-(int)mycameras[mycam].u0;
  columna=(int)mycameras[mycam].v0;
  cross[0]=(fila%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+(columna%SIFNTSC_COLUMNS);
  cross[1]=((fila-1)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-1)%SIFNTSC_COLUMNS);
  cross[2]=((fila-2)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-2)%SIFNTSC_COLUMNS);
  cross[3]=((fila-3)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-3)%SIFNTSC_COLUMNS);
  cross[4]=((fila-4)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-4)%SIFNTSC_COLUMNS);
  cross[5]=((fila+1)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-1)%SIFNTSC_COLUMNS);
  cross[6]=((fila+2)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-2)%SIFNTSC_COLUMNS);
  cross[7]=((fila+3)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-3)%SIFNTSC_COLUMNS);
  cross[8]=((fila+4)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna-4)%SIFNTSC_COLUMNS);
  cross[9]=((fila-1)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+1)%SIFNTSC_COLUMNS);
  cross[10]=((fila-2)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+2)%SIFNTSC_COLUMNS);
  cross[11]=((fila-3)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+3)%SIFNTSC_COLUMNS);
  cross[12]=((fila-4)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+4)%SIFNTSC_COLUMNS);
  cross[13]=((fila+1)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+1)%SIFNTSC_COLUMNS);
  cross[14]=((fila+2)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+2)%SIFNTSC_COLUMNS);
  cross[15]=((fila+3)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+3)%SIFNTSC_COLUMNS);
  cross[16]=((fila+4)%SIFNTSC_ROWS)*SIFNTSC_COLUMNS+((columna+4)%SIFNTSC_COLUMNS);
  for(i=0; i<17; i++){ 	
    mybuffer[cross[i]*3]=0; /* Blue Byte */
    mybuffer[cross[i]*3+1]=(char)255; /* Green Byte */
    mybuffer[cross[i]*3+2]=0; /* Red Byte */
  }

  /*horizon line, using planar projective geometry */
  ax=mycameras[mycam].foa.X;
  ay=mycameras[mycam].foa.Y;
  az=1;
  bx=mycameras[mycam].position.X;
  by=mycameras[mycam].position.Y;
  bz=1;
  rx=ay-by;
  ry=bx-ax;
  rz=ax*by-bx*ay;
  sx=ry;
  sy=-rx;
  sz=-sx*ax-sy*ay;
  l=100000.; /* far away, 100m */
  s1x=sx;
  s1y=sy;
  s1z=sz+l*(float)sqrt((double)(sx*sx+sy*sy));
  ex=s1y*rz-ry*s1z;
  ey=s1z*rx-rz*s1x;
  ez=s1x*ry-rx*s1y;
  ex=ex/ez;
  ey=ey/ez;
  ez=1.;
  if (((ex-bx)*(ax-bx)+(ey-by)*(ay-by))<=0)
    {
      s1z=sz-l*(float)sqrt((double)(sx*sx+sy*sy));
      ex=s1y*rz-ry*s1z;
      ey=s1z*rx-rz*s1x;
      ez=s1x*ry-rx*s1y;
      ex=ex/ez;
      ey=ey/ez;
      ez=1.;
    }

  d=l+(float)sqrt((double)((ex-bx)*(ex-bx)+(ey-by)*(ey-by)));
  d=d*(float)tan((double)PI/8.); /*  +-30deg around FOA line */
  r1x=rx;
  r1y=ry;
  r1z=rz-d*(float)sqrt((double)(rx*rx+ry*ry));
  r2x=rx;
  r2y=ry;
  r2z=rz+d*(float)sqrt((double)(rx*rx+ry*ry));
  cx=s1y*r2z-r2y*s1z;
  cy=s1z*r2x-r2z*s1x;
  cz=s1x*r2y-r2x*s1y;
  dx=s1y*r1z-r1y*s1z;
  dy=s1z*r1x-r1z*s1x;
  dz=s1x*r1y-r1x*s1y;
  H1.H=1.;
  H1.X=cx/cz;
  H1.Y=cy/cz;
  H1.Z=0;
  H2.H=1.;
  H2.X=dx/dz;
  H2.Y=dy/dz;
  H2.Z=0;
  project(H1,&a,mycameras[mycam]);
  project(H2,&b,mycameras[mycam]);
  drawline(a,b,FL_GREEN,mycameras[mycam],mybuffer);

  /* rendering of virtual image */
  /* reset_virtual_buffer */
  for(i=0;i<SIFNTSC_ROWS*SIFNTSC_COLUMNS*3;i++)
    {
      myvirtual_buffer[i] = (char)255;
    } 
 			
  /*camera1 field of view*/
  a.x=SIFNTSC_ROWS-1.;
  a.y=SIFNTSC_COLUMNS-1.;
  a.h=1.;
  backproject(&a3A,a,mycameras[mycam]);
  project(a3A,&a,mycameras[vcam]);
  project(mycameras[mycam].position,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
  a.x=0.;
  a.y=SIFNTSC_COLUMNS-1.;
  a.h=1.;
  backproject(&a3B,a,mycameras[mycam]);
  project(a3B,&a,mycameras[vcam]);
  project(mycameras[mycam].position,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
  a.x=0.;
  a.y=0.;
  a.h=1.;
  backproject(&a3C,a,mycameras[mycam]);
  project(a3C,&a,mycameras[vcam]);
  project(mycameras[mycam].position,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
  a.x=SIFNTSC_ROWS-1.;
  a.y=0.;
  a.h=1.;
  backproject(&a3D,a,mycameras[mycam]);
  project(a3D,&a,mycameras[vcam]);
  project(mycameras[mycam].position,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
	
  project(a3A,&a,mycameras[vcam]);
  project(a3B,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
  project(a3B,&a,mycameras[vcam]);
  project(a3C,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
  project(a3C,&a,mycameras[vcam]);
  project(a3D,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
  project(a3D,&a,mycameras[vcam]);
  project(a3A,&b,mycameras[vcam]);
  drawline(a,b,FL_DEEPPINK,mycameras[vcam],myvirtual_buffer);
				
  /* room in virtual camera */
  for(i=0;i<room_lines;i++){
    project(room[i*2+0],&a,mycameras[vcam]);
    project(room[i*2+1],&b,mycameras[vcam]);
    drawline(a,b,FL_BLACK,mycameras[vcam],myvirtual_buffer);
  }

 /* Draw XY, XZ, YZ grids */
  for(i=0;i<SLOTS;i++)
    {
      project(gridXY[2*i],&a,mycameras[vcam]);
      project(gridXY[2*i+1],&b,mycameras[vcam]);
      drawline(a,b,FL_RED,mycameras[vcam],myvirtual_buffer);
      project(gridXY[2*SLOTS+2*i],&a,mycameras[vcam]);
      project(gridXY[2*SLOTS+2*i+1],&b,mycameras[vcam]);
      drawline(a,b,FL_RED,mycameras[vcam],myvirtual_buffer);
    }
  for(i=0;i<SLOTS;i++)
    {
      project(gridYZ[2*i],&a,mycameras[vcam]);
      project(gridYZ[2*i+1],&b,mycameras[vcam]);
      drawline(a,b,FL_PALEGREEN,mycameras[vcam],myvirtual_buffer);
      project(gridYZ[2*SLOTS+2*i],&a,mycameras[vcam]);
      project(gridYZ[2*SLOTS+2*i+1],&b,mycameras[vcam]);
      drawline(a,b,FL_PALEGREEN,mycameras[vcam],myvirtual_buffer);
    }
  for(i=0;i<SLOTS;i++)
    {
      project(gridXZ[2*i],&a,mycameras[vcam]);
      project(gridXZ[2*i+1],&b,mycameras[vcam]);
      drawline(a,b,FL_BLUE,mycameras[vcam],myvirtual_buffer);
      project(gridXZ[2*SLOTS+2*i],&a,mycameras[vcam]);
      project(gridXZ[2*SLOTS+2*i+1],&b,mycameras[vcam]);
      drawline(a,b,FL_BLUE,mycameras[vcam],myvirtual_buffer);
    }

  /* camera axis in virtual camera */
  XaxisRel.X=1000.; XaxisRel.Y=0.; XaxisRel.Z=0.; XaxisRel.H=1.;
  YaxisRel.X=0.; YaxisRel.Y=1000.; YaxisRel.Z=0.; YaxisRel.H=1.;
  ZaxisRel.X=0.; ZaxisRel.Y=0.; ZaxisRel.Z=1000.; ZaxisRel.H=1.; 
  relativas2absolutas(XaxisRel,&Xaxis);
  project(mycameras[mycam].position,&a,mycameras[vcam]);
  project(Xaxis,&b,mycameras[vcam]);
  drawline(a,b,FL_PALEGREEN,mycameras[vcam],myvirtual_buffer);
  relativas2absolutas(YaxisRel,&Yaxis);
  project(mycameras[mycam].position,&a,mycameras[vcam]);
  project(Yaxis,&b,mycameras[vcam]);
  drawline(a,b,FL_BLUE,mycameras[vcam],myvirtual_buffer);
  relativas2absolutas(ZaxisRel,&Zaxis);
  project(mycameras[mycam].position,&a,mycameras[vcam]);
  project(Zaxis,&b,mycameras[vcam]);
  drawline(a,b,FL_RED,mycameras[vcam],myvirtual_buffer);

 /*cameras axis*/
  project(mycameras[mycam].position,&a,mycameras[vcam]);
  project(mycameras[mycam].foa,&b,mycameras[vcam]);
  /*FL_WHEAT,FL_PALEGREEN*/
  drawline(a,b,FL_RED,mycameras[vcam],myvirtual_buffer);  



  for(i=0; i<SIFNTSC_ROWS*SIFNTSC_COLUMNS; i++){ 	
    image_buf[i*4]=mybuffer[i*3]; /* Blue Byte */
    image_buf[i*4+1]=mybuffer[i*3+1]; /* Green Byte */
    image_buf[i*4+2]=mybuffer[i*3+2]; /* Red Byte */
    image_buf[i*4+3]=0; /* dummy byte */ 

    virtual_image_buf[i*4]=myvirtual_buffer[i*3]; /* Blue Byte */
    virtual_image_buf[i*4+1]=myvirtual_buffer[i*3+1]; /* Green Byte */
    virtual_image_buf[i*4+2]=myvirtual_buffer[i*3+2]; /* Red Byte */
    virtual_image_buf[i*4+3]=0; /* dummy byte */ 
  };

  /* pintamos la imagen modificada */
  XPutImage(mydisplay,extrinsics_win,extrinsics_gc,extrinsics_imagen,0,0,fd_extrinsicsgui->color_freeobject->x, fd_extrinsicsgui->color_freeobject->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);

  XPutImage(mydisplay,extrinsics_win,extrinsics_gc,virtual_extrinsics_imagen,0,0,fd_extrinsicsgui->virtual_freeobject->x, fd_extrinsicsgui->virtual_freeobject->y,  SIFNTSC_COLUMNS, SIFNTSC_ROWS);
}

void extrinsics_hide_aux(void){
  all[extrinsics_id].guistate=off;
  mydelete_buttonscallback(extrinsics_guibuttons);
  mydelete_displaycallback(extrinsics_guidisplay);
  fl_hide_form(fd_extrinsicsgui->extrinsicsgui);

  /* calling stop functions for colors */
  if(mycolorAstop!=NULL){
    mycolorAstop();
  }
}

void extrinsics_hide(){
   static callback fn=NULL;

   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)extrinsics_hide_aux);
      }
   }
   else{
      fn ((gui_function)extrinsics_hide_aux);
   }
}

int myclose_form(FL_FORM *form, void *an_argument)
{
  extrinsics_hide();
  return FL_IGNORE;
}

void extrinsics_show_aux(void)
{
  static int k=0;
  int vmode,i;
  static XGCValues gc_values;

  all[extrinsics_id].guistate=on;
  if (k==0){ /* not initialized */
     k++;
     fd_extrinsicsgui = create_form_extrinsicsgui();
     fl_set_form_position(fd_extrinsicsgui->extrinsicsgui,400,50);
     fl_show_form(fd_extrinsicsgui->extrinsicsgui,FL_PLACE_POSITION,
		  FL_FULLBORDER,"extrinsics");
     fl_set_form_atclose(fd_extrinsicsgui->extrinsicsgui,myclose_form,0);
     
     extrinsics_win = FL_ObjWin(fd_extrinsicsgui->color_freeobject);

     /* Inicializa las ventanas, la paleta de colores y memoria compartida para visualizacion*/
     gc_values.graphics_exposures = False;
     extrinsics_gc = XCreateGC(mydisplay,extrinsics_win, GCGraphicsExposures, &gc_values);
     
     vmode= fl_get_vclass();
     
     if ((vmode==TrueColor)&&(fl_state[vmode].depth==16))
       {
	 printf("16 bits mode\n");
	 extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),16,ZPixmap,0,image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	 virtual_extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),16,ZPixmap,0,virtual_image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
       }
     else if ((vmode==TrueColor)&&(fl_state[vmode].depth==24))
       {
	 printf("24 bits mode\n");
	 extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24,ZPixmap,0,image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	 virtual_extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),24,ZPixmap,0,virtual_image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
       }
     else if ((vmode==TrueColor)&&(fl_state[vmode].depth==32))
       {
	 printf("32 bits mode\n");
	 extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32,ZPixmap,0,image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	 virtual_extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),32,ZPixmap,0,virtual_image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);  
       }
     else if ((vmode==PseudoColor)&&(fl_state[vmode].depth==8))
       {
	 printf("8 bits mode\n");
	 extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8,ZPixmap,0,image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
	 virtual_extrinsics_imagen = XCreateImage(mydisplay,DefaultVisual(mydisplay,*myscreen),8,ZPixmap,0,virtual_image_buf,SIFNTSC_COLUMNS,SIFNTSC_ROWS,8,0);
       }
     else
       {
	 perror("Unsupported color mode in X server");exit(1);
       }

     /* XYgrid, XZgrid, YZgrid in 3D */
     for(i=0;i<SLOTS;i++)
       {     
	 gridXY[2*i].X=DELTA*(i+1); 
	 gridXY[2*i].Y=0; 
	 gridXY[2*i].Z=0; 
	 gridXY[2*i].H=1;
	 gridXY[2*i+1].X=DELTA*(i+1); 
	 gridXY[2*i+1].Y=DELTA*SLOTS; 
	 gridXY[2*i+1].Z=0; 
	 gridXY[2*i+1].H=1;
	 
	 gridXY[2*SLOTS+2*i].X=0; 
	 gridXY[2*SLOTS+2*i].Y=DELTA*(i+1);
	 gridXY[2*SLOTS+2*i].Z=0;
	 gridXY[2*SLOTS+2*i].H=1;
	 gridXY[2*SLOTS+2*i+1].X=DELTA*SLOTS;
	 gridXY[2*SLOTS+2*i+1].Y=DELTA*(i+1);
	 gridXY[2*SLOTS+2*i+1].Z=0;
	 gridXY[2*SLOTS+2*i+1].H=1;
	 gridYZ[2*i].X=0;
	 gridYZ[2*i].Y=DELTA*(i+1);
	 gridYZ[2*i].Z=0;
	 gridYZ[2*i].H=1;
	 gridYZ[2*i+1].X=0;
	 gridYZ[2*i+1].Y=DELTA*(i+1);
	 gridYZ[2*i+1].Z=DELTA*SLOTS;
	 gridYZ[2*i+1].H=1;
	 
	 gridYZ[2*SLOTS+2*i].X=0;
	 gridYZ[2*SLOTS+2*i].Y=0;
	 gridYZ[2*SLOTS+2*i].Z=DELTA*(i+1);
	 gridYZ[2*SLOTS+2*i].H=1;
	 gridYZ[2*SLOTS+2*i+1].X=0;
	 gridYZ[2*SLOTS+2*i+1].Y=DELTA*SLOTS;
	 gridYZ[2*SLOTS+2*i+1].Z=DELTA*(i+1);
	 gridYZ[2*SLOTS+2*i+1].H=1;
	 
	 gridXZ[2*i].X=DELTA*(i+1);
	 gridXZ[2*i].Y=0;
	 gridXZ[2*i].Z=0;
	 gridXZ[2*i].H=1;
	 gridXZ[2*i+1].X=DELTA*(i+1);
	 gridXZ[2*i+1].Y=0;
	 gridXZ[2*i+1].Z=DELTA*SLOTS;
	 gridXZ[2*i+1].H=1;
	 
	 gridXZ[2*SLOTS+2*i].X=0;
	 gridXZ[2*SLOTS+2*i].Y=0;
	 gridXZ[2*SLOTS+2*i].Z=DELTA*(i+1);
	 gridXZ[2*SLOTS+2*i].H=1;
	 gridXZ[2*SLOTS+2*i+1].X=DELTA*SLOTS;
	 gridXZ[2*SLOTS+2*i+1].Y=0;
	 gridXZ[2*SLOTS+2*i+1].Z=DELTA*(i+1);
	 gridXZ[2*SLOTS+2*i+1].H=1;
       }


  }
  else{
    fl_show_form(fd_extrinsicsgui->extrinsicsgui,FL_PLACE_POSITION,
		 FL_FULLBORDER,"extrinsics");
    extrinsics_win = FL_ObjWin(fd_extrinsicsgui->color_freeobject);
  /* the window (extrinsics_win) changes every time the form is hided and showed again. They need to be updated before displaying anything again */
  }

  myregister_displaycallback(extrinsics_guidisplay);
  myregister_buttonscallback(extrinsics_guibuttons);

    /* Translation */
  fl_set_slider_bounds(fd_extrinsicsgui->x_cam_slider,ROOM_MAX_X,ROOM_MIN_X);
  fl_set_slider_value(fd_extrinsicsgui->x_cam_slider,(double)0.);
  fl_set_slider_bounds(fd_extrinsicsgui->y_cam_slider,ROOM_MAX_Y,ROOM_MIN_Y);
  fl_set_slider_value(fd_extrinsicsgui->y_cam_slider,(double)0.);
  fl_set_slider_bounds(fd_extrinsicsgui->z_cam_slider,ROOM_MAX_Z,ROOM_MIN_Z);
  fl_set_slider_value(fd_extrinsicsgui->z_cam_slider,(double)0.);
  fl_set_slider_bounds(fd_extrinsicsgui->X_cam_slider,ROOM_MAX_X,ROOM_MIN_X);
  fl_set_slider_value(fd_extrinsicsgui->X_cam_slider,(double)0.);
  fl_set_slider_bounds(fd_extrinsicsgui->Y_cam_slider,ROOM_MAX_Y,ROOM_MIN_Y);
  fl_set_slider_value(fd_extrinsicsgui->Y_cam_slider,(double)0.);
  fl_set_slider_bounds(fd_extrinsicsgui->Z_cam_slider,ROOM_MAX_Z,ROOM_MIN_Z);
  fl_set_slider_value(fd_extrinsicsgui->Z_cam_slider,(double)0.);
  
  fl_set_positioner_xbounds(fd_extrinsicsgui->cam_joystick,-1.,1.);
  fl_set_positioner_xvalue(fd_extrinsicsgui->cam_joystick,(double) 0.);
  fl_set_positioner_ybounds(fd_extrinsicsgui->cam_joystick,-1,1); 
  fl_set_positioner_yvalue(fd_extrinsicsgui->cam_joystick,(double) 0.);

  /* Intrinsic parameters: focal distance,etc */
  fl_set_slider_bounds(fd_extrinsicsgui->focus_slider,(double)FDIST_MAX,(double)FDIST_MIN);
  fl_set_slider_value(fd_extrinsicsgui->focus_slider,(double)ISIGHT_PINHOLE_FDIST);
  fl_set_slider_bounds(fd_extrinsicsgui->u0,(double)SIFNTSC_ROWS,(double)0);
  fl_set_slider_value(fd_extrinsicsgui->u0,(double)0);
  fl_set_slider_bounds(fd_extrinsicsgui->v0,(double)SIFNTSC_COLUMNS,(double)0);
  fl_set_slider_value(fd_extrinsicsgui->v0,(double)0);

  /* Rotation angles */
  fl_set_slider_bounds(fd_extrinsicsgui->roll_slider,180.,-180.);
  fl_set_slider_value(fd_extrinsicsgui->roll_slider,(double)0.);

  set_sliders_position();

  /* calling run functions for colors */
  if(mycolorArun!=NULL){
    mycolorArun(extrinsics_id,NULL,NULL);
  }
}

void extrinsics_show(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)extrinsics_show_aux);
      }
   }
   else{
      fn ((gui_function)extrinsics_show_aux);
   }
}


/* returns lower of the two values */
int lower_of(int x1,int x2){
  if(x1>x2) return x2;
  else return x1;
}

/* returns greater of the two values */
int greater_of(int x1,int x2){
  if(x1>x2) return x1;
  else return x2;
}

int virtual_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
  float r,rold;
  float px,py,pz,lati,longi;

  /* event will capture mouse (x,y) coordenates or adjust camera D radious value (when using mouse wheel) */
  if(event==FL_PUSH){ /* EVENT IS TO CLICK WITH A MOUSE BUTTON */
      
    if((key!=5)&&(key!=4)){ /* ONLY IF CLICKED WITH LEFT, CENTER OR RIGHT MOUSE BUTTON */
      int x,y;
      x=mx-fd_extrinsicsgui->virtual_freeobject->x+1;
      y=my-fd_extrinsicsgui->virtual_freeobject->y+1;
      if(x<0) x=0; if(y<0) y=0;

      /*printf("Click on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
			  
      mouse_pressed=1;
      aux_point_x=x;
      aux_point_y=y;
	
    }else if((key==4)||(key==5)){ /* ONLY IF MOUSE WHEEL MOVED */
      
      px=(float) mycameras[vcam].position.X;
      pz=(float) mycameras[vcam].position.Z;
      py=(float) mycameras[vcam].position.Y;
      r=(float)sqrt((double)(px*px+py*py+pz*pz));
      rold=r;
      if(key==4) r=r-500;
      else if(key==5) r=r+500;
      if(r<=0.) r=0.;
      else if(r>=MAX_Z) r=MAX_Z;
     
      mycameras[vcam].position.X=px*rold/r;
      mycameras[vcam].position.Y=py*rold/r;
      mycameras[vcam].position.Z=pz*rold/r;
      mycameras[vcam].position.H=1.;
      update_camera_matrix(&mycameras[vcam]);
    }
    
    /* event will capture mouse (x,y) coordenates and adjust camera D position and focus, or drawing a rectangle (when center button is hold) */
  }else if(event==FL_MOUSE){ /* LEFT OR RIGHT MOUSE IS BEING HOLD AND MOUSE MOVED */
    int x,y,diff_x,diff_y;
    
    x=mx-fd_extrinsicsgui->virtual_freeobject->x+1;
    y=my-fd_extrinsicsgui->virtual_freeobject->y+1;
    if(x<0) x=0; if(y<0) y=0;
    
    diff_x=aux_point_x-x;
    diff_y=aux_point_y-y;
    aux_point_x=x;
    aux_point_y=y;
    /*printf("Moved (%d,%d)\n",diff_x,diff_y);*/
    
    if(key==1){ /* LEFT MOUSE IS BEING HOLD AND MOUSE MOVED */
      int y,z;
      py=(float) mycameras[vcam].position.Y;
      pz=(float) mycameras[vcam].position.Z;
      py=py+(int)diff_x*100;
      pz=pz+(int)diff_y*(-100);
      if(py>=MAX_Z) py=MAX_Z;
      else if(py<=-MAX_Z) py=-MAX_Z;
      if(pz>=MAX_Z) pz=MAX_Z;
      else if(pz<=-MAX_Z) pz=-MAX_Z;
      px=(float) mycameras[vcam].position.X;
      r=(float)sqrt((double)(px*px+py*py+pz*pz));
      lati=(float)asin(pz/r)*360./(2.*PI);
      longi=(float)atan2((float)py,(float)px)*360./(2.*PI);

      y=(float) mycameras[vcam].foa.Y;
      z=(float) mycameras[vcam].foa.Z;
      y=y+(int)diff_x*(-100);
      z=z+(int)diff_y*100;
      if(y>=MAX_Z) y=MAX_Z;
      else if(y<=-MAX_Z) y=-MAX_Z;
      if(z>=MAX_Z) z=MAX_Z;
      else if(z<=-MAX_Z) z=-MAX_Z;
      
      mycameras[vcam].foa.Y=y;
      mycameras[vcam].foa.Z=z;
      mycameras[vcam].foa.H=1.;
      
      mycameras[vcam].position.X=px;
      mycameras[vcam].position.Y=py;
      mycameras[vcam].position.Z=pz;
      mycameras[vcam].position.H=1.;
      update_camera_matrix(&mycameras[vcam]); 

    }else if(key==3){ /* RIGHT BUTTON IS BEING HOLD AND MOUSE MOVED */
      int y,z;
      y=(float) mycameras[vcam].foa.Y;
      z=(float) mycameras[vcam].foa.Z;
      y=y+(int)diff_x*25;
      z=z+(int)diff_y*(-25);
      if(y>=MAX_Z) y=MAX_Z;
      else if(y<=-MAX_Z) y=-MAX_Z;
      if(z>=MAX_Z) z=MAX_Z;
      else if(z<=-MAX_Z) z=-MAX_Z;
      
      mycameras[vcam].foa.Y=y;
      mycameras[vcam].foa.Z=z;
      mycameras[vcam].foa.H=1.;
      update_camera_matrix(&mycameras[vcam]);

    }else if(key==2){ /* CENTER BUTTON IS BEING HOLD AND MOUSE MOVED */
      float aux_roll;
      aux_roll=(float)mycameras[vcam].roll;
      aux_roll=aux_roll+(((float)((int)(diff_x))/25));
      if(aux_roll>=180.) aux_roll=180.;
      else if(aux_roll<=-180.) aux_roll=-180.;
      
      mycameras[vcam].roll=aux_roll;
      update_camera_matrix(&mycameras[vcam]);
    }
    
  }else if((event==FL_RELEASE)&&(mouse_pressed==1)){  /*MOUSE BUTTON RELEASED */
    mouse_pressed=0;
  }

  return 0;
}

int color_handle(FL_OBJECT *obj, int event, FL_Coord mx, FL_Coord my,int key, void *xev){
 float deltaX,deltaY,deltaZ;
 HPoint3D newpos,ZaxisRel;

  /*event was making click with a mouse button*/
  if(event==FL_PUSH){
    
    /*key 5 and key 4 are mouse wheel buttons*/
    if((key!=5)&&(key!=4)){

            unsigned int x,y;
      /*if(flip_images==0){
	x=mx-fd_extrinsicsgui->color_freeobject->x+1;
	y=my-fd_extrinsicsgui->color_freeobject->y+1;
      }else{
	x=SIFNTSC_COLUMNS-1-(mx-fd_extrinsicsgui->color_freeobject->x+1);
	y=SIFNTSC_ROWS-1-(my-fd_extrinsicsgui->color_freeobject->y+1);
      }
      */
      /*printf("Click on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
      mouse_pressed=1;
      aux_point_x=x;
      aux_point_y=y;
      
    }else if((key==4)||(key==5)){
     
      if (key==5) ZaxisRel.Z=-50; /* wheel, up */
      else if (key==4) ZaxisRel.Z=50; /* wheel, down */
      ZaxisRel.X=0.; ZaxisRel.Y=0.; ZaxisRel.H=1.; 
      relativas2absolutas(ZaxisRel,&newpos);
      deltaX=newpos.X-mycameras[mycam].position.X;
      deltaY=newpos.Y-mycameras[mycam].position.Y;
      deltaZ=newpos.Z-mycameras[mycam].position.Z;
     
      mycameras[mycam].position.X+=deltaX;
      mycameras[mycam].position.Y+=deltaY;
      mycameras[mycam].position.Z+=deltaZ;
      mycameras[mycam].position.H=1.;
      mycameras[mycam].foa.X+=deltaX;
      mycameras[mycam].foa.Y+=deltaY;
      mycameras[mycam].foa.Z+=deltaZ;
      mycameras[mycam].foa.H=1.;
      update_camera_matrix(&mycameras[mycam]);
      set_sliders_position();     
    }

  }else if((event==FL_MOUSE)&&(mouse_pressed==1)&&((key==1)||(key==3))) /*El evento es mantener pulsado boton del raton*/
    {
      unsigned int diff_x,diff_y;
      
      unsigned int x,y;
      if(flip_images==0){
	x=mx-fd_extrinsicsgui->color_freeobject->x+1;
	y=my-fd_extrinsicsgui->color_freeobject->y+1;
      }else{
	x=SIFNTSC_COLUMNS-1-(mx-fd_extrinsicsgui->color_freeobject->x+1);
	y=SIFNTSC_ROWS-1-(my-fd_extrinsicsgui->color_freeobject->y+1);
      }
      
      diff_x=aux_point_x-x;
      diff_y=aux_point_y-y;
      aux_point_x=x;
      aux_point_y=y;
					
      /*printf("Moved (%d,%d)\n",diff_x,diff_y);*/
					
      /*left button*/
      if(key==1)
	{
	  /*  int y,z;
	  py=(float) fl_get_slider_value(fd_extrinsicsgui->y_cam_slider);
	  pz=(float) fl_get_slider_value(fd_extrinsicsgui->z_cam_slider);
	  py=py+(int)diff_x*(-100);
	  pz=pz+(int)diff_y*100;
	  if(py>=MAX_Z) py=MAX_Z;
	  else if(py<=-MAX_Z) py=-MAX_Z;
	  if(pz>=MAX_Z) pz=MAX_Z;
	  else if(pz<=-MAX_Z) pz=-MAX_Z;
	  px=(float) fl_get_slider_value(fd_extrinsicsgui->x_cam_slider);
	  r=(float)sqrt((double)(px*px+py*py+pz*pz));
	  lati=(float)asin(pz/r)*360./(2.*PI);
	  longi=(float)atan2((float)py,(float)px)*360./(2.*PI);
	  fl_set_slider_value(fd_extrinsicsgui->y_cam_slider,(double)py);
	  fl_set_slider_value(fd_extrinsicsgui->z_cam_slider,(double)pz);
	  fl_set_positioner_xvalue(fd_extrinsicsgui->cam_joystick,(double) longi);
	  fl_set_positioner_yvalue(fd_extrinsicsgui->cam_joystick,(double) lati);
	  fl_set_slider_value(fd_extrinsicsgui->radious_slider,(double)r);


	  y=(float) fl_get_slider_value(fd_extrinsicsgui->Y_cam_slider);
	  z=(float) fl_get_slider_value(fd_extrinsicsgui->Z_cam_slider);
	  y=y+(int)diff_x*(-100);
	  z=z+(int)diff_y*100;
	  if(y>=MAX_Z) y=MAX_Z;
	  else if(y<=-MAX_Z) y=-MAX_Z;
	  if(z>=MAX_Z) z=MAX_Z;
	  else if(z<=-MAX_Z) z=-MAX_Z;
						
	  fl_set_slider_value(fd_extrinsicsgui->Y_cam_slider,(double)y);
	  fl_set_slider_value(fd_extrinsicsgui->Z_cam_slider,(double)z);
	  */  
	  /*mycameras[mycam].foa.Y=y;
	    mycameras[mycam].foa.Z=z;
	    mycameras[mycam].foa.H=1.;*/
	  /*  
	  mycameras[mycam].position.X=px;
	  mycameras[mycam].position.Y=py;
	  mycameras[mycam].position.Z=pz;
	  printf("Camera Pos: (X,Y,Z)=(%.1f,%.1f,%.1f)\n",px,py,pz);
	  mycameras[mycam].position.H=1.;
	  update_camera_matrix(&mycameras[mycam]); 
	  set_sliders_position();
	  */
	}
      else if(key==3) /* right button */
	{
	  /*
	  int y,z;
	  y=(float) fl_get_slider_value(fd_extrinsicsgui->Y_cam_slider);
	  z=(float) fl_get_slider_value(fd_extrinsicsgui->Z_cam_slider);
	  y=y+(int)diff_x*25;
	  z=z+(int)diff_y*(-25);
	  if(y>=MAX_Z) y=MAX_Z;
	  else if(y<=-MAX_Z) y=-MAX_Z;
	  if(z>=MAX_Z) z=MAX_Z;
	  else if(z<=-MAX_Z) z=-MAX_Z;
	  
	  fl_set_slider_value(fd_extrinsicsgui->Y_cam_slider,(double)y);
	  fl_set_slider_value(fd_extrinsicsgui->Z_cam_slider,(double)z);
		  
	  mycameras[mycam].foa.Y=y;
	  mycameras[mycam].foa.Z=z;
	  mycameras[mycam].foa.H=1.;
	  printf("Camera Foa: (X,Y,Z)=(%.1f,%.1f,%.1f)\n",(float)mycameras[mycam].foa.X,(float)y,(float)z);
	  update_camera_matrix(&mycameras[mycam]);
	  set_sliders_position();
	  */
	}
    }
  else if((event==FL_RELEASE)&&(mouse_pressed==1)) /* si se ha soltado el boton del raton */
    {
      if(key==2)
	{
	  unsigned int x,y;
	  if(flip_images==0){
	    x=mx-fd_extrinsicsgui->color_freeobject->x+1;
	    y=my-fd_extrinsicsgui->color_freeobject->y+1;
	  }else{
	    x=SIFNTSC_COLUMNS-1-(mx-fd_extrinsicsgui->color_freeobject->x+1);
	    y=SIFNTSC_ROWS-1-(my-fd_extrinsicsgui->color_freeobject->y+1);
	  }
	  
	  /*printf("Click on (%d,%d) of (%d,%d)\n",x,y,SIFNTSC_COLUMNS,SIFNTSC_ROWS);*/
	  /*lower_x=lower_of(x,aux_point_x);
	  lower_y=lower_of(y,aux_point_y);
	  greater_x=greater_of(x,aux_point_x);
	  greater_y=greater_of(y,aux_point_y);
				  
	  point1=(lower_y*SIFNTSC_COLUMNS+lower_x)*3;
	  point2=(greater_y*SIFNTSC_COLUMNS+lower_x)*3;
	  point3=(lower_y*SIFNTSC_COLUMNS+greater_x)*3;
	  point4=(greater_y*SIFNTSC_COLUMNS+greater_x)*3;
	  */
	  /*printf("P1(%d), P2(%d), P3(%d) and P4(%d). Maximo es 230400\n",point1,point2,point3,point4);*/
	  mouse_pressed=0;
	}
    }
  return 0;
}


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

 * jdec simulated3D driver provides video images to color variables from simulated3D
 * cameras using simulated3D library.
 *
 * This 4.3 version includes support for variable images. 
 *
 * @file simulated3D.c
 * @authors: Sara Marugán Alonso <s.marugan@alumnos.urjc.es>
 * @authors: José María Cañas Plaza <jmplaza@gsyc.es>
 * @version 4.3
 * @date 2009-05-16
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> /* usleep */
#include <math.h> /* sqrt */
#include <string.h>
#include <pthread.h>
#include <progeo.h>
#include <jde.h>
#include <interfaces/varcolor.h>


/** Max number of cameras detected by simulated3D driver.*/
#define MAXCAM 8
/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

/** pthread identifiers for jdec simulated3D driver threads.*/
pthread_t simulated3D_th[MAXCAM];
/** Arguments for simulated3D threads*/
int args[MAXCAM];
/** pthread state variable for jdec simulated3D driver.*/
int state[MAXCAM];
/** pthread mutex for jdec simulated3D driver.*/
pthread_mutex_t mymutex[MAXCAM];
/** pthread condition variable for jdec simulated3D driver.*/
pthread_cond_t condition[MAXCAM];

/** variable to detect when the pthread is created.*/
int simulated3D_thread_created[MAXCAM];
/** variable to detect when simulated3D driver was cleaned up.*/
int simulated3D_cleaned_up=0;
/** variable to detect when simulated3D driver was setup.*/
int simulated3D_setup=0;
/** variable to detect when pthread must end execution.*/
int simulated3D_terminate_command=0;

/* simulated3D driver API options */
/** simulated3D driver name.*/
char driver_name[256]="simulated3D";
/** colors detected in config file.*/
int serve_color[MAXCAM];
/** structure to know what colors are active in the gui.*/
int color_active[MAXCAM];

/** id set to colorA schema.*/
int colorA_schema_id;
/** id set to colorB schema.*/
int colorB_schema_id;
/** id set to colorC schema.*/
int colorC_schema_id;
/** id set to colorD schema.*/
int colorD_schema_id;
/** id for varcolorA schema.*/
int varcolorA_schema_id;
/** id for varcolorB schema.*/
int varcolorB_schema_id;
/** id for varcolorC schema.*/
int varcolorC_schema_id;
/** id for varcolorD schema.*/
int varcolorD_schema_id;

/*API variables servidas*/
/** 'colorA' schema image data*/
char *colorA; /* sifntsc image itself */
/** 'colorA' schema clock*/
unsigned long int imageA_clock;

/** 'colorB' schema image data*/
char *colorB; /* sifntsc image itself */
/** 'colorB' schema clock*/
unsigned long int imageB_clock;

/** 'colorC' schema image data*/
char *colorC; /* sifntsc image itself */
/** 'colorC' schema clock*/
unsigned long int imageC_clock;

/** 'colorD' schema image data*/
char *colorD; /* sifntsc image itself */
/** 'colorD' schema clock*/
unsigned long int imageD_clock;

Varcolor myA,myB,myC,myD;


/*Contadores de referencias*/
/** colorA ref counter*/
int colorA_refs=0;
/** colorB ref counter*/
int colorB_refs=0;
/** colorC ref counter*/
int colorC_refs=0;
/** colorD ref counter*/
int colorD_refs=0;
/** varcolorA ref counter*/
int varcolorA_refs=0;
/** varcolorB ref counter*/
int varcolorB_refs=0;
/** varcolorC ref counter*/
int varcolorC_refs=0;
/** varcolorD ref counter*/
int varcolorD_refs=0;

/** mutex for ref counters*/
pthread_mutex_t refmutex;


/******************************************** simulated variables ***************************************/
/* world file */
char worldfile[512];

/* color */
char readyColorAA[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3]; char notReadyColorAA[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3];
char readyColorBB[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3]; char notReadyColorBB[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3];
char readyColorCC[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3]; char notReadyColorCC[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3];
char readyColorDD[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3]; char notReadyColorDD[SIFNTSC_ROWS*SIFNTSC_COLUMNS*3];
char *notReadyColorSimA; char *notReadyColorSimB; char *notReadyColorSimC; char *notReadyColorSimD;
char *ReadyColorSimA; char *ReadyColorSimB; char *ReadyColorSimC; char *ReadyColorSimD;

/* varcolor */
char *varnotReadyColorAA; char *varnotReadyColorBB; char *varnotReadyColorCC; char *varnotReadyColorDD;
char *varReadyColorAA; char *varReadyColorBB; char *varReadyColorCC; char *varReadyColorDD;
char *varnotReadyColorSimA; char *varnotReadyColorSimB; char *varnotReadyColorSimC; char *varnotReadyColorSimD;
char *varReadyColorSimA; char *varReadyColorSimB; char *varReadyColorSimC; char *varReadyColorSimD;

/* 3d objects */
#define AXIS_Z 1000. /* mm */
#define SLOTS 4
#define MAX_LINES_IN_ROOM 250
#define READYCOLOR 0
float DELTA;
HPoint3D origin;
HPoint3D axisX[2], axisY[2], axisZ[2];
HPoint3D gridYZ[SLOTS*2*2], gridXZ[SLOTS*2*2], gridXY[SLOTS*2*2];
HPoint3D room[MAX_LINES_IN_ROOM*2]; /* mm */
int room_lines=0;

/* declarations for simulated3D cameras */
TPinHoleCamera cameras[MAXCAM];
/** number of simulated3D cameras detected.*/
int camCount=0;
/** int variable to detect when a image was captured.*/
unsigned long int lastimage=0;

/* image visualization and auxiliar 3d coordinates variables*/
int room_on=1;
int camaxis_on=0; 
int grid_on=0; 
int axis_on=0;

/* exported variables */
int simulator3D_cycle=33; /* ms */

/* ------------------------ VISUAL FUNCTIONS ----------------------------------------- */
enum mycolors {MY_BLACK,MY_RED,MY_BLUE,MY_PALEGREEN,MY_WHEAT,MY_DEEPPINK};

int lineinimage(char *img, int xa, int ya, int xb, int yb, int thiscolor, int columns, int rows)
{  
  float L;
  int i,imax,r,g,b;
  int lastx,lasty,thisx,thisy,lastcount;
  int threshold=1;
  int Xmax,Xmin,Ymax,Ymin;
  
  Xmin=0; Xmax=columns-1; Ymin=0; Ymax=rows-1;

  /* In this image always graf coordinates x=horizontal, y=vertical, starting at the top left corner of the image.
     They can't reach 240 or 320, their are not valid values for the pixels. */
  
  if (thiscolor==MY_BLACK) {r=0;g=0;b=0;}
  else if (thiscolor==MY_RED) {r=255;g=0;b=0;} 
  else if (thiscolor==MY_BLUE) {r=0;g=0;b=255;} 
  else if (thiscolor==MY_PALEGREEN) {r=113;g=198;b=113;} 
  else if (thiscolor==MY_WHEAT) {r=255;g=231;b=155;}
  else if (thiscolor==MY_DEEPPINK) {r=213;g=85;b=178; }   
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

int simulator_drawline(HPoint2D p1, HPoint2D p2, char sel, int colour, int image)
/* it takes care of important features: before/behind the focal plane, inside/outside the image frame */
{
  HPoint2D gooda,goodb;

  if(image==READYCOLOR){
    if(sel==0 && displayline(p1,p2,&gooda,&goodb,cameras[0])==1)
	lineinimage(notReadyColorSimA,(int)gooda.y,cameras[0].rows-1-(int)gooda.x,(int)goodb.y,cameras[0].rows-1-(int)goodb.x,colour,cameras[0].columns,cameras[0].rows);
    else if(sel==1 && displayline(p1,p2,&gooda,&goodb,cameras[1])==1) 		
	lineinimage(notReadyColorSimB,(int)gooda.y,cameras[1].rows-1-(int)gooda.x,(int)goodb.y,cameras[1].rows-1-(int)goodb.x,colour,cameras[1].columns,cameras[1].rows);
    else if(sel==2 && displayline(p1,p2,&gooda,&goodb,cameras[2])==1)		
	lineinimage(notReadyColorSimC,(int)gooda.y,cameras[2].rows-1-(int)gooda.x,(int)goodb.y,cameras[2].rows-1-(int)goodb.x,colour,cameras[2].columns,cameras[2].rows);
    else if(sel==3 && displayline(p1,p2,&gooda,&goodb,cameras[3])==1)
 	lineinimage(notReadyColorSimD,(int)gooda.y,cameras[3].rows-1-(int)gooda.x,(int)goodb.y,cameras[3].rows-1-(int)goodb.x,colour,cameras[3].columns,cameras[3].rows);

    else if(sel==4 && displayline(p1,p2,&gooda,&goodb,cameras[4])==1)
	lineinimage(varnotReadyColorSimA,(int)gooda.y,cameras[4].rows-1-(int)gooda.x,(int)goodb.y,cameras[4].rows-1-(int)goodb.x,colour,cameras[4].columns,cameras[4].rows);
    else if(sel==5 && displayline(p1,p2,&gooda,&goodb,cameras[5])==1) 		
	lineinimage(varnotReadyColorSimB,(int)gooda.y,cameras[5].rows-1-(int)gooda.x,(int)goodb.y,cameras[5].rows-1-(int)goodb.x,colour,cameras[5].columns,cameras[5].rows);
    else if(sel==6 && displayline(p1,p2,&gooda,&goodb,cameras[6])==1) 		
	lineinimage(varnotReadyColorSimC,(int)gooda.y,cameras[6].rows-1-(int)gooda.x,(int)goodb.y,cameras[6].rows-1-(int)goodb.x,colour,cameras[6].columns,cameras[6].rows);
    else if(sel==7 && displayline(p1,p2,&gooda,&goodb,cameras[7])==1)
 	lineinimage(varnotReadyColorSimD,(int)gooda.y,cameras[7].rows-1-(int)gooda.x,(int)goodb.y,cameras[7].rows-1-(int)goodb.x,colour,cameras[7].columns,cameras[7].rows);
  };
 return 0;
}


/* virtual object generator, double buffer for dynamic objects. The physic dynamics equations must be here for each object */
void object_generator(int index)
{
  int i;
  HPoint2D a,b;
  char *auxsim;
  
	    /*white background*/
	    switch(index){
		case 0: 
		    	reset_buffer(notReadyColorSimA,(double)255,(double)255,(double)255,cameras[0].columns,cameras[0].rows);
			break;
		case 1: 
		    	reset_buffer(notReadyColorSimB,(double)255,(double)255,(double)255,cameras[1].columns,cameras[1].rows);
			break;
		case 2: 
		    	reset_buffer(notReadyColorSimC,(double)255,(double)255,(double)255,cameras[2].columns,cameras[2].rows);
			break;
		case 3: 
		    	reset_buffer(notReadyColorSimD,(double)255,(double)255,(double)255,cameras[3].columns,cameras[3].rows);
			break;
		case 4: 
		    	reset_buffer(varnotReadyColorSimA,(double)255,(double)255,(double)255,cameras[4].columns,cameras[4].rows);
			break;
		case 5: 
		    	reset_buffer(varnotReadyColorSimB,(double)255,(double)255,(double)255,cameras[5].columns,cameras[5].rows);
			break;
		case 6: 
		    	reset_buffer(varnotReadyColorSimC,(double)255,(double)255,(double)255,cameras[6].columns,cameras[6].rows);
			break;
		case 7: 
		    	reset_buffer(varnotReadyColorSimD,(double)255,(double)255,(double)255,cameras[7].columns,cameras[7].rows);
			break;
	    }
	    
	    /*grid in image*/
  	    if(grid_on==1){
	      for(i=0;i<SLOTS;i++){
		project(gridXY[2*i],&a,cameras[index]);
		project(gridXY[2*i+1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_RED,READYCOLOR);
		project(gridXY[2*SLOTS+2*i],&a,cameras[index]);
		project(gridXY[2*SLOTS+2*i+1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_RED,READYCOLOR);
	      }
	      
	      for(i=0;i<SLOTS;i++){
		project(gridYZ[2*i],&a,cameras[index]);
		project(gridYZ[2*i+1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_PALEGREEN,READYCOLOR);
		project(gridYZ[2*SLOTS+2*i],&a,cameras[index]);
		project(gridYZ[2*SLOTS+2*i+1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_PALEGREEN,READYCOLOR);
	      }
		
	      for(i=0;i<SLOTS;i++){
		project(gridXZ[2*i],&a,cameras[index]);
		project(gridXZ[2*i+1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_BLUE,READYCOLOR);
		project(gridXZ[2*SLOTS+2*i],&a,cameras[index]);
		project(gridXZ[2*SLOTS+2*i+1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_BLUE,READYCOLOR);
	      }				 	
	    }
		      
	    /* drawing of X-Y-Z axis*/
	    if ((axis_on==1)||(grid_on==1)){
		project(axisX[0],&a,cameras[index]);
		project(axisX[1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_PALEGREEN,READYCOLOR);
	
		project(axisY[0],&a,cameras[index]);
		project(axisY[1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_BLUE,READYCOLOR);
	
		project(axisZ[0],&a,cameras[index]);
		project(axisZ[1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_RED,READYCOLOR);
	    }
	    
	    /* camera2, camera3, camera4 and virtual camera seen in camera */
	    if (camaxis_on==1){
	        project(cameras[1].position,&a,cameras[index]);
	        project(cameras[1].foa,&b,cameras[index]);
	        simulator_drawline(a,b,index,MY_WHEAT,READYCOLOR);      
	        project(cameras[2].position,&a,cameras[index]);
	        project(cameras[2].foa,&b,cameras[index]);
	        simulator_drawline(a,b,index,MY_WHEAT,READYCOLOR);      
	        project(cameras[3].position,&a,cameras[index]);
	        project(cameras[3].foa,&b,cameras[index]);
	        simulator_drawline(a,b,index,MY_WHEAT,READYCOLOR);
	        project(cameras[4].position,&a,cameras[index]);
	        project(cameras[4].foa,&b,cameras[index]);
	        simulator_drawline(a,b,index,MY_WHEAT,READYCOLOR);
	    }
	    
	    /* room in camera */
	    if(room_on==1){
	      for(i=0;i<room_lines;i++){
		project(room[i*2+0],&a,cameras[index]);
		project(room[i*2+1],&b,cameras[index]);
		simulator_drawline(a,b,index,MY_BLACK,READYCOLOR);
	      }
	    }
	    
	    switch(index){
		case 0: 
		    	auxsim=notReadyColorSimA; notReadyColorSimA=ReadyColorSimA; ReadyColorSimA=auxsim;
			break;
		case 1: 
		    	auxsim=notReadyColorSimB; notReadyColorSimB=ReadyColorSimB; ReadyColorSimB=auxsim;
			break;
		case 2: 
		    	auxsim=notReadyColorSimC; notReadyColorSimC=ReadyColorSimC; ReadyColorSimC=auxsim;
			break;
		case 3: 
		    	auxsim=notReadyColorSimD; notReadyColorSimD=ReadyColorSimD; ReadyColorSimD=auxsim;
			break;
		case 4: 
		    	auxsim=varnotReadyColorSimA; varnotReadyColorSimA=varReadyColorSimA; varReadyColorSimA=auxsim;
			break;
		case 5: 
		    	auxsim=varnotReadyColorSimB; varnotReadyColorSimB=varReadyColorSimB; varReadyColorSimB=auxsim;
			break;
		case 6: 
		    	auxsim=varnotReadyColorSimC; varnotReadyColorSimC=varReadyColorSimC; varReadyColorSimC=auxsim;
			break;
		case 7: 
		    	auxsim=varnotReadyColorSimD; varnotReadyColorSimD=varReadyColorSimD; varReadyColorSimD=auxsim;
			break;
  	   }
}


/* ------------------------------------------------------------- AUXILIAR SCHEMA FUNCTIONS ------------------------------------------------------------- */

/* gets the calibration of the camera from a file */
int load_cam_line(FILE *myfile,int cam)
{		
  char word1[MAX_BUFFER],word2[MAX_BUFFER];
  int i=0;
  char buffer_file[MAX_BUFFER];   

  buffer_file[0]=fgetc(myfile);
  if (feof(myfile)) return EOF;
  if (buffer_file[0]==(char)255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  /* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
  while((buffer_file[i]!='\n') && 
	(buffer_file[i] != (char)255) &&  
	(i<MAX_BUFFER-1) ) {
    buffer_file[++i]=fgetc(myfile);
  }
  
  if (i >= MAX_BUFFER-1) { 
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
      cameras[cam].position.X=(float)atof(word2);
    }
    else if (strcmp(word1,"positionY")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].position.Y=(float)atof(word2);
    }
    else if (strcmp(word1,"positionZ")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].position.Z=(float)atof(word2);
    }
    else if (strcmp(word1,"positionH")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].position.H=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionX")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].foa.X=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionY")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].foa.Y=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionZ")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].foa.Z=(float)atof(word2);
    }
    else if (strcmp(word1,"FOApositionH")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].foa.H=(float)atof(word2);
    }
    else if (strcmp(word1,"roll")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].roll=(float)atof(word2);
    }
    else if (strcmp(word1,"f")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].fdistx=(float)atof(word2);
      cameras[cam].fdisty=(float)atof(word2);
     }
    else if (strcmp(word1,"fx")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].fdistx=(float)atof(word2);
    }
    else if (strcmp(word1,"fy")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].fdisty=(float)atof(word2);
     }
    else if (strcmp(word1,"skew")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].skew=(float)atof(word2);
     }
    else if (strcmp(word1,"u0")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].u0=(float)atof(word2);
    }
    else if (strcmp(word1,"v0")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].v0=(float)atof(word2);
    } 
    else if (strcmp(word1,"rows")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].rows=(int)atoi(word2);
    } 
    else if (strcmp(word1,"columns")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].columns=(int)atoi(word2);
    } 
    
  }
 return 1;
}

/* gets the calibration of the camera from a file */
void load_cam(char *fich_in,int cam)
{
  FILE *entrada;
  int i;

  entrada=fopen(fich_in,"r");
   if(entrada==NULL){
     printf("simulated3D: camera input calibration file %s does not exits\n",fich_in);
   }else{
     do{i=load_cam_line(entrada,cam);}while(i!=EOF);
   } 
  fclose(entrada);
  update_camera_matrix(&cameras[cam]);
  /*mydebug();*/
}

/**
 * It reads a single line from config file, parses it and do the right thing.
 * @param myfile The config file descriptor.
 * @returns EOF in detects end of such file. Otherwise returns 0.
 */
int load_worldline(FILE *myfile)
{		
  char word1[MAX_BUFFER],word2[MAX_BUFFER],word3[MAX_BUFFER],word4[MAX_BUFFER],word5[MAX_BUFFER];
  char word6[MAX_BUFFER],word7[MAX_BUFFER],word8[MAX_BUFFER];
  char word[MAX_BUFFER];
  int i=0;
  char buffer_file[MAX_BUFFER];  
  char id; 

  buffer_file[0]=fgetc(myfile);
  if (feof(myfile)) return EOF;
  if (buffer_file[0]==(char)255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  /* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
  while((buffer_file[i]!='\n') && 
	(buffer_file[i] != (char)255) &&  
	(i<MAX_BUFFER-1) ) {
    buffer_file[++i]=fgetc(myfile);
  }
  
  if (i >= MAX_BUFFER-1) { 
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
       /* camera parameters are provided at individual camera files, not at the world file */
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

/* initializes all 3D objects */
int tridimensional_objects_init()
{
  int i=0;
  printf("Creating 3d objects...\n");

  /* points and lines in the 3D world */
  /* axis in 3D */
  axisX[0].X=0;
  axisX[0].Y=0;
  axisX[0].Z=0;
  axisX[0].H=1.;
  axisX[1].X=AXIS_Z;
  axisX[1].Y=0;
  axisX[1].Z=0;
  axisX[1].H=1.;
  
  axisY[0].X=0;
  axisY[0].Y=0;
  axisY[0].Z=0;
  axisY[0].H=1.;
  axisY[1].X=0.;
  axisY[1].Y=AXIS_Z;
  axisY[1].Z=0;
  axisY[1].H=1.;
  
  axisZ[0].X=0;
  axisZ[0].Y=0;
  axisZ[0].Z=0;
  axisZ[0].H=1.;
  axisZ[1].X=0.;
  axisZ[1].Y=0.;
  axisZ[1].Z=AXIS_Z;
  axisZ[1].H=1.;
  
  DELTA=AXIS_Z/SLOTS;
  /* XYgrid, XZgrid, YZgrid in 3D */
  for(i=0;i<SLOTS;i++){     
    gridXY[2*i].X=DELTA*(i+1); 
    gridXY[2*i].Y=0; 
    gridXY[2*i].Z=0; 
    gridXY[2*i].H=1;
    gridXY[2*i+1].X=DELTA*(i+1); 
    gridXY[2*i+1].Y=AXIS_Z; 
    gridXY[2*i+1].Z=0; 
    gridXY[2*i+1].H=1;
    
    gridXY[2*SLOTS+2*i].X=0; 
    gridXY[2*SLOTS+2*i].Y=DELTA*(i+1);
    gridXY[2*SLOTS+2*i].Z=0;
    gridXY[2*SLOTS+2*i].H=1;
    gridXY[2*SLOTS+2*i+1].X=AXIS_Z;
    gridXY[2*SLOTS+2*i+1].Y=DELTA*(i+1);
    gridXY[2*SLOTS+2*i+1].Z=0;
    gridXY[2*SLOTS+2*i+1].H=1;
    
    gridYZ[2*i].X=0;
    gridYZ[2*i].Y=DELTA*(i+1);
    gridYZ[2*i].Z=0;
    gridYZ[2*i].H=1;
    gridYZ[2*i+1].X=0;
    gridYZ[2*i+1].Y=DELTA*(i+1);
    gridYZ[2*i+1].Z=AXIS_Z;
    gridYZ[2*i+1].H=1;
    
    gridYZ[2*SLOTS+2*i].X=0;
    gridYZ[2*SLOTS+2*i].Y=0;
    gridYZ[2*SLOTS+2*i].Z=DELTA*(i+1);
    gridYZ[2*SLOTS+2*i].H=1;
    gridYZ[2*SLOTS+2*i+1].X=0;
    gridYZ[2*SLOTS+2*i+1].Y=AXIS_Z;
    gridYZ[2*SLOTS+2*i+1].Z=DELTA*(i+1);
    gridYZ[2*SLOTS+2*i+1].H=1;
    
    gridXZ[2*i].X=DELTA*(i+1);
    gridXZ[2*i].Y=0;
    gridXZ[2*i].Z=0;
    gridXZ[2*i].H=1;
    gridXZ[2*i+1].X=DELTA*(i+1);
    gridXZ[2*i+1].Y=0;
    gridXZ[2*i+1].Z=AXIS_Z;
    gridXZ[2*i+1].H=1;
    
    gridXZ[2*SLOTS+2*i].X=0;
    gridXZ[2*SLOTS+2*i].Y=0;
    gridXZ[2*SLOTS+2*i].Z=DELTA*(i+1);
    gridXZ[2*SLOTS+2*i].H=1;
    gridXZ[2*SLOTS+2*i+1].X=AXIS_Z;
    gridXZ[2*SLOTS+2*i+1].Y=0;
    gridXZ[2*SLOTS+2*i+1].Z=DELTA*(i+1);
    gridXZ[2*SLOTS+2*i+1].H=1;
  }

  printf("3d objects created\n");
  return 0;
}
/*--fin 3d init-**/



/************************************** simulated3D DRIVER FUNCTIONS ****************************/

/** simulated3D driver closing function invoked when stopping driver.*/
void simulated3D_terminate(){
  simulated3D_terminate_command=1;
  free(varReadyColorAA); 
  free(varnotReadyColorAA);
  free(varReadyColorBB); 
  free(varnotReadyColorBB);
  free(varReadyColorCC); 
  free(varnotReadyColorCC);
  free(varReadyColorDD); 
  free(varnotReadyColorDD);

  printf("driver simulated3D off\n");
}

/** colorA run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorA_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (colorA_refs>0){
      colorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[0]==1)&&(color_active[0]==0)){
         color_active[0]=1;
         printf("colorA schema run (simulated3D driver)\n");
         all[colorA_schema_id].father = father;
         all[colorA_schema_id].fps = 0.;
         all[colorA_schema_id].k =0;
         all[colorA_schema_id].k =0;
         put_state(colorA_schema_id,winner);
         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[0]);
         state[0]=winner;
         pthread_cond_signal(&condition[0]);
         pthread_mutex_unlock(&mymutex[0]);
      }
   }
   return 0;
}

/** colorA stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorA_stop(){

   pthread_mutex_lock(&refmutex);
   if (colorA_refs>1){
      colorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[0]==1)&&(color_active[0]==1)){
         color_active[0]=0;
         put_state(colorA_schema_id,slept);
         printf("colorA schema stop (simulated3D driver)\n");
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[0]);
         state[0]=slept;
         pthread_mutex_unlock(&mymutex[0]);
      }
   }
   return 0;
}

/** colorB run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorB_run(int father, int *brothers, arbitration fn){

   pthread_mutex_lock(&refmutex);
   if (colorB_refs>0){
      colorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[1]==1)&&(color_active[1]==0)){
         color_active[1]=1;
         printf("colorB schema run (simulated3D driver)\n");
         all[colorB_schema_id].father = father;
         all[colorB_schema_id].fps = 0.;
         all[colorB_schema_id].k =0;
   
         put_state(colorB_schema_id,winner);

         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[1]);
         state[1]=winner;
         pthread_cond_signal(&condition[1]);
         pthread_mutex_unlock(&mymutex[1]);
      }
   }
   return 0;
}

/** colorB stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorB_stop(){

   pthread_mutex_lock(&refmutex);
   if (colorB_refs>1){
      colorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[1]==1)&&(color_active[1]==1)){
         color_active[1]=0;
         printf("colorB schema stop (simulated3D driver)\n");
         put_state(colorB_schema_id,slept);
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[1]);
         state[1]=slept;
         pthread_mutex_unlock(&mymutex[1]);
      }
   }
   return 0;
}

/** colorC run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorC_run(int father, int *brothers, arbitration fn){

   pthread_mutex_lock(&refmutex);
   if (colorC_refs>0){
      colorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[2]==1)&&(color_active[2]==0)){
         color_active[2]=1;
         printf("colorC schema run (simulated3D driver)\n");
         all[colorC_schema_id].father = father;
         all[colorC_schema_id].fps = 0.;
         all[colorC_schema_id].k =0;
         put_state(colorC_schema_id,winner);
         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[2]);
         state[2]=winner;
         pthread_cond_signal(&condition[2]);
         pthread_mutex_unlock(&mymutex[2]);
      }
   }
   return 0;
}

/** colorC stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorC_stop(){
   
   pthread_mutex_lock(&refmutex);
   if (colorC_refs>1){
      colorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[2]==1)&&(color_active[2]==1)){
         color_active[2]=0;
         printf("colorC schema stop (simulated3D driver)\n");
         put_state(colorC_schema_id,slept);
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[2]);
         state[2]=slept;
         pthread_mutex_unlock(&mymutex[2]);
      }
   }
   return 0;
}

/** colorD run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorD_run(int father, int *brothers, arbitration fn){

   pthread_mutex_lock(&refmutex);
   if (colorD_refs>0){
      colorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[3]==1)&&(color_active[3]==0)){
         color_active[3]=1;
         printf("colorD schema run (simulated3D driver)\n");
         all[colorD_schema_id].father = father;
         all[colorD_schema_id].fps = 0.;
         all[colorD_schema_id].k =0;
         put_state(colorD_schema_id,winner);
         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[3]);
         state[3]=winner;
         pthread_cond_signal(&condition[3]);
         pthread_mutex_unlock(&mymutex[3]);
      }
   }
   return 0;
}

/** colorD stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int mycolorD_stop(){
   
   pthread_mutex_lock(&refmutex);
   if (colorD_refs>1){
      colorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[3]==1)&&(color_active[3]==1)){
         color_active[3]=0;
         printf("colorD schema stop (simulated3D driver)\n");
         put_state(colorD_schema_id,slept);
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[3]);
         state[3]=slept;
         pthread_mutex_unlock(&mymutex[3]);
      }
   }
   return 0;
}

/** varcolorA run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorA_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>0){
      varcolorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[4]==1)&&(color_active[4]==0)){
         color_active[4]=1;
         printf("varcolorA schema run (simulated3D driver)\n");
         all[varcolorA_schema_id].father = father;
         all[varcolorA_schema_id].fps = 0.;
         all[varcolorA_schema_id].k =0;
         put_state(varcolorA_schema_id,winner);
         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[4]);
         state[4]=winner;
         pthread_cond_signal(&condition[4]);
         pthread_mutex_unlock(&mymutex[4]);
      }
   }
   return 0;
}

/** varcolorA stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorA_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>1){
      varcolorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[4]==1)&&(color_active[4]==1)){
         color_active[4]=0;
         printf("varcolorA schema stop (simulated3D driver)\n");
         put_state(varcolorA_schema_id,slept);
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[4]);
         state[4]=slept;
         pthread_mutex_unlock(&mymutex[4]);
      }
   }
   return 0;
}

/** varcolorB run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorB_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>0){
      varcolorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[5]==1)&&(color_active[5]==0)){
         color_active[5]=1;
         printf("varcolorB schema run (simulated3D driver)\n");
         all[varcolorB_schema_id].father = father;
         all[varcolorB_schema_id].fps = 0.;
         all[varcolorB_schema_id].k =0;
         put_state(varcolorB_schema_id,winner);
         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[5]);
         state[5]=winner;
         pthread_cond_signal(&condition[5]);
         pthread_mutex_unlock(&mymutex[5]);
      }
   }
   return 0;
}

/** varcolorB stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorB_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>1){
      varcolorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[5]==1)&&(color_active[5]==1)){
         color_active[5]=0;
         printf("varcolorB schema stop (simulated3D driver)\n");
         put_state(varcolorB_schema_id,slept);
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[5]);
         state[5]=slept;
         pthread_mutex_unlock(&mymutex[5]);
      }
   }
   return 0;
}

/** varcolorC run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorC_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>0){
      varcolorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[6]==1)&&(color_active[6]==0)){
         color_active[6]=1;
         printf("varcolorC schema run (simulated3D driver)\n");
         all[varcolorC_schema_id].father = father;
         all[varcolorC_schema_id].fps = 0.;
         all[varcolorC_schema_id].k =0;
         put_state(varcolorC_schema_id,winner);
         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[6]);
         state[6]=winner;
         pthread_cond_signal(&condition[6]);
         pthread_mutex_unlock(&mymutex[6]);
      }
   }
   return 0;
}

/** varcolorC stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorC_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>1){
      varcolorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[6]==1)&&(color_active[6]==1)){
         color_active[6]=0;
         printf("varcolorC schema stop (simulated3D driver)\n");
         put_state(varcolorC_schema_id,slept);
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[6]);
         state[6]=slept;
         pthread_mutex_unlock(&mymutex[6]);
      }
   }
   return 0;
}

/** varcolorD run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorD_run(int father, int *brothers, arbitration fn){
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>0){
      varcolorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[7]==1)&&(color_active[7]==0)){
         color_active[7]=1;
         printf("varcolorD schema run (simulated3D driver)\n");
         all[varcolorD_schema_id].father = father;
         all[varcolorD_schema_id].fps = 0.;
         all[varcolorD_schema_id].k =0;
         put_state(varcolorD_schema_id,winner);
         /* simulated3D thread goes winner */
         pthread_mutex_lock(&mymutex[7]);
         state[7]=winner;
         pthread_cond_signal(&condition[7]);
         pthread_mutex_unlock(&mymutex[7]);
      }
   }
   return 0;
}

/** varcolorD stop function following jdec platform API schemas.
 *  @return integer stoping result.*/
int myvarcolorD_stop(){
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>1){
      varcolorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      if((serve_color[7]==1)&&(color_active[7]==1)){
         color_active[7]=0;
         printf("varcolorD schema stop (simulated3D driver)\n");
         put_state(varcolorD_schema_id,slept);
         /* simulated3D thread goes sleep */
         pthread_mutex_lock(&mymutex[7]);
         state[7]=slept;
         pthread_mutex_unlock(&mymutex[7]);
      }
   }
   return 0;
}

/** simulated3D driver pthread function.*/
void *simulated3D_thread(void *id)
{
   int i;
   i=*((int*)id);

 
  /* MAIN LOOP */
  while(simulated3D_terminate_command==0){
        
    pthread_mutex_lock(&mymutex[i]);

    if (state[i]==slept){
      printf("simulated3D thread in sleep mode\n");
      pthread_cond_wait(&condition[i],&mymutex[i]);
      printf("simulated3D thread woke up\n");
      pthread_mutex_unlock(&mymutex[i]);
      
    }else{      
      pthread_mutex_unlock(&mymutex[i]);

      object_generator(i);

      if(serve_color[0]==1 && i==0){
		memcpy(colorA,ReadyColorSimA,cameras[0].columns*cameras[0].rows*3);
		speedcounter(colorA_schema_id);
		imageA_clock=lastimage;
      }
      else if(serve_color[1]==1 && i==1){
		memcpy(colorB,ReadyColorSimB,cameras[1].columns*cameras[1].rows*3);
		speedcounter(colorB_schema_id);
		imageB_clock=lastimage;
      }
      else if(serve_color[2]==1 && i==2){
		memcpy(colorC,ReadyColorSimC,cameras[2].columns*cameras[2].rows*3);
		speedcounter(colorC_schema_id);
		imageC_clock=lastimage;
      }
      else if(serve_color[3]==1 && i==3){
		memcpy(colorD,ReadyColorSimD,cameras[3].columns*cameras[3].rows*3);
		speedcounter(colorD_schema_id);
		imageD_clock=lastimage;
      }
      else if(serve_color[4]==1 && i==4){
		memcpy(myA.img,varReadyColorSimA,cameras[4].columns*cameras[4].rows*3);
		speedcounter(varcolorA_schema_id);
		myA.clock=lastimage;
      }
      else if(serve_color[5]==1 && i==5){
		memcpy(myB.img,varReadyColorSimB,cameras[5].columns*cameras[5].rows*3);
		speedcounter(varcolorB_schema_id);
		myB.clock=lastimage;
      }
      else if(serve_color[6]==1 && i==6){
		memcpy(myC.img,varReadyColorSimC,cameras[6].columns*cameras[6].rows*3);
		speedcounter(varcolorC_schema_id);
		myC.clock=lastimage;
      }
      else if(serve_color[7]==1 && i==7){
		memcpy(myD.img,varReadyColorSimD,cameras[7].columns*cameras[7].rows*3);
		speedcounter(varcolorD_schema_id);
		myD.clock=lastimage;
      }
      lastimage++;

      usleep(simulator3D_cycle*1000);

    }// end else
  }// end while

  pthread_exit(0);
}


/** simulated3D driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int simulated3D_parseconf(char *configfile){

   int end_parse=0; int end_section=0; int driver_config_parsed=0;
   FILE *myfile;
   FILE *camroomconfig;
   /* camera configfiles */
   char cameraINfile[MAX_BUFFER];

   if ((myfile=fopen(configfile,"r"))==NULL){
      printf("simulated3D: cannot find config file\n");
      return -1;
   }

   do{
    
      char word[256],word2[256],buffer_file[1024];
      int i=0; int j=0;

      buffer_file[0]=fgetc(myfile);
    
      /* end of file */
      if (feof(myfile)){
         end_section=1;
         end_parse=1;
      
         /* line comment */
      }else if (buffer_file[0]=='#') {
         while(fgetc(myfile)!='\n');
      
         /* white spaces */
      }else if (buffer_file[0]==' ') {
         while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);

         /* tabs */
      }else if(buffer_file[0]=='\t') {
         while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);
         /* storing line in buffer */
      }else{
      
         while(buffer_file[i]!='\n') buffer_file[++i]=fgetc(myfile);
         buffer_file[++i]='\0';

         if (i >= MAX_BUFFER-1) {
            printf("%s...\n", buffer_file);
            printf ("simulated3D: line too long in config file!\n");
            exit(-1);
         }
      
         /* first word of the line */
         if (sscanf(buffer_file,"%s",word)==1){
            if (strcmp(word,"driver")==0) {
               while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
               sscanf(&buffer_file[j],"%s",word2);
	  
               /* checking if this section matchs our driver name */
               if (strcmp(word2,driver_name)==0){
                  /* the sections match */
                  do{
	      
		    char buffer_file2[1024],word3[256],word4[256], word5[256];
                     int k=0; int z=0;

                     buffer_file2[0]=fgetc(myfile);
	      
                     /* end of file */
                     if (feof(myfile)){
                        end_section=1;
                        end_parse=1;
		
                        /* line comment */
                     }else if (buffer_file2[0]=='#') {
                        while(fgetc(myfile)!='\n');
	      
                        /* white spaces */
                     }else if (buffer_file2[0]==' ') {
                        while(buffer_file2[0]==' ') buffer_file2[0]=fgetc(myfile);

                        /* tabs */
                     }else if(buffer_file2[0]=='\t') {
                        while(buffer_file2[0]=='\t') buffer_file2[0]=fgetc(myfile);

                        /* storing line in buffer */
                     }else{
		
                        while(buffer_file2[k]!='\n') buffer_file2[++k]=fgetc(myfile);
                        buffer_file2[++k]='\0';
		
                        /* first word of the line */
                        if (sscanf(buffer_file2,"%s",word3)==1){
                           if (strcmp(word3,"end_driver")==0) {
                              while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
                              driver_config_parsed=1;
                              end_section=1;
                              end_parse=1;
		    
                           }else if (strcmp(word3,"driver")==0) {
                              while((buffer_file2[z]!='\n')&&
                                     (buffer_file2[z]!=' ')&&
                                     (buffer_file2[z]!='\0')&&
                                     (buffer_file2[z]!='\t')) z++;
                              printf("simulated3D: error in config file.\n'end_section' keyword required before starting new driver section.\n");
                              end_section=1; end_parse=1;

			   }else if(strcmp(word3,"worldfile")==0){
			      int words;
                              while((buffer_file2[z]!='\n')&&
                                     (buffer_file2[z]!=' ')&&
                                     (buffer_file2[z]!='\0')&&
                                     (buffer_file2[z]!='\t')) z++;
                              words=sscanf(buffer_file2,"%s %s",word4,worldfile);
			      if(words<2){
				     printf("simulated3D: error in config file.\nYou must indicate world file path.\n");
				     end_section=1; end_parse=1;
                              }
			      
			      /* parses world file */
			      camroomconfig=fopen(worldfile,"r");
			      if(camroomconfig==NULL){
				printf("world file %s does not exits\n",worldfile);
				return (-1);
			      }else{
				printf("file %s exists. reading data...\n",worldfile);
				do{i=load_worldline(camroomconfig);}while(i!=EOF);
			      }
			      
                           }else if(strcmp(word3,"provides")==0){
                              int words;
                              while((buffer_file2[z]!='\n')&&
                                     (buffer_file2[z]!=' ')&&
                                     (buffer_file2[z]!='\0')&&
                                     (buffer_file2[z]!='\t')) z++;
                              words=sscanf(buffer_file2,"%s %s %s",word3,word4,word5);
                              if(words==3){
				strcpy(cameraINfile,word5);
				if(strcmp(word4,"colorA")==0){
				  serve_color[0]=1;
				  camCount++;
				  cameras[0].rows=SIFNTSC_ROWS;
				  cameras[0].columns=SIFNTSC_COLUMNS;
				  load_cam(cameraINfile,0); /* camera file */
				}else if(strcmp(word4,"colorB")==0){
				  serve_color[1]=1;
				  camCount++;
				  cameras[1].rows=SIFNTSC_ROWS;
				  cameras[1].columns=SIFNTSC_COLUMNS;
				  load_cam(cameraINfile,1); /* camera file */
				}else if(strcmp(word4,"colorC")==0){
				  serve_color[2]=1;
				  camCount++;
				  load_cam(cameraINfile,2); /* camera file */
				}else if(strcmp(word4,"colorD")==0){
				  serve_color[3]=1;
				  camCount++;
				  cameras[3].rows=SIFNTSC_ROWS;
				  cameras[3].columns=SIFNTSC_COLUMNS;
				  load_cam(cameraINfile,3); /* camera file */
				}else if(strcmp(word4,"varcolorA")==0){
				  serve_color[4]=1;
				  camCount++;
				  load_cam(cameraINfile,4); /* camera file */
				}else if(strcmp(word4,"varcolorB")==0){
				  serve_color[5]=1;
				  camCount++;				    
				  load_cam(cameraINfile,5); /* camera file */
				}else if(strcmp(word4,"varcolorC")==0){
				  serve_color[6]=1;
				  camCount++;
				  load_cam(cameraINfile,6); /* camera file */
				}else if(strcmp(word4,"varcolorD")==0){
				  serve_color[7]=1;
				  camCount++;
				  load_cam(cameraINfile,7); /* camera file */
                                 }
			      }else{
                                   printf("simulated3D: provides line incorrect\n");
                                }
                           }else printf("simulated3D: I don't know what to do with '%s'\n",buffer_file2);
                        }
                     }
                  }while(end_section==0);
                  end_section=0;
               }
            }
         }
      }
   }while(end_parse==0);
  
   /* checking if a driver section was read */
   if(driver_config_parsed==1){
      if((serve_color[0]==0)&&(serve_color[1]==0)&&(serve_color[2]==0)&&(serve_color[3]==0)
	  &&(serve_color[4]==0)&&(serve_color[5]==0)&&(serve_color[6]==0)&&(serve_color[7]==0)){
         printf("simulated3D: warning! no color provided.\n");
      }
      return 0;
   }else return -1;
}


/** simulated3D driver init function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void simulated3D_init(char *configfile)
{
  int i;

  printf("simulated3D driver init\n");
  /* reseting serve color array and setting default options */
  for(i=0;i<MAXCAM;i++){
     serve_color[i]=0;
     color_active[i]=0;
     cameras[i].columns=-1;
  }

  /* we call the function to parse the config file */
  if(simulated3D_parseconf(configfile)==-1){
    printf("simulated3D: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }
 
  /* Add 3D objects in the world */
  if(tridimensional_objects_init()==-1){
     exit(-1);
  }


  /* threads */
  for (i=0; i<MAXCAM; i++){
     if(simulated3D_thread_created[i]!=1){
        pthread_mutex_lock(&mymutex[i]);
        state[i]=slept;
        if (serve_color[i]){
           args[i]=i;
           pthread_create(&simulated3D_th[i],NULL,simulated3D_thread,(void*)&args[i]);
        }
        simulated3D_thread_created[i]=1;
        pthread_mutex_unlock(&mymutex[i]);
     }
  }

  if(serve_color[0]==1){
    all[num_schemas].id = (int *) &colorA_schema_id;
    strcpy(all[num_schemas].name,"colorA");
    all[num_schemas].run = (runFn) mycolorA_run;
    all[num_schemas].stop = (stopFn) mycolorA_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("colorA","id",&colorA_schema_id);
    myexport("colorA","colorA",&colorA);
    myexport("colorA","clock", &imageA_clock);
    myexport("colorA","width",&cameras[0].columns);
    myexport("colorA","height",&cameras[0].rows);
    myexport("colorA","run",(void *)mycolorA_run);
    myexport("colorA","stop",(void *)mycolorA_stop);
    
    colorA=(char *)malloc (cameras[0].columns*cameras[0].rows*3);
    notReadyColorSimA=notReadyColorAA; ReadyColorSimA=readyColorAA;
  }

  if(serve_color[1]==1){
    all[num_schemas].id = (int *) &colorB_schema_id;
    strcpy(all[num_schemas].name,"colorB");
    all[num_schemas].run = (runFn) mycolorB_run;
    all[num_schemas].stop = (stopFn) mycolorB_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("colorB","id",&colorB_schema_id);
    myexport("colorB","colorB",&colorB);
    myexport("colorB","clock", &imageB_clock);
    myexport("colorB","width",&cameras[1].columns);
    myexport("colorB","height",&cameras[1].rows);
    myexport("colorB","run",(void *)mycolorB_run);
    myexport("colorB","stop",(void *)mycolorB_stop);
    
    colorB=(char *)malloc (cameras[1].columns*cameras[1].rows*3);
    notReadyColorSimB=notReadyColorBB; ReadyColorSimB=readyColorBB;
  }

  if(serve_color[2]==1){
    all[num_schemas].id = (int *) &colorC_schema_id;
    strcpy(all[num_schemas].name,"colorC");
    all[num_schemas].run = (runFn) mycolorC_run;
    all[num_schemas].stop = (stopFn) mycolorC_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("colorC","id",&colorC_schema_id);
    myexport("colorC","colorC",&colorC);
    myexport("colorC","clock", &imageC_clock);
    myexport("colorC","width",&cameras[2].columns);
    myexport("colorC","height",&cameras[2].rows);
    myexport("colorC","run",(void *)mycolorC_run);
    myexport("colorC","stop",(void *)mycolorC_stop);
    
    colorC=(char *)malloc (cameras[2].columns*cameras[2].rows*3);
    notReadyColorSimC=notReadyColorCC; ReadyColorSimC=readyColorCC; 
  }

  if(serve_color[3]==1){
    all[num_schemas].id = (int *) &colorD_schema_id;
    strcpy(all[num_schemas].name,"colorD");
    all[num_schemas].run = (runFn) mycolorD_run;
    all[num_schemas].stop = (stopFn) mycolorD_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("colorD","id",&colorD_schema_id);
    myexport("colorD","colorD",&colorD);
    myexport("colorD","clock", &imageD_clock);
    myexport("colorD","width",&cameras[3].columns);
    myexport("colorD","height",&cameras[3].rows);
    myexport("colorD","run",(void *)mycolorD_run);
    myexport("colorD","stop",(void *)mycolorD_stop);
    
    colorD=(char *)malloc (cameras[3].columns*cameras[3].rows*3);
    notReadyColorSimD=notReadyColorDD; ReadyColorSimD=readyColorDD;
  }
  /*creates new schema for varcolorA*/
  if(serve_color[4]==1){  
    all[num_schemas].id = (int *) &varcolorA_schema_id;
    strcpy(all[num_schemas].name,"varcolorA");
    all[num_schemas].run = (runFn) myvarcolorA_run;
    all[num_schemas].stop = (stopFn) myvarcolorA_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("varcolorA","id",&varcolorA_schema_id);
    myexport("varcolorA","varcolorA",&myA);
    myexport("varcolorA","run",(void *)myvarcolorA_run);
    myexport("varcolorA","stop",(void *)myvarcolorA_stop);

    if((cameras[4].rows>=0)&&(cameras[4].columns>=0)){
      myA.height=cameras[4].rows;
      myA.width=cameras[4].columns;
      myA.img=(char *)malloc (cameras[4].columns*cameras[4].rows*3);
      varReadyColorAA=(char *)malloc (cameras[4].columns*cameras[4].rows*3);
      varnotReadyColorAA=(char *)malloc (cameras[4].columns*cameras[4].rows*3);
      varnotReadyColorSimA=varnotReadyColorAA; varReadyColorSimA=varReadyColorAA;
    }}

  /*creates new schema for varcolorB*/
  if(serve_color[5]==1){
    all[num_schemas].id = (int *) &varcolorB_schema_id;
    strcpy(all[num_schemas].name,"varcolorB");
    all[num_schemas].run = (runFn) myvarcolorB_run;
    all[num_schemas].stop = (stopFn) myvarcolorB_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("varcolorB","id",&varcolorB_schema_id);
    myexport("varcolorB","varcolorB",&myB);
    myexport("varcolorB","run",(void *)myvarcolorB_run);
    myexport("varcolorB","stop",(void *)myvarcolorB_stop);
    
    if((cameras[5].rows>=0)&&(cameras[5].columns>=0)){
      myB.height=cameras[5].rows;
      myB.width=cameras[5].columns;
      myB.img=(char *)malloc (cameras[5].columns*cameras[5].rows*3);
      varReadyColorBB=(char *)malloc (cameras[5].columns*cameras[5].rows*3);
      varnotReadyColorBB=(char *)malloc (cameras[5].columns*cameras[5].rows*3);
      varnotReadyColorSimB=varnotReadyColorBB; varReadyColorSimB=varReadyColorBB;
    }}

  /*creates new schema for varcolorC*/
  if(serve_color[6]==1){
    all[num_schemas].id = (int *) &varcolorC_schema_id;
    strcpy(all[num_schemas].name,"varcolorC");
    all[num_schemas].run = (runFn) myvarcolorC_run;
    all[num_schemas].stop = (stopFn) myvarcolorC_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("varcolorC","id",&varcolorC_schema_id);
    myexport("varcolorC","varcolorC",&myC);
    myexport("varcolorC","run",(void *)myvarcolorC_run);
    myexport("varcolorC","stop",(void *)myvarcolorC_stop);

    if((cameras[6].rows>=0)&&(cameras[6].columns>=0)){
      myC.height=cameras[6].rows;
      myC.width=cameras[6].columns;
      myC.img=(char *)malloc (cameras[6].columns*cameras[6].rows*3);
      varReadyColorCC=(char *)malloc (cameras[6].columns*cameras[6].rows*3);
      varnotReadyColorCC=(char *)malloc (cameras[6].columns*cameras[6].rows*3);
      varnotReadyColorSimC=varnotReadyColorCC; varReadyColorSimC=varReadyColorCC;
    }}

  /*creates new schema for varcolorD*/
  if(serve_color[7]==1){
    all[num_schemas].id = (int *) &varcolorD_schema_id;
    strcpy(all[num_schemas].name,"varcolorD");
    all[num_schemas].run = (runFn) myvarcolorD_run;
    all[num_schemas].stop = (stopFn) myvarcolorD_stop;
    printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
    (*(all[num_schemas].id)) = num_schemas;
    all[num_schemas].fps = 0.;
    all[num_schemas].k =0;
    all[num_schemas].state=slept;
    all[num_schemas].terminate = NULL;
    all[num_schemas].handle = NULL;
    num_schemas++;
    myexport("varcolorD","id",&varcolorD_schema_id);
    myexport("varcolorD","varcolorD",&myD);
    myexport("varcolorD","run",(void *)myvarcolorD_run);
    myexport("varcolorD","stop",(void *)myvarcolorD_stop);

    if((cameras[7].rows>=0)&&(cameras[7].columns>=0)){
      myD.height=cameras[7].rows;
      myD.width=cameras[7].columns;
      myD.img=(char *)malloc (cameras[7].columns*cameras[7].rows*3);
      varReadyColorDD=(char *)malloc (cameras[7].columns*cameras[7].rows*3);
      varnotReadyColorDD=(char *)malloc (cameras[7].columns*cameras[7].rows*3);
      varnotReadyColorSimD=varnotReadyColorDD; varReadyColorSimD=varReadyColorDD;
    }}

  printf("simulated3D driver started up\n");
}

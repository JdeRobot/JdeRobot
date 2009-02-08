
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
 *  Authors : Jose Maria Ca�as Plaza <jmplaza@gsyc.es> 
 *            Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>
 *
 */

/**
 *  imagefile driver provides video images to color variables from static PPM and PNM image files with 320x240 resolution
 *  @file imagefile.c
 *  @author Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es> and Jose Maria Ca�as Plaza <jmplaza@gsyc.es>
 *  @version 4.1
 *  @date 30-05-2007
 *
 *
 *  + other sizes different 320x240 are now supported through varcolorX interfaces.
 *  + autosize feature for varcolorX interfaces
 *  @author Jose Maria Ca�as Plaza <jmplaza@gsyc.es>
 *  @date 2009-01-03
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jde.h"
/** Max number of images that can be loaded.*/
#define MAXIMAGES 8
/** Max char size for a string buffer.*/
#define MAX_LINE 1024

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

/** imagefile driver name.*/
char driver_name[256]="imagefile";
/** colors or varcolors detected in config file.*/
int serve_color[MAXIMAGES];
/** filename for each color or varcolor.*/
char name_color[MAXIMAGES][256];

/** width of each served video**/
int width[MAXIMAGES];
/** height of each served video**/
int height[MAXIMAGES];
/** whether the size of each image source should be taken from the image file or not*/
int autosize[MAXIMAGES];

/** id for colorA schema.*/
int colorA_schema_id;
/** id for colorB schema.*/
int colorB_schema_id;
/** id for colorC schema.*/
int colorC_schema_id;
/** id for colorD schema.*/
int colorD_schema_id;
/** id for varcolorA schema.*/
int varcolorA_schema_id;
/** id for varcolorB schema.*/
int varcolorB_schema_id;
/** id for varcolorC schema.*/
int varcolorC_schema_id;
/** id for varcolorD schema.*/
int varcolorD_schema_id;


/*Variables compartidas*/
/** 'colorA' schema image data*/
char *colorA=NULL; /* sifntsc image itself */
/** 'colorA' schema clock*/
unsigned long int colorA_clock;

/** 'colorB' schema image data*/
char *colorB=NULL; /* sifntsc image itself */
/** 'colorB' schema clock*/
unsigned long int colorB_clock;

/** 'colorC' schema image data*/
char *colorC=NULL; /* sifntsc image itself */
/** 'colorC' schema clock*/
unsigned long int colorC_clock;

/** 'colorD' schema image data*/
char *colorD=NULL; /* sifntsc image itself */
/** 'colorD' schema clock*/
unsigned long int colorD_clock;

/** 'varcolorA' schema image data*/
char *varcolorA=NULL; /* image itself */
/** 'varcolorA' schema clock*/
unsigned long int varcolorA_clock;

/** 'varcolorB' schema image data*/
char *varcolorB=NULL; /* image itself */
/** 'varcolorB' schema clock*/
unsigned long int varcolorB_clock;

/** 'varcolorC' schema image data*/
char *varcolorC=NULL; /* image itself */
/** 'varcolorC' schema clock*/
unsigned long int varcolorC_clock;

/** 'varcolorD' schema image data*/
char *varcolorD=NULL; /* image itself */
/** 'varcolorD' schema clock*/
unsigned long int varcolorD_clock;


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

/** function to read images depending on the source (colorA, colorB, colorC, colorD, varcolorA, varcolorB, varcolorC or varcolorD).
 *  @param source selected color.*/
void load_image(int source)
{
  int i,leidos, marca,c,r,last=0;
  int f2;
  char buff[MAX_LINE];
  char *dest=NULL;

  if (serve_color[source])
    {
      f2=open(name_color[source],O_RDONLY);
      /*lseek(f2,SEEK_SET,0);	  */
      
      if (f2==-1) 
	fprintf(stderr,"I can't open the image file %s\n",name_color[source]);
	  else 
	    {
	      i=0;
	      while(i<3) 
		/* three head lines in ppm file:  
		   "P6\n%d %d\n%d\n",width,height,255 */
		{
		  marca=0; buff[marca]='\n';
		  do
		    {
		      leidos=read(f2,&(buff[marca]),1);
		      if (leidos>0) marca+=leidos;
		      if (marca>0) last=marca-1;
		    } 
		  while((buff[last]!='\n')||(leidos<=0));
		  buff[last]='\0';
		  if (buff[0]!='#') 
		    {
		      i++; /* to skip comment lines */
		      /*printf("input %d: %s\n",i,buff); */
		      if (i==1) 
			{
			  if (strcmp(buff,"P6")!=0) 
			    fprintf(stderr,"file %s: non supported image format, must be raw PPM\n",name_color[source]);
			}
		      else if (i==2)
			{
			  if (sscanf(buff,"%d %d",&c,&r)!=2) 
			    fprintf(stderr,"imagefile: error parsing %s file\n",name_color[source]);
			  else /* (sscanf(buff,"%d %d",&c,&r)==2)*/
			    {
			      if (autosize[source]) /* autosize */
				{ width[source]=c;  height[source]=r;}
			      else if ((c!=width[source])||(r!=height[source]))
				{
				  fprintf(stderr,"imagefile: image size mismatch in %s. Image size in that file is %d*%d, but the configured size is %dx%d.\nCheck the configuration of the imagefile driver and remember that colorA, colorB, colorC and colorD are defined as 320x240\n",name_color[source],c,r,width[source],height[source]);
				  jdeshutdown(-1);
				}
			    }
			}
		    }
		}
	      
	      /* read the pixels. The initial call reserves the memory for the image */
	      if (source==0) 
		{ if (colorA==NULL) colorA=(char *)malloc(width[source]*height[source]*3);
		  dest=colorA;
		}
	      else if (source==1)	
		{ if (colorB==NULL) colorB=(char *)malloc(width[source]*height[source]*3);
		  dest=colorB;
		} 
	      else if (source==2) 
		{ if (colorC==NULL) colorC=(char *)malloc(width[source]*height[source]*3);
		  dest=colorC;
		}
	      else if (source==3) 	
		{ if (colorD==NULL) colorD=(char *)malloc(width[source]*height[source]*3);
		  dest=colorD;
		}
	      else if (source==4) 
		{ if (varcolorA==NULL) varcolorA=(char *)malloc(width[source]*height[source]*3);
		  dest=varcolorA;
		}
	      else if (source==5) 
		{ if (varcolorB==NULL) varcolorB=(char *)malloc(width[source]*height[source]*3);
		  dest=varcolorB;
		}
	      else if (source==6) 
		{ if (varcolorC==NULL) varcolorC=(char *)malloc(width[source]*height[source]*3);
		  dest=varcolorC;
		}
	      else if (source==7) 
		{ if (varcolorD==NULL) varcolorD=(char *)malloc(width[source]*height[source]*3);
		  dest=varcolorD;
		}

	      for(i=0;i<width[source]*height[source];i++)
		{
		  read(f2,(void *)buff,(size_t)3);
		  dest[i*3]=buff[2];
		  dest[i*3+1]=buff[1];
		  dest[i*3+2]=buff[0];
		}
	      close(f2);
	    }
    }
}

/** colorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorA_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (colorA_refs>0){
      colorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("colorA schema resume (imagefile driver)\n");
      load_image(0);
      colorA_clock++;
      all[colorA_schema_id].father = father;
      all[colorA_schema_id].fps = 0.;
      all[colorA_schema_id].k =0;
      put_state(colorA_schema_id,winner);
   }
   return 0;
}

/** colorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorA_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (colorA_refs>1){
      colorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("colorA schema suspend (imagefile driver)\n");
      put_state(colorA_schema_id,slept);
   }
   return 0;
}

/** colorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorB_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (colorB_refs>0){
      colorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("colorB schema resume (imagefile driver)\n");
      load_image(1);
      colorB_clock++;
      all[colorB_schema_id].father = father;
      all[colorB_schema_id].fps = 0.;
      all[colorB_schema_id].k =0;
      put_state(colorB_schema_id,winner);
   }
   return 0;
}

/** colorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorB_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (colorB_refs>1){
      colorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("colorB schema suspend (imagefile driver)\n");
      put_state(colorB_schema_id,slept);
   }
   return 0;
}

/** colorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorC_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (colorC_refs>0){
      colorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("colorC schema resume (imagefile driver)\n");
      load_image(2);
      colorC_clock++;
      all[colorC_schema_id].father = father;
      all[colorC_schema_id].fps = 0.;
      all[colorC_schema_id].k =0;
      put_state(colorC_schema_id,winner);
   }
   return 0;
}

/** colorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorC_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (colorC_refs>1){
      colorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("colorC schema suspend (imagefile driver)\n");
      put_state(colorC_schema_id,slept);
   }
   return 0;
}

/** colorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int mycolorD_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (colorD_refs>0){
      colorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("colorD schema resume (imagefile driver)\n");
      load_image(3);
      colorD_clock++;
      all[colorD_schema_id].father = father;
      all[colorD_schema_id].fps = 0.;
      all[colorD_schema_id].k =0;
      put_state(colorD_schema_id,winner);
   }
   return 0;
}

/** colorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int mycolorD_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (colorD_refs>1){
      colorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      colorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("colorD schema suspend (imagefile driver)\n");
      put_state(colorD_schema_id,slept);
   }
   return 0;
}

/** varcolorA resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorA_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>0){
      varcolorA_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorA schema resume (imagefile driver)\n");
      load_image(4);
      varcolorA_clock++;
      all[varcolorA_schema_id].father = father;
      all[varcolorA_schema_id].fps = 0.;
      all[varcolorA_schema_id].k =0;
      put_state(varcolorA_schema_id,winner);
   }
   return 0;
}

/** varcolorA suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorA_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorA_refs>1){
      varcolorA_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorA_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorA schema suspend (imagefile driver)\n");
      put_state(varcolorA_schema_id,slept);
   }
   return 0;
}

/** varcolorB resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorB_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>0){
      varcolorB_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorB schema resume (imagefile driver)\n");
      load_image(5);
      varcolorB_clock++;
      all[varcolorB_schema_id].father = father;
      all[varcolorB_schema_id].fps = 0.;
      all[varcolorB_schema_id].k =0;
      put_state(varcolorB_schema_id,winner);
   }
   return 0;
}

/** varcolorB suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorB_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorB_refs>1){
      varcolorB_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorB_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorB schema suspend (imagefile driver)\n");
      put_state(varcolorB_schema_id,slept);
   }
   return 0;
}

/** varcolorC resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorC_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>0){
      varcolorC_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorC schema resume (imagefile driver)\n");
      load_image(6);
      varcolorC_clock++;
      all[varcolorC_schema_id].father = father;
      all[varcolorC_schema_id].fps = 0.;
      all[varcolorC_schema_id].k =0;
      put_state(varcolorC_schema_id,winner);
   }
   return 0;
}

/** varcolorC suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorC_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorC_refs>1){
      varcolorC_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorC_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorC schema suspend (imagefile driver)\n");
      put_state(varcolorC_schema_id,slept);
   }
   return 0;
}

/** varcolorD resume function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param arbitration function for this schema.
 *  @return integer resuming result.*/
int myvarcolorD_resume(int father, int *brothers, arbitration fn)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>0){
      varcolorD_refs++;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=1;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorD schema resume (imagefile driver)\n");
      load_image(7);
      varcolorD_clock++;
      all[varcolorD_schema_id].father = father;
      all[varcolorD_schema_id].fps = 0.;
      all[varcolorD_schema_id].k =0;
      put_state(varcolorD_schema_id,winner);
   }
   return 0;
}

/** varcolorD suspend function following jdec platform API schemas.
 *  @return integer suspending result.*/
int myvarcolorD_suspend(void)
{
   pthread_mutex_lock(&refmutex);
   if (varcolorD_refs>1){
      varcolorD_refs--;
      pthread_mutex_unlock(&refmutex);
   }
   else{
      varcolorD_refs=0;
      pthread_mutex_unlock(&refmutex);
      printf("varcolorD schema suspend (imagefile driver)\n");
      put_state(varcolorD_schema_id,slept);
   }
   return 0;
}
/** imagefile driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int imagefile_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("imagefile: cannot find config file\n");
    return -1;
  }

  do{
    
    char word[MAX_BUFFER],word2[MAX_BUFFER],buffer_file[MAX_BUFFER];
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
	printf ("Line too long in config file!\n"); 
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
	      
	      char buffer_file2[256],word3[256],word4[256],word5[256],word6[256],word7[256];
	      int k=0; int z=0; int parsedwords=0;

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
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    printf("imagefile: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    
		    parsedwords=sscanf(buffer_file2,"%s %s %s %s %s",word3,word4,word5,word6,word7);
		    if(parsedwords>=3){
		      if(strcmp(word4,"colorA")==0){
                         serve_color[0]=1;
                         strcpy(name_color[0],word5);
                         width[0] = SIFNTSC_COLUMNS;
                         height[0] = SIFNTSC_ROWS;
                      }
		      else if(strcmp(word4,"colorB")==0){
                         serve_color[1]=1;
                         strcpy(name_color[1],word5);
                         width[1] = SIFNTSC_COLUMNS;
                         height[1] = SIFNTSC_ROWS;
                      }
		      else if(strcmp(word4,"colorC")==0){
                         serve_color[2]=1;
                         strcpy(name_color[2],word5);
                         width[2] = SIFNTSC_COLUMNS;
                         height[2] = SIFNTSC_ROWS;
                      }
		      else if(strcmp(word4,"colorD")==0){
                         serve_color[3]=1;
                         strcpy(name_color[3],word5);
                         width[3] = SIFNTSC_COLUMNS;
                         height[3] = SIFNTSC_ROWS;
                      }
		      else if(strcmp(word4,"varcolorA")==0){
                         serve_color[4]=1;
                         strcpy(name_color[4],word5);
			 if (parsedwords==5) 
			   {width[4] = atoi(word6); height[4] = atoi(word7);}
			 else autosize[4]=1; /* autosize */ 
                      }
		      else if(strcmp(word4,"varcolorB")==0){
                         serve_color[5]=1;
                         strcpy(name_color[5],word5);
                         if (parsedwords==5) 
			   {width[5] = atoi(word6); height[5] = atoi(word7);}
			 else autosize[5]=1; /* autosize */ 
                      }
		      else if(strcmp(word4,"varcolorC")==0){
                         serve_color[6]=1;
                         strcpy(name_color[6],word5);
			 if (parsedwords==5) 
			   {width[6] = atoi(word6); height[6] = atoi(word7);}
			 else autosize[6]=1; /* autosize */ 
                      }
		      else if(strcmp(word4,"varcolorD")==0){
                         serve_color[7]=1;
                         strcpy(name_color[7],word5);
			 if (parsedwords==5) 
			   {width[7] = atoi(word6); height[7] = atoi(word7);}
			 else autosize[7]=1; /* autosize */ 
                      }
		    }else{
		      printf("imagefile: provides line incorrect\n");
		    }
		  }else printf("imagefile: i don't know what to do with '%s'\n",buffer_file2);
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
    if((serve_color[0]==0)&&(serve_color[1]==0)&&(serve_color[2]==0)&&(serve_color[3]==0)&&
       (serve_color[4]==0)&&(serve_color[5]==0)&&(serve_color[6]==0)&&(serve_color[7]==0)){
      printf("imagefile: warning! no color provided.\n");
    }
    return 0;
  }else return -1;
}


/** imagefile driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void imagefile_startup(char *configfile)
{
  int i;

  /* reseting serve color array and autosize */
  for(i=0;i<MAXIMAGES;i++) {serve_color[i]=0; autosize[i]=0;}
  
  /* we call the function to parse the config file */
  if(imagefile_parseconf(configfile)==-1){
    printf("imagefile: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }
  printf("imagefile driver started up\n");

  /* register the sensor schemas that this driver supports */
  for(i=0;i<MAXIMAGES;i++) 
    if (serve_color[i])
      {
	if (num_schemas>=MAX_SCHEMAS) 
	  {
	    if (i==0)
	      printf("WARNING: No entry available for colorA schema\n");
	    else if (i==1)
	      printf("WARNING: No entry available for colorB schema\n");
	    else if (i==2)
	      printf("WARNING: No entry available for colorC schema\n");
	    else if (i==3)
	      printf("WARNING: No entry available for colorD schema\n");
	    else if (i==4)
	      printf("WARNING: No entry available for varcolorA schema\n");
	    else if (i==5)
	      printf("WARNING: No entry available for varcolorB schema\n");
	    else if (i==6)
	      printf("WARNING: No entry available for varcolorC schema\n");
	    else if (i==7)
	      printf("WARNING: No entry available for varcolorD schema\n");
	    exit(1);
	  }


	if (i==0)
	  {
	    all[num_schemas].id = (int *) &colorA_schema_id;
	    strcpy(all[num_schemas].name,"colorA");
	    all[num_schemas].resume = (resumeFn) mycolorA_resume;
	    all[num_schemas].suspend = (suspendFn) mycolorA_suspend;
            load_image(i); /* for memory allocation */
	    printf("colorA:%s %d*%d\n",name_color[i],width[i],height[i]);	    
            myexport("colorA","id",&colorA_schema_id);
            myexport("colorA","colorA",&colorA);
            myexport("colorA","clock", &colorA_clock);
            myexport("colorA","width", &width[i]);
            myexport("colorA","height", &height[i]);
            myexport("colorA","resume",(void *)mycolorA_resume);
            myexport("colorA","suspend",(void *)mycolorA_suspend);
	  }
	else if (i==1)
	  {
	    all[num_schemas].id = (int *) &colorB_schema_id;
	    strcpy(all[num_schemas].name,"colorB");
	    all[num_schemas].resume = (resumeFn) mycolorB_resume;
	    all[num_schemas].suspend = (suspendFn) mycolorB_suspend;
            load_image(i); /* for memory allocation */
	    printf("colorB:%s %d*%d\n",name_color[i],width[i],height[i]);
            myexport("colorB","id",&colorB_schema_id);
            myexport("colorB","colorB",&colorB);
            myexport("colorB","clock", &colorB_clock);
            myexport("colorB","width", &width[1]);
            myexport("colorB","height", &height[1]);
            myexport("colorB","resume",(void *)mycolorB_resume);
            myexport("colorB","suspend",(void *)mycolorB_suspend);
	  }
	else if (i==2)
	  {
	    all[num_schemas].id = (int *) &colorC_schema_id;
	    strcpy(all[num_schemas].name,"colorC");
	    all[num_schemas].resume = (resumeFn) mycolorC_resume;
	    all[num_schemas].suspend = (suspendFn) mycolorC_suspend;
            load_image(i); /* for memory allocation */
	    printf("colorC:%s %d*%d\n",name_color[i],width[i],height[i]);
            myexport("colorC","id",&colorC_schema_id);
            myexport("colorC","colorC",&colorC);
            myexport("colorC","clock", &colorC_clock);
            myexport("colorC","width", &width[2]);
            myexport("colorC","height", &height[2]);
            myexport("colorC","resume",(void *)mycolorC_resume);
            myexport("colorC","suspend",(void *)mycolorC_suspend);
	  }
	else if (i==3)
	  {
	    all[num_schemas].id = (int *) &colorD_schema_id;
	    strcpy(all[num_schemas].name,"colorD");
	    all[num_schemas].resume = (resumeFn) mycolorD_resume;
	    all[num_schemas].suspend = (suspendFn) mycolorD_suspend;
	    load_image(i); /* for memory allocation */
	    printf("colorD:%s %d*%d\n",name_color[i],width[i],height[i]);
            myexport("colorD","id",&colorD_schema_id);
            myexport("colorD","colorD",&colorD);
            myexport("colorD","clock", &colorD_clock);
            myexport("colorD","width", &width[i]);
            myexport("colorD","height", &height[i]);
            myexport("colorD","resume",(void *)mycolorD_resume);
            myexport("colorD","suspend",(void *)mycolorD_suspend);
	  }
	else if (i==4)
	  {
	    all[num_schemas].id = (int *) &varcolorA_schema_id;
	    strcpy(all[num_schemas].name,"varcolorA");
	    all[num_schemas].resume = (resumeFn) myvarcolorA_resume;
	    all[num_schemas].suspend = (suspendFn) myvarcolorA_suspend;
	    load_image(i); /* for memory allocation */
	    printf("varcolorA:%s %d*%d",name_color[i],width[i],height[i]);
	    if (autosize[i]) printf(" (autosized)\n");
	    else printf("\n");            
	    myexport("varcolorA","id",&varcolorA_schema_id);
            myexport("varcolorA","varcolorA",&varcolorA);
            myexport("varcolorA","clock", &varcolorA_clock);
            myexport("varcolorA","width", &width[i]);
            myexport("varcolorA","height", &height[i]);
            myexport("varcolorA","resume",(void *)myvarcolorA_resume);
            myexport("varcolorA","suspend",(void *)myvarcolorA_suspend);
	  }
	else if (i==5)
	  {
	    all[num_schemas].id = (int *) &varcolorB_schema_id;
	    strcpy(all[num_schemas].name,"varcolorB");
	    all[num_schemas].resume = (resumeFn) myvarcolorB_resume;
	    all[num_schemas].suspend = (suspendFn) myvarcolorB_suspend;
	    load_image(i); /* for memory allocation */
	    printf("varcolorB:%s %d*%d",name_color[i],width[i],height[i]);
            if (autosize[i]) printf(" (autosized)\n");
	    else printf("\n");  
	    myexport("varcolorB","id",&varcolorB_schema_id);
            myexport("varcolorB","varcolorB",&varcolorB);
            myexport("varcolorB","clock", &varcolorB_clock);
            myexport("varcolorB","width", &width[i]);
            myexport("varcolorB","height", &height[i]);
            myexport("varcolorB","resume",(void *)myvarcolorB_resume);
            myexport("varcolorB","suspend",(void *)myvarcolorB_suspend);
	  }
	else if (i==6)
	  {
	    all[num_schemas].id = (int *) &varcolorC_schema_id;
	    strcpy(all[num_schemas].name,"varcolorC");
	    all[num_schemas].resume = (resumeFn) myvarcolorC_resume;
	    all[num_schemas].suspend = (suspendFn) myvarcolorC_suspend;
	    load_image(i); /* for memory allocation */
	    printf("varcolorC:%s %d*%d",name_color[i],width[i],height[i]);
	    if (autosize[i]) printf(" (autosized)\n");
	    else printf("\n");  
            myexport("varcolorC","id",&varcolorC_schema_id);
            myexport("varcolorC","varcolorC",&varcolorC);
            myexport("varcolorC","clock", &varcolorC_clock);
            myexport("varcolorC","width", &width[i]);
            myexport("varcolorC","height", &height[i]);
            myexport("varcolorC","resume",(void *)myvarcolorC_resume);
            myexport("varcolorC","suspend",(void *)myvarcolorC_suspend);
	  }
	else if (i==7)
	  {
	    all[num_schemas].id = (int *) &varcolorD_schema_id;
	    strcpy(all[num_schemas].name,"varcolorD");
	    all[num_schemas].resume = (resumeFn) myvarcolorD_resume;
	    all[num_schemas].suspend = (suspendFn) myvarcolorD_suspend;
	    load_image(i); /* for memory allocation */
	    printf("varcolorD:%s %d*%d",name_color[i],width[i],height[i]);
	    if (autosize[i]) printf(" (autosized)\n");
	    else printf("\n");  
            myexport("varcolorD","id",&varcolorD_schema_id);
            myexport("varcolorD","varcolorD",&varcolorD);
            myexport("varcolorD","clock", &varcolorD_clock);
            myexport("varcolorD","width", &width[i]);
            myexport("varcolorD","height", &height[i]);
            myexport("varcolorD","resume",(void *)myvarcolorD_resume);
            myexport("varcolorD","suspend",(void *)myvarcolorD_suspend);
	  }

	printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
	(*(all[num_schemas].id)) = num_schemas;
	all[num_schemas].fps = 0.;
	all[num_schemas].k =0;
	all[num_schemas].state=slept;
	all[num_schemas].close = NULL;
	all[num_schemas].handle = NULL;
	num_schemas++;
      }
}

/** imagefile driver closing function invoked when stopping driver.*/
void imagefile_close()
{
  printf("imagefile driver closed\n");
}

/*
 *  Copyright (C) 2006 Antonio Pineda Cabello, Jose Maria Ca�as Plaza
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
 *  Authors : Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Jose Maria Ca�as Plaza <jmplaza@gsyc.escet.urjc.es>
 */

/**
 *  imagefile driver provides video images to color variables from static image files with 320x240 resolution and .ppm or .pnm extension.
 *
 *  @file imagefile.c
 *  @author Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es> and Jose Maria Ca�as Plaza <jmplaza@gsyc.escet.urjc.es>
 *  @version 4.1
 *  @date 30-05-2007
 */

#include <stdio.h>
#include <string.h>
#include "jde.h"
/** Max number of images that can be loaded.*/
#define MAXIMAGES 4
/** Max char size for a string buffer.*/
#define MAX_LINE 1024

/** Image standard number of rows*/
#define SIFNTSC_ROWS 240
/** Image standard number of columns*/
#define SIFNTSC_COLUMNS 320

/** imagefile driver name.*/
char driver_name[256]="imagefile";
/** colors detected in config file.*/
int serve_color[MAXIMAGES];
/** imagefile filename for all colors.*/
char name_color[MAXIMAGES][256];

/** id for colorA schema.*/
int colorA_schema_id;
/** id for colorB schema.*/
int colorB_schema_id;
/** id for colorC schema.*/
int colorC_schema_id;
/** id for colorD schema.*/
int colorD_schema_id;

/*Variables compartidas*/
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

/** width of each served video**/
int width[MAXIMAGES];
/** height of each served video**/
int height[MAXIMAGES];

/*Contadores de referencias*/
/** colorA ref counter*/
int colorA_refs=0;
/** colorB ref counter*/
int colorB_refs=0;
/** colorC ref counter*/
int colorC_refs=0;
/** colorD ref counter*/
int colorD_refs=0;

/** mutex for ref counters*/
pthread_mutex_t refmutex;

/** function to read images depending on the source (colorA, colorB, colorC or colorD).
 *  @param source selected color.*/
void load_image(int source)
{
  int i,leidos, marca,c,r,last=0;
  int f2;
  char buff[MAX_LINE];
  char *dest;

  if (serve_color[source])
    {
      if (source==0) dest=colorA;
      else if (source==1) dest=colorB;
      else if (source==2) dest=colorC;
      else if (source==3) dest=colorD;
      
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
			  if ((sscanf(buff,"%d %d",&c,&r)!=2)||(c!=SIFNTSC_COLUMNS)||(r!=SIFNTSC_ROWS)) 
			    fprintf(stderr,"file %s: non supported image size, must be 320x240\n",name_color[source]);
			}
		    }
		}
	      
	      /* read the pixels */
	      for(i=0;i<SIFNTSC_ROWS*SIFNTSC_COLUMNS;i++)
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
      imageA_clock++;
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
      imageB_clock++;
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
      imageC_clock++;
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
      imageD_clock++;
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

/** imagefile driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int imagefile_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  FILE *myfile;
  const int limit = 256;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("imagefile: cannot find config file\n");
    return -1;
  }

  do{
    
    char word[256],word2[256],buffer_file[256];
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

      if (i >= limit-1) { 
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
	      
	      char buffer_file2[256],word3[256],word4[256],word5[256];
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
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    printf("imagefile: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s",word3,word4,word5)==3){
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
    if((serve_color[0]==0)&&(serve_color[1]==0)&&(serve_color[2]==0)&&(serve_color[3]==0)){
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

  /* reseting serve color array */
  for(i=0;i<MAXIMAGES;i++) serve_color[i]=0;
  
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
	    exit(1);
	  }


	if (i==0)
	  {
	    all[num_schemas].id = (int *) &colorA_schema_id;
	    strcpy(all[num_schemas].name,"colorA");
	    all[num_schemas].resume = (resumeFn) mycolorA_resume;
	    all[num_schemas].suspend = (suspendFn) mycolorA_suspend;
            colorA=(char *)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    printf("colorA:%s\n",name_color[i]);
            myexport("colorA","id",&colorA_schema_id);
            myexport("colorA","colorA",&colorA);
            myexport("colorA","clock", &imageA_clock);
            myexport("colorA","width", &width[0]);
            myexport("colorA","height", &height[0]);
            myexport("colorA","resume",(void *)mycolorA_resume);
            myexport("colorA","suspend",(void *)mycolorA_suspend);
	  }
	else if (i==1)
	  {
	    all[num_schemas].id = (int *) &colorB_schema_id;
	    strcpy(all[num_schemas].name,"colorB");
	    all[num_schemas].resume = (resumeFn) mycolorB_resume;
	    all[num_schemas].suspend = (suspendFn) mycolorB_suspend;
            colorB=(char *)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    printf("colorB:%s\n",name_color[i]);
            myexport("colorB","id",&colorB_schema_id);
            myexport("colorB","colorB",&colorB);
            myexport("colorB","clock", &imageB_clock);
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
            colorC=(char *)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    printf("colorC:%s\n",name_color[i]);
            myexport("colorC","id",&colorC_schema_id);
            myexport("colorC","colorC",&colorC);
            myexport("colorC","clock", &imageC_clock);
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
            colorD=(char *)malloc(SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
	    printf("colorD:%s\n",name_color[i]);
            myexport("colorD","id",&colorD_schema_id);
            myexport("colorD","colorD",&colorD);
            myexport("colorD","clock", &imageD_clock);
            myexport("colorD","width", &width[3]);
            myexport("colorD","height", &height[3]);
            myexport("colorD","resume",(void *)mycolorD_resume);
            myexport("colorD","suspend",(void *)mycolorD_suspend);
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

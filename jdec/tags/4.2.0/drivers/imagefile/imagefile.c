/*
 *  Copyright (C) 2006 Antonio Pineda Cabello, Jose Maria Cañas Plaza
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
 *  Authors : Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Jose Maria Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 */

/************************************************
 * jdec imagefile driver                        *
 ************************************************/

#include <stdio.h>
#include <string.h>
#include "jde.h"
#define MAXIMAGES 4
#define MAX_LINE 1024

char driver_name[256]="imagefile";
int serve_color[MAXIMAGES];
char name_color[MAXIMAGES][256];

int mycolorA_resume(void)
{
  printf("imagefile: colorA resume\n");
  return 0;
}

int mycolorA_suspend(void)
{
  printf("imagefile: colorA suspend\n");
  return 0;
}

void imagefile_suspend()
{
  printf("driver imagefile off\n");
}

void imagefile_resume()
{
  int i,j,leidos, marca,c,r,last=0;
  int f2;
  char buff[MAX_LINE];
  char *dest;

  printf("driver imagefile on\n");
  for(j=0;j<MAXIMAGES;j++)
    {
      if (serve_color[j])
	{
	  if (j==0) 
	    {dest=colorA; 
	    printf("colorA:%s\n",name_color[j]);
	    imageA_resume=mycolorA_resume;
	    imageA_suspend=mycolorA_suspend;
	    }
	  else if (j==1) {dest=colorB; printf("colorB:%s\n",name_color[j]);}
	  else if (j==2) {dest=colorC; printf("colorC:%s\n",name_color[j]);}
	  else if (j==3) {dest=colorD; printf("colorD:%s\n",name_color[j]);}
	  
	  f2=open(name_color[j],O_RDONLY);
	  /*lseek(f2,SEEK_SET,0);	  */
	  
	  if (f2==-1) 
	    fprintf(stderr,"I can't open the image file %s\n",name_color[j]);
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
			    fprintf(stderr,"file %s: non supported image format, must be raw PPM\n",name_color[j]);
			}
		      else if (i==2)
			{
			  if ((sscanf(buff,"%d %d",&c,&r)!=2)||(c!=SIFNTSC_COLUMNS)||(r!=SIFNTSC_ROWS)) 
			    fprintf(stderr,"file %s: non supported image size, must be 320x240\n",name_color[j]);
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
}

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
		      if(strcmp(word4,"colorA")==0){serve_color[0]=1; strcpy(name_color[0],word5);}
		      else if(strcmp(word4,"colorB")==0){serve_color[1]=1; strcpy(name_color[1],word5);}
		      else if(strcmp(word4,"colorC")==0){serve_color[2]=1; strcpy(name_color[2],word5);}
		      else if(strcmp(word4,"colorD")==0){serve_color[3]=1; strcpy(name_color[3],word5);}		      
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

  imagefile_resume();
}

void imagefile_close()
{
  printf("driver imagefile close\n");
}

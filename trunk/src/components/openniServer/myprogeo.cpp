/*
*  Copyright (C) 1997-2011 JDERobot Developers Team
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
 *   Authors : Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "myprogeo.h"


namespace openniServer {
myprogeo::myprogeo(){
	std::cout << "CREADO" << std::endl;
}

myprogeo::~myprogeo(){
}


/* gets the calibration of the camera from a file */
int myprogeo::load_cam_line(FILE *myfile,int cam)
{		
  char word1[MAX_BUFFER],word2[MAX_BUFFER];
  int i=0;
  char buffer_file[MAX_BUFFER];   
  double roll;

  buffer_file[0]=fgetc(myfile);
  if (feof(myfile)) return EOF;
  if (buffer_file[0]==(char)255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  //Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. 
  while((buffer_file[i]!='\n') && 
	(buffer_file[i] != (char)255) &&  
	(i<MAX_BUFFER-1) ) {
    buffer_file[++i]=fgetc(myfile);
  }
  
  if (i >= MAX_BUFFER-1) { 
    printf("%s...\n", buffer_file); 
    printf ("Line too long in config file!\n"); 
    return -1;
  }
  buffer_file[++i]='\0';


  if (sscanf(buffer_file,"%s",word1)!=1) return 0; 
  // return EOF; empty line
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
    else if (strcmp(word1,"columns")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].columns=(int)atoi(word2);
    } 
    else if (strcmp(word1,"rows")==0){
      sscanf(buffer_file,"%s %s",word1,word2);
      cameras[cam].rows=(int)atoi(word2);
    } 
  }
 return 1;
}

/* gets the calibration of the camera from a file */
void myprogeo::load_cam(char *fich_in,int cam)
{
  FILE *entrada;
  int i;

	xmlReader(&(this->cameras[cam]), fich_in);
	/*this->cameras[cam].position.H=1;
	this->cameras[cam].foa.H=1;*/

	/*std::cout << fich_in << std::endl;
  entrada=fopen(fich_in,"r");
   if(entrada==NULL){
     printf("tracker3D: camera input calibration file %s does not exits\n",fich_in);
   }else{
     do{i=load_cam_line(entrada,cam);}while(i!=EOF);
     fclose(entrada);
   } */
  update_camera_matrix(&cameras[cam]);
  display_camerainfo(cameras[cam]);
}


void
myprogeo::mybackproject(float x, float y, float* xp, float* yp, float* zp, float* camx, float* camy, float* camz, int cam){
	HPoint2D p;
	HPoint3D pro;
	



	p.x=GRAPHIC_TO_OPTICAL_X(x,y); 
	p.y=GRAPHIC_TO_OPTICAL_Y(x,y);
	p.h=1;
	backproject(&pro,p,cameras[cam]);
	*xp=pro.X;
	*yp=pro.Y;
	*zp=pro.Z;

	*camx=cameras[cam].position.X;
	*camy=cameras[cam].position.Y;
	*camz=cameras[cam].position.Z;
}

void 
myprogeo::myproject(float x, float y, float z, float* xp, float* yp, int cam){
	HPoint2D p;
	HPoint3D p3;

	p3.X=x;
	p3.Y=y;
	p3.Z=z;
	p3.H=1;
	
	project(p3, &p, cameras[cam]);
	*xp=p.x;
	*yp=p.y;
}

void
myprogeo::mygetcameraposition(float *x, float *y, float *z, int cam){
	*x=cameras[cam].position.X;
	*y=cameras[cam].position.Y;
	*z=cameras[cam].position.Z;
}

void 
myprogeo::mygetcamerafoa(float *x, float *y, float *z, int cam){
	*x=cameras[cam].foa.X;
	*y=cameras[cam].foa.Y;
	*z=cameras[cam].foa.Z;
}

void 
myprogeo::mygetcamerasize(float *w, float *h, int cam){
	*w = cameras[cam].columns;
	*h = cameras[cam].rows;
}

TPinHoleCamera 
myprogeo::getCamera(int camera){
	return cameras[camera];
}

} //namespace

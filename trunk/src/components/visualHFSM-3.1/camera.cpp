/*  
 * Copyright (C) 2008 Roberto Calvo Palomino
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *   
 *   Authors : Roberto Calvo Palomino <rocapal@gsyc.escet.urjc.es>,
 * 			   Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include <stdio.h>
#include <string.h>
#include "camera.h"

#define DEBUG(x...) printf(x)
#define N_LINES_CAM 16
#define DESVIACIONY 0
#define DESVIACIONZ 0


camera::camera (char* configFile): m_nameFile(NULL),
								   m_R (NULL),
								   m_RES (NULL),
								   m_RT (NULL),
								   m_T (NULL),
								   m_K (NULL),
								   m_pos (NULL)
{
	if (configFile!=NULL)
	{
		m_nameFile = (char*) malloc (sizeof(char)*(strlen(configFile)+1));
		if (m_nameFile)
		{
			strncpy(m_nameFile,configFile,strlen(configFile));
			m_nameFile[strlen(configFile)]='\0';
		}
	}
	
	m_R = gsl_matrix_alloc(3,3);	
	m_K = gsl_matrix_alloc(3,3);
  	m_T = gsl_matrix_alloc(3,4);
  	m_RT = gsl_matrix_alloc(4,4);
  	m_RES = gsl_matrix_alloc(3,4);
  	
	m_pos = gsl_vector_alloc(3);	
	
}

/// \brief Destructor
camera::~camera()
{
	if (m_nameFile)
		free(m_nameFile);
	
	if (m_R)
		gsl_matrix_free(m_R);
		
	if (m_RES)
		gsl_matrix_free(m_RES);
		
	if (m_RT)
		gsl_matrix_free(m_RT);
		
	if (m_T)
		gsl_matrix_free(m_T);
		
	if (m_K)
		gsl_matrix_free(m_K);
		
	if (m_pos)
		gsl_vector_free(m_pos);
		
}

TPinHoleCamera& camera::readConfig()
{
	
	const int limit = 256;	
	char word1[limit],word2[limit];
	int i=0;
	char buffer_file[limit];  
	int number;
	FILE *myfile;
	int k=0;
	int counter=0;

	if ((myfile=fopen(m_nameFile,"r"))==NULL){
		printf("Cannot find camera configuration file\n");
		//return -1;
	}


	DEBUG("camera::readConfig: %s\n",m_nameFile);	

	do{
		again:
		i=0;
		buffer_file[0]=fgetc(myfile);
		if (feof(myfile)) 
			k=EOF;
		if (buffer_file[0]==(char)255) 
			k=EOF; 
		if (buffer_file[0]==' '){
			while(buffer_file[0]==' ') 
				buffer_file[0]=fgetc(myfile);
		}
		if (buffer_file[0]=='#'){
			while(buffer_file[0]=fgetc(myfile)!='\n'); 
			goto again;
		}
		if (buffer_file[0]==' '){
			while(buffer_file[0]==' ') 
				buffer_file[0]=fgetc(myfile);
		}
	  	if (buffer_file[0]=='\t'){
			while(buffer_file[0]=='\t') 
				buffer_file[0]=fgetc(myfile);
		}

	


 /* Captures a line and then we will process it with sscanf checking that the last character is \n. We can't doit with fscanf because this function does not difference \n from blank space. */
		while ((buffer_file[i]!='\n')&&(buffer_file[i] != (char)255) && (i<limit-1)) {
			buffer_file[++i]=fgetc(myfile);
		}
		buffer_file[i]='\n';
	
	  
		if (i >= limit-1) { 
			printf("%s...\n", buffer_file); 
			printf ("Line too long in config file!\n"); 
			exit(-1);
		}
		buffer_file[++i]='\0';
	
		if (sscanf(buffer_file,"%s",word1)!=1){
		}
		else {
			number=sscanf(buffer_file,"%s %s ",word1,word2);
			if (strcmp(word1,"positionX")==0){
				cam.position.X=atof(word2);
				counter++;
			}
			else if (strcmp(word1,"positionY")==0){
				cam.position.Y=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"positionZ")==0){
				cam.position.Z=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"positionH")==0){
				cam.position.H=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"FOApositionX")==0){
				cam.foa.X=atof(word2);
				counter++;;
			} 
			else if (strcmp(word1,"FOApositionY")==0){
				cam.foa.Y=atof(word2)-DESVIACIONY;
				counter++;
			} 
			else if (strcmp(word1,"FOApositionZ")==0){
				cam.foa.Z=atof(word2)-DESVIACIONZ;
				counter++;
			} 
			else if (strcmp(word1,"FOApositionH")==0){
				cam.foa.H=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"roll")==0){
				cam.roll=atof(word2);
				//cam.roll=3.14/2;
				counter++;
			} 
			else if (strcmp(word1,"fx")==0){
				cam.fdistx=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"fy")==0){
				cam.fdisty=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"skew")==0){
				cam.skew=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"u0")==0){
				cam.u0=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"v0")==0){
				cam.v0=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"columns")==0){
				cam.columns=atof(word2);
				counter++;
			} 
			else if (strcmp(word1,"rows")==0){
				cam.rows=atof(word2);
				counter++;
			} 
			else{
				printf("NaoVision: line: %s not valid in camera configuration file\n",buffer_file);
			}
		}
	} while(k!=EOF);	

	update_camera_matrix(&cam);
	//if (counter==N_LINES_CAM){
		//return 1;
	//}

	
	//updateMatrix();
	
	//return true;
	return cam;
}

void camera::updateMatrix()
{
/*
	// Fill matrix T 
	gsl_matrix_set(m_T, 0,0,1);
	gsl_matrix_set(m_T, 1,1,1);
	gsl_matrix_set(m_T, 2,2,1);
	
	gsl_matrix_set(m_T,0,3,gsl_vector_get(m_pos,0));
	gsl_matrix_set(m_T,1,3,gsl_vector_get(m_pos,1));
	gsl_matrix_set(m_T,2,3,gsl_vector_get(m_pos,2));	
	
	// Multiplicate R * T  = RES 
	gsl_linalg_matmult (m_R,m_T,m_RES);	
	
	
	for (int i=0;i<3;i++)
    	for (int j=0;j<4;j++)
      		gsl_matrix_set(m_RT,i,j,gsl_matrix_get(m_RES,i,j));
      		
	// set 0001 in the last row of RT 
	gsl_matrix_set(m_RT,3,0,0);
	gsl_matrix_set(m_RT,3,1,0);
	gsl_matrix_set(m_RT,3,2,0);
	gsl_matrix_set(m_RT,3,3,1);
	
	//gsl_matrix_set(m_K,0,2,142.60000610);
	//gsl_matrix_set(m_K,1,2,150.39999390);
	
	//gsl_matrix_set(m_K,1,1,gsl_matrix_get(m_K,0,0));
	gsl_matrix_set(m_K,0,1,0.0);
 */   
	
}

TPinHoleCamera& camera::getProgeoCam()
{
/*	
	HPoint3D positionCam;
	
	positionCam.X= -gsl_vector_get(m_pos,0);
	positionCam.Y= -gsl_vector_get(m_pos,1);
	positionCam.Z= -gsl_vector_get(m_pos,2);
	positionCam.H= 0.0;	
	
	m_progeoCam.position = positionCam;
	
	// Seting intrensic matrix
	m_progeoCam.k11 = gsl_matrix_get(m_K,0,0);
	m_progeoCam.k12 = gsl_matrix_get(m_K,0,1);
	m_progeoCam.k13 = gsl_matrix_get(m_K,0,2);
	m_progeoCam.k14 = 0;
	
	m_progeoCam.k21 = gsl_matrix_get(m_K,1,0);
	m_progeoCam.k22 = gsl_matrix_get(m_K,1,1);
	m_progeoCam.k23 = gsl_matrix_get(m_K,1,2);
	m_progeoCam.k24 = 0;
	
	m_progeoCam.k31 = gsl_matrix_get(m_K,2,0);
	m_progeoCam.k32 = gsl_matrix_get(m_K,2,1);
	m_progeoCam.k33 = gsl_matrix_get(m_K,2,2);
	m_progeoCam.k34 = 0;
	
	// Seting extrensic
	m_progeoCam.rt11 = gsl_matrix_get(m_RT,0,0);
	m_progeoCam.rt12 = gsl_matrix_get(m_RT,0,1);
	m_progeoCam.rt13 = gsl_matrix_get(m_RT,0,2);
	m_progeoCam.rt14 = gsl_matrix_get(m_RT,0,3);

	m_progeoCam.rt21 = gsl_matrix_get(m_RT,1,0);
	m_progeoCam.rt22 = gsl_matrix_get(m_RT,1,1);
	m_progeoCam.rt23 = gsl_matrix_get(m_RT,1,2);
	m_progeoCam.rt24 = gsl_matrix_get(m_RT,1,3);

	m_progeoCam.rt31 = gsl_matrix_get(m_RT,2,0);
	m_progeoCam.rt32 = gsl_matrix_get(m_RT,2,1);
	m_progeoCam.rt33 = gsl_matrix_get(m_RT,2,2);
	m_progeoCam.rt34 = gsl_matrix_get(m_RT,2,3);

	m_progeoCam.rt41 = gsl_matrix_get(m_RT,3,0);
	m_progeoCam.rt42 = gsl_matrix_get(m_RT,3,1);
	m_progeoCam.rt43 = gsl_matrix_get(m_RT,3,2);
	m_progeoCam.rt44 = gsl_matrix_get(m_RT,3,3);
	
	return m_progeoCam;
*/	
}

void camera::test()
{
	
	HPoint3D point3D;
	HPoint2D point2D;
	
	point3D.X = 10;
	point3D.Y = 10;
	point3D.Z = 10;
	point3D.H = 1;
	
	printf ("======== TEST 1 ========\n");	
	
	if (project(point3D, &point2D, cam))
		printf("Project a 2D: %2.2f,%2.2f,%2.2f \n",point2D.x,point2D.y,point2D.h);	
	else
		printf("Error in project\n");
	
    point2D.h=1.0;
	if (backproject(&point3D,point2D,cam)!=-1)
		printf("Backproject a 3D (de nuevo): %.2f,%.2f,%.2f,%.2f \n",point3D.X, point3D.Y, point3D.Z, point3D.H);
	else
		printf("Error in backproject\n");
		
	printf ("======== TEST 2 ========\n");
	
	point2D.x = 85.;
	point2D.y = 154.;
	point2D.h = 1.;
	
	printf ("Pto en 2D: %2.2f,%2.2f,%2.2f \n",point2D.x,point2D.y,point2D.h);
	
	if (backproject(&point3D,point2D,cam)!=-1)
		printf("Backproject a 3D: %.2f,%.2f,%.2f,%.2f \n",point3D.X, point3D.Y, point3D.Z, point3D.H);
	else
		printf("Error in backproject\n");
			
	if (project(point3D, &point2D, cam))
		printf("Project a 2D (de nuevo): %2.2f,%2.2f,%2.2f \n",point2D.x,point2D.y,point2D.h);	
	else
		printf("Error in project\n");


}













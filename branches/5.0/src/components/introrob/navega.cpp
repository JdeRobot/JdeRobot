/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : Javier Vázquez Pereda <javiervazper@yahoo.es>
 *
 */

#include "navega.h" 
#include <iostream>

namespace introrob{

  navega::navega() {	
	BOX_WIDTH=75;	
	BOX_HIGH=1;
	gira=0;
	}

  navega::~navega() {}


  //  return mainwindow->is_visible();



  float navega::getV(){
	return v;
  }
    
  float navega::getW(){
	return w;
  }

	void navega::navegacion( const int width, const int height,  unsigned char * imageData, jderobot::LaserDataPtr laserData, jderobot::EncodersDataPtr encodersData )
//  void navega::with_image( const int width, const int height, unsigned char * data  )
  {
	printf("[alto,ancho]: [%d,%d]\n",height,width);
	sensor_camera_red_line_aligned(width,height,imageData);
	reaccion_camera_red_line_aligned();
  }
   

	bool  navega::isYellow(int r,int g,int b) {

//			printf("r>=[200],g=r,b<=[150] = %d,%d,%d\n",r,g,b);		
		if ((r>=200)&&(b<=g)&&(r==g)) {
//printf("hola\n");
			return true;
		}
		else return false;
	}
//calculates if red line is aligned with camera.
  bool navega::sensor_camera_red_line_aligned(int width, int height, unsigned char * imagenRGB){
	int i,j,box_row,box_column,row_ini,column_ini, column_end;


	int yellow_count;
	for (j=0;j<3;j++){
		yellow_count=0;
		//calculates sub-square coordinates
		switch (j){
			case 0 : row_ini=((height*3)/4);
					 column_ini=width/2-2*BOX_WIDTH;
					 column_end=column_ini+BOX_WIDTH;
					 break;
			case 1 : row_ini=((height*3)/4);
					 column_ini=width/2-BOX_WIDTH;
					 column_end=column_ini+2*BOX_WIDTH;

//					 column_ini=width/2-BOX_WIDTH/2;
					 break;
			case 2 : row_ini=((height*3)/4);
					 column_ini=width/2+BOX_WIDTH;
					 column_end=column_ini+BOX_WIDTH;
//					 column_ini=width/2-BOX_WIDTH/2+BOX_WIDTH;
					 break;
			/*case 3 : row_ini=height-2*BOX_HIGH;
					 column_ini=width/2-BOX_WIDTH/2-BOX_WIDTH;
					 break;
			case 4 : row_ini=height-2*BOX_HIGH;
					 column_ini=width/2-BOX_WIDTH/2;
					 break;
			case 5 : row_ini=height-2*BOX_HIGH;
					 column_ini=width/2-BOX_WIDTH/2+BOX_WIDTH;
					 break;
			case 6 : row_ini=height-BOX_HIGH;
					 column_ini=width/2-BOX_WIDTH/2-BOX_WIDTH;
					 break;
			case 7 : row_ini=height-BOX_HIGH;
					 column_ini=width/2-BOX_WIDTH/2;
					 break;
			case 8 : row_ini=height-BOX_HIGH;
					 column_ini=width/2-BOX_WIDTH/2+BOX_WIDTH;
					 break;*/
		}
		for (i=0;i<(column_end-column_ini)*BOX_HIGH;i++){
				box_row=(int)i/BOX_WIDTH;
				box_column=(int)i%BOX_WIDTH;
			if (isYellow(imagenRGB[((row_ini+box_row)*width+column_ini+box_column)*3],
									imagenRGB[((row_ini+box_row)*width+column_ini+box_column)*3+1],
									imagenRGB[((row_ini+box_row)*width+column_ini+box_column)*3+2])) {
//				printf("r,g,b: %d %d %d \n", imagenRGB[((row_ini+box_row)*width+column_ini+box_column)*3],
//									imagenRGB[((row_ini+box_row)*width+column_ini+box_column)*3+1],
//									imagenRGB[((row_ini+box_row)*width+column_ini+box_column)*3+2]);
				yellow_count++;
			}
		}
		

		yellow_percentage[j]=(float)yellow_count/((float)(column_end-column_ini)*(float)BOX_HIGH);
		switch (j){
			case 0: printf("[%d]=%.2f, ",j,yellow_percentage[j]);
					 break;
			case 1: printf("[%d]=%.2f\n",j,yellow_percentage[j]);
					break;
			case 2: printf("[%d]=%.2f\n\n",j,yellow_percentage[j]);
					 break;
		}

	}
				box_row=(height*6)/8;
				box_column=width/2;
	//	printf("hjhj %d %d - %d %d %d\n",box_row,box_column,imagenRGB[(box_row*width+box_column)*3],
//					imagenRGB[(box_row*width+box_column)*3+1],
//					imagenRGB[(box_row*width+box_column)*3+2]);
		if (!isYellow(imagenRGB[(box_row*width+box_column)*3],
					imagenRGB[(box_row*width+box_column)*3+1],
					imagenRGB[(box_row*width+box_column)*3+2]))
		{
			if (gira==0) {
				if (yellow_percentage[0]>yellow_percentage[2]) 
					gira=1;
				else
					gira=-1;
			}
			printf("gira: %d\n",gira);
		}
		else
			gira=0;

	return false;
  }


	void navega::reaccion_camera_red_line_aligned(){
		if (((yellow_percentage[0]>=0.5) && (yellow_percentage[0]<0.9)) && (yellow_percentage[2]<0.3)) {
			v=600;
			w=0.1;
		}
		else if (((yellow_percentage[2]>=0.5) && (yellow_percentage[2]<0.9))  && (yellow_percentage[0]<0.3)) {
			v=600;
			w=-0.1;
		}
		else if ((yellow_percentage[0]>=0.9)  && (yellow_percentage[2]<0.3)) {
			v=400;
			w=0.3;
		}
		else if ((yellow_percentage[2]>=0.9)  && (yellow_percentage[0]<0.3)) {
			v=400;
			w=-0.3;
		}
		else
		{
			w=0;
			v=600;
		}
		//if ((gira==1 && w<0) || (gira==-1 && w>0)) w=-w;
		if (gira==1) {
			w=0.3;v=200;
		}
		else if (gira==-1) {
			w=-0.3;v=200;
		}
	
		/*if ((yellow_percentage[1]>0.5) && (yellow_percentage[4]>0.5)) {
			v=500;
			w=0;
		}
		else if ((yellow_percentage[0]<0.5) && (yellow_percentage[1]<0.5) && (yellow_percentage[2]<0.5)) {
			if (yellow_percentage[6]>yellow_percentage[8]) {
				v=100;
				w=0.17;
				printf("GIRO IZQUIERDA, %.2f > %.2f\n", yellow_percentage[6],yellow_percentage[8]);
			}
			else if (yellow_percentage[6]<yellow_percentage[8]) {
				v=100;
				w=-0.17;
				printf("GIRO DERECHA, %.2f < %.2f\n", yellow_percentage[6],yellow_percentage[8]);
			}
		}
		else if ((yellow_percentage[4]<0.8) && (yellow_percentage[3]>yellow_percentage[5])) {
			v=500;
			w=0.17;
			printf("CORRECCION IZQUIERDA\n");
		}
		else if ((yellow_percentage[4]<0.8) && (yellow_percentage[3]<yellow_percentage[5])) {
			v=500;
			w=-0.17;
			printf("CORRECCION DERECHA\n");
		}
*/
//v=300; w=0;
	}

}//namespace

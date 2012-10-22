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
 *  Author : Julio Vega <julio.vega@urjc.es>
 *
 */

#include "navega.h"

/* FUNCTIONS TO USE:
v: velocidad lineal (mm./s.) a comandar al robot
this->controller->setV (float v);

w: velocidad rotacional (deg./s.) a comandar al robot
this->controller->setW (float w);

latitude: posición tilt (deg.) a comandar al cuello mecánico 
longitude: posición pan (deg.) a comandar al cuello mecánico
this->controller->setPT1 (float latitude, float longitude);

latitude: posición tilt (deg.) a comandar al cuello mecánico 
longitude: posición pan (deg.) a comandar al cuello mecánico
this->controller->setPT2 (float latitude, float longitude);

Formato estructura color RGB => color.x = R; color.y = G; color.z B)
this->navegacion->pintaSegmento (CvPoint3D32f a, CvPoint3D32f b, CvPoint3D32f color);

Calcula la posicion relativa respecto del robot de un punto absoluto. 
El robot se encuentra en robotx, roboty con orientacion robotheta respecto
al sistema de referencia absoluto
this->navegacion->absolutas2relativas(CvPoint3D32f in, CvPoint3D32f *out);

Calcula la posicion absoluta de un punto expresado en el sistema de 
coordenadas solidario al robot. El robot se encuentra en robotx, roboty 
con orientacion robottheta respecto al sistema de referencia absoluto
this->navegacion->relativas2absolutas(CvPoint3D32f in, CvPoint3D32f *out);

image: vector correspondiente a la imagen
this->navegacion->cogerImagen1(unsigned char* image);

image: vector correspondiente a la imagen
this->navegacion->cogerImagen2(unsigned char* image);

Formato estructura myPoint => myPoint.x = X (mm.); myPoint.y = Y (mm.);
myPoint.z = Theta (deg.)
this->navegacion->cogerPosicion(CvPoint3D32f* myPoint);

Return: nº lásers leídos
Parámetro laser: vector de distancias (mm.) vertidos por el láser
this->navegacion->cogerLaser(std::vector<float>* laser);

Parámetro destino: posición canvas OpenGL establecida por usuario
mediante botón central del ratón
this->navegacion->cogerDestino(CvPoint2D32f* destino);
*/

#define maxLaser 50
#define maxPosition 100


namespace introrob {

    float extra_lines[MAX_LINES][9];
    float points[MAX_POINTS][4];

    int scl=100; // to convert mm into m
    int count=0;
    int numpoints=0;
    int numlines=0;

    bool triangulate=false;

    //Variables para pintar el laser y el rastro    
    CvPoint3D32f datosLaser[maxLaser][180];
    int indiceLaser=0;
    
    float robotPosition[maxPosition][2];
    int indicePosition=0;
    
    bool Position=false;
    bool boolMaxLaser=false;


	Navega::Navega (Controller* controller, Navegacion* navegacion) {
		this->controller = controller;
		this->navegacion = navegacion;
	}


	void Navega::iteracionControl () {
		/* TODO: ADD YOUR ITERATION CODE HERE */	
		/*	    
		// example: how to get pioneer position
		CvPoint3D32f myPoint;
		this->navegacion->cogerPosicion (&myPoint);
		//printf ("encoders: X=%f mm, Y=%f mm, Theta=%f (grados)\n", myPoint.x, myPoint.y, myPoint.z);

		// example: how to get laser readings
		std::vector<float> laser;
		this->navegacion->cogerLaser(&laser);	
		//printf("laser: %f (mm)\n",laser[90]);

		// example: how to get image stream
		unsigned char *image1;
		this->navegacion->cogerImagen1 (&image1);
		//printf ("image: %d\n", image1[0]);
*/

/*
		// movement command to robot wheels
		//this->controller->setV(0.); // mm./s.
		//this->controller->setW(30.); // deg./s.

		// example of pantilt movement:
		this->controller->setPT1 (-15.,0.);
		this->controller->setPT2 (-15.,0.);
		*/
		
/*		
		//Destino
		CvPoint2D32f destino;
		this->navegacion->cogerDestino (&destino);
		printf ("myPoint = [%f, %f]\n", destino.x, destino.y);
*/		
        printf("..............................................\n");
        
		CvPoint3D32f myPoint;
		this->navegacion->cogerPosicion (&myPoint);
		printf ("relativas encoders: X=%f mm, Y=%f mm, Theta=%f (grados)\n", myPoint.x, myPoint.y, myPoint.z);
		
		CvPoint3D32f absolutas;
		navegacion->relativas2absolutas(myPoint, &absolutas);	
		printf ("absolutas encoders: X=%f mm, Y=%f mm, Theta=%f (grados)\n", absolutas.x, absolutas.y, absolutas.z);
		
				
		CvPoint3D32f relativas;
		navegacion->relativas2absolutas(absolutas, &relativas);	
		printf ("relativas encoders: X=%f mm, Y=%f mm, Theta=%f (grados)\n", relativas.x, relativas.y, relativas.z);
		
	}

	void Navega::iteracionGrafica () {
	    
		////////////////Pinta laser alrededor del robot/// ALEX
		std::vector<float> distanceData;
		float  Xp_sensor, Yp_sensor;
	    Tvoxel start,end; 
	    int numLasers;  
	    float robotx;
	    float roboty;
	    float robottheta;
	    
	    //cogemos la posicion del robot
		CvPoint3D32f robot;
		this->navegacion->cogerPosicion (&robot);
		robotx = robot.x;
		roboty = robot.y;
		robottheta = robot.z;
		
		//elegimos color y ancho de linea para pintar el laser
	    glLineWidth(5.0f);  
		glColor3f( 0., 0., 1. );	
		
		//Recogemos datos del laser	
		numLasers = this->navegacion->cogerLaser(&distanceData);	
	    
	    
	    //comprobamos que no nos pasamos en la memoria del lasers
	    int indiceLaserMod =indiceLaser%maxLaser;
	    if(indiceLaserMod>48)
	        boolMaxLaser=true;
	        
        if(boolMaxLaser)
            indiceLaserMod=maxLaser;
            
        //Pintamos la memoria

	    for(int t = 0; t < indiceLaserMod;t++){
	        for(int k = 0; k<numLasers; k++){
                    glBegin(GL_LINES);
                    CvPoint3D32f absolutas, relativas;
           	        absolutas.x = datosLaser[t][k].x ;
           	        absolutas.y = datosLaser[t][k].y ;
           	        absolutas.z = datosLaser[t][k].z;
		            navegacion->relativas2absolutas(absolutas, &relativas);	
                                        
                    v3f( absolutas.x, absolutas.y, 0 );   
                    v3f( absolutas.x, absolutas.y, 1 ); 
                    glEnd();
            }
	    }
	      
        indiceLaser++;
        
        //Guardamos el laser actual en la memoria
	    
		start.x=laser_coord[0]*10.;
		start.y=laser_coord[1];
		
	    glBegin(GL_LINES);
		for(int k=1;k<numLasers;k++) {

			Xp_sensor = distanceData[k]*cos(((float)k-90.)*DEGTORAD);
			Yp_sensor = distanceData[k]*sin(((float)k-90.)*DEGTORAD);

			// Coordenadas del punto detectado por el US con respecto al sistema del sensor, eje x+ normal al sensor
			end.x = laser_coord[0]*10. + Xp_sensor*laser_coord[3] - Yp_sensor*laser_coord[4];
			end.y = laser_coord[1] + Yp_sensor*laser_coord[3] + Xp_sensor*laser_coord[4];
			       	    
    	    float X = (start.x/100.)* cos(robottheta*3.1416/180) - (start.y/100.)*sin(robottheta*3.1416/180);
    	    float Y = (start.x/100.)* sin(robottheta*3.1416/180) + (start.y/100.)*cos(robottheta*3.1416/180);
           
       	    int indiceLaserMod =indiceLaser%maxLaser;
           
            if(robotx==0)
       	        std::cout<< k << std::endl;
       	        
   	        CvPoint3D32f absolutas, relativas;
   	        relativas.x = X+robotx/100. ;
   	        relativas.y = Y+roboty/100. ;
   	        relativas.z = 5.0;
		    navegacion->relativas2absolutas(relativas, &absolutas);	
       	        
            datosLaser[indiceLaserMod][k].x= absolutas.x;
            datosLaser[indiceLaserMod][k].y= absolutas.y;
            datosLaser[indiceLaserMod][k].z= absolutas.z;

            //Pinta el estado actual del laser           
            //v3f( datosLaser[indiceLaserMod][k].x, datosLaser[indiceLaserMod][k].y, 0 );   
            //v3f( datosLaser[indiceLaserMod][k].x, datosLaser[indiceLaserMod][k].y, 1 ); 
                        
			start.x=end.x;
			start.y=end.y;
		}
		glEnd();		
		
		////////////////Pinta rastro en el suelo /// ALEX
		glColor3f( 1., 0., 0. );
		int indicePositionFor = 0;
		
		if(indicePosition==maxPosition)
		    Position=true;
	    if(Position){
	        indicePositionFor=maxPosition;
	    }else{
	        indicePositionFor=indicePosition;
	    }
	    
        indicePosition++;
		
		robotPosition[indicePosition%maxPosition][0] = roboty;
		robotPosition[indicePosition%maxPosition][1] = robotx;
		
		for(int y=0; y<indicePositionFor; y++){
            glBegin(GL_LINES);
            v3f( robotPosition[y][1]/100., robotPosition[y][0]/100., 0 );   
            v3f( robotPosition[y][1]/100., robotPosition[y][0]/100., 1 ); 
            glEnd();
		}
		


//////////
		
    if (triangulate){
    
        triangulate = false;
		
		HPoint3D recta1A,recta2A,recta1B,recta2B;

		recta1A.X=extra_lines[count][0];
		recta1A.Y=extra_lines[count][1];
		recta1A.Z=extra_lines[count][2];

		recta2A.X=extra_lines[count][4];
		recta2A.Y=extra_lines[count][5];
		recta2A.Z=extra_lines[count][6];
        count++;

		recta1B.X=extra_lines[count][0];
		recta1B.Y=extra_lines[count][1];
		recta1B.Z=extra_lines[count][2];

		recta2B.X=extra_lines[count][4];
		recta2B.Y=extra_lines[count][5];
		recta2B.Z=extra_lines[count][6];
        count++;

		HPoint3D p3d = this->navegacion->calculate_intersection(recta1A,recta2A,recta1B,recta2B);
		//printf("Valor triangulado = %f,%f,%f\n",p3d.X,p3d.Y,p3d.Z);
		this->add_point(p3d);
    }
		this->drawPoints(10.,0.,0.,0.);// size, color.X,color.Y,color.Z
		this->drawLines();
		//this->navegacion->drawProjectionLines();

	}
	
    void Navega::add_point(HPoint3D p){

	    if (numpoints <MAX_POINTS){
		    points[numpoints][0] = p.X;
		    points[numpoints][1] = p.Y;
		    points[numpoints][2] = p.Z; 
		    points[numpoints][3] = 1; //para implementar color del punto
		    numpoints++;
	    }
	    else{
		    printf("error, too much points in the world file configuration.\n");
	    }
    } 

	
	void Navega::drawPoints(float size,float cx,float cy, float cz){
	    for(int i=0;i<numpoints;i++) {
		    glPointSize(size);
     		glColor3f( cx, cy, cz);
		    glBegin(GL_POINTS);
		    v3f(points[i][0]/scl,points[i][1]/scl,points[i][2]/scl); 
		    glEnd();
	    }
	}
	
	void Navega::drawLines(){

        for(int i=0;i<numlines;i++) {

		        CvPoint3D32f a,b,aa,ba;
		        CvPoint3D32f color;
		        a.x=extra_lines[i][0]/scl;
		        a.y=extra_lines[i][1]/scl;
		        a.z=extra_lines[i][2]/scl;
		        b.x=extra_lines[i][4]/scl;
		        b.y=extra_lines[i][5]/scl;
		        b.z=extra_lines[i][6]/scl;
        		color.x = 1.; // Red
		        color.y = 0.; // Green
		        color.z = 0.; // Blue
                //printf("Linea a pintar = (%f,%f,%f) --(%f,%f,%f)\n",a.x,a.y,a.z,b.x,b.y,b.z);
		        if (extra_lines[i][8]==1){color.x = 1.;color.y = 0.;color.z = 0.;}else{color.x = 0.;color.y = 0.;color.z = 1.;}
		        this->navegacion->pintaSegmento (a, b, color);
	        }
    }
    void Navega::add_line(float x0,float y0, float z0, float x1, float y1, float z1,int color){

	    if(numlines%2==1){triangulate=true;}

	    if (numlines <MAX_LINES){
		    extra_lines[numlines][0] = x0;
		    extra_lines[numlines][1] = y0;
		    extra_lines[numlines][2] = z0;
		    extra_lines[numlines][3] = 0;
		    extra_lines[numlines][4] = x1;
		    extra_lines[numlines][5] = y1;
		    extra_lines[numlines][6] = z1;
		    extra_lines[numlines][7] = 0;
		    extra_lines[numlines][8] = color;
		    numlines++;
	    }
	    else{
		    printf("error, too much lines in the world file configuration.\n");
	    }
    } 

	Navega::~Navega () {}
}

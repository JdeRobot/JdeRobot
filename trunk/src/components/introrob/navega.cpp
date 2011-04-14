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
 *           JoseMaria Cañas <jmplaza@gsyc.es> 
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

namespace introrob {
	void Navega::iteracionControl () {
			
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

		/* TODO: ADD YOUR ITERATION CODE HERE */	

		CvPoint2D32f destino;
		this->navegacion->cogerDestino (&destino);
		//printf ("myPoint = [%f, %f]\n", destino.x, destino.y);

		// movement command to robot wheels
		//this->controller->setV(0.); // mm./s.
		//this->controller->setW(30.); // deg./s.

		// example of pantilt movement:
		this->controller->setPT1 (-15.,0.);
		this->controller->setPT2 (-15.,0.);
		this->navegacion->updatePT1 (0., -15.);
		this->navegacion->updatePT2 (0., -15.);

		// example: how to get a projection line from Pioneer cameras
		this->navegacion->updateCamerasPos(); // actualizamos la posición de las cámaras en el mundo
/*		HPoint2D pixelImagen;
		pixelImagen.x=160.;
		pixelImagen.y=120.;
		pixelImagen.h=1.0;

		TPinHoleCamera *myCam;
		HPoint3D pixelImagen3D;
		this->navegacion->getMyLeftCam (&myCam);

		this->navegacion->calculate_projection_line(pixelImagen,1);
		this->navegacion->get3DPositionZ(myCam,pixelImagen3D,pixelImagen,0.);// 5000 son los 5 metros a los que está la pared.
		this->navegacion->add_line(myCam->position.X, myCam->position.Y, myCam->position.Z,(float)pixelImagen3D.X,(float)pixelImagen3D.Y, (float)pixelImagen3D.Z,1);
*/
	}

	void Navega::iteracionGrafica () {
		/* TODO: ADD YOUR GRAPHIC CODE HERE */
		CvPoint3D32f aa,bb;
		CvPoint2D32f destino;
		CvPoint3D32f a,b;
		CvPoint3D32f c,d;
		CvPoint3D32f color;

		// ejemplo de segmento en posición absoluta
		//a.x = 0.;	a.y = 0.;	a.z = 0.; // en mm.	
		/*
		this->navegacion->cogerDestino (&destino);
		bb.x=destino.x;
		bb.y=destino.y;
		bb.z=0.;
		this->navegacion->cogerPosicion (&aa);
		aa.z=0.;
		color.x = 1.; // Red
		color.y = 0.; // Green
		color.z = 0.; // Blue
		this->navegacion->pintaSegmento (aa, bb, color); // ROJO
		*/

		// ejemplo de segmento en posición relativa al robot
		/*
		aa.x=0.; aa.y=0.;
		this->navegacion->relativas2absolutas(aa,&a);
		aa.x = 1000.; aa.y = -2000.;  // en mm.	     		
		this->navegacion->relativas2absolutas(aa,&b);
		color.x = 0.; // Red
		color.y = 0.; // Green
		color.z = 1.; // Blue
		this->navegacion->pintaSegmento (a, b, color); // AZUL
		*/

		// example: drawing projection lines
		this->navegacion->drawProjectionLines();
	}

	Navega::Navega (Controller* controller, Navegacion* navegacion) {
		this->controller = controller;
		this->navegacion = navegacion;
	}

	Navega::~Navega () {}
}

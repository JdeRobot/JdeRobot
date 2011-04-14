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

#ifndef INTROROB_NAVEGACION_H
#define INTROROB_NAVEGACION_H

#include <string>
#include <iostream>
#include <GL/gl.h>
#include <jderobot/camera.h>
#include <cv.h>
#include "view.h"
#include "controller.h"
#include "navega.h"
#include "camera.h"

#define NUM_LASERS 180
#define MAX_LINES 200
#define SCALE 100

namespace introrob {

	class View;
	class Controller;
	class Navega;

	class Navegacion {
		private:
			jderobot::CameraPrx cprx1;
			jderobot::CameraPrx cprx2;
			introrob::View *view;
			introrob::Controller *controller;
			bool running;

			pthread_t thread;
		  void** ret;
	
			//Progeo cameras
			TPinHoleCamera myCamA, myCamB;

			float extra_lines[MAX_LINES][9];
			int numlines;

			float robotx;
			float roboty;
			float robottheta;

			float panA;
			float tiltA;

			float panB;
			float tiltB;

			unsigned char *myImage1;
			unsigned char *myImage2;

			void pixel2optical(TPinHoleCamera *cam, HPoint2D *p);

		public:
			introrob::Navega *navega;

			void setView (View* view);
			void run (introrob::Controller * controller);
			int join ();
			void start();
			void stop ();
			int main ();

			/* Formato estructura color RGB => color.x = R; color.y = G; color.z B) */
			int pintaSegmento (CvPoint3D32f a, CvPoint3D32f b, CvPoint3D32f color);

			/* Calcula la posicion relativa respecto del robot de un punto absoluto. 
			El robot se encuentra en robotx, roboty con orientacion robotheta respecto
			al sistema de referencia absoluto */
			int absolutas2relativas(CvPoint3D32f in, CvPoint3D32f *out);

			/* Calcula la posicion absoluta de un punto expresado en el sistema de 
			coordenadas solidario al robot. El robot se encuentra en robotx, roboty 
			con orientacion robottheta respecto al sistema de referencia absoluto */
			int relativas2absolutas(CvPoint3D32f in, CvPoint3D32f *out);

			/* image: vector correspondiente a la imagen */
			void cogerImagen1(unsigned char** image);

			/* image: vector correspondiente a la imagen */
			void cogerImagen2(unsigned char** image);

			/* Formato estructura myPoint => myPoint.x = X (mm.); myPoint.y = Y (mm.);
			myPoint.z = Theta (deg.) */
			void cogerPosicion(CvPoint3D32f* myPoint);

			/* Return: nº lásers leídos
				 Parámetro laser: vector de distancias (mm.) vertidos por el láser */
			int cogerLaser(std::vector<float>* laser);

			/* Parámetro destino: posición canvas OpenGL establecida por usuario
      			mediante botón central del ratón */
			void cogerDestino(CvPoint2D32f* destino);
			void getMyLeftCam(TPinHoleCamera **myCam);
						
			void initCameras();		
			void calculate_projection_line(HPoint2D pix, int idcamera);	
			void drawProjectionLines();
			void resetLines();
			void updateCamerasPos();
			void printCameraInformation (TPinHoleCamera* actualCamera);
	   	void updatePT1(float pan, float tilt);
	   	void updatePT2(float pan, float tilt);
			void add_line(float x0,float y0, float z0, float x1, float y1, float z1,int color);
			void get3DPositionX(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, double Z);
			void get3DPositionZ(TPinHoleCamera * camera, HPoint3D &res, HPoint2D in, float Z);
	};
} // namespace

#endif /*INTROROB_NAVEGACION_H*/

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
 *  Authors : Javier Vázuqez Pereda <javiervazper@yahoo.es>
 *
 */

#ifndef INTROROB_NAVEGA_H
#define INTROROB_NAVEGA_H

#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <string>
#include <colorspaces/colorspacesmm.h>
#include <jderobot/camera.h>
#include <jderobot/laser.h>
#include <jderobot/encoders.h>

namespace introrob{

  class navega
  {
  public:
    navega();
    ~navega();

    //! this function provides a input robot image. Algorithm must asign v a w speeds to robot.
	void navegacion( const int width, const int height,  unsigned char * imageData, jderobot::LaserDataPtr laserData, jderobot::EncodersDataPtr encodersData );
//	void with_image( const int width, const int height,  unsigned char * data );
    float getV();
    float getW();


	

  private:
	float v, w;
	bool sensor_camera_red_line_aligned(int width, int height, unsigned char * imagenRGB);
	void reaccion_camera_red_line_aligned();
	bool isYellow(int b,int g,int r);
	//anchura caja comprobación seguridad
	int BOX_WIDTH;
	//alturacaja comprobación seguridad
	int BOX_HIGH;
	//guarda los porcentajes de valor de rojo de la cuadrícula que usar el robot para seguir la
	//linea.
	float yellow_percentage[9];
	int gira;


  };

}//namespace

#endif //INTROROB_NAVEGA_H

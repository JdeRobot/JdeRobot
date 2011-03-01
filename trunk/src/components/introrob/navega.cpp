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

namespace introrob {
	void Navega::iteracionControl () {
		/* TODO: ADD YOUR ITERATION CODE HERE */
		this->controller->goLeft(); // example of movement
	}

	void Navega::iteracionGrafica () {
		/* TODO: ADD YOUR GRAPHIC CODE HERE */

		CvPoint3D32f aa;
		CvPoint3D32f a,b;
		CvPoint3D32f c,d;
		CvPoint3D32f color;

		// ejemplo de segmento en posición absoluta
		a.x = 0.;	a.y = 0.;	a.z = 5.; // en mm.

		b.x = 50.; b.y = 50.;	b.z = 7.; // en mm.

		color.x = 1.; // Red
		color.y = 0.; // Green
		color.z = 0.; // Blue

		this->navegacion->pintaSegmento (a, b, color); // ROJO

		// ejemplo de segmento en posición absoluta
		aa.x=50.; aa.y=50.;
		this->navegacion->absolutas2relativas(aa,&a);
		aa.x=0.; aa.y=0.;
		this->navegacion->absolutas2relativas(aa,&b);

		color.x = 0.; // Red
		color.y = 0.; // Green
		color.z = 1.; // Blue

		this->navegacion->pintaSegmento (a, b, color); // AZUL
	}

	Navega::Navega (Controller* controller, Navegacion* navegacion, DrawArea* drawarea) {
		this->controller = controller;
		this->navegacion = navegacion;
		this->drawarea = drawarea;
	}

	Navega::~Navega () {}
}

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

#ifndef INTROROB_NAVEGA_H
#define INTROROB_NAVEGA_H

#define MAX_LINES 500
#define MAX_POINTS 100

#include <string>
#include <iostream>
#include <cv.h>
#include "navegacion.h"
#include "controller.h"

namespace introrob {

	class Controller;
	class Navegacion;

	class Navega {
		private:
			CvPoint2D32f destino;
			Controller* controller;
			Navegacion* navegacion;

		public:

			Navega (Controller* controller, Navegacion* navegacion);
			~Navega ();
			void iteracionGrafica();
			void iteracionControl();
			
            void add_point(HPoint3D p);
			void drawPoints(float size,float cx,float cy, float cz);
			void drawLines();
  			void add_line(float x0,float y0, float z0, float x1, float y1, float z1, int color);
	};
} // namespace

#endif /*INTROROB_NAVEGA_H*/

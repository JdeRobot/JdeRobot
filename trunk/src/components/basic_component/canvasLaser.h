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
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>
 *
 */

#ifndef MYCOMPONENT_CANVASLASER_H
#define MYCOMPONENT_CANVASLASER_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
#include <libgnomecanvasmm.h>
#include "gui.h"


namespace mycomponent {
	
	class CanvasLaser : public Gnome::Canvas::CanvasAA 
		{
			public:
				jderobot::IntSeq laserDistances;
				int laserDataNum;
				int a;
				CanvasLaser(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& builder);
				virtual ~CanvasLaser();
				double previous_x;
				double previous_y;
				void printLaser();
				Gnome::Canvas::Group m_canvasgroup;

				//Gnome::Canvas::Line *m_line_black;

			protected:
				//bool on_canvas_event(GdkEvent * event, Gnome::Canvas::Item * item);
				Gnome::Canvas::Line *m_line_red;
				Gnome::Canvas::Ellipse *m_ellipse;
				Gnome::Canvas::Points m_points_red;
				Gnome::Canvas::Line *m_line_green;
				Gnome::Canvas::Line *m_line_black;
				Gnome::Canvas::Text *m_text;
				
				
		};


} // namespace

#endif /*MYCOMPONENT_CANVASLASER_H*/

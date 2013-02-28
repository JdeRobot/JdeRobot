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

#include "canvasTeleoperateCameras.h"

namespace mycomponent {

	

	CanvasControlCameras::CanvasControlCameras(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& builder)
  		: Gnome::Canvas::CanvasAA(cobject),m_canvasgroup(*(root()), 0, 0)
	{ 		
		// create some elements there
                Gnome::Canvas::Points m_points_green;
		Gnome::Canvas::Points m_points_black;
                int i,j;
                double x1,y1,x2,y2;
                bool borde=false;
		this->previous_x=50;
		this->previous_y=50;
  		m_points_green.push_back(Gnome::Art::Point(-50, -50));
  		m_points_green.push_back(Gnome::Art::Point(-50, 150));
  		m_points_green.push_back(Gnome::Art::Point(-50, -50));
  		m_points_green.push_back(Gnome::Art::Point(150, -50));
  		m_points_green.push_back(Gnome::Art::Point(150, 150));
  		m_points_green.push_back(Gnome::Art::Point(-50, 150));
		
		m_points_black.push_back(Gnome::Art::Point(50,-50));
		m_points_black.push_back(Gnome::Art::Point(50,150));
		m_points_black.push_back(Gnome::Art::Point(50,50));
		
		m_points_black.push_back(Gnome::Art::Point(-50,50));
		m_points_black.push_back(Gnome::Art::Point(150,50));
		
		
                j=0;
                for(i=-50;i<=150;i=i+20){
                            if(borde){
                                m_points_green.push_back(Gnome::Art::Point(i, -50));
                                
                                m_points_green.push_back(Gnome::Art::Point(i, 150));
                                borde=false;
                            }else{
                                m_points_green.push_back(Gnome::Art::Point(i, 150));                        
                                
                                m_points_green.push_back(Gnome::Art::Point(i, -50));
                                borde=true;
                            }
                }
                
                for(i=-50;i<=150;i=i+20){
                        
                            if(borde){
                                m_points_green.push_back(Gnome::Art::Point(-50, i));
                                
                                m_points_green.push_back(Gnome::Art::Point(150, i));
                                borde=false;
                            }else{
                                m_points_green.push_back(Gnome::Art::Point(150, i));                        
                                
                                m_points_green.push_back(Gnome::Art::Point(-50, i));
                                borde=true;
                            }
                }


  		// we want to use the stream like interface
  		using namespace Gnome::Canvas;
                
  		m_line_green = new Gnome::Canvas::Line(m_canvasgroup, m_points_green);
  		*m_line_green << Properties::fill_color("gray")
		  		<< Properties::width_pixels(1);
				
		m_line_black = new Gnome::Canvas::Line(m_canvasgroup, m_points_black);
  		*m_line_black << Properties::fill_color("Black")
		  		<< Properties::width_pixels(1);
                                
                               
                
  		m_text = new Gnome::Canvas::Ellipse(m_canvasgroup,35, 35,60,60);
  		*m_text << Properties::fill_color("black");
                
		m_text->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &CanvasControlCameras::on_canvas_event),m_text));

                

	}

	CanvasControlCameras::~CanvasControlCameras()
	{
  		delete m_line_green;
		delete m_line_black;
  		delete m_ellipse;
  		delete m_text;
	}

	bool CanvasControlCameras::on_canvas_event(GdkEvent * event, Gnome::Canvas::Item * item)
	{
		/* Static variables for drag-and-drop */
  		static gboolean dragging = FALSE;
  		//static double this->previous_x, this->previous_y;
  		double event_x = event->button.x;
  		double event_y = event->button.y;
		

		switch (event->type)
  		{
  			case GDK_BUTTON_PRESS:
    			if (event->button.button == 1)
    			{
      				GdkCursor *cursor;

      				/* Store these coordinates for later... */
      				this->previous_x = event_x;
      				this->previous_y = event_y;
      				cursor = gdk_cursor_new(GDK_FLEUR);
      				item->grab(GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK, event->button.time);
      				gdk_cursor_destroy(cursor);
      				dragging = TRUE;
    			}
    			break;

  			case GDK_MOTION_NOTIFY:
    			if (dragging && (event->motion.state & GDK_BUTTON1_MASK))
    			{
      				item->move(event_x - this->previous_x,
        			event_y - this->previous_y);

      				/* Remember these for the next pass */
      				this->previous_x = event_x;
      				this->previous_y = event_y;
				this->moved=1;

    			}
    			break;

  			case GDK_BUTTON_RELEASE:
    			item->ungrab(event->button.time);
    			dragging = FALSE;
    			break;

  			default:
    			break;
  		}
		
		

  		return FALSE;
	}
}//namespace

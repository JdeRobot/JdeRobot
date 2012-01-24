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

#include "canvasLaser.h"

namespace mycomponent {

	class Control;
	class Gui;
	
	CanvasLaser::CanvasLaser(BaseObjectType* cobject, const Glib::RefPtr<Gnome::Glade::Xml>& builder)
  		: Gnome::Canvas::CanvasAA(cobject),m_canvasgroup(*(root()), 0, 0)
	{
            
		//a=5;

		// create some elements there
                Gnome::Canvas::Points m_points_green;
		Gnome::Canvas::Points m_points_black;
		//Gnome::Canvas::Points m_points_red;
                int i,j;
                //double x1,y1,x2,y2;
                bool borde=false;
		
  		m_points_green.push_back(Gnome::Art::Point(-50, -50));
  		m_points_green.push_back(Gnome::Art::Point(-50, 150));
  		m_points_green.push_back(Gnome::Art::Point(-50, -50));
  		m_points_green.push_back(Gnome::Art::Point(150, -50));
  		m_points_green.push_back(Gnome::Art::Point(150, 150));
  		m_points_green.push_back(Gnome::Art::Point(-50, 150));

		m_points_red.push_back(Gnome::Art::Point(-50,-50));
		m_points_red.push_back(Gnome::Art::Point(150,150));
		
		
		
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
		        //pthread_mutex_lock(&control->controlGui);
		
	//printf("%d\n",this.control->laserDataGui->numLaser);
	
		        //pthread_mutex_unlock(&control->controlGui);

		

  		// we want to use the stream like interface
  		using namespace Gnome::Canvas;
                
  		m_line_green = new Gnome::Canvas::Line(m_canvasgroup, m_points_green);
  		*m_line_green << Properties::fill_color("gray")
		  		<< Properties::width_pixels(1);
 		m_line_red = new Gnome::Canvas::Line(m_canvasgroup, m_points_red);
  		*m_line_red << Properties::fill_color("red")
		  		<< Properties::width_pixels(1);

				
		m_line_black = new Gnome::Canvas::Line(m_canvasgroup, m_points_black);
  		*m_line_black << Properties::fill_color("Black")
		  		<< Properties::width_pixels(1);
                              
                
		m_ellipse = new Gnome::Canvas::Ellipse(m_canvasgroup,35, 35,60,60);
  		*m_ellipse << Properties::fill_color("black");

  		m_text = new Gnome::Canvas::Text(m_canvasgroup, 0, 0, "X");
  		*m_text << Properties::font("-Adobe-Helvetica-Medium-R-Normal--*-100-*-*-*-*-*-*")
          		<< Properties::fill_color("black"); //Changes the color of the text.


		//Connecting events
		//m_ellipse->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &CanvasLaser::on_canvas_event),m_ellipse));
                

	}

	CanvasLaser::~CanvasLaser()
	{
		delete m_line_red;
  		delete m_line_green;
		//delete m_line_black;
  		delete m_ellipse;
	}

/*
	bool CanvasLaser::on_canvas_event(GdkEvent * event, Gnome::Canvas::Item * item)
	{
		// Static variables for drag-and-drop 
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

      				// Store these coordinates for later... 
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

      				// Remember these for the next pass 
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
 */
	
	void CanvasLaser::printLaser(){
		
		//Gnome::Canvas::Points m_points_black;
		double k=-0.038461;
		double p=-161.53845;
		int point;
				//printf("%d\n",laserDistances[90]);
		point=laserDistances[90]/54;

	    printf("%d\n",point);
	    printf("%d\n",laserDistances[90]);
		
/*
	    for(int i=0;i<a;i++){
		point=k*laserDistances[i]+p;
		m_points_red.push_back(Gnome::Art::Point(i-50,100));
	    }
*/
	    m_points_red.push_back(Gnome::Art::Point(0,point));

		using namespace Gnome::Canvas;
		
		

			delete m_line_red;

  		m_line_red = new Gnome::Canvas::Line(m_canvasgroup, m_points_red);
  		*m_line_red << Properties::fill_color("red")
		  		<< Properties::width_pixels(1);

	
		
		/*
                j=0;
                for(i=-50;i<=this->a-50;i=i+1){
				double pos=(k*this->laserDistances[i+50])+p;
                                m_points_green.push_back(Gnome::Art::Point(pos, i));
                }
                
		
				using namespace Gnome::Canvas;
                
  		m_line_green = new Gnome::Canvas::Line(m_canvasgroup, m_points_green);
  		*m_line_green << Properties::fill_color("gray")
		  		<< Properties::width_pixels(1);
				
		m_line_black = new Gnome::Canvas::Line(m_canvasgroup, m_points_black);
  		*m_line_black << Properties::fill_color("Black")
		  		<< Properties::width_pixels(1);
                                
                               
                
		m_ellipse = new Gnome::Canvas::Ellipse(m_canvasgroup,35, 35,60,60);
  		*m_ellipse << Properties::fill_color("black");
		*/
	    
	}
	


}//namespace

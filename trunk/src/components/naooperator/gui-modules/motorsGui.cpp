/*
 *  Copyright (C) 1997-2010 JDE Developers Team
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
 *  Authors : Jose María Cañas <jmplaza@gsyc.es>
			Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>
			
 */

/** \file motorsGui.cpp
 * \brief SubClass of naooperatorgui to manage the widgets about the motors module
 */

#include "motorsGui.h" 
#include <iostream>
#include <cmath>

namespace guiModules
{
/**
* \brief motorsGui class construtor
* \return void
*/
motorsGui::motorsGui(DevicesController::MotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml)
{
	motors_ctr = m;
	this->refXml=xml;
	/* As all the widget are hided we have to show the widgets that this module use from the glade interface*/
	Gtk::HBox *w_main_hbox;
	refXml->get_widget("hbox_body",w_main_hbox);
	w_main_hbox->show();

	refXml->get_widget("button_up_v",w_up_v);
	refXml->get_widget("button_down_v",w_down_v);
	refXml->get_widget("button_up_w",w_up_w);
	refXml->get_widget("button_down_w",w_down_w);
	refXml->get_widget("button_stop",w_stop);
	refXml->get_widget("button_side_right",w_right_side);
	refXml->get_widget("button_side_left",w_left_side);
	refXml->get_widget("vscale_v",w_vscale_v);
	refXml->get_widget("hscale_w",w_hscale_w);

	pt_joystick_x_walk=0;
	pt_joystick_y_walk=0;

	MINV=-100;
	MINW=-100;
	MAXV=100;
	MAXW=100;

	/*body canvas*/
	refXml->get_widget("canvas_body",w_canvas_walk);
	w_canvas_walk->signal_unrealize();
	w_canvas_walk->signal_realize();
	gc_walk = Gdk::GC::create(w_canvas_walk->get_window());
	colormap = w_canvas_walk->get_colormap();
	color_white = Gdk::Color("#FFFFFF");
	color_black = Gdk::Color("#000000");
	colormap->alloc_color(color_white);
	colormap->alloc_color(color_black);
	pt_joystick_x_walk=w_canvas_walk->get_width()/2;
	pt_joystick_y_walk=w_canvas_walk->get_height()/2;
	w_canvas_walk->set_child_visible(TRUE);
	w_canvas_walk->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::VISIBILITY_NOTIFY_MASK);
	this->v=0;
	this->w=0;
	this->side=0;
	w_up_v->signal_clicked().connect(sigc::mem_fun(this,&motorsGui::on_clicked_up_v));	
	w_down_v->signal_clicked().connect(sigc::mem_fun(this,&motorsGui::on_clicked_down_v));	
	w_up_w->signal_clicked().connect(sigc::mem_fun(this,&motorsGui::on_clicked_up_w));	
	w_down_w->signal_clicked().connect(sigc::mem_fun(this,&motorsGui::on_clicked_down_w));	
	w_stop->signal_clicked().connect(sigc::mem_fun(this,&motorsGui::on_clicked_stop));	
	w_right_side->signal_clicked().connect(sigc::mem_fun(this,&motorsGui::on_clicked_right_side));	
	w_left_side->signal_clicked().connect(sigc::mem_fun(this,&motorsGui::on_clicked_left_side));	
	w_canvas_walk->signal_expose_event().connect(sigc::mem_fun(this,&motorsGui::on_expose_event_canvas_walk));	
	w_canvas_walk->signal_button_press_event().connect(sigc::mem_fun(this,&motorsGui::on_button_press_canvas_walk));
	w_vscale_v->signal_change_value().connect(sigc::mem_fun(this,&motorsGui::on_change_value_vscale_v));
	w_hscale_w->signal_change_value().connect(sigc::mem_fun(this,&motorsGui::on_change_value_hscale_w));
}
    
/**
* \brief motorsGui class destructor
* \return Nothing
*/
motorsGui::~motorsGui() 
{	
}


/**
* \brief Function that describes what to do if button_up_v widget recieves a click event
* \return void
*/
void
motorsGui::on_clicked_up_v()
{
	Gtk::Entry *entry;
	char buff[50];
	
	this->v++;
	refXml->get_widget("entry_v", entry);
	sprintf(buff,"%f",this->v);
	entry->set_text(buff);
	canvas_body_controller(2);
	motors_ctr->setV(this->v);
}

/**
* \brief Function that describes what to do if button_down_v widget recieves a click event
* \return void
*/
void
motorsGui::on_clicked_down_v()
{
	Gtk::Entry *entry;
	char buff[50];

	v--;
	refXml->get_widget("entry_v", entry);
	sprintf(buff,"%f",this->v);
	entry->set_text(buff);
	canvas_body_controller(2);
	motors_ctr->setV(this->v);
}

/**
* \brief Function that describes what to do if button_up_w widget recieves a click event
* \return void
*/
void
motorsGui::on_clicked_up_w()
{
	Gtk::Entry *entry;
	char buff[50];
	
	w++;
	refXml->get_widget("entry_w", entry);
	sprintf(buff,"%f",this->w);
	entry->set_text(buff);
	canvas_body_controller(2);
	motors_ctr->setW(this->w);
}

/**
* \brief Function that describes what to do if button_down_w widget recieves a click event
* \return void
*/
void
motorsGui::on_clicked_down_w()
{
	Gtk::Entry *entry;
	char buff[50];
	
	this->w--;	
	refXml->get_widget("entry_w", entry);
	sprintf(buff,"%f",this->w);
	entry->set_text(buff);
	canvas_body_controller(2);
	motors_ctr->setW(this->w);
}

/**
* \brief Function that describes what to do if button_right_side widget recieves a click event
* \return void
*/
void
motorsGui::on_clicked_right_side()
{
	this->side++;
	motors_ctr->setL(this->side);
}

/**
* \brief Function that describes what to do if button_left_side widget recieves a click event
* \return void
*/
void
motorsGui::on_clicked_left_side()
{
	this->side--;	
	motors_ctr->setL(this->side);
}

/**
* \brief Function that describes what to do if button_stop widget recieves a click event
* \return void
*/
void motorsGui::on_clicked_stop()
{
	Gtk::Entry *entry;
	char buff[50];

	w=0;
	v=0;
	side=0;
	refXml->get_widget("entry_w", entry);
	sprintf(buff,"%f",this->w);
	entry->set_text(buff);
	refXml->get_widget("entry_v", entry);
	sprintf(buff,"%f",this->v);
	entry->set_text(buff);
	canvas_body_controller(2);
	motors_ctr->setW(w);
	motors_ctr->setV(v);
	motors_ctr->setL(side);
}

/**
* \brief Function that controlles the body canvas (redrawing when is needed)
* \return void
*/
void 
motorsGui::canvas_body_controller(int type)
{
	gc_walk->set_foreground(color_white);
	w_canvas_walk->get_window()->draw_rectangle(gc_walk, true, 0, 0, w_canvas_walk->get_allocation().get_width(), w_canvas_walk->get_allocation().get_height() ) ;
	gc_walk->set_foreground(color_black);

	if (type==1){
		w_canvas_walk->get_window()->draw_line(gc_walk,0,pt_joystick_y_walk,w_canvas_walk->get_allocation().get_width(),pt_joystick_y_walk);
		w_canvas_walk->get_window()->draw_line(gc_walk,pt_joystick_x_walk,0,pt_joystick_x_walk,w_canvas_walk->get_allocation().get_height());
		/*modificar sliders */
	}
	else if (type==2){
		int posy,posx;
		posy=w_canvas_walk->get_allocation().get_height()-(v-MINV)/(MAXV-MINV)*w_canvas_walk->get_allocation().get_height();
		posx=w_canvas_walk->get_allocation().get_width()- (w-MINW)/(MAXV-MINW)*w_canvas_walk->get_allocation().get_width();
		posx=w_canvas_walk->get_allocation().get_width()-posx;
		w_canvas_walk->get_window()->draw_line(gc_walk,0,posy,w_canvas_walk->get_allocation().get_width(),posy);
		w_canvas_walk->get_window()->draw_line(gc_walk,posx,0,posx,w_canvas_walk->get_allocation().get_height());
		/*modificar sliders*/
	}
}

/**
* \brief Function that describes what to do if w_canvas_walk widget recieves an expose event
* \return void
*/
gboolean
motorsGui::on_expose_event_canvas_walk(GdkEventExpose *event)
{
	gc_walk->set_foreground(color_white);

	w_canvas_walk->get_window()->draw_rectangle(gc_walk, true, 0, 0, w_canvas_walk->get_allocation().get_width(), w_canvas_walk->get_allocation().get_height() ) ;
	gc_walk->set_foreground(color_black);

	w_canvas_walk->get_window()->draw_line(gc_walk,0,pt_joystick_y_walk,w_canvas_walk->get_allocation().get_width(),pt_joystick_y_walk);
	w_canvas_walk->get_window()->draw_line(gc_walk,pt_joystick_x_walk,0,pt_joystick_x_walk,w_canvas_walk->get_allocation().get_height());

	return TRUE;
}

/**
* \brief Function that describes what to do if a click event is detected on the canvas walk
* \return void
*/
gboolean
motorsGui::on_button_press_canvas_walk(GdkEventButton* event)
{
	float delta;
	float joystick_x, joystick_y;
	Gtk::Entry *entry;
	char buff[50];

	if(GDK_BUTTON1_MASK)
	{
		pt_joystick_x_walk=event->x;
		pt_joystick_y_walk=event->y;
		joystick_x=pt_joystick_x_walk / (float) w_canvas_walk->get_allocation().get_width();	
		joystick_y=pt_joystick_y_walk/(float)w_canvas_walk->get_allocation().get_height();
		delta = (joystick_x-0.5)*2; /* entre +-1 */
		if (delta<0){
			w=-1*delta*(MINW);
		}
		else{
			w=delta*MAXW;
		}
		delta = (joystick_y - 0.5)*2; /* entre 0 y 1 */
		if (delta<0){
			v=delta*(MAXV);
		}
		else{
			v=delta*MAXV;
		}
		v=v*(-1);
		motors_ctr->setV(v);
		motors_ctr->setW(w);
		refXml->get_widget("entry_w", entry);
		sprintf(buff,"%f",this->w);
		entry->set_text(buff);
		refXml->get_widget("entry_v", entry);
		sprintf(buff,"%f",this->v);
		entry->set_text(buff);
		canvas_body_controller(1);
	}	
	return TRUE;
}

bool 
motorsGui::on_change_value_vscale_v(Gtk::ScrollType scroll, double value){
	this->v=value;
	canvas_body_controller(2);
	motors_ctr->setV(this->v);
	return TRUE;
}

bool 
motorsGui::on_change_value_hscale_w(Gtk::ScrollType scroll, double value){
	this->w=value;
	canvas_body_controller(2);
	motors_ctr->setW(this->w);
	return TRUE;
}


}//namespace

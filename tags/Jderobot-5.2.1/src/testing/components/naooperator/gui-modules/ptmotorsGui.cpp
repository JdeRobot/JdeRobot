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

/** \file ptmotorsGui.cpp
 * \brief SubClass of naooperatorgui to manage the widgets about the ptmotors module
 */

#include "ptmotorsGui.h" 
#include <iostream>
#include <cmath>

namespace guiModules
{
/**
* \brief ptmotorsGui class construtor
* \return void
*/
ptmotorsGui::ptmotorsGui(DevicesController::PTMotorsController *m, Glib::RefPtr<Gnome::Glade::Xml> xml, Gtk::DrawingArea *w,std::vector<float> parametres)
{
	Gtk::Table *table;
	Gtk::HBox *h;
	Gtk::VBox *v;
	ptmotors_ctr = m;
	this->refXml=xml;
	/* As all the widget are hided we have to show the widgets that this module use from the glade interface*/
	w_canvas_head=w;
	gc_head = Gdk::GC::create(w_canvas_head->get_window());
	colormap = w_canvas_head->get_colormap();
	color_white = Gdk::Color("#FFFFFF");
	color_black = Gdk::Color("#000000");
	colormap->alloc_color(color_white);
	colormap->alloc_color(color_black);
	pt_joystick_x_head=w_canvas_head->get_width()/2;
	pt_joystick_y_head=w_canvas_head->get_height()/2;
	refXml->get_widget("table_entry_encoders",table);
	table->show();
	refXml->get_widget("table_ptm",table);
	table->show();
	refXml->get_widget("hbox_ptm_buttons",h);	
	h->show();
	refXml->get_widget("vbox_ptm_origen",v);
	v->show();
	refXml->get_widget("button_up_p",w_up);
	refXml->get_widget("button_down_p",w_down);
	refXml->get_widget("button_up_y",w_right);
	refXml->get_widget("button_down_y",w_left);
	refXml->get_widget("button_origin",w_button_origin);
	refXml->get_widget("hscale_longitude_speed",w_longitude_speed);
	refXml->get_widget("hscale_latitude_speed",w_latitude_speed);
	refXml->get_widget("button_go_head",w_go_head);

	MINPAN=parametres[1];
	MAXPAN=parametres[0];
	MINTILT=parametres[3];
	MAXTILT=parametres[2];

	w_longitude_speed->set_range(1,parametres[4]);
	w_longitude_speed->set_range(1,parametres[5]);

	w_up->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_up));	
	w_down->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_down));	
	w_right->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_right));	
	w_left->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_left));	
	w_canvas_head->signal_button_press_event().connect(sigc::mem_fun(this,&ptmotorsGui::on_button_press_canvas_head));
	w_canvas_head->signal_expose_event().connect(sigc::mem_fun(this,&ptmotorsGui::on_expose_event_canvas_head));
	w_button_origin->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_button_origin));	
	w_longitude_speed->signal_change_value().connect(sigc::mem_fun(this,&ptmotorsGui::on_longitude_speed_changed));
	w_latitude_speed->signal_change_value().connect(sigc::mem_fun(this,&ptmotorsGui::on_latitude_speed_changed));
	w_go_head->signal_clicked().connect(sigc::mem_fun(this,&ptmotorsGui::on_clicked_go_head));

	longitude=0;
	latitude=0;
	longitudeSpeed=1;
	latitudeSpeed=1;
	w_longitude_speed->set_value(longitudeSpeed);
	w_latitude_speed->set_value(latitudeSpeed);

	//std::cout << "valores: " << MINTILT << ", " << MAXTILT << ", " << MINPAN << ", " << MAXPAN << std::endl;
}
    
/**
* \brief ptmotorsGui class destructor
* \return Nothing
*/
ptmotorsGui::~ptmotorsGui() 
{	
}


/**
* \brief Function that describes what to do if the up (ptmotors) button is clicked
* \return bool: true if the MaiWindows have been closed
*/
void
ptmotorsGui::on_clicked_up(){
	Gtk::Entry *entry;
	char buff[50];

	latitude=latitude+1;
	refXml->get_widget("entry_p", entry);
	sprintf(buff,"%f",this->latitude);
	entry->set_text(buff);
	canvas_head_controller(2);
	ptmotors_ctr->setPTMotorsData(longitude, longitudeSpeed, latitude, latitudeSpeed);
	
}

/**
* \brief Function that describes what to do if the down (ptmotors) button is clicked
* \return bool: true if the MaiWindows have been closed
*/
void
ptmotorsGui::on_clicked_down(){
	Gtk::Entry *entry;
	char buff[50];

	latitude=latitude-1;
	refXml->get_widget("entry_p", entry);
	sprintf(buff,"%f",this->latitude);
	entry->set_text(buff);
	canvas_head_controller(2);
	ptmotors_ctr->setPTMotorsData(longitude, longitudeSpeed, latitude, latitudeSpeed);	
}

/**
* \brief Function that describes what to do if the right (ptmotors) button is clicked
* \return bool: true if the MaiWindows have been closed
*/
void
ptmotorsGui::on_clicked_right(){
	Gtk::Entry *entry;
	char buff[50];

	longitude=longitude+1;
	refXml->get_widget("entry_y", entry);
	sprintf(buff,"%f",this->longitude);
	entry->set_text(buff);
	canvas_head_controller(2);
	ptmotors_ctr->setPTMotorsData(longitude, longitudeSpeed, latitude, latitudeSpeed);
}

/**
* \brief Function that describes what to do if the left (ptmotors) button is clicked
* \return bool: true if the MaiWindows have been closed
*/
void
ptmotorsGui::on_clicked_left(){
	Gtk::Entry *entry;
	char buff[50];

	longitude=longitude-1;
	refXml->get_widget("entry_y", entry);
	sprintf(buff,"%f",this->longitude);
	entry->set_text(buff);
	canvas_head_controller(2);
	ptmotors_ctr->setPTMotorsData(longitude, longitudeSpeed, latitude, latitudeSpeed);
}
/**
* \brief Function that controlles the head canvas (redrawing when is needed)
* \return void
*/
void 
ptmotorsGui::canvas_head_controller(int type)
{
	gc_head->set_foreground(color_white);
	w_canvas_head->get_window()->draw_rectangle(gc_head, true, 0, 0, w_canvas_head->get_allocation().get_width(), w_canvas_head->get_allocation().get_height() ) ;
	gc_head->set_foreground(color_black);

	if (type==1){
		w_canvas_head->get_window()->draw_line(gc_head,0,pt_joystick_y_head,w_canvas_head->get_allocation().get_width(),pt_joystick_y_head);
		w_canvas_head->get_window()->draw_line(gc_head,pt_joystick_x_head,0,pt_joystick_x_head,w_canvas_head->get_allocation().get_height());
		/*modificar sliders */
	}
	else if (type==2){
		int posy,posx;
		posy=w_canvas_head->get_allocation().get_height()-(((-1)*latitude)-MINPAN)/(MAXPAN-MINPAN)*w_canvas_head->get_allocation().get_height();
		posx=w_canvas_head->get_allocation().get_width()- (longitude-MINTILT)/(MAXTILT-MINTILT)*w_canvas_head->get_allocation().get_width();
		posx=w_canvas_head->get_allocation().get_width()-posx;
		w_canvas_head->get_window()->draw_line(gc_head,0,posy,w_canvas_head->get_allocation().get_width(),posy);
		w_canvas_head->get_window()->draw_line(gc_head,posx,0,posx,w_canvas_head->get_allocation().get_height());
		/*modificar sliders*/
	}
}
/**
* \brief Function that describes what to do if a click event is detected on the canvas head
* \return void
*/
gboolean
ptmotorsGui::on_button_press_canvas_head(GdkEventButton* event)
{
	float delta;
	float joystick_x, joystick_y;
	Gtk::Entry *entry;
	char buff[50];

	if(GDK_BUTTON1_MASK)
	{
		pt_joystick_x_head=event->x;
		pt_joystick_y_head=event->y;
		joystick_x=pt_joystick_x_head / (float) w_canvas_head->get_allocation().get_width();	
		joystick_y=pt_joystick_y_head/(float)w_canvas_head->get_allocation().get_height();
		delta = (joystick_x-0.5)*2; /* entre -1 y 1 */
		if (delta<0){
			longitude=(-1)*delta*(MINPAN);
		}
		else{
			longitude=delta*(MAXPAN);
		}
		delta = (joystick_y - 0.5)*2; /* entre 0 y 1 */
		latitude=delta*(MAXTILT);

		longitude=longitude*(-1);

		//motors_ctr->setV(longitude);
		//motors_ctr->setW(latitude);
		refXml->get_widget("entry_p", entry);
		sprintf(buff,"%f",this->latitude);
		entry->set_text(buff);
		refXml->get_widget("entry_y", entry);
		sprintf(buff,"%f",this->longitude);
		entry->set_text(buff);
		canvas_head_controller(1);
	}	
	ptmotors_ctr->setPTMotorsData(longitude, longitudeSpeed, latitude, latitudeSpeed);
	return TRUE;
}

/**
* \brief Function that describes what to do if w_canvas_head widget recieves an expose event
* \return void
*/
gboolean
ptmotorsGui::on_expose_event_canvas_head(GdkEventExpose *event)
{
	gc_head->set_foreground(color_white);

	w_canvas_head->get_window()->draw_rectangle(gc_head, true, 0, 0, w_canvas_head->get_allocation().get_width(), w_canvas_head->get_allocation().get_height() ) ;
	gc_head->set_foreground(color_black);

	w_canvas_head->get_window()->draw_line(gc_head,0,pt_joystick_y_head,w_canvas_head->get_allocation().get_width(),pt_joystick_y_head);
	w_canvas_head->get_window()->draw_line(gc_head,pt_joystick_x_head,0,pt_joystick_x_head,w_canvas_head->get_allocation().get_height());

	return TRUE;
}

/**
* \brief Function that describes what to do if the button_origin is pressed
* \return void
*/
void
ptmotorsGui::on_clicked_button_origin(){
	Gtk::Entry *entry;
	char buff[50];


	longitude=0;
	latitude=0;
	refXml->get_widget("entry_y", entry);
	sprintf(buff,"%f",this->longitude);
	entry->set_text(buff);
	refXml->get_widget("entry_p", entry);
	sprintf(buff,"%f",this->latitude);
	entry->set_text(buff);
	canvas_head_controller(2);
	ptmotors_ctr->setPTMotorsData(longitude, longitudeSpeed, latitude, latitudeSpeed);
}

/**
* \brief Function that describes what to do if the latitude speed scale value changes
* \return void
*/
gboolean
ptmotorsGui::on_latitude_speed_changed(Gtk::ScrollType scroll, double new_value){
	latitudeSpeed=new_value;
}


/**
* \brief Function that describes what to do if the longitude speed scale value changes
* \return void
*/
gboolean
ptmotorsGui::on_longitude_speed_changed(Gtk::ScrollType scroll, double new_value){
	longitudeSpeed=new_value;
}

/**
* \brief Function that describes what to do if the button_go_head is clicked
* \return void
*/
void
ptmotorsGui::on_clicked_go_head(){
	std::string text;
	Gtk::Entry *entry;
	char* cadena[50];

	refXml->get_widget("entry_y", entry);
	text=entry->get_text();
	longitude=strtod(text.c_str(),NULL);

	refXml->get_widget("entry_p", entry);
	text=entry->get_text();
	latitude=strtod(text.c_str(),NULL);
	canvas_head_controller(2);
	ptmotors_ctr->setPTMotorsData(longitude, longitudeSpeed, latitude, latitudeSpeed);
}


}//namespace

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

/** \file ptencodersGui.cpp
 * \brief SubClass of naooperatorgui to manage the widgets about the ptencoders module
 */

#include "ptencodersGui.h" 
#include <iostream>
#include <cmath>

namespace guiModules
{
/**
* \brief ptencodersGui class construtor
* \return void
*/
ptencodersGui::ptencodersGui(DevicesController::PTEncodersController *m, Glib::RefPtr<Gnome::Glade::Xml> xml, Gtk::DrawingArea *w, std::vector<float> parametres)
{
	ptencoders_ctr = m;
	this->refXml=xml;
	/* As all the widget are hided we have to show the widgets that this module use from the glade interface*/
	Gtk::Table *table;

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

	MINPAN=parametres[1];
	MAXPAN=parametres[0];
	MINTILT=parametres[3];
	MAXTILT=parametres[2];
	
	w_canvas_head->signal_expose_event().connect(sigc::mem_fun(this,&ptencodersGui::on_expose_event_canvas_head));	
}
/**
* \brief Function that describes what to do if w_canvas_head widget recieves an expose event
* \return void
*/
gboolean
ptencodersGui::on_expose_event_canvas_head(GdkEventExpose *event)
{
	gc_head->set_foreground(color_white);

	w_canvas_head->get_window()->draw_rectangle(gc_head, true, 0, 0, w_canvas_head->get_allocation().get_width(), w_canvas_head->get_allocation().get_height() ) ;
	gc_head->set_foreground(color_black);

	w_canvas_head->get_window()->draw_line(gc_head,0,pt_joystick_y_head,w_canvas_head->get_allocation().get_width(),pt_joystick_y_head);
	w_canvas_head->get_window()->draw_line(gc_head,pt_joystick_x_head,0,pt_joystick_x_head,w_canvas_head->get_allocation().get_height());

	return TRUE;
}

/**
* \brief Function that updates the encoders value at gui
* \return void
*/
void
ptencodersGui::update(){
	std::vector<float> values;
	
	values=ptencoders_ctr->getValues();
	//std::cout << values[0] << "pan: " << values[1] << "tilt: " << values[2] << std::endl;
	if (values[0]){
		pan=values[1];
		tilt=values[2];
		this->canvas_head_controller();
	}
	
}

/**
* \brief Function that controlls the head canvas at gui when a value changes
* \return void
*/
void 
ptencodersGui::canvas_head_controller()
{
	Gtk::Entry *entry;
	char buff[50];

	refXml->get_widget("entry_realp", entry);
	sprintf(buff,"%f",this->tilt);
	entry->set_text(buff);
	refXml->get_widget("entry_realy", entry);
	sprintf(buff,"%f",this->pan);
	entry->set_text(buff);

	gc_head->set_foreground(color_white);
	w_canvas_head->get_window()->draw_rectangle(gc_head, true, 0, 0, w_canvas_head->get_allocation().get_width(), w_canvas_head->get_allocation().get_height() ) ;
	gc_head->set_foreground(color_black);

	int posy,posx;
	posx=w_canvas_head->get_allocation().get_width()-((pan-MINPAN)/(MAXPAN-MINPAN))*w_canvas_head->get_allocation().get_width();
	posy=w_canvas_head->get_allocation().get_height()-((((-1)*tilt)-MINTILT)/(MAXTILT-MINTILT))*w_canvas_head->get_allocation().get_height();
	w_canvas_head->get_window()->draw_line(gc_head,0,posy,w_canvas_head->get_allocation().get_width(),posy);
	w_canvas_head->get_window()->draw_line(gc_head,posx,0,posx,w_canvas_head->get_allocation().get_height());
	/*modificar sliders*/
}



}//namespace

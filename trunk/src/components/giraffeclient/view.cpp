/*
 *  Copyright (C) 2010 Eduardo Perdices García
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
 *   Authors : Eduardo Perdices García <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "view.h"

namespace giraffeClient {

	View::View(Controller * controller): gtkmain(0,0) {

		/*Create controller*/
		this->controller = controller;

		std::cout << "Loading glade\n";
		refXml = Gnome::Glade::Xml::create(controller->getGladePath());

		/*Get widgets*/
    refXml->get_widget("mainwindow",mainwindow);
    refXml->get_widget("button_pan_c",button_pan_c);
    refXml->get_widget("button_tilt_c",button_tilt_c);
    refXml->get_widget("button_left_c",button_left_c);
    refXml->get_widget("button_right_c",button_right_c);
    refXml->get_widget("button_lookat",button_lookat);
    refXml->get_widget("pos_pan",vscale_pos_pan);
    refXml->get_widget("pos_tilt",vscale_pos_tilt);
    refXml->get_widget("pos_left",vscale_pos_left);
    refXml->get_widget("pos_right",vscale_pos_right);
    refXml->get_widget("look_x",hscale_look_x);
    refXml->get_widget("look_y",hscale_look_y);
    refXml->get_widget("look_z",hscale_look_z);
    refXml->get_widget("val_pan",label_val_pan);
    refXml->get_widget("val_tilt",label_val_tilt);
    refXml->get_widget("val_left",label_val_left);
    refXml->get_widget("val_right",label_val_right);
    refXml->get_widget("image",gtk_image);		
        
		/*Set default config*/
		vscale_pos_pan->set_value((double)0.0);
		vscale_pos_tilt->set_value((double)0.0);
		vscale_pos_left->set_value((double)0.0);
		vscale_pos_right->set_value((double)0.0);
		hscale_look_x->set_value((double)1000.0);
		hscale_look_y->set_value((double)0.0);
		hscale_look_z->set_value((double)0.0);
		label_val_pan->set_label("0.0");
		label_val_tilt->set_label("0.0");
		label_val_left->set_label("0.0");
		label_val_right->set_label("0.0");

		/*Create callbacks*/
		button_pan_c->signal_clicked().connect(sigc::mem_fun(this,&View::button_pan_c_clicked));
		button_tilt_c->signal_clicked().connect(sigc::mem_fun(this,&View::button_tilt_c_clicked));
		button_left_c->signal_clicked().connect(sigc::mem_fun(this,&View::button_left_c_clicked));
		button_right_c->signal_clicked().connect(sigc::mem_fun(this,&View::button_right_c_clicked));
		button_lookat->signal_clicked().connect(sigc::mem_fun(this,&View::button_lookat_clicked));
		vscale_pos_pan->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_pan_changed));
		vscale_pos_tilt->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_tilt_changed));
		vscale_pos_left->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_left_changed));
		vscale_pos_right->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_right_changed));
	}

	View::~View() {
		delete this->controller;
	}

  bool View::isVisible(){
    return mainwindow->is_visible();
  }

  void View::display(const colorspaces::Image& image)
  {
		/*Manage image*/
		this->controller->drawWorld(image);

		/*Set image*/
		colorspaces::ImageRGB8 img_rgb8(image);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff = Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8.width,
				    img_rgb8.height,
				    img_rgb8.step); 
    gtk_image->clear();
    gtk_image->set(imgBuff);

		/*Show window*/
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }

	void View::setInitialValues(float pan, float tilt, float left, float right)
	{
		vscale_pos_pan->set_value((double)pan);
		vscale_pos_tilt->set_value((double)tilt);
		vscale_pos_left->set_value((double)left);
		vscale_pos_right->set_value((double)right);			
	}

	void View::setRealValues(float pan, float tilt, float left, float right)
	{
		std::stringstream span, stilt, sleft, sright;
		span << pan;
		stilt << tilt;
		sleft << left;
		sright << right;

		label_val_pan->set_label(span.str());
		label_val_tilt->set_label(stilt.str());
		label_val_left->set_label(sleft.str());
		label_val_right->set_label(sright.str());	
	}

	void View::button_pan_c_clicked(){
		vscale_pos_pan->set_value((double)0.0);
		this->controller->setMotorPos(Controller::MOTOR_PAN, (float)0.0);
  }

	void View::button_tilt_c_clicked(){
		vscale_pos_tilt->set_value((double)0.0);
		this->controller->setMotorPos(Controller::MOTOR_TILT, (float)0.0);
  }

	void View::button_left_c_clicked(){
		vscale_pos_left->set_value((double)0.0);
		this->controller->setMotorPos(Controller::MOTOR_LEFT, (float)0.0);
  }

	void View::button_right_c_clicked(){
		vscale_pos_right->set_value((double)0.0);
		this->controller->setMotorPos(Controller::MOTOR_RIGHT, (float)0.0);
  }

	void View::button_lookat_clicked(){
		this->controller->lookAt((float)hscale_look_x->get_value(), (float)hscale_look_y->get_value(), (float)hscale_look_z->get_value());
  }

	void View::pos_pan_changed(){
		this->controller->setMotorPos(Controller::MOTOR_PAN, (float)vscale_pos_pan->get_value());
  }

	void View::pos_tilt_changed(){
		this->controller->setMotorPos(Controller::MOTOR_TILT, (float)vscale_pos_tilt->get_value());
  }

	void View::pos_left_changed(){
		this->controller->setMotorPos(Controller::MOTOR_LEFT, (float)vscale_pos_left->get_value());
  }
	void View::pos_right_changed(){
		this->controller->setMotorPos(Controller::MOTOR_RIGHT, (float)vscale_pos_right->get_value());
  }
  
}//namespace

/*
*  Copyright (C) 1997-2010 JDERobot Developers Team
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
 *   Authors : Eduardo Perdices <eperdices@gsyc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "view.h"

namespace calibrator {

	View::View(Controller * controller): gtkmain(0,0) {

		/*Create controller*/
		this->controller = controller;

		std::cout << "Loading glade\n";
		refXml = Gnome::Glade::Xml::create(this->controller->getGladePath());

		/*Get widgets*/
    refXml->get_widget("mainwindow",mainwindow);
		refXml->get_widget("pos_x",vscale_pos_x);
		refXml->get_widget("pos_y",vscale_pos_y);
		refXml->get_widget("pos_z",vscale_pos_z);
		refXml->get_widget("foa_x",vscale_foa_x);
		refXml->get_widget("foa_y",vscale_foa_y);
		refXml->get_widget("foa_z",vscale_foa_z);
		refXml->get_widget("fx",vscale_fx);
		refXml->get_widget("fy",vscale_fy);
		refXml->get_widget("u0",vscale_u0);
		refXml->get_widget("v0",vscale_v0);
		refXml->get_widget("roll",vscale_roll);
    refXml->get_widget("image",gtk_image);	
    refXml->get_widget("button_center",button_center);	
        
		/*Set default config*/
		vscale_pos_x->set_value((double)this->controller->getPos()->X);
		vscale_pos_y->set_value((double)this->controller->getPos()->Y);
		vscale_pos_z->set_value((double)this->controller->getPos()->Z);
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z);		
		vscale_fx->set_value((double)this->controller->getFdistX());
		vscale_fy->set_value((double)this->controller->getFdistY());
		vscale_u0->set_value((double)this->controller->getU0());
		vscale_v0->set_value((double)this->controller->getV0());
		vscale_roll->set_value((double)this->controller->getRoll());
		
		/*Create callbacks*/
		vscale_pos_x->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_x_changed));
		vscale_pos_y->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_y_changed));
		vscale_pos_z->signal_value_changed().connect(sigc::mem_fun(this,&View::pos_z_changed));
		vscale_foa_x->signal_value_changed().connect(sigc::mem_fun(this,&View::foa_x_changed));
		vscale_foa_y->signal_value_changed().connect(sigc::mem_fun(this,&View::foa_y_changed));
		vscale_foa_z->signal_value_changed().connect(sigc::mem_fun(this,&View::foa_z_changed));
		vscale_fx->signal_value_changed().connect(sigc::mem_fun(this,&View::fx_changed));
		vscale_fy->signal_value_changed().connect(sigc::mem_fun(this,&View::fy_changed));
		vscale_u0->signal_value_changed().connect(sigc::mem_fun(this,&View::u0_changed));
		vscale_v0->signal_value_changed().connect(sigc::mem_fun(this,&View::v0_changed));
		vscale_roll->signal_value_changed().connect(sigc::mem_fun(this,&View::roll_changed));
		button_center->signal_clicked().connect(sigc::mem_fun(this,&View::button_center_clicked));
	}

	View::~View() {
		delete this->controller;
	}

  bool View::isVisible(){
    return mainwindow->is_visible();
  }

  void View::display(const colorspaces::Image& image)
  {
		/*Change button*/

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

	void View::pos_x_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value()); 
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z);
  }

	void View::pos_y_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value()); 
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z);
  }

	void View::pos_z_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value());
		vscale_foa_x->set_value((double)this->controller->getFoa()->X);
		vscale_foa_y->set_value((double)this->controller->getFoa()->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa()->Z); 
  }

	void View::foa_x_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value()); 
  }

	void View::foa_y_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value()); 
  }

	void View::foa_z_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value()); 
  }

	void View::fx_changed(){
		this->controller->setFdistX((float)vscale_fx->get_value()); 
  }

	void View::fy_changed(){
		this->controller->setFdistY((float)vscale_fy->get_value()); 
  }

	void View::u0_changed(){
		this->controller->setU0((float)vscale_u0->get_value()); 
  }

	void View::v0_changed(){
		this->controller->setV0((float)vscale_v0->get_value()); 
  }

	void View::roll_changed(){
		this->controller->setRoll((float)vscale_roll->get_value()); 
  }

	void View::button_center_clicked(){
		this->controller->changeDrawCenter();
  }

  
}//namespace

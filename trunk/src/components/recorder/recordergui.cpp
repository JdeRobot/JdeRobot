/*
*  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *   Authors : Francisco Miguel Rivas Montero <franciscomiguel.rivas@urjc.es>,
 *             Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "recordergui.h"

namespace recorder {

	recordergui::recordergui(): gtkmain(0,0) {
		recording=false;
		exit=false;


		std::cout << "Loading glade\n";
	refXml = Gnome::Glade::Xml::create("./recordergui.glade");

		/*Get widgets*/
    refXml->get_widget("window1",mainwindow);
    refXml->get_widget("entry2",this->w_entry_fps);
    refXml->get_widget("entry1",this->w_entry_iteration);
    refXml->get_widget("button1",this->w_record);
    refXml->get_widget("button2",this->w_stop);
    refXml->get_widget("button3",this->w_exit);

	this->w_record->signal_clicked().connect(sigc::mem_fun(this,&recordergui::button_record_clicked));
	this->w_stop->signal_clicked().connect(sigc::mem_fun(this,&recordergui::button_stop_clicked));
	this->w_exit->signal_clicked().connect(sigc::mem_fun(this,&recordergui::button_exit_clicked));

	this->w_stop->hide();

    mainwindow->show();

        
		/*Set default config*/
		/*vscale_pos_x->set_value((double)this->controller->getPos()->X);
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
		button_center->signal_clicked().connect(sigc::mem_fun(this,&View::button_center_clicked));*/
	}

	recordergui::~recordergui() {

	}

	void recordergui::update(){
		while (gtkmain.events_pending())
			gtkmain.iteration();
	}	

	void recordergui::set_iteration(int value){
		std::stringstream v;//create a stringstream
		v << value;//add number to the stream
		this->w_entry_iteration->set_text(v.str().c_str());
	}

	void recordergui::set_fps(int value){
		std::stringstream v;//create a stringstream
				v << value;//add number to the stream
			this->w_entry_fps->set_text(v.str().c_str());
		}

	void recordergui::button_record_clicked(){
		this->recording=true;
		this->w_record->hide();
		this->w_stop->show();
	}

	void recordergui::button_stop_clicked(){
		this->w_stop->hide();
		this->recording=false;
		}

	void recordergui::button_exit_clicked(){
		this->recording=false;
		this->exit=true;
		}

	bool recordergui::get_active(){
		return !this->exit;
	}

	bool recordergui::get_recording(){
		return this->recording;
	}

  
}//namespace

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

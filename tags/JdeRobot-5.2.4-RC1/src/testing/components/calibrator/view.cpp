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
 *             Alejandro Hernández Cordero <ahcorde@gmail.com>
 */

#include "view.h"

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>



namespace calibrator {

/*
Comentarios de procedimiento
*/

	View::View(Glib::RefPtr<Gtk::ListStore> m_refTreeModel, Module_DLT * module_dlt, Module_Extrinsics * module_extrinsics, Module_Rectifier * module_rectifier): gtkmain(0,0) {

		/*Init OpenGL*/
		if(!Gtk::GL::init_check(NULL, NULL))	{
			std::cerr << "Couldn't initialize GL\n";
			std::exit(1);
		}
		active_camera = 0;
		std::cout << "Loading glade\n";
		refXml = Gnome::Glade::Xml::create(std::string(GLADE_DIR) + std::string("/calibrator.glade"));



		/*Get widgets Panels*/
		refXml->get_widget("mainwindow",mainwindow);
		refXml->get_widget("image_patron",gtk_patron);
		refXml->get_widget("calibrator",mode[0]);
		refXml->get_widget("extrinsics",mode[1]);
		refXml->get_widget("rectifier",mode[2]);
		refXml->get_widget("estereo",mode[3]);
		refXml->get_widget("dlt_panel",dlt_panel);
		refXml->get_widget("extrinsics_panel",extrinsics_panel);
		refXml->get_widget("rectifier_panel",rectifier_panel);
		refXml->get_widget("estereo_panel",estereo_panel);


		/*Get matrix*/
		for(int i=1; i<4;i++){
			for(int j=1; j<5;j++){
				std::stringstream labelString;
				labelString << "c1k"<< int(i) << int(j);
				refXml->get_widget(labelString.str(),k[(i-1)*4+j-1]);


			}
		}

		for(int i=1; i<5;i++){
			for(int j=1; j<5;j++){
				std::stringstream labelString;
				labelString << "c1rt"<< int(i) << int(j);
				refXml->get_widget(labelString.str(),rt[(i-1)*4+j-1]);


			}
		}

		/* OpenGL World */
		refXml->get_widget_derived("gl_world",world);
		refXml->get_widget("load_world_button",button_Load_world);
		button_Load_world->signal_clicked().connect(sigc::mem_fun(this,&View::button_load_word_clicked));
		

		/* Update World OpenGl*/
		world->setToCamera1();
		

		mode[0]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_calibrator));
		mode[1]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_extrinsics));
		mode[2]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_rectifier));
		mode[3]->signal_toggled().connect(sigc::mem_fun(this,&View::on_toggled_estereo));

		/* Get cameras and load camera combo */
		refXml->get_widget("camera_combo",camera_set);
		camera_set->set_model(m_refTreeModel);

		/* Buttons */
		refXml->get_widget("save_calibration_button",save_calibration_button);
		refXml->get_widget("load_calibration_button",load_calibration_button);
		save_calibration_button->signal_clicked().connect(sigc::mem_fun(this,&View::on_save_calibration_button_clicked));
		load_calibration_button->signal_clicked().connect(sigc::mem_fun(this,&View::on_load_calibration_button_clicked));

		camera_set->set_active(0);
		camera_set->signal_changed().connect(sigc::mem_fun(this,&View::on_changed_camera_set));

		active_panel  = 0;

/* Calibrator */

		/*Create module_dlt*/
		this->module_dlt = module_dlt;
		this->module_dlt->get_widgets(refXml);

/* Extrinsics */
		this->module_extrinsics = module_extrinsics;
		this->module_extrinsics->get_widgets(refXml);
		this->module_extrinsics->set_mainwindow(mainwindow);
		

/* Module_Rectifier*/      

		this->module_rectifier = module_rectifier;
		this->module_rectifier->get_widgets(refXml);



		mainwindow->show();
	}

	View::~View() {
		delete this->module_dlt;
		delete this->module_extrinsics;
		delete this->module_rectifier;
	}

	bool View::isVisible(){
		return mainwindow->is_visible();
	}

	void View::display(const colorspaces::Image& image)
	{

		colorspaces::ImageRGB8 image_rgb8(image);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff_rgb8 = Gdk::Pixbuf::create_from_data((const guint8*)image_rgb8.data,
			Gdk::COLORSPACE_RGB,
			false,
			8,
			image_rgb8.width,
			image_rgb8.height,
			image_rgb8.step); 
		gtk_patron->clear();
		gtk_patron->set(imgBuff_rgb8);


		// Execute selected module
		colorspaces::ImageRGB8 image_dlt = image_rgb8.clone();
		colorspaces::ImageRGB8 image_extrinsics = image_rgb8.clone();
		colorspaces::ImageRGB8 image_rectifier = image_rgb8.clone();


		switch (active_panel)
		{
		case 0:

		/* Calibrator */
				/*Manage image*/
				//		this->module_dlt->drawWorld(image);
				/*Set image */
				
				this->module_dlt->display(image_dlt);
				camera = this->module_dlt->getCam();
		break;
		case 1:
		/* Extrinsics */
				/*Set image*/
				this->module_extrinsics->display(image_extrinsics);
				camera = this->module_extrinsics->getCam();
		break;
		default:
		/* Module_Rectifier*/
				
				this->module_rectifier->display(image_rectifier);
		}

		
		/* Update information matrix */
		std::stringstream labelString;
		labelString.precision(4);
		labelString << camera.k11;
		k[0]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k12;
		k[1]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k13;
		k[2]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k14;
		k[3]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k21;
		k[4]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k22;
		k[5]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k23;
		k[6]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k24;
		k[7]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k31;
		k[8]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k32;
		k[9]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k33;
		k[10]->set_label(labelString.str());
		labelString.str(""); labelString << camera.k34;
		k[11]->set_label(labelString.str());


		labelString.str(""); labelString << camera.rt11;
		rt[0]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt12;
		rt[1]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt13;
		rt[2]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt14;
		rt[3]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt21;
		rt[4]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt22;
		rt[5]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt23;
		rt[6]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt24;
		rt[7]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt31;
		rt[8]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt32;
		rt[9]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt33;
		rt[10]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt34;
		rt[11]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt41;
		rt[12]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt42;
		rt[13]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt43;
		rt[14]->set_label(labelString.str());
		labelString.str(""); labelString << camera.rt44;
		rt[15]->set_label(labelString.str());


		//world->draw_camera(10, 20, 1);

		while (gtkmain.events_pending())
			gtkmain.iteration();

	}

int View::get_active_camera(){
	return active_camera;
}

/* Common Events */

	void View::on_toggled_calibrator(){
		active_panel = 0;
		std::cout << " Calibrator " << std::endl;
		dlt_panel->set_visible(true);
		extrinsics_panel->set_visible(false);
		rectifier_panel->set_visible(false);
		estereo_panel->set_visible(false);
	}
	void View::on_toggled_extrinsics(){
		active_panel = 1;
		std::cout << " Extrinsics " << std::endl;
		extrinsics_panel->set_visible(true);
		dlt_panel->set_visible(false);
		rectifier_panel->set_visible(false);
		estereo_panel->set_visible(false);
	}
	void View::on_toggled_rectifier(){
		active_panel = 2;
		std::cout << " rectifier " << std::endl;
		dlt_panel->set_visible(false);
		extrinsics_panel->set_visible(false);
		rectifier_panel->set_visible(true);
		estereo_panel->set_visible(false);
	}
	void View::on_toggled_estereo(){
		active_panel = 3;
		std::cout << " estereo " << std::endl;
		dlt_panel->set_visible(false);
		extrinsics_panel->set_visible(false);
		rectifier_panel->set_visible(false);
		estereo_panel->set_visible(true);
	}

	void View::on_changed_camera_set(){
		active_camera = camera_set->get_active_row_number();
		std::cout << "Camara activa " << active_camera << std::endl;
	}

	void View::on_save_calibration_button_clicked(){
		Gtk::FileChooserDialog dialog("Please choose a calibration file", Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.set_transient_for(*mainwindow);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button("Save", Gtk::RESPONSE_OK);

		int result = dialog.run();

		//Handle the response:
		switch(result){
			case(Gtk::RESPONSE_OK):{
				display_camerainfo(camera);
				xmlWriter(camera, dialog.get_filename().data());
				break;
			}

			case(Gtk::RESPONSE_CANCEL):{
				std::cout << "Cancel clicked." << std::endl;
				break;
			}

			default:{
				std::cout << "Unexpected button clicked." << std::endl;
				break;
			}
		}
	}

	void View::on_load_calibration_button_clicked(){
		Gtk::FileChooserDialog dialog("Please choose a calibration file", Gtk::FILE_CHOOSER_ACTION_OPEN);
		dialog.set_transient_for(*mainwindow);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button("Load", Gtk::RESPONSE_OK);

		int result = dialog.run();

		//Handle the response:
		switch(result){
			case(Gtk::RESPONSE_OK):{

				display_camerainfo(camera);
				xmlReader(&camera, dialog.get_filename().data());
				display_camerainfo(camera);

				// Actualizar componente extrinsics
				this->module_extrinsics->setCam(camera);
				break;
			}
			case(Gtk::RESPONSE_CANCEL):{
				std::cout << "Cancel clicked." << std::endl;
				break;
			}
			default:{
				std::cout << "Unexpected button clicked." << std::endl;
				break;
			}
		}
	}

	void View::button_load_word_clicked()
	{
		int i=0;
		FILE *worldconfig;

		Gtk::FileChooserDialog dialog("Please choose a folder", Gtk::FILE_CHOOSER_ACTION_OPEN);
		dialog.set_transient_for(*mainwindow);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button("Select", Gtk::RESPONSE_OK);

		int result = dialog.run();

		//Handle the response:
		switch(result){
			case(Gtk::RESPONSE_OK):{
				this->module_extrinsics->button_load_clicked(dialog.get_filename().data());
				world->readFile(dialog.get_filename());				
				break;
			}
			case(Gtk::RESPONSE_CANCEL):{
				std::cout << "Cancel clicked." << std::endl;
				break;
			}
			default:{
				std::cout << "Unexpected button clicked." << std::endl;
				break;
			}
		}
	}

}//namespace

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
#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>

namespace CalibratorKinect {



	View::View(Controller * controller, std::string path, int nCameras): gtkmain(0,0) {

		/*Create controller*/
		this->controller = controller;

		std::cout << "Loading glade\n";
		refXml = Gnome::Glade::Xml::create(this->controller->getGladePath());
		this->nCameras=nCameras;
		cam=1; //comienzo con la camara 0

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
		refXml->get_widget("toggle_fix",w_fix);
		refXml->get_widget("tg_depth",w_depth);	
		refXml->get_widget("window_gl",w_window_gl);
		refXml->get_widget("combobox_cams",m_Combo);
		refXml->get_widget("button_save",w_save);
		refXml->get_widget("toggle_colour",w_colour);
		



		if(!Gtk::GL::init_check(NULL, NULL))	{
			std::cerr << "Couldn't initialize GL\n";
			std::exit(1);
		}
		//opengl world
		refXml->get_widget_derived("gl_world",world);
		//OJO fijar por configuracion
		world->setCamerasResolution(640,480);
		world->readFile(path);

        
		/*Set default config*/
		std::cout << (double)this->controller->getPos(cam)->X << std::endl;
		//vscale_pos_x->set_value((double)this->controller->getPos(cam)->X);
		vscale_pos_x->set_value(12);
		vscale_pos_y->set_value((double)this->controller->getPos(cam)->Y);
		vscale_pos_z->set_value((double)this->controller->getPos(cam)->Z);
		vscale_foa_x->set_value((double)this->controller->getFoa(cam)->X);
		vscale_foa_y->set_value((double)this->controller->getFoa(cam)->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa(cam)->Z);	
		vscale_fx->set_value((double)this->controller->getFdistX(cam));
		vscale_fy->set_value((double)this->controller->getFdistY(cam));
		vscale_u0->set_value((double)this->controller->getU0(cam));
		vscale_v0->set_value((double)this->controller->getV0(cam));
		vscale_roll->set_value((double)this->controller->getRoll(cam));
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
		w_colour->signal_clicked().connect(sigc::mem_fun(this,&View::button_colour_clicked));
		w_save->signal_clicked().connect(sigc::mem_fun(this,&View::button_save_clicked));
		m_Combo->signal_changed().connect(sigc::mem_fun(this,&View::combo_changed));

		this->trueColor=false;
		w_window_gl->show();


 		
		Glib::RefPtr<Gtk::ListStore>  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_Combo->set_model(m_refTreeModel);

  //Fill the ComboBox's Tree Model:

		for (int i=0; i<nCameras; i++){
			std::ostringstream sTemp;
		  Gtk::TreeModel::Row row = *(m_refTreeModel->append());
		  row[m_Columns.m_col_id] = i+1;
		  sTemp << "Camera: " << i;
		  row[m_Columns.m_col_name] = sTemp.str();
		}

  //Add the model columns to the Combo (which is a kind of view),
  //rendering them in the default way:
  m_Combo->pack_start(m_Columns.m_col_id);
  m_Combo->pack_start(m_Columns.m_col_name);


	std::cout << "creado" << std::endl;

	}

	View::~View() {
		delete this->controller;
	}

  bool View::isVisible(){
    return mainwindow->is_visible();
  }

  void View::display(std::vector<CalibratorKinect::kinectData> sources)
  {
		/*Change button*/

		/*Manage image*/
		

		/*Set image*/
		if (w_depth->get_active()){
			jderobot::ImageDataPtr dataDEPTH = sources[cam].cDEPTHprx->getImageData();
			colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(dataDEPTH->description->format);
			if (!fmt2)
				throw "Format not supported";
			colorspaces::Image imageDEPTH(dataDEPTH->description->width, dataDEPTH->description->height, fmt2, &(dataDEPTH->pixelData[0]));

			this->controller->drawWorld(imageDEPTH, cam);
			colorspaces::ImageRGB8 img_rgb8(imageDEPTH);//conversion will happen if needed
			Glib::RefPtr<Gdk::Pixbuf> imgBuff = Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8.width,
				    img_rgb8.height,
				    img_rgb8.step); 
			gtk_image->clear();
    		gtk_image->set(imgBuff);
			mainwindow->resize(1,1);
			while (gtkmain.events_pending()){
      			gtkmain.iteration();
	}
		}
		else{
			jderobot::ImageDataPtr dataRGB = sources[cam].cRGBprx->getImageData();
			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(dataRGB->description->format);
			if (!fmt)
				throw "Format not supported";
			colorspaces::Image image(dataRGB->description->width, dataRGB->description->height, fmt, &(dataRGB->pixelData[0]));
			this->controller->drawWorld(image, cam);
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
			mainwindow->resize(1,1);
			while (gtkmain.events_pending()){
      			gtkmain.iteration();
	}
		}

		//creo el mundo en 3D.

		if (w_fix->get_active()){
			this->world->clear_points();
			for (int pos=0; pos< nCameras; pos++){
				int colour;
				jderobot::ImageDataPtr dataDEPTH = sources[pos].cDEPTHprx->getImageData();
				colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(dataDEPTH->description->format);
				if (!fmt2)
					throw "Format not supported";
				colorspaces::Image imageDEPTH(dataDEPTH->description->width, dataDEPTH->description->height, fmt2, &(dataDEPTH->pixelData[0]));
				jderobot::ImageDataPtr dataRGB = sources[pos].cRGBprx->getImageData();
				colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(dataRGB->description->format);
				if (!fmt)
					throw "Format not supported";
				colorspaces::Image image(dataRGB->description->width, dataRGB->description->height, fmt, &(dataRGB->pixelData[0]));
				if (this->trueColor)
					colour=0;
				else
					colour=pos+1;
				this->controller->add_depth_pointsImage(imageDEPTH,image, world, pos,10,colour);
			}
			mainwindow->resize(1,1);
			while (gtkmain.events_pending()){
      			gtkmain.iteration();
	}
		}
		else{
			this->world->clear_points();
			jderobot::ImageDataPtr dataDEPTH = sources[cam].cDEPTHprx->getImageData();
			colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(dataDEPTH->description->format);
			if (!fmt2)
				throw "Format not supported";
			colorspaces::Image imageDEPTH(dataDEPTH->description->width, dataDEPTH->description->height, fmt2, &(dataDEPTH->pixelData[0]));
			jderobot::ImageDataPtr dataRGB = sources[cam].cRGBprx->getImageData();
			colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(dataRGB->description->format);
			if (!fmt)
				throw "Format not supported";
			colorspaces::Image image(dataRGB->description->width, dataRGB->description->height, fmt, &(dataRGB->pixelData[0]));
			
			this->controller->add_depth_pointsImage(imageDEPTH,image, world, cam,5,0);
			mainwindow->resize(1,1);
			while (gtkmain.events_pending()){
      			gtkmain.iteration();
	}
		}
    

		/*Show window*/
    
    
  }


	void View::pos_x_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value(),cam); 
		vscale_foa_x->set_value((double)this->controller->getFoa(cam)->X);
		vscale_foa_y->set_value((double)this->controller->getFoa(cam)->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa(cam)->Z);
  }

	void View::pos_y_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value(),cam); 
		vscale_foa_x->set_value((double)this->controller->getFoa(cam)->X);
		vscale_foa_y->set_value((double)this->controller->getFoa(cam)->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa(cam)->Z);
  }

	void View::pos_z_changed(){
		this->controller->setPos((float)vscale_pos_x->get_value(), (float)vscale_pos_y->get_value(), (float)vscale_pos_z->get_value(),cam);
		vscale_foa_x->set_value((double)this->controller->getFoa(cam)->X);
		vscale_foa_y->set_value((double)this->controller->getFoa(cam)->Y);
		vscale_foa_z->set_value((double)this->controller->getFoa(cam)->Z); 
  }

	void View::foa_x_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value(),cam); 
  }

	void View::foa_y_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value(),cam); 
  }

	void View::foa_z_changed(){
		this->controller->setFoa((float)vscale_foa_x->get_value(), (float)vscale_foa_y->get_value(), (float)vscale_foa_z->get_value(),cam); 
  }

	void View::fx_changed(){
		this->controller->setFdistX((float)vscale_fx->get_value(),cam); 
  }

	void View::fy_changed(){
		this->controller->setFdistY((float)vscale_fy->get_value(),cam); 
  }

	void View::u0_changed(){
		this->controller->setU0((float)vscale_u0->get_value(),cam); 
  }

	void View::v0_changed(){
		this->controller->setV0((float)vscale_v0->get_value(),cam); 
  }

	void View::roll_changed(){
		this->controller->setRoll((float)vscale_roll->get_value(),cam); 
  }

	void View::button_center_clicked(){
		this->controller->changeDrawCenter();
  }

	int View::getActiveCam(){
		return this->cam;
	}
	void View::combo_changed(){
		Gtk::TreeModel::iterator iter = m_Combo->get_active();
  if(iter)
  {
    Gtk::TreeModel::Row row = *iter;
    if(row)
    {
      //Get the data for the selected row, using our knowledge of the tree
      //model:
      int id = row[m_Columns.m_col_id];
      Glib::ustring name = row[m_Columns.m_col_name];

	  cam=id-1;
    }
  }
		
		double temp_x=(double)this->controller->getFoa(cam)->X;
		double temp_y=(double)this->controller->getFoa(cam)->Y;
		double temp_z=(double)this->controller->getFoa(cam)->Z;
		vscale_pos_x->set_value((double)this->controller->getPos(cam)->X);
		vscale_pos_y->set_value((double)this->controller->getPos(cam)->Y);
		vscale_pos_z->set_value((double)this->controller->getPos(cam)->Z);
		vscale_foa_x->set_value(temp_x);
		vscale_foa_y->set_value(temp_y);
		vscale_foa_z->set_value(temp_z);		
		vscale_fx->set_value((double)this->controller->getFdistX(cam));
		vscale_fy->set_value((double)this->controller->getFdistY(cam));
		vscale_u0->set_value((double)this->controller->getU0(cam));
		vscale_v0->set_value((double)this->controller->getV0(cam));
		vscale_roll->set_value((double)this->controller->getRoll(cam));
}

  
	void View::button_save_clicked(){
		this->controller->saveCameras(this->nCameras);
		
	}

	void View::button_colour_clicked(){
		this->trueColor=this->w_colour->get_active();
	}
}//namespace

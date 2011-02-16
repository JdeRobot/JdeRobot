/*
 *  Copyright (C) 2010 Julio Vega
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
 *  Authors : Julio Vega <julio.vega@urjc.es>
 *
 */

#include "view.h"

namespace teleoperator {
	View::View(Controller * controller): gtkmain(0,0) {
		/*Create controller*/
		this->controller = controller;

		/*Init OpenGL*/
		if(!Gtk::GL::init_check(NULL, NULL))	{
			std::cerr << "Couldn't initialize GL\n";
			std::exit(1);
		}

    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(controller->getGladePath());

		/*Get widgets*/
    refXml->get_widget("mainwindow", mainwindow);

		// Base del robot
    refXml->get_widget("stopButton", stopButton);
    refXml->get_widget("upButton", upButton);
    refXml->get_widget("downButton", downButton);
    refXml->get_widget("leftButton", leftButton);
    refXml->get_widget("rightButton", rightButton);

		// Giraffe
    refXml->get_widget("stopButton1", stopButton1);
    refXml->get_widget("upButton1", upButton1);
    refXml->get_widget("downButton1", downButton1);
    refXml->get_widget("leftButton1", leftButton1);
    refXml->get_widget("rightButton1", rightButton1);

		// Imagen de la cámara
    refXml->get_widget("image",gtk_image);
    refXml->get_widget("image2",gtk_image2);

		// Mundo OpenGL
    refXml->get_widget_derived("world",world);

		// Create callbacks
		//g_signal_connect (G_OBJECT (world), "button_press_event", G_CALLBACK (button_press_event), NULL);
		//g_signal_connect (G_OBJECT (world), "motion_notify_event", G_CALLBACK (motion_notify_event), NULL);
		//g_signal_connect (G_OBJECT (world), "scroll-event", G_CALLBACK (scroll_event), world);
        
		stopButton->signal_clicked().connect(sigc::mem_fun(this,&View::stopButton_clicked));
		upButton->signal_clicked().connect(sigc::mem_fun(this,&View::upButton_clicked));
		downButton->signal_clicked().connect(sigc::mem_fun(this,&View::downButton_clicked));
		leftButton->signal_clicked().connect(sigc::mem_fun(this,&View::leftButton_clicked));
		rightButton->signal_clicked().connect(sigc::mem_fun(this,&View::rightButton_clicked));

		stopButton1->signal_clicked().connect(sigc::mem_fun(this,&View::stopButton1_clicked));
		upButton1->signal_clicked().connect(sigc::mem_fun(this,&View::upButton1_clicked));
		downButton1->signal_clicked().connect(sigc::mem_fun(this,&View::downButton1_clicked));
		leftButton1->signal_clicked().connect(sigc::mem_fun(this,&View::leftButton1_clicked));
		rightButton1->signal_clicked().connect(sigc::mem_fun(this,&View::rightButton1_clicked));

		/*Show window. Note: Set window visibility to false in Glade, otherwise opengl won't work*/
		mainwindow->show();
	}

	View::~View() {
		delete this->controller;
	}

  void View::display(const colorspaces::Image& image1, const colorspaces::Image& image2) {
		this->controller->drawWorld(image1, image2); // dibujar la imagen de la cámara

		this->getEncoders(); // cogemos del controller
		this->getSonars();
		this->getLaser();

		this->setEncoders(); // ponemos en el drawarea
		this->setSonars();
		this->setLaser();

		/*Set image1*/
		colorspaces::ImageRGB8 img_rgb8881(image1);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff1 = Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8881.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8881.width,
				    img_rgb8881.height,
				    img_rgb8881.step); 
    gtk_image->clear();
    gtk_image->set(imgBuff1);

		/*Set image2*/
		colorspaces::ImageRGB8 img_rgb8882(image2);//conversion will happen if needed
		Glib::RefPtr<Gdk::Pixbuf> imgBuff2 = Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8882.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8882.width,
				    img_rgb8882.height,
				    img_rgb8882.step); 
    gtk_image2->clear();
    gtk_image2->set(imgBuff2);

		/*Show window*/
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }

	void View::getEncoders () {
		this->robotx = this->controller->ed->robotx;
		this->roboty = this->controller->ed->roboty;
		this->robottheta = this->controller->ed->robottheta;
	}

	void View::getSonars () {
		int k;
		this->us.clear ();
		this->numSonars = 0;
		for (k = 0; k < this->controller->sd->numSonars; k++) {
			this->us.push_back (this->controller->sd->us[k]);
			this->numSonars ++;
		}
	}

	void View::getLaser () {
		int k;
		this->distanceData.clear();
		this->numLasers = 0;
		for (k = 0; k < this->controller->ld->numLaser; k++) {
			this->distanceData.push_back (this->controller->ld->distanceData[k]);
			this->numLasers ++;
		}
	}

	void View::setEncoders () {
		this->world->robotx = this->robotx;
		this->world->roboty = this->roboty;
		this->world->robottheta = this->robottheta;
	}

	void View::setSonars () {
		int k;
		this->world->us.clear ();
		this->world->numSonars = 0;
		for (k = 0; k < this->numSonars; k++) {
			this->world->us.push_back (this->us[k]);
			this->world->numSonars ++;
		}
	}

	void View::setLaser () {
		int k;
		this->world->distanceData.clear();
		this->world->numLasers = 0;
		for (k = 0; k < this->numLasers; k++) {
			this->world->distanceData.push_back (this->distanceData[k]);
			this->world->numLasers ++;
		}
	}

  bool View::isVisible() {
    return mainwindow->is_visible();
  }

	void View::stopButton_clicked() {
		this->controller->stopMotors();
	}

	void View::upButton_clicked() {
		this->controller->goUp();
	}

	void View::downButton_clicked() {
		this->controller->goDown();
	}

	void View::leftButton_clicked() {
		this->controller->goLeft();
	}

	void View::rightButton_clicked() {
		this->controller->goRight();
	}

	void View::stopButton1_clicked() {
		this->controller->stopMotors1();
	}

	void View::upButton1_clicked() {
		this->controller->goUp1();
	}

	void View::downButton1_clicked() {
		this->controller->goDown1();
	}

	void View::leftButton1_clicked() {
		this->controller->goLeft1();
	}

	void View::rightButton1_clicked() {
		this->controller->goRight1();
	}
  
}//namespace

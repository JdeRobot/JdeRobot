/*
 *  Copyright (C) 1997-2011 JDERobot Developers Team
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

namespace introrob {
	View::View (Controller* controller, Navegacion* navegacion): gtkmain(0,0) {
		this->controller = controller;
		this->navegacion = navegacion;

		this->isFollowing = false;

		/*Init OpenGL*/
		if(!Gtk::GL::init_check(NULL, NULL))	{
			std::cerr << "Couldn't initialize GL\n";
			std::exit(1);
		}

    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(controller->getGladePath());

		/*Get widgets*/
    refXml->get_widget("mainwindow", mainwindow);

		// Botones para base del robot
    refXml->get_widget("stopButton", stopButton);
    refXml->get_widget("upButton", upButton);
    refXml->get_widget("downButton", downButton);
    refXml->get_widget("leftButton", leftButton);
    refXml->get_widget("rightButton", rightButton);

		// Botones para cámaras de OGL
    refXml->get_widget("camera1Button", camera1Button);
    refXml->get_widget("camera2Button", camera2Button);
    refXml->get_widget("camera3Button", camera3Button);
    refXml->get_widget("camera4Button", camera4Button);
    refXml->get_widget("pioneerCameraButton", pioneerCameraButton);

    refXml->get_widget("yourCodeButton", yourCodeButton);
    refXml->get_widget("stopCodeButton", stopCodeButton);
    refXml->get_widget("exitButton", exitButton);
    refXml->get_widget("deleteLinesButton", deleteLinesButton);

		// Imágenes de la cámara
    refXml->get_widget("image1",gtk_image1);
    refXml->get_widget("image2",gtk_image2);

		// Eventbox para las imágenes
    refXml->get_widget("eventbox_left",eventbox_left);
    eventbox_left->add_events(Gdk::BUTTON_PRESS_MASK);
    eventbox_left->signal_button_press_event().connect(sigc::mem_fun(this, &View::on_left_clicked));

    refXml->get_widget("eventbox_right",eventbox_right);
    eventbox_right->add_events(Gdk::BUTTON_PRESS_MASK);
    eventbox_right->signal_button_press_event().connect(sigc::mem_fun(this, &View::on_right_clicked));
	
		// Mundo OpenGL
    refXml->get_widget_derived("world",this->world);
		this->world->navega = this->navegacion->navega;

		stopButton->signal_clicked().connect(sigc::mem_fun(this,&View::stopButton_clicked));
		upButton->signal_clicked().connect(sigc::mem_fun(this,&View::upButton_clicked));
		downButton->signal_clicked().connect(sigc::mem_fun(this,&View::downButton_clicked));
		leftButton->signal_clicked().connect(sigc::mem_fun(this,&View::leftButton_clicked));
		rightButton->signal_clicked().connect(sigc::mem_fun(this,&View::rightButton_clicked));
		camera1Button->signal_clicked().connect(sigc::mem_fun(this,&View::camera1Button_clicked));
		camera2Button->signal_clicked().connect(sigc::mem_fun(this,&View::camera2Button_clicked));
		camera3Button->signal_clicked().connect(sigc::mem_fun(this,&View::camera3Button_clicked));
		camera4Button->signal_clicked().connect(sigc::mem_fun(this,&View::camera4Button_clicked));
		pioneerCameraButton->signal_clicked().connect(sigc::mem_fun(this,&View::pioneerCameraButton_clicked));
		yourCodeButton->signal_clicked().connect(sigc::mem_fun(this,&View::yourCodeButton_clicked));
		stopCodeButton->signal_clicked().connect(sigc::mem_fun(this,&View::stopCodeButton_clicked));
		exitButton->signal_clicked().connect(sigc::mem_fun(this,&View::exitButton_clicked));
		deleteLinesButton->signal_clicked().connect(sigc::mem_fun(this,&View::deleteLinesButton_clicked));

		/*Show window. Note: Set window visibility to false in Glade, otherwise opengl won't work*/
		mainwindow->show();
		stopCodeButton->hide();

		this->navegacion->initCameras();
	}

	View::~View() {
		delete this->controller;
		delete this->world;
	}

	/** prepare2draw ************************************************************************
	* Prepare an image to draw it with a GtkImage in BGR format with 3 pixels per byte.	*
	*	src: source image								*
	*	dest: destiny image. It has to have the same size as the source image and	*
	*	      3 bytes per pixel.
	*****************************************************************************************/
	void View::prepare2draw (IplImage &image) {
		int i;
		int pos;
		char temp;

		for (i=0; i<image.width*image.height; i++) {
			pos = i*3;
			temp = image.imageData[pos+2];
			image.imageData[pos+2]=image.imageData[pos];
			image.imageData[pos]=temp;
		}
	}

  void View::display(const colorspaces::Image& image1, const colorspaces::Image& image2) {
		this->getEncoders(); // cogemos del controller
		this->getLaser();

		this->setEncoders(); // ponemos en el drawarea
		this->setLaser();
		this->setCamara(image1, 1);
		this->setCamara(image2, 2);

		if (this->isFollowing == true)
			this->world->setToPioneerCamera();

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

	void View::getLaser () {
		int k;
		this->distanceData.clear();
		this->numLasers = 0;
		for (k = 0; k < this->controller->ld->numLaser; k++) {
			this->distanceData.push_back (this->controller->ld->distanceData[k]);
			this->numLasers ++;
		}
	}

	void View::setDestino () {
		this->world->getDestino(&this->destino);
	}

	void View::setEncoders () {
		this->world->robotx = this->robotx;
		this->world->roboty = this->roboty;
		this->world->robottheta = this->robottheta;
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

	void View::setCamara (const colorspaces::Image& image, int id) {
		// Set image
		IplImage src; // conversión a IplImage
		src = image;

		prepare2draw (src); // cambio del orden de bytes

		colorspaces::ImageRGB8 img_rgb888(image); // conversion will happen if needed

		Glib::RefPtr<Gdk::Pixbuf> imgBuff = Gdk::Pixbuf::create_from_data((const guint8*)img_rgb888.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb888.width,
				    img_rgb888.height,
				    img_rgb888.step);

		if (id == 1) {
		  gtk_image1->clear();
		  gtk_image1->set(imgBuff);
		} else {
		  gtk_image2->clear();
		  gtk_image2->set(imgBuff);
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

	void View::camera1Button_clicked() {
		this->isFollowing = false;
		this->world->setToCamera1();
	}

	void View::camera2Button_clicked() {
		this->isFollowing = false;
		this->world->setToCamera2();
	}

	void View::camera3Button_clicked() {
		this->isFollowing = false;
		this->world->setToCamera3();
	}

	void View::camera4Button_clicked() {
		this->isFollowing = false;
		this->world->setToCamera4();
	}

	void View::pioneerCameraButton_clicked() {
		this->isFollowing = true;
		this->world->setToPioneerCamera();
	}

	void View::yourCodeButton_clicked() {
		/*Create a thread with the students algorithm */
		this->navegacion->start();
		yourCodeButton->hide();
		stopCodeButton->show();
	}

	void View::stopCodeButton_clicked() {
		this->navegacion->stop();
		yourCodeButton->show();
		stopCodeButton->hide();
	}

	void View::exitButton_clicked() {
		this->navegacion->stop();
		this->navegacion->join();
		mainwindow->hide();
		exit (0);
	}

	void View::deleteLinesButton_clicked() {
		this->navegacion->resetLines ();
	}

	bool View::on_right_clicked(GdkEventButton * event){
		gint x,y;
		gdk_window_at_pointer(&x,&y);

		pixB.x=x;
		pixB.y=y;
		pixB.h=1.0;
		//printf("click en camera derecha, punto %f,%f\n",pixB.x,pixB.y);
		this->navegacion->calculate_projection_line(pixB,2);
		return true;
	}

	bool View::on_left_clicked(GdkEventButton * event){
		gint x,y;
		gdk_window_at_pointer(&x,&y);

		pixA.x=x;
		pixA.y=y;
		pixA.h=1.0;
		//printf("click en camera izquierda, punto %f,%f\n",pixA.x,pixA.y);
		this->navegacion->calculate_projection_line(pixA,1);
		return true;
	}


} // namespace

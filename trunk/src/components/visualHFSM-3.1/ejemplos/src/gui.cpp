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
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>
 *
 */

#include "gui.h"

namespace mycomponent {
    Gui::Gui (Control* control): gtkmain(0,0) {
        
        this->yourCode_button_isPressed=false;
	this->control=control;
        this->showCanvasWin = false; 
	    /*Init OpenGL*/
	if(!Gtk::GL::init_check(NULL, NULL))	{
	    std::cerr << "Couldn't initialize GL\n";
	    std::exit(1);
	}
        Gnome::Canvas::init(); 

	std::cout << "Loading glade\n";
	this->gladepath=std::string("./mycomponent.glade");
	refXml = Gnome::Glade::Xml::create("./mycomponent.glade");

	//Get widgets
        
        refXml->get_widget("secondarywindow", secondarywindow);


        // Button, area and window canvas teleoperate robot
       	refXml->get_widget("windowTeleoperate", windowTeleoperate);
        refXml->get_widget("stop_button", stop_button); //canvas
       	refXml->get_widget_derived("canvas_control", this->canvas_control); //canvas
        canvas_control->moved=1;

        // Button, area and window canvas teleoperate cameras
       	refXml->get_widget_derived("canvas_control_cameras", this->canvas_control_cameras); //canvas
        


	refXml->get_widget("yourCodeButton", yourCodeButton);
	refXml->get_widget("stopCodeButton", stopCodeButton);
	refXml->get_widget("exitButton", exitButton);

	// Camera images
	refXml->get_widget("image1",gtk_image1);
	refXml->get_widget("image2",gtk_image2);


	yourCodeButton->signal_clicked().connect(sigc::mem_fun(this,&Gui::yourCodeButton_clicked));
	stopCodeButton->signal_clicked().connect(sigc::mem_fun(this,&Gui::stopCodeButton_clicked));
	exitButton->signal_clicked().connect(sigc::mem_fun(this,&Gui::exitButton_clicked));
        stop_button->signal_clicked().connect(sigc::mem_fun(this,&Gui::stop_button_clicked));

	stopCodeButton->hide();
        windowTeleoperate->show();
        secondarywindow->show();
        
        camera *mycameraA = new camera("cameras/calibA");
        myCamA= mycameraA->readConfig();
        camera *mycameraB = new camera("cameras/calibB");
	myCamB= mycameraB->readConfig();

    }

	Gui::~Gui() {
/*	 
		delete this->controller;
		delete this->world;
*/	
	}



    void Gui::display(const colorspaces::Image& image1, const colorspaces::Image& image2) {

	setCamara(image1, 1);
	setCamara(image2, 2);
        
        if(!(this->yourCode_button_isPressed))
            this->teleOperate();
        //else
            //control->iterationControl(1);

            
        this->teleOperateCameras();
	while (gtkmain.events_pending())
	    gtkmain.iteration();
    }
    
    void Gui::teleOperate(){
        double k=-0.01;
        double p=0.5;
        double v,w,v_normalized,w_normalized;
        
        
        w=canvas_control->previous_x;
        v=canvas_control->previous_y;
        
        v_normalized=400*(k*v+p);
        w_normalized=20*(k*w+p);
        this->control->mprx->setV(v_normalized);
        this->control->mprx->setW(w_normalized);
    }
    
        void Gui::teleOperateCameras(){
        double k=-0.01;
        double p=0.5;
        double v,w,v_normalized,w_normalized;
        jderobot::PTMotorsData* myData;
        jderobot::PTMotorsData* myData2;
      
        
        w=canvas_control_cameras->previous_x;
        v=canvas_control_cameras->previous_y;
        v_normalized=28*(k*v+p);
        w_normalized=-45*(k*w+p);
        myData = new jderobot::PTMotorsData ();
        myData2 = new jderobot::PTMotorsData ();
        myData->latitude = v_normalized;
        myData->longitude = w_normalized;
        myData2->latitude = v_normalized;
        myData2->longitude = w_normalized;
        this->control->ptmprx1->setPTMotorsData (myData);
        this->control->ptmprx2->setPTMotorsData (myData2);
}

    void Gui::setCamara (const colorspaces::Image& image, int id) {
	    // Set image
	    IplImage src; // conversión a IplImage
	    src = image;

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

    bool Gui::isVisible() {
	return windowTeleoperate->is_visible();
    }
    void Gui::yourCodeButton_clicked() {
	    yourCodeButton->hide();
	    stopCodeButton->show();
            yourCode_button_isPressed=true;
    }

    void Gui::stopCodeButton_clicked() {
            control->resetControl();
	    yourCodeButton->show();
	    stopCodeButton->hide();
                        yourCode_button_isPressed=false;

            
    }

    void Gui::exitButton_clicked() {
	    windowTeleoperate->hide();
	    exit (0);
    }
    void Gui::stop_button_clicked(){
        if(canvas_control->moved)
            this->canvas_control->m_text->move(50-canvas_control->previous_x,50-canvas_control->previous_y);
        canvas_control->moved=0;
        this->control->mprx->setV(0);
        this->control->mprx->setW(0);
        canvas_control->previous_x=50;
        canvas_control->previous_y=50;
        yourCode_button_isPressed=false;
    }
} // namespace

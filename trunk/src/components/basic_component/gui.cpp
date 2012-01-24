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
    Gui::Gui (Api *api): gtkmain(0,0) {
        this->yourCode_button_isPressed=false;
        press_stop_button=false;
        press_stopCodeButton=false;
        this->showCanvasWin = false;
	this->api=api;
	    /*Init OpenGL*/
	if(!Gtk::GL::init_check(NULL, NULL))	{
	    std::cerr << "Couldn't initialize GL\n";
	    std::exit(1);
	}
        Gnome::Canvas::init();
	std::cout << "Loading glade\n";
	this->gladepath=std::string("./basic_component.glade");
	refXml = Gnome::Glade::Xml::create("./basic_component.glade");

	//Get widgets       
        refXml->get_widget("secondarywindow", secondarywindow);


        // Button, area and window canvas teleoperate robot
       	refXml->get_widget("windowTeleoperate", windowTeleoperate);
        refXml->get_widget("stop_button", stop_button); //canvas
       	refXml->get_widget_derived("canvas_control", this->canvas_control); //canvas
        canvas_control->moved=1;
	
	// Area canvas laser
	refXml->get_widget("windowLaser", windowLaser);
	refXml->get_widget_derived("canvas_laser", this->canvas_laser); //canvas


        // Button, area and window canvas teleoperate cameras
       	refXml->get_widget_derived("canvas_control_cameras", this->canvas_control_cameras); //canvas
	refXml->get_widget("yourCodeButton", yourCodeButton);
	refXml->get_widget("stopCodeButton", stopCodeButton);
	refXml->get_widget("exitButton", exitButton);

	// Camera images
	refXml->get_widget("image1",gtk_image1);
	refXml->get_widget("image2",gtk_image2);

	// Events
	yourCodeButton->signal_clicked().connect(sigc::mem_fun(this,&Gui::yourCodeButton_clicked));
	stopCodeButton->signal_clicked().connect(sigc::mem_fun(this,&Gui::stopCodeButton_clicked));
	exitButton->signal_clicked().connect(sigc::mem_fun(this,&Gui::exitButton_clicked));
        stop_button->signal_clicked().connect(sigc::mem_fun(this,&Gui::stop_button_clicked));

	// Init buttons
	stopCodeButton->hide();
        windowTeleoperate->show();
        secondarywindow->show();
	//windowLaser->show();

	//Calibrate cameras        
        camera *mycameraA = new camera("cameras/calibA");
        myCamA= mycameraA->readConfig();
        camera *mycameraB = new camera("cameras/calibB");
	myCamB= mycameraB->readConfig();

    }

	Gui::~Gui() {
		
	}
        
    void Gui::ShowImages(Api *api) {
        pthread_mutex_lock(&api->controlGui);
        this->updateCameras(api);
        pthread_mutex_unlock(&api->controlGui);        
	setCamara(*this->image1, 1);
	setCamara(*this->image2, 2);
    }

   void Gui::updateCameras(Api *api){
  
      colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(api->imageData1->description->format);
      if (!fmt1)
         throw "Format not supported";
      this->image1 = new colorspaces::Image (api->imageData1->description->width, api->imageData1->description->height, fmt1, &(api->imageData1->pixelData[0])); // Prepare the image to use with openCV

      colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(api->imageData2->description->format);
      if (!fmt2)
         throw "Format not supported";
      this->image2 = new colorspaces::Image (api->imageData2->description->width, api->imageData2->description->height, fmt2, &(api->imageData2->pixelData[0])); // Prepare the image to use with openCV
              
   }

    void Gui::display(Api *api) {

        if(!(this->yourCode_button_isPressed)){
            this->teleOperate(api);
            //pthread_mutex_lock(&control->controlGui);
            api->iterationControlActivated=false;
            //pthread_mutex_unlock(&control->controlGui);
        }
        else{
            //pthread_mutex_lock(&control->controlGui);	    
            api->iterationControlActivated=true;
            //pthread_mutex_unlock(&control->controlGui);
        }
	api->guiVisible=windowTeleoperate->is_visible();

        this->teleOperateCameras(api);
        while (gtkmain.events_pending())
            gtkmain.iteration();	

    }
        
     void Gui::teleOperate(Api *api){
        
        double k=-0.01;
        double p=0.5;
        double v,w,v_normalized,w_normalized;
        
        w=canvas_control->previous_x;
        v=canvas_control->previous_y;
        v_normalized=400*(k*v+p);
        w_normalized=20*(k*w+p);
	
        //pthread_mutex_lock(&control->controlGui);
        api->setMotorV(v_normalized);
        api->setMotorW(w_normalized);	
        //pthread_mutex_unlock(&control->controlGui);
        
    }
       
    void Gui::teleOperateCameras(Api *api){
        
        double k=-0.01;
        double p=0.5;
        double v,w;
      
        w=canvas_control_cameras->previous_x;
        v=canvas_control_cameras->previous_y;
	
        //pthread_mutex_lock(&control->controlGui);
        api->v_normalized=28*(k*v+p);
        api->w_normalized=-45*(k*w+p);
        //pthread_mutex_unlock(&control->controlGui);
        
    }
    
    void Gui::updateCanvasLaser(){
        /*	
        //pthread_mutex_lock(&control->controlGui);
        canvas_laser->a=this->control->laserDataGui->numLaser;
	canvas_laser->laserDistances=this->control->laserDataGui->distanceData;
	
	//canvas_laser->laserDataDistance=this->control->laserDataGui->distanceData;
	//canvas_laser->control=this->control;
	//printf("%d\n",this->control->laserDataGui->distanceData[90]);
        //pthread_mutex_unlock(&control->controlGui);
        */
	
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

    void Gui::isVisible() {
    }
    
    void Gui::yourCodeButton_clicked() {
        
        yourCodeButton->hide();
        stopCodeButton->show();
        canvas_control->hide();
        stop_button->hide();
        yourCode_button_isPressed=true;
	//api->iterationControlActivated=true;
        
    }

    void Gui::stopCodeButton_clicked() {
        
        yourCodeButton->show();
        stopCodeButton->hide();
        canvas_control->show();
        stop_button->show();
        press_stop_button=true;
	resetAPI(this->api);
        this->yourCode_button_isPressed=false;
	//api->iterationControlActivated=false;
        
        
    }

    void Gui::exitButton_clicked() {
        windowTeleoperate->hide();
        exit (0);
    }
    
    void Gui::stop_button_clicked(){
        
        if(canvas_control->moved)
            this->canvas_control->m_text->move(50-canvas_control->previous_x,50-canvas_control->previous_y);
        canvas_control->moved=0;
        press_stop_button=true;
        canvas_control->previous_x=50;
        canvas_control->previous_y=50;
        resetAPI(this->api);
    }

    void Gui::resetAPI(Api *api){
        
        api->setMotorV(0);
        api->setMotorW(0);
        api->setMotorL(0);
        
    }
    
    
} // namespace    
       

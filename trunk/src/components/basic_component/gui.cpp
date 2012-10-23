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

namespace basic_component {
    Gui::Gui (Api *api): gtkmain(0,0) {

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

	// Camera images
	refXml->get_widget("image1",gtk_image1);
	//refXml->get_widget("image2",gtk_image2);

	//Calibrate cameras        
        camera *mycameraA = new camera("cameras/calibA");
        myCamA= mycameraA->readConfig();
        //camera *mycameraB = new camera("cameras/calibB");
	//myCamB= mycameraB->readConfig();

        secondarywindow->show();
        
    }

	Gui::~Gui() {
		
	}
        
    void Gui::ShowImages(Api *api) {
        pthread_mutex_lock(&api->controlGui);
        this->updateCameras(api);
        pthread_mutex_unlock(&api->controlGui);        
	setCamara(*this->image1, 1);
	//setCamara(*this->image2, 2);
    }

   void Gui::updateCameras(Api *api){
  
      colorspaces::Image::FormatPtr fmt1 = colorspaces::Image::Format::searchFormat(api->imageData1->description->format);
      if (!fmt1)
         throw "Format not supported";
      this->image1 = new colorspaces::Image (api->imageData1->description->width, api->imageData1->description->height, fmt1, &(api->imageData1->pixelData[0])); // Prepare the image to use with openCV
/*
      colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(api->imageData2->description->format);
      if (!fmt2)
         throw "Format not supported";
      this->image2 = new colorspaces::Image (api->imageData2->description->width, api->imageData2->description->height, fmt2, &(api->imageData2->pixelData[0])); // Prepare the image to use with openCV
*/              
   }

    void Gui::display(Api *api) {

        while (gtkmain.events_pending())
            gtkmain.iteration();	

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
        
    
} // namespace    
       

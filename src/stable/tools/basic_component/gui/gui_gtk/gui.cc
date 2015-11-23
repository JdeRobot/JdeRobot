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

extern "C" IGui* create_gui(int a, char** v)
{
    return new Gui();
}

extern "C" void destroy_gui( IGui* object )
{
    delete object;
}

Gui::Gui() : gtkmain(0, 0) {

    //std::cout << "Loading glade\n";

    refXml = Gnome::Glade::Xml::create("./basic_component.glade");
    //Get widgets       
    refXml->get_widget("secondarywindow", secondarywindow);
    // Camera images
    refXml->get_widget("image1", gtk_image1);

    secondarywindow->show();
}

void Gui::mysleep(unsigned long milisec) {
		
    //unsigned long milisec = 3000L;
    struct timespec req={0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    while(nanosleep(&req,&req)==-1)
        continue;
}


int Gui::runGui(basic_component::Shared* sm) {

    this->sm = sm;
    while(this->secondarywindow->get_visible()) 
	;//update();

    //the gui window has been closed.
    this->sm->setClosed(true);

    return 0;
}

Gui::~Gui() { 

}



void Gui::ShowImage() {

    //needed to avoid a segfault with the image stored in the shared memory... no idea why (?¿)
    mysleep(15);
    this->image1 = this->sm->getImage();

    setCamara(this->image1, 1);
}

void Gui::update() {

    ShowImage();
    while (gtkmain.events_pending())
    	gtkmain.iteration();
}

// First parameter is the widget which will show the image and the id indicates which widget is. This is useful when we have
// two cameras and we want to choose which one will offer us the image.
void Gui::setCamara(const cv::Mat _image, int id) {

    // Set image
     imgBuff = Gdk::Pixbuf::create_from_data((const guint8*) _image.data,
        Gdk::COLORSPACE_RGB,
        false,
        8,
        _image.cols,
        _image.rows,
        _image.step);

    if (id == 1) {
    	gtk_image1->clear();
    	gtk_image1->set(imgBuff);
    } else {
    	gtk_image2->clear();
    	gtk_image2->set(imgBuff); 
    }
}

//    void Gui::isVisible() {
//    }

  


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
 *            Francisco Pérez <f.perez475@gmail.com>
 *
 */

#include "gui.h"

namespace basic_component {

    Gui::Gui(Shared* sm) : gtkmain(0, 0) {

        this->sm = sm;

        std::cout << "Loading glade\n";

        refXml = Gnome::Glade::Xml::create("./basic_component.glade");
        //Get widgets       
        refXml->get_widget("secondarywindow", secondarywindow);
        // Camera images
        refXml->get_widget("image1", gtk_image1);

        secondarywindow->show();
    }

    Gui::~Gui() {

    }

    void Gui::ShowImage() {
	this->image = this->sm->getImage();
        setCamara(this->image, 1);
    }

    void Gui::display() {

        ShowImage();
        while (gtkmain.events_pending())
            gtkmain.iteration();

    }

    // First parameter is the widget which will show the image and the id indicates which widget is. This is useful when we have
    // two cameras and we want to choose which one will offer us the image.
    void Gui::setCamara(const cv::Mat image, int id) {

        // Set image
        Glib::RefPtr<Gdk::Pixbuf> imgBuff = Gdk::Pixbuf::create_from_data((const guint8*) image.data,
                Gdk::COLORSPACE_RGB,
                false,
                8,
                image.cols,
                image.rows,
                image.step);

        if (id == 1) {
            gtk_image1->clear();
            gtk_image1->set(imgBuff);
        } else {
            gtk_image2->clear();
            gtk_image2->set(imgBuff);
        }

    }

} // namespace    


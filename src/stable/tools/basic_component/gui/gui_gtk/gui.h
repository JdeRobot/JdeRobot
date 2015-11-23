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

#ifndef BASIC_COMPONENT_GUI_H
#define BASIC_COMPONENT_GUI_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <gtk-2.0/gtk/gtk.h>
#include <gtk-2.0/gdk/gdk.h>
#include <gtkmm-2.4/gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include <libglademm.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include "../../shared.h"
#include "../igui.h"


class Gui : public IGui {
public:

    Gui();
    virtual ~Gui();

    int runGui(basic_component::Shared* sm);
    //Public Methods
    //void isVisible();
    void update();

    //void updateCameras(Shared* sm);

private:

    basic_component::Shared* sm;
    cv::Mat image1; // Image camera1 processed to manipulate with openCV

    //GTK stuff
    Gtk::Main gtkmain;
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    std::string gladepath;
    // Windows
    Gtk::Window *secondarywindow;
    Glib::RefPtr<Gdk::Pixbuf> imgBuff;
    // Cameras
    Gtk::Image *gtk_image1;
    Gtk::Image *gtk_image2;

    // Private Methods
    void setCamara(const cv::Mat image, int id);
    void ShowImage();
    void mysleep(unsigned long);

}; //class

#endif //BASIC_COMPONENT_GUI_H

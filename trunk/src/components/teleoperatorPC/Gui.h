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
 *  Authors : Maikel González <m.gonzalezbai@gmail.com>,
 *              José María Cañas <jmplaza@gsyc.es>
 *
 */

#ifndef GUI_H
#define	GUI_H

#include <colorspaces/colorspacesmm.h>
#include<gtk-2.0/gtk/gtk.h>
#include<gtk-2.0/gdk/gdk.h>
#include<gtkmm-2.4/gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

#include <math.h>
#include <string>
#include <iostream>
#include <sstream>

#include "SharedMemory.h"

class Gui {
public:
    Gui(SharedMemory *interfacesData);
    Gui(const Gui& orig);
    void display();
    virtual ~Gui();
private:
    cv::Mat imagenO;
    Gtk::Window* main_window;
    //MOTORS
    Gtk::Window* motors_window;
    Gtk::Label* motors_linear_velocity;
    Gtk::Label* motors_rot_velocity;
    Gtk::DrawingArea* motors_canvas;
    Gtk::Button* stop_button;
    Gtk::CheckButton* check_motors;
    Gtk::CheckButton* checkICEmotors;
    bool motorsShowed;
    bool on_button_press_motors_canvas(GdkEvent* event);
    void stop_button_clicked();
    int previous_event_x, previous_event_y, cameras_previous_event_x, cameras_previous_event_y;
    void displayMotorsData();
    void motors_handler();
    void widget2motors(int x, int y, int* v, int* w);
    void widget2Pose3Dmotors(int x, int y, float* v, float* w);
    void check_motors_clicked();
    void check_ICEmotors_clicked();

    typedef struct motorsData_t {
        double v;
        double w;
        double l;
    } motorsData_t;
    motorsData_t motorsData;
    //END_MOTORS 

    //ENCODERS
    Gtk::Window* encoders_window;
    Gtk::CheckButton* check_encoders;
    Gtk::CheckButton* checkICEencoders;    
    Gtk::Label* encoders_x;
    Gtk::Label* encoders_y;
    Gtk::Label* encoders_theta;
    bool encodersShowed;
    void displayEncodersData();
    void check_encoders_clicked();
    void check_ICEencoders_clicked();
    //END_ENCODERS   

    //LASER
    Gtk::DrawingArea* laser_canvas;
    Gtk::Window* laser_window;
    Gtk::ToggleButton* check_laser;
    void check_laser_clicked();
    void drawLaser();
    bool laserShowed;
    //END_LASER

    //CAMERAS
    Gtk::Window* cameras_window;
    Gtk::Image* image_left_camera;
    Gtk::Image* image_right_camera;
    Gtk::DrawingArea* cameras_canvas;
    Gtk::CheckButton* check_left_camera;
    Gtk::CheckButton* check_right_camera;
    Gtk::CheckButton* check_cameras;
    Gtk::Button* cameras_stop_button;
    void cameras_stop_button_clicked();
    void check_cameras_clicked();
    void displayCameras();
    bool on_button_press_cameras_canvas(GdkEvent* event);
    void cameras_handler();
    void displayPose3DEncodersData();
    void Pose3Dencoders2widget(float* v, float* w);
    bool camerasShowed;
    //END_CAMERAS


    //CHECK ICE
    Gtk::CheckButton* checkICElaser;
    void check_ICElaser_clicked();
    Gtk::CheckButton* checkICECameras;
    void check_ICECameras_clicked();
    Gtk::CheckButton* checkICEpose3DEncoders;
    void check_ICEpose3DEncoders_clicked();
    Gtk::CheckButton* checkICEpose3DMotors;
    void check_ICEpose3DMotors_clicked();

    //OTHERS
    Gtk::Main gtkmain;
    Glib::RefPtr<Gtk::Builder> builder;
    Gtk::Button* button_exit;
    void exit_button_clicked();
    SharedMemory *interfacesData;
    std::string float2string(float n);
    Gtk::MessageDialog* help_window;

};

#endif	/* GUI_H */


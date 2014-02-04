/*
 *  Copyright (C) 1997-2013 JDERobot Developers Team
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
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors : Borja Menéndez <borjamonserrano@gmail.com>
 *
 */

#ifndef NAOOPERATOR_H
#define NAOOPERATOR_H

#include <map>
#include <iostream>
#include <stdio.h>
#include <gtkmm.h>
#include <gtkmm/stock.h>
#include <goocanvasmm-2.0/goocanvasmm.h>
#include <opencv2/core/core.hpp>

#include "control.h"
#include "common.h"

class NaoOperator : public Gtk::Window {
public:
    // Constructor
    NaoOperator ( Control* control );
    
    // Destructor
    virtual ~NaoOperator ();
    
    // Another functions
    void display ( cv::Mat& image );
    bool isFollowingBall ();

private:
    Gtk::Window* mainwindow;
    Gtk::Button *head_up, *head_down, *head_right, *head_left, *head_stop;
    Gtk::Button *move_front, *move_back, *move_right, *move_left, *move_stop;
    Gtk::Button *standup_back, *standup_front, *right_kick, *left_kick;
    Gtk::Button *button_lat_left, *button_lat_right;
    Gtk::Button *intercept, *change_camera, *reset_naoqi;
    Gtk::Image* camera_image;
    Gtk::Viewport *viewport_move, *viewport_head;
    Gtk::CheckButton* checkbutton_followball;
    
    Goocanvas::Canvas *canvas_head, *canvas_move;
    Glib::RefPtr<Goocanvas::ItemModel> root_head;
    Glib::RefPtr<Goocanvas::ItemModel> root_move;
    Glib::RefPtr<Goocanvas::PolylineModel> h_head, v_head, h_move, v_move;
    bool drag;
    
    Control* control;
    
    std::map<Hinge, Gtk::Scale*> mapScales;
    
    int getAllWidgets ();
    void assignSignals ();
    
    void create_lines_move ();
    void create_lines_head ();
    
    void on_scale_pitch_left_shoulder ();
    void on_scale_roll_left_shoulder ();
    void on_scale_yawpitch_left_hip ();
    void on_scale_pitch_left_hip ();
    void on_scale_roll_left_hip ();
    void on_scale_yawpitch_right_hip ();
    void on_scale_pitch_right_hip ();
    void on_scale_roll_right_hip ();
    void on_scale_pitch_right_shoulder ();
    void on_scale_roll_right_shoulder ();
    void on_scale_yaw_left_elbow ();
    void on_scale_roll_left_elbow ();
    void on_scale_pitch_left_knee ();
    void on_scale_pitch_right_knee ();
    void on_scale_yaw_right_elbow ();
    void on_scale_roll_right_elbow ();
    void on_scale_pitch_left_ankle ();
    void on_scale_roll_left_ankle ();
    void on_scale_pitch_right_ankle ();
    void on_scale_roll_right_ankle ();
    
    void on_item_head_created ( const Glib::RefPtr<Goocanvas::Item>& item,
                                const Glib::RefPtr<Goocanvas::ItemModel>& model );
    
    void on_item_move_created ( const Glib::RefPtr<Goocanvas::Item>& item,
                                const Glib::RefPtr<Goocanvas::ItemModel>& model );
    
    bool on_move_press ( GdkEvent* event );
    bool on_head_press ( GdkEvent* event );
    
    void on_button_head_up ();
    void on_button_head_down ();
    void on_button_head_right ();
    void on_button_head_left ();
    void on_button_head_stop ();
    
    void on_button_move_front ();
    void on_button_move_back ();
    void on_button_move_right ();
    void on_button_move_left ();
    void on_button_move_stop ();
    void on_button_move_lat_right ();
    void on_button_move_lat_left ();
    
    void on_button_standup_back ();
    void on_button_standup_front ();
    void on_button_right_kick ();
    void on_button_left_kick ();
    void on_button_intercept ();
    void on_button_change_camera ();
    void on_button_reset_naoqi ();
    
    void redraw_horizontal_head ( int index, double posx, double posy );
    void redraw_vertical_head ( int index, double posx, double posy );
    
    void redraw_horizontal_move ( int index, double posx, double posy );
    void redraw_vertical_move ( int index, double posx, double posy );
    
}; // Class NaoOperator

#endif // NAOOPERATOR_H

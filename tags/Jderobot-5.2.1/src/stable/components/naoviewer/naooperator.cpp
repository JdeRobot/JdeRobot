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

#include "naooperator.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
NaoOperator::NaoOperator ( Control* control ) {
    for ( int i = 0; i < HINGE_END - 1; i++ ) {
        Hinge h = (Hinge) i;
        
        Gtk::Scale* newScale = NULL;
        this->mapScales[h] = newScale;
    }
    
    if (this->getAllWidgets() != -1) {
        this->assignSignals();
        this->control = control;
        
        this->root_head = Goocanvas::GroupModel::create();
        this->canvas_head->set_root_item_model(this->root_head);
        this->canvas_head->set_visible(true);
        this->viewport_head->add(*(this->canvas_head));
        
        this->root_move = Goocanvas::GroupModel::create();
        this->canvas_move->set_root_item_model(this->root_move);
        this->canvas_move->set_visible(true);
        this->viewport_move->add(*(this->canvas_move));
        
        this->create_lines_head();
        this->create_lines_move();
    }
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
NaoOperator::~NaoOperator () {
    delete this->control;
}

/*************************************************************
 * ANOTHER FUNCTIONS
 *************************************************************/
void NaoOperator::display ( cv::Mat& image ) {
    Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
            Gdk::Pixbuf::create_from_data((const guint8*)image.data,
				                          Gdk::COLORSPACE_RGB,
				                          false,
				                          8,
				                          image.cols,
				                          image.rows,
				                          image.step);       
    
    this->camera_image->clear();
    this->camera_image->set(imgBuff);
}

void NaoOperator::create_lines_head () {
    this->h_head = Goocanvas::PolylineModel::create(0.0, 100.0, 200.0, 100.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->h_head->property_line_width() = 1.0;
    this->h_head->property_visibility() = Goocanvas::ITEM_VISIBLE;
    #else
    this->h_head->set_property("line_width", 1.0);
    this->h_head->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_head->add_child(this->h_head);
    
    this->v_head = Goocanvas::PolylineModel::create(100.0, 0.0, 100.0, 200.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->v_head->property_line_width() = 1.0;
    this->v_head->property_visibility() = Goocanvas::ITEM_VISIBLE;
    #else
    this->v_head->set_property("line_width", 1.0);
    this->v_head->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_head->add_child(this->v_head);
    
    Glib::RefPtr<Goocanvas::PolylineModel> h_reference = Goocanvas::PolylineModel::create(0.0, 100.0, 200.0, 100.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    h_reference->property_line_width() = 1.0;
    h_reference->property_visibility() = Goocanvas::ITEM_VISIBLE;
    h_reference->property_stroke_color() = "red";
    #else
    h_reference->set_property("line_width", 1.0);
    h_reference->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    h_reference->set_property("stroke_color", Glib::ustring("red"));
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_head->add_child(h_reference);
    
    Glib::RefPtr<Goocanvas::PolylineModel> v_reference = Goocanvas::PolylineModel::create(100.0, 0.0, 100.0, 200.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    v_reference->property_line_width() = 1.0;
    v_reference->property_visibility() = Goocanvas::ITEM_VISIBLE;
    v_reference->property_stroke_color() = "red";
    #else
    v_reference->set_property("line_width", 1.0);
    v_reference->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    v_reference->set_property("stroke_color", Glib::ustring("red"));
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_head->add_child(v_reference);
}

void NaoOperator::create_lines_move () {
    this->h_move = Goocanvas::PolylineModel::create(0.0, 100.0, 200.0, 100.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->h_move->property_line_width() = 1.0;
    this->h_move->property_visibility() = Goocanvas::ITEM_VISIBLE;
    #else
    this->h_move->set_property("line_width", 1.0);
    this->h_move->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_move->add_child(this->h_move);
    
    this->v_move = Goocanvas::PolylineModel::create(100.0, 0.0, 100.0, 200.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    this->v_move->property_line_width() = 1.0;
    this->v_move->property_visibility() = Goocanvas::ITEM_VISIBLE;
    #else
    this->v_move->set_property("line_width", 1.0);
    this->v_move->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_move->add_child(this->v_move);
    
    Glib::RefPtr<Goocanvas::PolylineModel> h_reference = Goocanvas::PolylineModel::create(0.0, 100.0, 200.0, 100.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    h_reference->property_line_width() = 1.0;
    h_reference->property_visibility() = Goocanvas::ITEM_VISIBLE;
    h_reference->property_stroke_color() = "red";
    #else
    h_reference->set_property("line_width", 1.0);
    h_reference->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    h_reference->set_property("stroke_color", Glib::ustring("red"));
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_move->add_child(h_reference);
    
    Glib::RefPtr<Goocanvas::PolylineModel> v_reference = Goocanvas::PolylineModel::create(100.0, 0.0, 100.0, 200.0);
    #ifdef GLIBMM_PROPERTIES_ENABLED
    v_reference->property_line_width() = 1.0;
    v_reference->property_visibility() = Goocanvas::ITEM_VISIBLE;
    v_reference->property_stroke_color() = "red";
    #else
    v_reference->set_property("line_width", 1.0);
    v_reference->set_property("visibility", Goocanvas::ITEM_VISIBLE);
    v_reference->set_property("stroke_color", Glib::ustring("red"));
    #endif //GLIBMM_PROPERTIES_ENABLED
    this->root_move->add_child(v_reference);
}

bool NaoOperator::isFollowingBall () {
    return this->checkbutton_followball->get_active();
}

int NaoOperator::getAllWidgets () {
    int returned = 0;
    // Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("main_gui.glade");
    } catch (const Glib::FileError& ex) {
        std::cerr << "FileError: " << ex.what() << std::endl;
        returned = -1;
    } catch(const Glib::MarkupError& ex) {
        std::cerr << "MarkupError: " << ex.what() << std::endl;
        returned = -1;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << "BuilderError: " << ex.what() << std::endl;
        returned = -1;
    }
    
    if (returned == 0) {
        refBuilder->get_widget("main_window", mainwindow);
        
        refBuilder->get_widget("scale_pitch_left_shoulder", this->mapScales[PITCH_LEFT_SHOULDER]);
        refBuilder->get_widget("scale_roll_left_shoulder", this->mapScales[ROLL_LEFT_SHOULDER]);
        refBuilder->get_widget("scale_yawpitch_left_hip", this->mapScales[YAWPITCH_LEFT_HIP]);
        refBuilder->get_widget("scale_pitch_left_hip", this->mapScales[PITCH_LEFT_HIP]);
        refBuilder->get_widget("scale_roll_left_hip", this->mapScales[ROLL_LEFT_HIP]);
        refBuilder->get_widget("scale_yawpitch_right_hip", this->mapScales[YAWPITCH_RIGHT_HIP]);
        refBuilder->get_widget("scale_pitch_right_hip", this->mapScales[PITCH_RIGHT_HIP]);
        refBuilder->get_widget("scale_roll_right_hip", this->mapScales[ROLL_RIGHT_HIP]);
        refBuilder->get_widget("scale_pitch_right_shoulder", this->mapScales[PITCH_RIGHT_SHOULDER]);
        refBuilder->get_widget("scale_roll_right_shoulder", this->mapScales[ROLL_RIGHT_SHOULDER]);
        refBuilder->get_widget("scale_yaw_left_elbow", this->mapScales[YAW_LEFT_ELBOW]);
        refBuilder->get_widget("scale_roll_left_elbow", this->mapScales[ROLL_LEFT_ELBOW]);
        refBuilder->get_widget("scale_pitch_left_knee", this->mapScales[PITCH_LEFT_KNEE]);
        refBuilder->get_widget("scale_pitch_right_knee", this->mapScales[PITCH_RIGHT_KNEE]);
        refBuilder->get_widget("scale_yaw_right_elbow", this->mapScales[YAW_RIGHT_ELBOW]);
        refBuilder->get_widget("scale_roll_right_elbow", this->mapScales[ROLL_RIGHT_ELBOW]);
        refBuilder->get_widget("scale_pitch_left_ankle", this->mapScales[PITCH_LEFT_ANKLE]);
        refBuilder->get_widget("scale_roll_left_ankle", this->mapScales[ROLL_LEFT_ANKLE]);
        refBuilder->get_widget("scale_pitch_right_ankle", this->mapScales[PITCH_RIGHT_ANKLE]);
        refBuilder->get_widget("scale_roll_right_ankle", this->mapScales[ROLL_RIGHT_ANKLE]);
        
        refBuilder->get_widget("viewport_move", this->viewport_move);
        refBuilder->get_widget("viewport_head", this->viewport_head);
        
        refBuilder->get_widget("button_up_head", this->head_up);
        refBuilder->get_widget("button_down_head", this->head_down);
        refBuilder->get_widget("button_right_head", this->head_right);
        refBuilder->get_widget("button_left_head", this->head_left);
        refBuilder->get_widget("button_stop_head", this->head_stop);
        
        refBuilder->get_widget("button_front", this->move_front);
        refBuilder->get_widget("button_back", this->move_back);
        refBuilder->get_widget("button_right", this->move_right);
        refBuilder->get_widget("button_left", this->move_left);
        refBuilder->get_widget("button_stop", this->move_stop);
        refBuilder->get_widget("button_lat_right", this->button_lat_right);
        refBuilder->get_widget("button_lat_left", this->button_lat_left);
        
        refBuilder->get_widget("button_standup_back", this->standup_back);
        refBuilder->get_widget("button_standup_front", this->standup_front);
        refBuilder->get_widget("button_right_kick", this->right_kick);
        refBuilder->get_widget("button_left_kick", this->left_kick);
        refBuilder->get_widget("button_intercept", this->intercept);
        refBuilder->get_widget("button_change_camera", this->change_camera);
        refBuilder->get_widget("button_reset_naoqi", this->reset_naoqi);
        
        refBuilder->get_widget("checkbutton_followball", this->checkbutton_followball);
        
        refBuilder->get_widget("camera_image", this->camera_image);
    }
    
    return returned;
}

void NaoOperator::assignSignals () {
    this->mapScales[PITCH_LEFT_SHOULDER]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_left_shoulder));
    
    this->mapScales[ROLL_LEFT_SHOULDER]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_left_shoulder));
                                                            
    this->mapScales[YAWPITCH_LEFT_HIP]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_yawpitch_left_hip));
                                                            
    this->mapScales[PITCH_LEFT_HIP]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_left_hip));
                                                            
    this->mapScales[ROLL_LEFT_HIP]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_left_hip));
                                                            
    this->mapScales[YAWPITCH_RIGHT_HIP]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_yawpitch_right_hip));
                                                            
    this->mapScales[PITCH_RIGHT_HIP]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_right_hip));
    
    this->mapScales[ROLL_RIGHT_HIP]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_right_hip));
                                                            
    this->mapScales[PITCH_RIGHT_SHOULDER]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_right_shoulder));
                                                            
    this->mapScales[ROLL_RIGHT_SHOULDER]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_right_shoulder));
                                                            
    this->mapScales[YAW_LEFT_ELBOW]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_yaw_left_elbow));
                                                            
    this->mapScales[ROLL_LEFT_ELBOW]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_left_elbow));
    
    this->mapScales[PITCH_LEFT_KNEE]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_left_knee));
                                                            
    this->mapScales[PITCH_RIGHT_KNEE]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_right_knee));
                                                            
    this->mapScales[YAW_RIGHT_ELBOW]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_yaw_right_elbow));
                                                            
    this->mapScales[ROLL_RIGHT_ELBOW]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_right_elbow));
                                                            
    this->mapScales[PITCH_LEFT_ANKLE]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_left_ankle));
                                                            
    this->mapScales[ROLL_LEFT_ANKLE]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_left_ankle));
                                                            
    this->mapScales[PITCH_RIGHT_ANKLE]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_pitch_right_ankle));
                                                            
    this->mapScales[ROLL_RIGHT_ANKLE]->signal_value_changed().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_scale_roll_right_ankle));
                                                            
    this->viewport_move->signal_event().connect(sigc::mem_fun(this, &NaoOperator::on_move_press));
    this->viewport_head->signal_event().connect(sigc::mem_fun(this, &NaoOperator::on_head_press));
    
    this->canvas_head = Gtk::manage(new Goocanvas::Canvas());
    this->canvas_head->signal_item_created().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_item_head_created));
    this->canvas_move = Gtk::manage(new Goocanvas::Canvas());
    this->canvas_move->signal_item_created().connect(sigc::mem_fun(this,
                                                            &NaoOperator::on_item_move_created));
                                                            
    this->head_up->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_head_up));
    this->head_down->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_head_down));
    this->head_right->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_head_right));
    this->head_left->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_head_left));
    this->head_stop->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_head_stop));
    
    this->move_front->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_move_front));
    this->move_back->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_move_back));
    this->move_right->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_move_right));
    this->move_left->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_move_left));
    this->move_stop->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_move_stop));
    this->button_lat_right->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_move_lat_right));
    this->button_lat_left->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_move_lat_left));
    
    this->standup_back->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_standup_back));
    this->standup_front->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_standup_front));
    this->right_kick->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_right_kick));
    this->left_kick->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_left_kick));
    this->intercept->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_intercept));
    this->change_camera->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_change_camera));
    this->reset_naoqi->signal_clicked().connect(sigc::mem_fun(this, &NaoOperator::on_button_reset_naoqi));
}

void NaoOperator::on_scale_pitch_left_shoulder () {
    this->control->setMovement(LEFTSHOULDERMOTORS, 0.0,
                this->mapScales[PITCH_LEFT_SHOULDER]->get_value(), this->mapScales[ROLL_LEFT_SHOULDER]->get_value());
}

void NaoOperator::on_scale_roll_left_shoulder () {
    this->control->setMovement(LEFTSHOULDERMOTORS, 0.0,
                this->mapScales[PITCH_LEFT_SHOULDER]->get_value(), this->mapScales[ROLL_LEFT_SHOULDER]->get_value());
}

void NaoOperator::on_scale_yawpitch_left_hip () {
    this->control->setMovement(LEFTHIPMOTORS, this->mapScales[YAWPITCH_LEFT_HIP]->get_value(),
                this->mapScales[PITCH_LEFT_HIP]->get_value(), this->mapScales[ROLL_LEFT_HIP]->get_value());
}

void NaoOperator::on_scale_pitch_left_hip () {
    this->control->setMovement(LEFTHIPMOTORS, this->mapScales[YAWPITCH_LEFT_HIP]->get_value(),
                this->mapScales[PITCH_LEFT_HIP]->get_value(), this->mapScales[ROLL_LEFT_HIP]->get_value());
}

void NaoOperator::on_scale_roll_left_hip () {
    this->control->setMovement(LEFTHIPMOTORS, this->mapScales[YAWPITCH_LEFT_HIP]->get_value(),
                this->mapScales[PITCH_LEFT_HIP]->get_value(), this->mapScales[ROLL_LEFT_HIP]->get_value());
}

void NaoOperator::on_scale_yawpitch_right_hip () {
    this->control->setMovement(RIGHTHIPMOTORS, this->mapScales[YAWPITCH_RIGHT_HIP]->get_value(),
                this->mapScales[PITCH_RIGHT_HIP]->get_value(), this->mapScales[ROLL_RIGHT_HIP]->get_value());
}

void NaoOperator::on_scale_pitch_right_hip () {
    this->control->setMovement(RIGHTHIPMOTORS, this->mapScales[YAWPITCH_RIGHT_HIP]->get_value(),
                this->mapScales[PITCH_RIGHT_HIP]->get_value(), this->mapScales[ROLL_RIGHT_HIP]->get_value());
}

void NaoOperator::on_scale_roll_right_hip () {
    this->control->setMovement(RIGHTHIPMOTORS, this->mapScales[YAWPITCH_RIGHT_HIP]->get_value(),
                this->mapScales[PITCH_RIGHT_HIP]->get_value(), this->mapScales[ROLL_RIGHT_HIP]->get_value());
}

void NaoOperator::on_scale_pitch_right_shoulder () {
    this->control->setMovement(RIGHTSHOULDERMOTORS, 0.0,
                this->mapScales[PITCH_RIGHT_SHOULDER]->get_value(), this->mapScales[ROLL_RIGHT_SHOULDER]->get_value());
}

void NaoOperator::on_scale_roll_right_shoulder () {
    this->control->setMovement(RIGHTSHOULDERMOTORS, 0.0,
                this->mapScales[PITCH_RIGHT_SHOULDER]->get_value(), this->mapScales[ROLL_RIGHT_SHOULDER]->get_value());
}

void NaoOperator::on_scale_yaw_left_elbow () {
    this->control->setMovement(LEFTELBOWMOTORS, this->mapScales[YAW_LEFT_ELBOW]->get_value(), 0.0,
                this->mapScales[ROLL_LEFT_ELBOW]->get_value());
}

void NaoOperator::on_scale_roll_left_elbow () {
    this->control->setMovement(LEFTELBOWMOTORS, this->mapScales[YAW_LEFT_ELBOW]->get_value(), 0.0,
                this->mapScales[ROLL_LEFT_ELBOW]->get_value());
}

void NaoOperator::on_scale_pitch_left_knee () {
    this->control->setMovement(LEFTKNEEMOTORS, this->mapScales[PITCH_LEFT_KNEE]->get_value(), 0.0, 0.0);
}

void NaoOperator::on_scale_pitch_right_knee () {
    this->control->setMovement(LEFTKNEEMOTORS, this->mapScales[PITCH_RIGHT_KNEE]->get_value(), 0.0, 0.0);
}

void NaoOperator::on_scale_yaw_right_elbow () {
    this->control->setMovement(RIGHTELBOWMOTORS, this->mapScales[YAW_RIGHT_ELBOW]->get_value(), 0.0,
                this->mapScales[ROLL_RIGHT_ELBOW]->get_value());
}

void NaoOperator::on_scale_roll_right_elbow () {
    this->control->setMovement(RIGHTELBOWMOTORS, this->mapScales[YAW_RIGHT_ELBOW]->get_value(), 0.0,
                this->mapScales[ROLL_RIGHT_ELBOW]->get_value());
}

void NaoOperator::on_scale_pitch_left_ankle () {
    this->control->setMovement(LEFTANKLEMOTORS, 0.0, this->mapScales[PITCH_LEFT_ANKLE]->get_value(),
                this->mapScales[ROLL_LEFT_ANKLE]->get_value());
}

void NaoOperator::on_scale_roll_left_ankle () {
    this->control->setMovement(LEFTANKLEMOTORS, 0.0, this->mapScales[PITCH_LEFT_ANKLE]->get_value(),
                this->mapScales[ROLL_LEFT_ANKLE]->get_value());
}

void NaoOperator::on_scale_pitch_right_ankle () {
    this->control->setMovement(RIGHTANKLEMOTORS, 0.0, this->mapScales[PITCH_RIGHT_ANKLE]->get_value(),
                this->mapScales[ROLL_RIGHT_ANKLE]->get_value());
}

void NaoOperator::on_scale_roll_right_ankle () {
    this->control->setMovement(RIGHTANKLEMOTORS, 0.0, this->mapScales[PITCH_RIGHT_ANKLE]->get_value(),
                this->mapScales[ROLL_RIGHT_ANKLE]->get_value());
}

void NaoOperator::on_item_head_created ( const Glib::RefPtr<Goocanvas::Item>& item,
                                         const Glib::RefPtr<Goocanvas::ItemModel>& /* model */) {
    Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
    if (group)
        return;
}

void NaoOperator::on_item_move_created ( const Glib::RefPtr<Goocanvas::Item>& item,
                                         const Glib::RefPtr<Goocanvas::ItemModel>& /* model */) {
    Glib::RefPtr<Goocanvas::Group> group = Glib::RefPtr<Goocanvas::Group>::cast_dynamic(item);
    if (group)
        return;
}

bool NaoOperator::on_move_press ( GdkEvent* event ) {
    if (event->type == GDK_BUTTON_PRESS) {
        this->drag = true;
    } else if (event->type == GDK_BUTTON_RELEASE) {
        this->drag = false;
        this->redraw_horizontal_move(0, 0.0, event->button.y);
        this->redraw_horizontal_move(1, 200.0, event->button.y);
        this->redraw_vertical_move(0, event->button.x, 0.0);
        this->redraw_vertical_move(1, event->button.x, 200.0);
        this->control->setDirectMovement(true);
        this->control->setMovement(MOTORS, (float) event->button.y, (float) event->button.x, 0.0);
    } else if (event->type == GDK_MOTION_NOTIFY) {
        if (drag) {
            this->redraw_horizontal_move(0, 0.0, event->button.y);
            this->redraw_horizontal_move(1, 200.0, event->button.y);
            this->redraw_vertical_move(0, event->button.x, 0.0);
            this->redraw_vertical_move(1, event->button.x, 200.0);
        }
    }

    return false;
}

bool NaoOperator::on_head_press ( GdkEvent* event ) {
    if (event->type == GDK_BUTTON_PRESS) {
        this->drag = true;
    } else if (event->type == GDK_BUTTON_RELEASE) {
        this->drag = false;
        this->redraw_horizontal_head(0, 0.0, event->button.y);
        this->redraw_horizontal_head(1, 200.0, event->button.y);
        this->redraw_vertical_head(0, event->button.x, 0.0);
        this->redraw_vertical_head(1, event->button.x, 200.0);
        this->control->setDirectHead(true);
        this->control->setMovement(HEADMOTORS, (float) event->button.x, (float) event->button.y, 0.0);
    } else if (event->type == GDK_MOTION_NOTIFY) {
        if (drag) {
            this->redraw_horizontal_head(0, 0.0, event->button.y);
            this->redraw_horizontal_head(1, 200.0, event->button.y);
            this->redraw_vertical_head(0, event->button.x, 0.0);
            this->redraw_vertical_head(1, event->button.x, 200.0);
        }
    }
    
    return false;
}

void NaoOperator::on_button_head_up () {
    this->control->setDirectHead(false);
    this->control->setMovement(HEADMOTORS, 0.0, -0.1, 0.0);
}

void NaoOperator::on_button_head_down () {
    this->control->setDirectHead(false);
    this->control->setMovement(HEADMOTORS, 0.0, 0.1, 0.0);
}

void NaoOperator::on_button_head_right () {
    this->control->setDirectHead(false);
    this->control->setMovement(HEADMOTORS, -0.1, 0.0, 0.0);
}

void NaoOperator::on_button_head_left () {
    this->control->setDirectHead(false);
    this->control->setMovement(HEADMOTORS, 0.1, 0.0, 0.0);
}

void NaoOperator::on_button_head_stop () {
    this->control->setDirectHead(false);
    this->control->setMovement(HEADMOTORS, 0.0, 0.0, 0.0);
}

void NaoOperator::on_button_move_front () {
    this->control->setDirectMovement(false);
    this->control->setMovement(MOTORS, 0.1, 0.0, 0.0);
}

void NaoOperator::on_button_move_back () {
    this->control->setDirectMovement(false);
    this->control->setMovement(MOTORS, -0.1, 0.0, 0.0);
}

void NaoOperator::on_button_move_right () {
    this->control->setDirectMovement(false);
    this->control->setMovement(MOTORS, 0.0, -0.1, 0.0);
}

void NaoOperator::on_button_move_left () {
    this->control->setDirectMovement(false);
    this->control->setMovement(MOTORS, 0.0, 0.1, 0.0);
}

void NaoOperator::on_button_move_lat_right () {
    this->control->setDirectMovement(false);
    this->control->setMovement(MOTORS, 0.0, 0.0, -0.1);
}

void NaoOperator::on_button_move_lat_left () {
    this->control->setDirectMovement(false);
    this->control->setMovement(MOTORS, 0.0, 0.0, 0.1);
}

void NaoOperator::on_button_move_stop () {
    this->control->setDirectMovement(false);
    this->control->setMovement(MOTORS, 0.0, 0.0, 0.0);
}

void NaoOperator::on_button_standup_back () {
    this->control->setAction(STANDUP_BACK);
}

void NaoOperator::on_button_standup_front () {
    this->control->setAction(STANDUP_FRONT);
}

void NaoOperator::on_button_right_kick () {
    this->control->setAction(RIGHTKICK);
}

void NaoOperator::on_button_left_kick () {
    this->control->setAction(LEFTKICK);
}

void NaoOperator::on_button_intercept () {
    this->control->setAction(INTERCEPT);
}

void NaoOperator::on_button_change_camera () {
    this->control->setAction(CHANGECAMERA);
}

void NaoOperator::on_button_reset_naoqi () {
    this->control->setAction(RESETNAOQI);
}

void NaoOperator::redraw_horizontal_head ( int index, double posx, double posy ) {
    Goocanvas::Points points;
	#ifdef GLIBMM_PROPERTIES_ENABLED
    points = this->h_head->property_points();
    #else
    points = this->h_head->get_property("points");
    #endif //GLIBMM_PROPERTIES_ENABLED
    
	points.set_coordinate(index, posx, posy);
	
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->h_head->property_points() = points;
    #else
    this->h_head->set_property("points", points);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

void NaoOperator::redraw_vertical_head ( int index, double posx, double posy ) {
    Goocanvas::Points points;
	#ifdef GLIBMM_PROPERTIES_ENABLED
    points = this->v_head->property_points();
    #else
    points = this->v_head->get_property("points");
    #endif //GLIBMM_PROPERTIES_ENABLED
    
	points.set_coordinate(index, posx, posy);
	
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->v_head->property_points() = points;
    #else
    this->v_head->set_property("points", points);
    #endif //GLIBMM_PROPERTIES_ENABLED
}
    
void NaoOperator::redraw_horizontal_move ( int index, double posx, double posy ) {
    Goocanvas::Points points;
	#ifdef GLIBMM_PROPERTIES_ENABLED
    points = this->h_move->property_points();
    #else
    points = this->h_move->get_property("points");
    #endif //GLIBMM_PROPERTIES_ENABLED
    
	points.set_coordinate(index, posx, posy);
	
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->h_move->property_points() = points;
    #else
    this->h_move->set_property("points", points);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

void NaoOperator::redraw_vertical_move ( int index, double posx, double posy ) {
    Goocanvas::Points points;
	#ifdef GLIBMM_PROPERTIES_ENABLED
    points = this->v_move->property_points();
    #else
    points = this->v_move->get_property("points");
    #endif //GLIBMM_PROPERTIES_ENABLED
    
	points.set_coordinate(index, posx, posy);
	
	#ifdef GLIBMM_PROPERTIES_ENABLED
    this->v_move->property_points() = points;
    #else
    this->v_move->set_property("points", points);
    #endif //GLIBMM_PROPERTIES_ENABLED
}

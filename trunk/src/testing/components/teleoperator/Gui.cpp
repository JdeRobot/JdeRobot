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

#include "Gui.h"
#include "SharedMemory.h"
#include "sigc++/functors/mem_fun.h"
#define PI 3.14159265

Gui::Gui(SharedMemory *interfacesData) : gtkmain(0, 0) {

    this->interfacesData = interfacesData;

    this->laserShowed = FALSE;
    this->camerasShowed = FALSE;
    this->motorsShowed = FALSE;
    this->encodersShowed = FALSE;

    std::cout << "Loading glade\n";
    this->builder = Gtk::Builder::create_from_file("TeleoperatorPC.glade");
    std::cout << "GUI LOADED" << std::endl;

    this->builder->get_widget("button_exit", this->button_exit);
    this->button_exit->signal_clicked().connect(sigc::mem_fun(this, &Gui::exit_button_clicked));

    this->builder->get_widget("main_window", this->main_window);
    this->builder->get_widget("help_window", this->help_window);

    //MOTORS    
    //GET WINDOWS    
    this->builder->get_widget("motors_window", this->motors_window);
    //GET LABELS 
    this->builder->get_widget("motors_linear_velocity", this->motors_linear_velocity);
    this->builder->get_widget("motors_rot_velocity", this->motors_rot_velocity);
    this->builder->get_widget("motors_canvas", this->motors_canvas);
    //GET BUTTONS
    this->builder->get_widget("stop_button", this->stop_button);
    this->builder->get_widget("check_motors", this->check_motors);
    //Signals
    this->motors_canvas->signal_event().connect(sigc::mem_fun(this, &Gui::on_button_press_motors_canvas));
    this->stop_button->signal_clicked().connect(sigc::mem_fun(this, &Gui::stop_button_clicked));
    this->check_motors->signal_toggled().connect(sigc::mem_fun(this, &Gui::check_motors_clicked));
    this->motors_window->signal_hide().connect(sigc::mem_fun(this, &Gui::on_motors_window_hide));
    //Init widgets
    this->motors_canvas->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::VISIBILITY_NOTIFY_MASK | Gdk::BUTTON1_MOTION_MASK);
    this->motors_canvas->get_size_request(this->previous_event_x, this->previous_event_y);
    //END_MOTORS

    //ENCODERS
    //WINDOW
    this->builder->get_widget("encoders_window", this->encoders_window);
    //BUTTONS
    this->builder->get_widget("check_encoders", this->check_encoders);
    //SIGNALS
    this->check_encoders->signal_toggled().connect(sigc::mem_fun(this, &Gui::check_encoders_clicked));
    this->encoders_window->signal_hide().connect(sigc::mem_fun(this, &Gui::on_encoders_window_hide));
    //LABELS
    this->builder->get_widget("encoders_x", this->encoders_x);
    this->builder->get_widget("encoders_y", this->encoders_y);
    this->builder->get_widget("encoders_theta", this->encoders_theta);
    //END_ENCODERS

    //LASER
    //WINDOW
    this->builder->get_widget("laser_window", this->laser_window);
    //CANVAS
    this->builder->get_widget("laser_canvas", this->laser_canvas);
    //CHECK_BUTTON
    this->builder->get_widget("check_laser", this->check_laser);
    //SIGNALS
    this->check_laser->signal_toggled().connect(sigc::mem_fun(this, &Gui::check_laser_clicked));
    this->laser_window->signal_hide().connect(sigc::mem_fun(this, &Gui::on_laser_window_hide));
    //END_LASER

    //CAMERAS
    //WINDOW
    this->builder->get_widget("images_window", this->images_window);
    this->builder->get_widget("teleoperatorCameras_window", this->teleoperatorCameras_window);
    //IMAGES
    this->builder->get_widget("image_left_camera", this->image_left_camera);
    this->builder->get_widget("image_right_camera", this->image_right_camera);
    //CHECK BUTTONS
    this->builder->get_widget("check_cameras", this->check_cameras);
    this->builder->get_widget("check_pose3d", this->check_pose3d);
    //CANVAS
    this->builder->get_widget("cameras_canvas", this->cameras_canvas);
    //BUTTONS
    this->builder->get_widget("cameras_stop_button", this->cameras_stop_button);
    //SIGNALS
    this->check_cameras->signal_toggled().connect(sigc::mem_fun(this, &Gui::check_cameras_clicked));
    this->check_pose3d->signal_toggled().connect(sigc::mem_fun(this, &Gui::check_pose3d_clicked));
    this->cameras_canvas->signal_event().connect(sigc::mem_fun(this, &Gui::on_button_press_cameras_canvas));
    this->cameras_stop_button->signal_clicked().connect(sigc::mem_fun(this, &Gui::cameras_stop_button_clicked));
    //Init widgets
    this->cameras_canvas->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::VISIBILITY_NOTIFY_MASK | Gdk::BUTTON1_MOTION_MASK);
    this->cameras_canvas->get_size_request(this->cameras_previous_event_x, this->cameras_previous_event_y);

    this->cameras_previous_event_y = this->cameras_previous_event_y / 2;
    this->cameras_previous_event_x = this->cameras_previous_event_x / 2;

    this->previous_event_y = this->previous_event_y / 2;
    this->previous_event_x = this->previous_event_x / 2;

    this->main_window->show();

}

void Gui::display() {

    show_windows();

}

Gui::~Gui() {
}

std::string Gui::float2string(float n) {
    std::string result;
    std::ostringstream convert;

    convert << floorf(n * 100) / 100;
    ;

    result = convert.str();
    return result;
}

void Gui::displayEncodersData() {
    this->encoders_x->set_text(float2string(this->interfacesData->encodersDataReceived->robotx));
    this->encoders_y->set_text(float2string(this->interfacesData->encodersDataReceived->roboty));
    this->encoders_theta->set_text(float2string(this->interfacesData->encodersDataReceived->robottheta));
}

void Gui::displayCameras() {
    //pthread_mutex_lock(&interfacesData->imagesData_mutex);
    if (interfacesData->imagesReady) {
        try {
            /*
            IplImage* image = cvCreateImage(cvSize(interfacesData->imageDataLeftReceived->description->width,interfacesData->imageDataLeftReceived->description->height), 8 ,3);
    
    memcpy((unsigned char *) image->imageData,&(interfacesData->imageDataLeftReceived->pixelData[0]),image->width*image->height * 3);
    
    colorspaces::Image::FormatPtr fmt = colorspaces::Image::Format::searchFormat(interfacesData->imageDataLeftReceived->description->format);
    
    if (!fmt)
       throw "Format not supported";
                
        Glib::RefPtr<Gdk::Pixbuf> imgBuff =
            Gdk::Pixbuf:: create_from_data((const guint8*)image->imageData,
                                        Gdk::COLORSPACE_RGB,
                                        false,
                                        image->depth,
                                        image->width,
                                        image->height,
                                        image->widthStep);

    IplImage* image2 = cvCreateImage(cvSize(interfacesData->imageDataRightReceived->description->width,interfacesData->imageDataRightReceived->description->height), 8 ,3);
    
    memcpy((unsigned char *) image2->imageData,&(interfacesData->imageDataRightReceived->pixelData[0]),image2->width*image2->height * 3);
    
    colorspaces::Image::FormatPtr fmt2 = colorspaces::Image::Format::searchFormat(interfacesData->imageDataRightReceived->description->format);
    
    if (!fmt2)
       throw "Format not supported";
                
        Glib::RefPtr<Gdk::Pixbuf> imgBuff2 =
            Gdk::Pixbuf:: create_from_data((const guint8*)image2->imageData,
                                        Gdk::COLORSPACE_RGB,
                                        false,
                                        image2->depth,
                                        image2->width,
                                        image2->height,
                                        image2->widthStep);

             */
            this->image_left_camera->clear();
            //if(interfacesData->imgBuff->get_pixels()!=NULL)
            //std::cout << interfacesData->imgBuff->get_pixels() << std::endl;
            this->image_left_camera->set(interfacesData->imgBuff);
            //if(interfacesData->imgBuff2->get_pixels()!=NULL)
            this->image_right_camera->clear();

            this->image_right_camera->set(interfacesData->imgBuff2);

            //cvReleaseImage(&image);
            //cvReleaseImage(&image2);

        } catch (const char* msg) {
            std::cerr << "CAZADO " << msg << std::endl;

        }

    }
    //pthread_mutex_unlock(&interfacesData->imagesData_mutex);
}

void Gui::drawLaser() {
    int vectorLaser[180];
    int i;

    Cairo::RefPtr<Cairo::Context> cr_red = this->laser_canvas->get_window()->create_cairo_context();
    cr_red->set_line_width(2.0);
    cr_red->set_source_rgb(0.0, 0.0, 0.0);
    cr_red->paint();
    cr_red->set_source_rgb(0.8, 0.0, 0.0);

    for (i = this->interfacesData->laserDataReceived->numLaser - 2; i >= 0; i--) {
        cr_red->move_to(180 - ((interfacesData->laserDataReceived->distanceData[i] / 45)*(cos((i) * PI / 180))), 180 - ((interfacesData->laserDataReceived->distanceData[i] / 45)*(sin((i) * PI / 180))));
        cr_red->line_to(180 - ((interfacesData->laserDataReceived->distanceData[i + 1] / 45)*(cos((i + 1) * PI / 180))), 180 - ((interfacesData->laserDataReceived->distanceData[i + 1] / 45)*(sin((i + 1) * PI / 180))));
    }

    cr_red->stroke();

}

void Gui::displayPose3DEncodersData() {
    float pan, tilt;

    Cairo::RefPtr<Cairo::Context> cr_red = this->cameras_canvas->get_window()->create_cairo_context();
    Cairo::RefPtr<Cairo::Context> cr_white = this->cameras_canvas->get_window()->create_cairo_context();

    cr_red->set_line_width(2.0);
    cr_white->set_line_width(2.0);
    cr_red->set_source_rgb(0.0, 0.0, 0.0);
    cr_red->paint();
    cr_white->set_source_rgb(1, 1, 1);
    cr_white->move_to(0, 100);
    cr_white->line_to(200, 100);
    cr_white->move_to(100, 0);
    cr_white->line_to(100, 200);
    cr_red->set_source_rgb(0.8, 0.0, 0.0);
    if ((this->interfacesData->pose3DEncodersInterface.activated)) {
        Pose3Dencoders2widget(&tilt, &pan);
    }
    cr_red->move_to(0, tilt);
    cr_red->line_to(200, tilt);
    cr_red->move_to(pan, 0);
    cr_red->line_to(pan, 200);
    cr_white->stroke();
    cr_red->stroke();

}

void Gui::displayMotorsData() {

    std::string v;
    std::string w;

    v = float2string(this->interfacesData->motorsDataReceived.v);
    w = float2string(this->interfacesData->motorsDataReceived.w);

    v = v.append(" m/s");
    w = w.append(" m/s");
    this->motors_linear_velocity->set_text(v);
    this->motors_rot_velocity->set_text(w);
    Cairo::RefPtr<Cairo::Context> cr_red = this->motors_canvas->get_window()->create_cairo_context();
    Cairo::RefPtr<Cairo::Context> cr_white = this->motors_canvas->get_window()->create_cairo_context();
    cr_red->set_line_width(2.0);
    cr_white->set_line_width(2.0);
    cr_red->set_source_rgb(0.0, 0.0, 0.0);
    cr_red->paint();
    cr_white->set_source_rgb(1, 1, 1);
    cr_white->move_to(0, 100);
    cr_white->line_to(200, 100);
    cr_white->move_to(100, 0);
    cr_white->line_to(100, 200);
    cr_red->set_source_rgb(0.8, 0.0, 0.0);
    cr_red->move_to(0, this->previous_event_y);
    cr_red->line_to(200, this->previous_event_y);
    cr_red->move_to(this->previous_event_x, 0);
    cr_red->line_to(this->previous_event_x, 200);
    cr_white->stroke();
    cr_red->stroke();

}

void Gui::cameras_handler() {
    float tilt, pan;

    widget2Pose3Dmotors(this->cameras_previous_event_x, this->cameras_previous_event_y, &tilt, &pan);

    this->interfacesData->Pose3DMotorsDataToSendLeft.tilt = tilt;
    this->interfacesData->Pose3DMotorsDataToSendLeft.pan = pan;
    this->interfacesData->Pose3DMotorsDataToSendRight.tilt = tilt;
    this->interfacesData->Pose3DMotorsDataToSendRight.pan = pan;
}

void Gui::motors_handler() {
    int v, w;

    widget2motors(this->previous_event_x, this->previous_event_y, &v, &w);
    v = v * 10;

    this->interfacesData->motorsDataToSend.v = v;
    this->interfacesData->motorsDataToSend.w = w;

}

bool Gui::on_button_press_cameras_canvas(GdkEvent* event) {
    float event_x = event->button.x;
    float event_y = event->button.y;
    static gboolean dragging = FALSE;

    switch (event->type) {
        case GDK_BUTTON_PRESS:

            if (event->button.button == 3) {
                this->cameras_previous_event_x = event->button.x;
                this->cameras_previous_event_y = event->button.y;
                this->cameras_handler();
            }

            if (event->button.button == 1) {
                GdkCursor *cursor;
                cursor = gdk_cursor_new(GDK_FLEUR);
                dragging = true;
            }
            break;
        case GDK_MOTION_NOTIFY:
            if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) {
                this->cameras_previous_event_x = event_x;
                this->cameras_previous_event_y = event_y;
                this->cameras_handler();
            }
            break;

        case GDK_BUTTON_RELEASE:
            dragging = FALSE;
            break;
        default:
            break;

    }

}

bool Gui::on_button_press_motors_canvas(GdkEvent* event) {
    float event_x = event->button.x;
    float event_y = event->button.y;
    static gboolean dragging = FALSE;

    if(event_x < 0) event_x = 0;
    if(event_y < 0) event_y = 0;

    if(event_x > 200) event_x = 200;
    if(event_y > 200) event_y = 200;

    switch (event->type) {
        case GDK_BUTTON_PRESS:

            if (event->button.button == 3) {
                this->previous_event_x = event->button.x;
                this->previous_event_y = event->button.y;
                this->motors_handler();
            }

            if (event->button.button == 1) {
                GdkCursor *cursor;
                cursor = gdk_cursor_new(GDK_FLEUR);
                dragging = true;
            }
            break;
        case GDK_MOTION_NOTIFY:
            if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) {
                this->previous_event_x = event_x;
                this->previous_event_y = event_y;
                this->motors_handler();
            }
            break;

        case GDK_BUTTON_RELEASE:
            dragging = FALSE;
            break;
        default:
            break;

    }
}

void Gui::on_motors_window_hide(){
    if(check_motors->get_active())
        this->check_motors->set_active(false);
}

void Gui::on_encoders_window_hide(){
    if(check_encoders->get_active())
        this->check_encoders->set_active(false);
}

void Gui::on_laser_window_hide(){
    if(check_laser->get_active())
        this->check_laser->set_active(false);
}


void Gui::stop_button_clicked() {
    this->previous_event_x = 100;
    this->previous_event_y = 100;
    this->motors_handler();
}

void Gui::cameras_stop_button_clicked() {
    this->cameras_previous_event_x = 100;
    this->cameras_previous_event_y = 100;
    this->cameras_handler();
}

void Gui::exit_button_clicked() {

    this->interfacesData->exit = true;
    exit(1);

}

void Gui::check_pose3d_clicked() {

    if (!check_pose3d->get_active()) {
        this->interfacesData->pose3DEncodersInterface.checkEnd = TRUE;
        this->interfacesData->pose3DMotorsInterface.checkEnd = TRUE;
        this->teleoperatorCameras_window->hide();
    } else {
        this->teleoperatorCameras_window->show();
        this->interfacesData->pose3DEncodersInterface.checkInit = TRUE;
        this->interfacesData->pose3DMotorsInterface.checkInit = TRUE;
    }
}

void Gui::check_laser_clicked() {
    if (!check_laser->get_active()) {
        this->interfacesData->laserInterface.checkEnd = TRUE;

        this->laser_window->hide();
    } else {
        this->laser_window->show();
        this->interfacesData->laserInterface.checkInit = TRUE;
    }
}

void Gui::check_motors_clicked() {
    if (!check_motors->get_active()) {
        this->interfacesData->motorsInterface.checkEnd = TRUE;
        this->motors_window->hide();
    } else {
        this->motors_window->show();
        this->interfacesData->motorsInterface.checkInit = TRUE;
    }
}

void Gui::check_encoders_clicked() {
    if (!check_encoders->get_active()) {
        this->interfacesData->encodersInterface.checkEnd = TRUE;
        this->encoders_window->hide();
    } else {
        this->encoders_window->show();
        this->interfacesData->encodersInterface.checkInit = TRUE;
    }
}

void Gui::check_cameras_clicked() {
    if (!check_cameras->get_active()) {
        this->interfacesData->camerasInterface.checkEnd = TRUE;
        this->images_window->hide();
    } else {
        this->images_window->show();
        this->interfacesData->camerasInterface.checkInit = TRUE;
    }
}

void Gui::widget2motors(int x, int y, int* v, int* w) {

    *w = -(x - 100);
    *v = -(y - 100);

}

void Gui::widget2Pose3Dmotors(int x, int y, float* tilt, float* pan) {

    float tilt_y = this->interfacesData->Pose3DEncodersDataReceivedLeft->maxTilt;
    float tilt_x = (this->interfacesData->Pose3DEncodersDataReceivedLeft->minTilt - tilt_y) / 200;
    float pan_y = this->interfacesData->Pose3DEncodersDataReceivedLeft->minPan;
    float pan_x = (this->interfacesData->Pose3DEncodersDataReceivedLeft->maxPan - pan_y) / 200;

    *pan = x * pan_x + pan_y;
    *tilt = y * tilt_x + tilt_y;

}

void Gui::Pose3Dencoders2widget(float* tilt, float* pan) {

    float tilt_x = 200 / (this->interfacesData->Pose3DEncodersDataReceivedLeft->minTilt + this->interfacesData->Pose3DEncodersDataReceivedLeft->minTilt);
    float tilt_y = this->interfacesData->Pose3DEncodersDataReceivedLeft->minTilt*tilt_x;

    float pan_x = 200 / (this->interfacesData->Pose3DEncodersDataReceivedLeft->maxPan + this->interfacesData->Pose3DEncodersDataReceivedLeft->maxPan);
    float pan_y = this->interfacesData->Pose3DEncodersDataReceivedLeft->maxPan*pan_x;

    *pan = (this->interfacesData->Pose3DEncodersDataReceivedLeft->pan * pan_x + pan_y);
    *tilt = (this->interfacesData->Pose3DEncodersDataReceivedLeft->tilt * tilt_x + tilt_y);

}

void Gui::show_windows() {

    if ((this->interfacesData->pose3DEncodersInterface.activated) && (this->interfacesData->pose3DMotorsInterface.activated)) {
        if (this->teleoperatorCameras_window->is_visible())
            displayPose3DEncodersData();
    }
    if ((this->interfacesData->laserInterface.activated) && (this->interfacesData->laserReady)) {
        if (this->laser_window->is_visible())
            drawLaser();
    }
    if (this->interfacesData->motorsInterface.activated) {
        if (this->motors_window->is_visible())
            displayMotorsData();
    }
    if (this->interfacesData->encodersInterface.activated) {
        if (this->encoders_window->is_visible() && (this->interfacesData->encodersReady))
            displayEncodersData();
    }
    pthread_mutex_lock(&interfacesData->imagesData_mutex);

    if (this->interfacesData->camerasInterface.activated) {
        if (this->images_window->is_visible())
            displayCameras();
    }

    pthread_mutex_unlock(&interfacesData->imagesData_mutex);

    while (gtkmain.events_pending()) {
        gtkmain.iteration(true);
    }
}

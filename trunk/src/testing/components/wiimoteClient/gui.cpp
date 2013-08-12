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
#include<iostream>
#include "gui.h"
#include "API.h"

namespace wiimoteClient {
    
    std::string Gui::int2str(int num) {



        std::string Result; // string which will contain the result

        std::ostringstream convert; // stream used for the conversion

        convert << num; // insert the textual representation of 'Number' in the characters in the stream

        Result = convert.str(); // set 'Result' to the contents of the stream

        return Result;
    }

    Gui::Gui(Api* api) : gtkmain(0, 0) {
        
        this->api = api;
        api->change_state_LED1=false;
        api->change_state_LED2=false;
        api->change_state_LED3=false;
        api->change_state_LED4=false;
        this->acc[0] = api->acc[0];
        this->acc[1] = api->acc[1];
        this->acc[2] = api->acc[2];
        this->button = api->button;
        this->ir1[0] = api->ir1[0];
        this->ir1[1] = api->ir1[1];
        this->ir2[0] = api->ir2[0];
        this->ir2[1] = api->ir2[1];
        this->ir3[0] = api->ir3[0];
        this->ir3[1] = api->ir3[1];
        this->ir4[0] = api->ir4[0];
        this->ir4[1] = api->ir4[1];        

        this->nunchukAcc[0] = api->nunchukAcc[0];
        this->nunchukAcc[1] = api->nunchukAcc[1];
        this->nunchukAcc[2] = api->nunchukAcc[2];
        this->nunchukButton = api->nunchukButton;
        this->nunchukStick[0] = api->nunchukStick[0];
        this->nunchukStick[1] = api->nunchukStick[1];

        /*Init OpenGL*/
        if (!Gtk::GL::init_check(NULL, NULL)) {
            std::cerr << "Couldn't initialize GL\n";
            std::exit(1);
        }
        Gnome::Canvas::init();
        std::cout << "Loading glade\n";
        refXml = Gnome::Glade::Xml::create("./wiimoteClient.glade");
        this->gladepath = std::string("./wiimoteClient.glade");

        // GET WIDGETS & WINDOWS
        // Get window
        refXml->get_widget("window1", this->mainWindow);

        //Get Buttons
        refXml->get_widget("buttonUp", buttonUp);
        refXml->get_widget("buttonLeft", this->buttonLeft);
        refXml->get_widget("buttonRight", this->buttonRight);
        refXml->get_widget("buttonDown", this->buttonDown);
        refXml->get_widget("button_b", this->buttonB);
        refXml->get_widget("button_menos", this->buttonMenos);
        refXml->get_widget("button_home", this->buttonHome);
        refXml->get_widget("button_mas", this->buttonMas);
        refXml->get_widget("button_a", this->buttonA);
        refXml->get_widget("button_1", this->buttonUno);
        refXml->get_widget("button_2", this->buttonDos);
        refXml->get_widget("ncButtonC", this->ncButtonC);
        refXml->get_widget("ncButtonZ", this->ncButtonZ);
        refXml->get_widget("buttonExit", this->buttonExit);

        //Get Canvas
        refXml->get_widget("canvasStick", this->canvasStick);
        
        //Get Labels
        refXml->get_widget("lblIR1x", ir1x);
        refXml->get_widget("lblIR1y", ir1y);
        refXml->get_widget("lblIR2x", ir2x);
        refXml->get_widget("lblIR2y", ir2y);
        refXml->get_widget("lblIR3x", ir3x);
        refXml->get_widget("lblIR3y", ir3y);
        refXml->get_widget("lblIR4x", ir4x);
        refXml->get_widget("lblIR4y", ir4y);

        refXml->get_widget("lblACCx", accX);
        refXml->get_widget("lblACCy", accY);
        refXml->get_widget("lblACCz", accZ);
        
        refXml->get_widget("ncAccX", ncAccX);
        refXml->get_widget("ncAccY", ncAccY);
        refXml->get_widget("ncAccZ", ncAccZ);

        //Check buttons
        refXml->get_widget("led1", this->led1);
        refXml->get_widget("led2", this->led2);
        refXml->get_widget("led3", this->led3);
        refXml->get_widget("led4", this->led4);
        
        //Create events
        buttonExit->signal_clicked().connect(sigc::mem_fun(this, &Gui::exitButton_clicked));
        led1->signal_toggled().connect(sigc::mem_fun(this, &Gui::led1_clicked));
        led2->signal_toggled().connect(sigc::mem_fun(this, &Gui::led2_clicked));
        led3->signal_toggled().connect(sigc::mem_fun(this, &Gui::led3_clicked));
        led4->signal_toggled().connect(sigc::mem_fun(this, &Gui::led4_clicked));
        
        //Show the window
        mainWindow->show();
        
        canvasStick->signal_unrealize();
        canvasStick->signal_realize();
        canvasStick->set_child_visible(true);
        
        gc_canvasStick = Gdk::GC::create(canvasStick->get_window());
        
        colormap = canvasStick->get_colormap();
        color_white = Gdk::Color("#");
        color_black = Gdk::Color("#000000");
        color_red = Gdk::Color("#FF0000");
        colormap->alloc_color(color_white);
        colormap->alloc_color(color_black);
        colormap->alloc_color(color_red);
        
    }

    void Gui::run(Api* api) {

        this->acc[0] = api->acc[0];
        this->acc[1] = api->acc[1];
        this->acc[2] = api->acc[2];
        this->button = api->button;
        this->ir1[0] = api->ir1[0];
        this->ir1[1] = api->ir1[1];
        this->ir2[0] = api->ir2[0];
        this->ir2[1] = api->ir2[1];
        this->ir3[0] = api->ir3[0];
        this->ir3[1] = api->ir3[1];
        this->ir4[0] = api->ir4[0];
        this->ir4[1] = api->ir4[1];

        this->nunchukAcc[0] = api->nunchukAcc[0];
        this->nunchukAcc[1] = api->nunchukAcc[1];
        this->nunchukAcc[2] = api->nunchukAcc[2];
        this->nunchukButton = api->nunchukButton;
        this->nunchukStick[0] = api->nunchukStick[0];
        this->nunchukStick[1] = api->nunchukStick[1];

        switch (this->button) {
            case 2048:
                this->buttonUp->activate();
                break;
            case 256:
                this->buttonLeft->activate();
                break;
            case 512:
                this->buttonRight->activate();
                break;
            case 1024:
                this->buttonDown->activate();
                break;
            case 4:
                this->buttonB->activate();
                break;
            case 8:
                this->buttonA->activate();
                break;
            case 16:
                this->buttonMenos->activate();
                break;
            case 128:
                this->buttonHome->activate();
                break;
            case 4096:
                this->buttonMas->activate();
                break;
            case 2:
                this->buttonUno->activate();
                break;
            case 1:
                this->buttonDos->activate();
                break;
            default:
                //std::cout << "Unknown button" << std::endl;
                break;
        }
        
        switch(this->nunchukButton){
            case 1:
                this->ncButtonZ->activate();
                break;
            case 2:
                this->ncButtonC->activate();
                break;
            default:
                break;
        }

//        if (led1->get_active())
//            api->change_state_LED1 = true;
//        if (led2->get_active())
//            api->change_state_LED2 = true;
//        if (led3->get_active())
//            api->change_state_LED3 = true;
//        if (led4->get_active())
//            api->change_state_LED4 = true;
        
        
//        if(this->nunchukButton==1)
//            this->ncButtonZ->activate();
//        if(this->nunchukButton==2)
//            this->ncButtonC->activate();
//        std::cout << this->nunchukButton << std::endl;
        this->accX->set_text(int2str(this->acc[0]));
        this->accY->set_text(int2str(this->acc[1]));
        this->accZ->set_text(int2str(this->acc[2]));

        this->ncAccX->set_text(int2str(this->nunchukAcc[0]));
        this->ncAccY->set_text(int2str(this->nunchukAcc[1]));
        this->ncAccZ->set_text(int2str(this->nunchukAcc[2]));
        
        
        
        this->ir1x->set_text(int2str(this->ir1[0]));
        this->ir1y->set_text(int2str(this->ir1[1]));
        this->ir2x->set_text(int2str(this->ir2[0]));
        this->ir2y->set_text(int2str(this->ir2[1]));
        this->ir3x->set_text(int2str(this->ir3[0]));
        this->ir3y->set_text(int2str(this->ir3[1]));
        this->ir4x->set_text(int2str(this->ir4[0]));
        this->ir4y->set_text(int2str(this->ir4[1]));        
        
        gc_canvasStick->set_foreground(color_black);
        canvasStick->get_window()->draw_rectangle(gc_canvasStick, true, 0, 0, 177, 131);
        
        gc_canvasStick->set_foreground(color_red);
        if(this->nunchukStick[0]-33<0){
            this->nunchukStick[0] = 33;
        }
        if(this->nunchukStick[0]-33>177){
            this->nunchukStick[0] = 209;
        }        
        if(-this->nunchukStick[1]+190<0){
           this->nunchukStick[1] = 190;
        }

        if(-this->nunchukStick[1]+190>131){
           this->nunchukStick[1] = 60;
        }
        
        canvasStick->get_window()->draw_line(gc_canvasStick, 0, -this->nunchukStick[1]+190,177, -this->nunchukStick[1]+190);
        canvasStick->get_window()->draw_line(gc_canvasStick, this->nunchukStick[0]-33, 0, this->nunchukStick[0]-33, 131);

        

        gc_canvasStick->set_foreground(color_white);
        canvasStick->get_window()->draw_line(gc_canvasStick, 100, 0, 100, 200);
        canvasStick->get_window()->draw_line(gc_canvasStick, 0, 100, 200, 100);
        

        

        while (gtkmain.events_pending())
            gtkmain.iteration();
    }

    void Gui::exitButton_clicked() {
        this->mainWindow->hide();
        exit(0);
    }

    void Gui::led1_clicked() {
        this->api->change_state_LED1 = true;
    }

    void Gui::led2_clicked() {
        this->api->change_state_LED2 = true;
    }

    void Gui::led3_clicked() {
        this->api->change_state_LED3 = true;
    }

    void Gui::led4_clicked() {
        this->api->change_state_LED4 = true;
    }

    Gui::~Gui() {
    }


}
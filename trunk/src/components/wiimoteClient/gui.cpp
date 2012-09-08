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
        //refXml->get_widget("buttonUp", this->buttonUp);
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

        //Get Labels
        refXml->get_widget("lblIR1x", ir1x);
        refXml->get_widget("lblIR1y", ir1y);
        refXml->get_widget("lblIR2x", ir2x);
        refXml->get_widget("lblIR2y", ir2y);
        refXml->get_widget("lblIR3x", ir3x);
        refXml->get_widget("lblIR3y", ir3y);

        refXml->get_widget("lblACCx", accX);
        refXml->get_widget("lblACCy", accY);
        refXml->get_widget("lblACCz", accZ);

        //Show the window
        mainWindow->show();


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

        this->accX->set_text(int2str(this->acc[0]));
        this->accY->set_text(int2str(this->acc[1]));
        this->accZ->set_text(int2str(this->acc[2]));

        while (gtkmain.events_pending())
            gtkmain.iteration();
    }

    Gui::~Gui() {
    }


}
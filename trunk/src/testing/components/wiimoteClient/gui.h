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
 *                 Jose María Cañas Plaza <jmplaza@gsyc.es>
 */

#include"API.h"

#ifndef WIIMOTECLIENT_GUI_H
#define WIIMOTECLIENT_GUI_H

namespace wiimoteClient{
    class Gui{
        
    public:

        Gui(Api *api);
        virtual ~Gui();

        void run(Api *api);
        
    private:
        
        Api* api;
        
        //SENSORS DATA
        int acc[3];
        int button;
        int ir1[2];
        int ir2[2];
        int ir3[2];
        int ir4[2];
        int nunchukAcc[3];
        int nunchukStick[2];
        int nunchukButton;
        bool guiVisible;        
        
        // Window
        Gtk::Window * mainWindow;
        
        //Canvas
        Gtk::DrawingArea *canvasStick;
        Glib::RefPtr<Gdk::GC> gc_canvasStick;
        Gdk::Color color_white;
        Gdk::Color color_black;
        Gdk::Color color_red;
        Glib::RefPtr<Gdk::Colormap> colormap;       
        
        //Buttons
        Gtk::Button *buttonUp;
        Gtk::Button *buttonLeft;
        Gtk::Button *buttonRight;
        Gtk::Button *buttonDown;
        Gtk::Button *buttonB;
        Gtk::Button *buttonMenos;
        Gtk::Button *buttonHome;
        Gtk::Button *buttonMas;
        Gtk::Button *buttonA;
        Gtk::Button *buttonUno;
        Gtk::Button *buttonDos;
        
        //Nunchuk Buttons
        Gtk::Button *ncButtonC;
        Gtk::Button *ncButtonZ;
        Gtk::Button *buttonExit;
        
        //Labels
        Gtk::Label *ir1x;
        Gtk::Label *ir1y;
        Gtk::Label *ir2x;
        Gtk::Label *ir2y;
        Gtk::Label *ir3x;
        Gtk::Label *ir3y;
        Gtk::Label *ir4x;
        Gtk::Label *ir4y;        
        
        Gtk::Label *accX;
        Gtk::Label *accY;
        Gtk::Label *accZ;
        
        //Check Buttons
        Gtk::CheckButton * led1;
        Gtk::CheckButton * led2;
        Gtk::CheckButton * led3;
        Gtk::CheckButton * led4;
        
        //Events
        void exitButton_clicked();
        
        //Nunchuk Labels
        Gtk::Label *ncAccX;
        Gtk::Label *ncAccY;
        Gtk::Label *ncAccZ;
        
        // Others			    
        Gtk::Main gtkmain;
        Glib::RefPtr<Gnome::Glade::Xml> refXml;
        std::string gladepath;
        
        
        //Functions
        std::string int2str(int num);
        void led1_clicked();
        void led2_clicked();
        void led3_clicked();
        void led4_clicked();
        
        
    };//class
    
    
}//end namespace
#endif //WIIMOTECLIENT_API_H

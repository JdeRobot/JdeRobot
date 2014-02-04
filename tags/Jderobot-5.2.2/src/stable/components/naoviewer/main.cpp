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
#include "control.h"
#include "displayer.h"

int main ( int argc, char** argv ) {
    Control* control = new Control();

    if ( control->connect() != -1 ) {
        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "jderobot.naooperator");
        
        NaoOperator* naooperator = new NaoOperator(control);
        
//        Displayer* displayer = new Displayer(control, naooperator);
//        displayer->init();
        
        int returned = app->run(*naooperator);
        
//        delete displayer;
        delete naooperator;
        
        return returned;
    }
    
    return -1;
}

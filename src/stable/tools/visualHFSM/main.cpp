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
 *  Authors : Borja Menéndez Moreno <b.menendez.moreno@gmail.com>
 *            José María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#include "visualhfsm.h"

int main (int argc, char **argv) {
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "jderobot.visualhfsm");


    //Load the Glade file and instiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/main_gui.glade");
    } catch ( const Glib::FileError& ex ) {
        std::cerr << BEGIN_RED << "FileError: " << ex.what() << END_COLOR << std::endl;
        return 1;
    } catch ( const Glib::MarkupError& ex ) {
        std::cerr << BEGIN_RED << "MarkupError: " << ex.what() << END_COLOR << std::endl;
        return 1;
    } catch ( const Gtk::BuilderError& ex ) {
        std::cerr << BEGIN_RED << "BuilderError: " << ex.what() << END_COLOR << std::endl;
        return 1;
    }

    //Get the GtkBuilder-instantiated dialog:
    VisualHFSM* visualhfsm = 0;
    refBuilder->get_widget_derived("DialogDerived", visualhfsm);
    if (visualhfsm) //Start:
        app->run(*visualhfsm);

    delete visualhfsm;

    return 0;
}

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

#ifndef LIBRARIESDIALOG_H
#define LIBRARIESDIALOG_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <gtkmm-3.0/gtkmm.h>
#include <sigc++/sigc++.h>

#include "../common.h"

// Definition of this class
class LibrariesDialog {
public:
	// Constructor
	LibrariesDialog ( std::list<std::string> listLibraries );

	// Destructor
	virtual ~LibrariesDialog ();

	// Popup initializer
	void init ();

	// signal accessor:
 	typedef sigc::signal<void, std::list<std::string> > type_signal;
  	type_signal signal_libraries ();

private:
	// Data structure
	Gtk::Dialog* dialog;
	Gtk::Button *button_accept, *button_cancel, *button_confirm;
	Gtk::Grid* grid;
	Gtk::Entry* entry_name;

	int row;
	std::list<std::string> listLibraries;
	
	// Private methods
	void on_button_confirm ();
	void on_button_delete ( int row );
	void on_button_accept ();
	void on_button_cancel ();

	type_signal m_signal;
};

#endif // LIBRARIESDIALOG_H
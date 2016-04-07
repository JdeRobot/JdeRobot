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

#ifndef LOADFILEDIALOG_H
#define LOADFILEDIALOG_H

#include <iostream>
#include <stdio.h>
#include <gtkmm-3.0/gtkmm.h>
#include <sigc++/sigc++.h>

#include "../common.h"

// Definition of this class
class LoadFileDialog {
public:
	// Constructor
	LoadFileDialog ();

	// Destructor
	virtual ~LoadFileDialog ();

	// Popup initializer
	void init ();

	//signal accessor:
  	typedef sigc::signal<void, std::string> type_signal;
  	type_signal signal_path ();

protected:
	// Data structure
	Gtk::FileChooserDialog* filechooserdialog;
	Gtk::Button *button_accept, *button_cancel;
	Gtk::Entry* entry_text;
	std::string filepath;
	
	// Private methods
	void on_button_accept ();
	bool on_key_released ( GdkEventKey* event );
	void on_button_cancel ();

	type_signal m_signal;
};

#endif // LOADFILEDIALOG_H
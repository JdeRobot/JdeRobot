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

#ifndef CONFIGFILEDIALOG_H
#define CONFIGFILEDIALOG_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <gtkmm-3.0/gtkmm.h>
#include <sigc++/sigc++.h>

#include "../common.h"
#include "../iceinterface.h"

// Definition of this class
class ConfigFileDialog {
public:
	// Constructor
	ConfigFileDialog ( std::list<IceInterface>& listInterfaces, std::map<std::string, std::string> mapInterfacesHeader );

	// Destructor
	virtual ~ConfigFileDialog ();

	// Popup initializer
	void init ();

	//signal accessor:
 	typedef sigc::signal<void, std::list<IceInterface>& > type_signal;
  	type_signal signal_config ();

private:
	// Data structure
	Gtk::Dialog* dialog;
	Gtk::Button *button_accept, *button_cancel, *button_confirm;
	Gtk::Grid* grid;
	Gtk::Entry *entry_name, *entry_ip, *entry_port;
	Gtk::ComboBox* combobox_interface;

	int row;
	std::list<IceInterface> listInterfaces;
	std::map<std::string, std::string> mapInterfacesHeader;

	class ModelColumns : public Gtk::TreeModel::ColumnRecord {
    public:
        ModelColumns () { add(m_col_name); }

        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };
    ModelColumns m_Columns;

    Glib::RefPtr<Gtk::ListStore> refTreeModel;
	
	// Private methods
	void on_button_confirm ();
	void on_button_delete ( int row );
	void on_button_accept ();
	void on_button_cancel ();

	type_signal m_signal;
};

#endif // CONFIGFILEDIALOG_H
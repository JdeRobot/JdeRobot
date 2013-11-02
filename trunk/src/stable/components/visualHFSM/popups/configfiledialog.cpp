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

#include "configfiledialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
ConfigFileDialog::ConfigFileDialog ( std::list<IceInterface>& listInterfaces, std::map<std::string, std::string> mapInterfacesHeader ) {
    this->listInterfaces = listInterfaces;
    this->mapInterfacesHeader = mapInterfacesHeader;
    this->row = 2;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
ConfigFileDialog::~ConfigFileDialog () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void ConfigFileDialog::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/configfile.glade");
    } catch (const Glib::FileError& ex) {
        std::cerr << BEGIN_RED << "FileError: " << ex.what() << END_COLOR << std::endl;
        fine = false;
    } catch(const Glib::MarkupError& ex) {
        std::cerr << BEGIN_RED << "MarkupError: " << ex.what() << END_COLOR << std::endl;
        fine = false;
    } catch(const Gtk::BuilderError& ex) {
        std::cerr << BEGIN_RED << "BuilderError: " << ex.what() << END_COLOR << std::endl;
        fine = false;
    }

    if (fine) {
        refBuilder->get_widget("dialog_configfile", this->dialog);

        refBuilder->get_widget("grid", this->grid);
        refBuilder->get_widget("entry_name", this->entry_name);
        refBuilder->get_widget("entry_ip", this->entry_ip);
        refBuilder->get_widget("entry_port", this->entry_port);
        refBuilder->get_widget("combobox_interface", this->combobox_interface);
        refBuilder->get_widget("button_confirm", this->button_confirm);

        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);

        this->refTreeModel = Gtk::ListStore::create(this->m_Columns);
        this->combobox_interface->set_model(this->refTreeModel);

        Gtk::TreeModel::Row row;
        for ( std::map<std::string, std::string>::iterator mapIterator = this->mapInterfacesHeader.begin();
                mapIterator != this->mapInterfacesHeader.end(); mapIterator++ ) {
            row = *(this->refTreeModel->append());
            row[m_Columns.m_col_name] = mapIterator->first;
        }

        this->combobox_interface->pack_start(m_Columns.m_col_name);

        for ( std::list<IceInterface>::iterator listInterfaceIterator = this->listInterfaces.begin();
                listInterfaceIterator != this->listInterfaces.end(); listInterfaceIterator++ ) {
            Gtk::Label* label_name = Gtk::manage(new Gtk::Label(listInterfaceIterator->getName()));
            Gtk::Label* label_ip = Gtk::manage(new Gtk::Label(listInterfaceIterator->getIp()));
            Gtk::Label* label_port = Gtk::manage(new Gtk::Label(listInterfaceIterator->getPort()));
            Gtk::Label* label_interface = Gtk::manage(new Gtk::Label(listInterfaceIterator->getInterface()));
            Gtk::Button* button = Gtk::manage(new Gtk::Button("Delete"));

            button->signal_clicked().connect(sigc::bind(
                            sigc::mem_fun(this, &ConfigFileDialog::on_button_delete), this->row));

            this->grid->attach(*label_name, 0, this->row, 1, 1);
            this->grid->attach(*label_ip, 1, this->row, 1, 1);
            this->grid->attach(*label_port, 2, this->row, 1, 1);
            this->grid->attach(*label_interface, 3, this->row, 1, 1);
            this->grid->attach(*button, 4, this->row, 1, 1);

            this->row++;
        }

        this->grid->show_all_children();

        this->button_confirm->signal_clicked().connect(sigc::mem_fun(this,
                                        &ConfigFileDialog::on_button_confirm));
        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &ConfigFileDialog::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &ConfigFileDialog::on_button_cancel));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void ConfigFileDialog::on_button_confirm () {
    Glib::ustring name;
    Gtk::TreeModel::iterator iter = this->combobox_interface->get_active();
    if (iter) {
        Gtk::TreeModel::Row row = *iter;
        name = row[m_Columns.m_col_name];
    } else
        std::cout << "Invalid iter" << std::endl;

    IceInterface iceinterface(std::string(this->entry_name->get_text()),
                                std::string(this->entry_ip->get_text()),
                                std::string(this->entry_port->get_text()),
                                std::string(name));
    this->listInterfaces.push_back(iceinterface);

    Gtk::Label* label_name = Gtk::manage(new Gtk::Label(this->entry_name->get_text()));
    Gtk::Label* label_ip = Gtk::manage(new Gtk::Label(this->entry_ip->get_text()));
    Gtk::Label* label_port = Gtk::manage(new Gtk::Label(this->entry_port->get_text()));
    Gtk::Label* label_interface = Gtk::manage(new Gtk::Label(name));
    Gtk::Button* button = Gtk::manage(new Gtk::Button("Delete"));

    button->signal_clicked().connect(sigc::bind(
                        sigc::mem_fun(this, &ConfigFileDialog::on_button_delete), this->row));

    this->grid->attach(*label_name, 0, this->row, 1, 1);
    this->grid->attach(*label_ip, 1, this->row, 1, 1);
    this->grid->attach(*label_port, 2, this->row, 1, 1);
    this->grid->attach(*label_interface, 3, this->row, 1, 1);
    this->grid->attach(*button, 4, this->row, 1, 1);

    this->grid->show_all_children();

    this->row++;
}

void ConfigFileDialog::on_button_delete ( int row ) {
    std::map<int, std::string> mapNames;
    for ( int i = 0; i < 5; i++ ) {
        if (i != 4)
            mapNames[i] = std::string(((Gtk::Label*)this->grid->get_child_at(i, row))->get_text());
        this->grid->remove(*this->grid->get_child_at(i, row));
    }

    std::list<IceInterface>::iterator listInterfacesIterator = this->listInterfaces.begin();
    while ( (!listInterfacesIterator->equals(mapNames[0], mapNames[1], mapNames[2], mapNames[3])) &&
            (listInterfacesIterator != this->listInterfaces.end()) )
        listInterfacesIterator++;

    if (listInterfacesIterator != this->listInterfaces.end())
        this->listInterfaces.erase(listInterfacesIterator);
}

void ConfigFileDialog::on_button_accept () {
	delete this->dialog;
    this->m_signal.emit(this->listInterfaces);
}

void ConfigFileDialog::on_button_cancel () {
    delete this->dialog;
}

ConfigFileDialog::type_signal ConfigFileDialog::signal_config () {
    return m_signal;
}
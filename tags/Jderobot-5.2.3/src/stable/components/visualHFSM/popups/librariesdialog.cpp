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

#include "librariesdialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
LibrariesDialog::LibrariesDialog ( std::list<std::string> listLibraries ) {
    this->listLibraries = listLibraries;
    this->row = 2;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
LibrariesDialog::~LibrariesDialog () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void LibrariesDialog::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/additional_libraries.glade");
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
        refBuilder->get_widget("dialog_additional_libraries", this->dialog);

        refBuilder->get_widget("grid", this->grid);
        refBuilder->get_widget("entry_name", this->entry_name);
        refBuilder->get_widget("button_confirm", this->button_confirm);

        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);

        for ( std::list<std::string>::iterator listLibrariesIterator = this->listLibraries.begin();
                listLibrariesIterator != this->listLibraries.end(); listLibrariesIterator++ ) {
            Gtk::Label* label_name = Gtk::manage(new Gtk::Label(*listLibrariesIterator));
            Gtk::Button* button = Gtk::manage(new Gtk::Button("Delete"));

            button->signal_clicked().connect(sigc::bind(
                            sigc::mem_fun(this, &LibrariesDialog::on_button_delete), this->row));

            this->grid->attach(*label_name, 0, this->row, 1, 1);
            this->grid->attach(*button, 1, this->row, 1, 1);

            this->row++;
        }

        this->grid->show_all_children();

        this->button_confirm->signal_clicked().connect(sigc::mem_fun(this,
                                        &LibrariesDialog::on_button_confirm));
        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &LibrariesDialog::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &LibrariesDialog::on_button_cancel));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void LibrariesDialog::on_button_confirm () {
    this->listLibraries.push_back(this->entry_name->get_text());
    
    Gtk::Label* label_name = Gtk::manage(new Gtk::Label(this->entry_name->get_text()));
    Gtk::Button* button = Gtk::manage(new Gtk::Button("Delete"));

    button->signal_clicked().connect(sigc::bind(
                        sigc::mem_fun(this, &LibrariesDialog::on_button_delete), this->row));

    this->grid->attach(*label_name, 0, this->row, 1, 1);
    this->grid->attach(*button, 1, this->row, 1, 1);

    this->grid->show_all_children();

    this->row++;
}

void LibrariesDialog::on_button_delete ( int row ) {
    std::string label(((Gtk::Label*)this->grid->get_child_at(0, row))->get_text());
    std::list<std::string>::iterator listLibrariesIterator = this->listLibraries.begin();
    while ( (listLibrariesIterator != this->listLibraries.end()) &&
            (listLibrariesIterator->compare(label) == 0) )
        listLibrariesIterator++;

    if (listLibrariesIterator != this->listLibraries.end())
        this->listLibraries.erase(listLibrariesIterator);

    for (int i = 0; i < 2; i++)
        this->grid->remove(*this->grid->get_child_at(i, row));
}

void LibrariesDialog::on_button_accept () {
	delete this->dialog;
    this->m_signal.emit(this->listLibraries);
}

void LibrariesDialog::on_button_cancel () {
    delete this->dialog;
}

LibrariesDialog::type_signal LibrariesDialog::signal_libraries () {
    return m_signal;
}
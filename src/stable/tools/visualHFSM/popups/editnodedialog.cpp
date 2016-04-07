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

#include "editnodedialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
EditNodeDialog::EditNodeDialog ( GuiNode* gnode ) {
    this->gnode = gnode;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
EditNodeDialog::~EditNodeDialog () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void EditNodeDialog::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/code.glade");
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
        refBuilder->get_widget("dialog_code", this->dialog);
        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);
        refBuilder->get_widget("textview", this->textview);

        Glib::RefPtr<Gtk::TextBuffer> textbuffer = Gtk::TextBuffer::create();
        textbuffer->set_text(this->gnode->getCode());
        this->textview->set_buffer(textbuffer);

        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &EditNodeDialog::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &EditNodeDialog::on_button_cancel));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void EditNodeDialog::on_button_accept () {
    this->gnode->setCode(this->textview->get_buffer()->get_text());
	delete this->dialog;
}

void EditNodeDialog::on_button_cancel () {
    delete this->dialog;
}
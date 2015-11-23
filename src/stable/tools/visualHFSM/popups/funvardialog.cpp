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

#include "funvardialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
FunVarDialog::FunVarDialog ( GuiSubautomata* gsubautomata ) {
    this->gsubautomata = gsubautomata;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
FunVarDialog::~FunVarDialog () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void FunVarDialog::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/funvar.glade");
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
        refBuilder->get_widget("dialog_funvar", this->dialog);

        refBuilder->get_widget("textview_functions", this->textview_functions);
        refBuilder->get_widget("textview_variables", this->textview_variables);

        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);

        Glib::RefPtr<Gtk::TextBuffer> textbufferFunctions = Gtk::TextBuffer::create();
        textbufferFunctions->set_text(this->gsubautomata->getFunctions());
        this->textview_functions->set_buffer(textbufferFunctions);

        Glib::RefPtr<Gtk::TextBuffer> textbufferVariables = Gtk::TextBuffer::create();
        textbufferVariables->set_text(this->gsubautomata->getVariables());
        this->textview_variables->set_buffer(textbufferVariables);

        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &FunVarDialog::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &FunVarDialog::on_button_cancel));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void FunVarDialog::on_button_accept () {
    this->gsubautomata->setVariables(this->textview_variables->get_buffer()->get_text());
    this->gsubautomata->setFunctions(this->textview_functions->get_buffer()->get_text());
	delete this->dialog;
}

void FunVarDialog::on_button_cancel () {
    delete this->dialog;
}
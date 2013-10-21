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

#include "timerdialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
TimerDialog::TimerDialog ( GuiSubautomata* gsubautomata ) {
    this->gsubautomata = gsubautomata;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
TimerDialog::~TimerDialog () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void TimerDialog::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/timing.glade");
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
        refBuilder->get_widget("dialog_timing", this->dialog);

        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);
        refBuilder->get_widget("entry_time", this->entry_text);

        this->entry_text->set_text(Glib::ustring(this->gsubautomata->getTime()));

        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &TimerDialog::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &TimerDialog::on_button_cancel));

        this->dialog->add_events(Gdk::KEY_PRESS_MASK);
        this->dialog->signal_key_release_event().connect(sigc::mem_fun(this,
                                        &TimerDialog::on_key_released));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void TimerDialog::on_button_accept () {
    this->gsubautomata->setTime(this->entry_text->get_text());
	delete this->dialog;
}

void TimerDialog::on_button_cancel () {
    delete this->dialog;
}

bool TimerDialog::on_key_released ( GdkEventKey* event ) {
    if (event->keyval == GDK_KEY_Return)
        this->on_button_accept();

    return true;
}
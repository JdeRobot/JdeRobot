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

#include "renametransitiondialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
RenameDialogTransition::RenameDialogTransition ( GuiTransition* gtransition ) {
    this->gtransition = gtransition;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
RenameDialogTransition::~RenameDialogTransition () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void RenameDialogTransition::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/name.glade");
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
        refBuilder->get_widget("dialog_name", this->dialog);

        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);
        refBuilder->get_widget("entry_text", this->entry_text);

        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &RenameDialogTransition::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &RenameDialogTransition::on_button_cancel));

        this->dialog->add_events(Gdk::KEY_PRESS_MASK);
        this->dialog->signal_key_release_event().connect(sigc::mem_fun(this,
                                        &RenameDialogTransition::on_key_released));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void RenameDialogTransition::on_button_accept () {
	this->gtransition->changeText(this->entry_text->get_text());
	delete this->dialog;
}

void RenameDialogTransition::on_button_cancel () {
    delete this->dialog;
}

bool RenameDialogTransition::on_key_released ( GdkEventKey* event ) {
    if (event->keyval == GDK_KEY_Return)
        this->on_button_accept();

    return true;
}
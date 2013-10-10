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

#include "edittransitiondialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
EditTransitionDialog::EditTransitionDialog ( GuiTransition* gtransition ) {
    this->gtransition = gtransition;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
EditTransitionDialog::~EditTransitionDialog () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void EditTransitionDialog::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/edittransition.glade");
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
        refBuilder->get_widget("dialog_edittransition", this->dialog);

        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);
        refBuilder->get_widget("entry_text", this->entry_text);
        refBuilder->get_widget("label_frame", this->label_frame);
        refBuilder->get_widget("radiobutton_temporal", this->radiobutton_temporal);
        refBuilder->get_widget("radiobutton_conditional", this->radiobutton_conditional);

        Gtk::RadioButton::Group group = this->radiobutton_temporal->get_group();
        this->radiobutton_conditional->set_group(group);

        this->radiobutton_temporal->signal_clicked().connect(sigc::mem_fun(this,
                                        &EditTransitionDialog::on_radio_temporal_clicked));
        this->radiobutton_conditional->signal_clicked().connect(sigc::mem_fun(this,
                                        &EditTransitionDialog::on_radio_conditional_clicked));
        
        if (this->gtransition->getTypeTrans().compare("condition") == 0) {
            this->on_radio_conditional_clicked();
            this->radiobutton_conditional->set_active();
        } else {
            this->on_radio_temporal_clicked();
            this->radiobutton_temporal->set_active();
        }

        this->entry_text->set_text(this->gtransition->getCodeTrans());

        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &EditTransitionDialog::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &EditTransitionDialog::on_button_cancel));

        this->dialog->add_events(Gdk::KEY_PRESS_MASK);
        this->dialog->signal_key_release_event().connect(sigc::mem_fun(this,
                                        &EditTransitionDialog::on_key_released));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void EditTransitionDialog::on_radio_temporal_clicked () {
    Glib::ustring str ("Time (ms):");
    this->putTextOnLabel(str);
    if (this->gtransition->getTypeTrans().compare("time") == 0)
        this->entry_text->set_text(this->gtransition->getCodeTrans());
    else
        this->entry_text->set_text("");
}

void EditTransitionDialog::on_radio_conditional_clicked () {
    Glib::ustring str ("Condition:");
    this->putTextOnLabel(str);
    if (this->gtransition->getTypeTrans().compare("condition") == 0)
        this->entry_text->set_text(this->gtransition->getCodeTrans());
    else
        this->entry_text->set_text("");
}

void EditTransitionDialog::on_button_accept () {
    if (this->radiobutton_conditional->get_active())
        this->gtransition->setTrans("condition", this->entry_text->get_text());
    else
        this->gtransition->setTrans("time", this->entry_text->get_text());

	delete this->dialog;
}

void EditTransitionDialog::on_button_cancel () {
    delete this->dialog;
}

bool EditTransitionDialog::on_key_released ( GdkEventKey* event ) {
    if (event->keyval == GDK_KEY_Return)
        this->on_button_accept();

    return true;
}

void EditTransitionDialog::putTextOnLabel ( const Glib::ustring& text ) {
    this->label_frame->set_text(text);
}
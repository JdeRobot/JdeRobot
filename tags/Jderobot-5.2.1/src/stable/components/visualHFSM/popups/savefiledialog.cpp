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

#include "savefiledialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
SaveFileDialog::SaveFileDialog () {}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
SaveFileDialog::~SaveFileDialog () {

}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void SaveFileDialog::init () {
    bool fine = true;
    // Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/save.glade");
    } catch ( const Glib::FileError& ex ) {
        std::cerr << BEGIN_RED << "FileError: " << ex.what() << END_COLOR << std::endl;
        fine = false;
    } catch ( const Glib::MarkupError& ex ) {
        std::cerr << BEGIN_RED << "MarkupError: " << ex.what() << END_COLOR << std::endl;
        fine = false;
    } catch ( const Gtk::BuilderError& ex ) {
        std::cerr << BEGIN_RED << "BuilderError: " << ex.what() << END_COLOR << std::endl;
        fine = false;
    }

    if (fine) {
        refBuilder->get_widget("filechooserdialog", this->filechooserdialog);

        refBuilder->get_widget("button_save", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);

        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                                &SaveFileDialog::on_button_accept));

        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                                &SaveFileDialog::on_button_cancel));

        Glib::RefPtr<Gtk::FileFilter> filter_xml = Gtk::FileFilter::create();
        filter_xml->set_name("VisualHFSM files");
        filter_xml->add_mime_type("text/xml");
        this->filechooserdialog->add_filter(filter_xml);

        Glib::RefPtr<Gtk::FileFilter> filter_any = Gtk::FileFilter::create();
        filter_any->set_name("Any files");
        filter_any->add_pattern("*");
        this->filechooserdialog->add_filter(filter_any);
        
        this->filechooserdialog->show_now();
    }
}

/*************************************************************
 * SIGNAL ACCESSOR
 *************************************************************/
SaveFileDialog::type_signal SaveFileDialog::signal_path () {
    return m_signal;
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void SaveFileDialog::on_button_accept () {
    this->filepath = this->filechooserdialog->get_filename();
    delete this->filechooserdialog;
    this->m_signal.emit(this->filepath);
}

void SaveFileDialog::on_button_cancel () {
    delete this->filechooserdialog;
}

bool SaveFileDialog::on_key_released ( GdkEventKey* event ) {
    if (event->keyval == GDK_KEY_Return)
        this->on_button_accept();

    return true;
}
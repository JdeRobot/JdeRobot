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

#include "importdialog.h"

/*************************************************************
 * CONSTRUCTOR
 *************************************************************/
ImportDialog::ImportDialog ( GuiSubautomata* gsubautomata ) {
    this->gsubautomata = gsubautomata;
}

/*************************************************************
 * DESTRUCTOR
 *************************************************************/
ImportDialog::~ImportDialog () {}

/*************************************************************
 * POPUP INITIALIZER
 *************************************************************/
void ImportDialog::init () {
    bool fine = true;
	// Load the GtkBuilder file and instantiate its widgets:
    Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
    try {
        refBuilder->add_from_file("gui/import.glade");
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
        refBuilder->get_widget("dialog_import", this->dialog);

        refBuilder->get_widget("button_accept", this->button_accept);
        refBuilder->get_widget("button_cancel", this->button_cancel);

        refBuilder->get_widget("checkbutton_laser", this->checkbutton_laser);
        refBuilder->get_widget("checkbutton_sonar", this->checkbutton_sonar);
        refBuilder->get_widget("checkbutton_camera", this->checkbutton_camera);
        refBuilder->get_widget("checkbutton_pose3dencoders", this->checkbutton_pose3dencoders);
        refBuilder->get_widget("checkbutton_pose3dmotors", this->checkbutton_pose3dmotors);

        if (gsubautomata->findInterface(std::string("laser")))
            this->checkbutton_laser->set_active(true);

        if (gsubautomata->findInterface(std::string("sonar")))
            this->checkbutton_sonar->set_active(true);

        if (gsubautomata->findInterface(std::string("camera")))
            this->checkbutton_camera->set_active(true);

        if (gsubautomata->findInterface(std::string("pose3dencoders")))
            this->checkbutton_pose3dencoders->set_active(true);

        if (gsubautomata->findInterface(std::string("pose3dmotors")))
            this->checkbutton_pose3dmotors->set_active(true);

        this->button_accept->signal_clicked().connect(sigc::mem_fun(this,
                                        &ImportDialog::on_button_accept));
        this->button_cancel->signal_clicked().connect(sigc::mem_fun(this,
                                        &ImportDialog::on_button_cancel));

        this->dialog->add_events(Gdk::KEY_PRESS_MASK);
        this->dialog->signal_key_release_event().connect(sigc::mem_fun(this,
                                        &ImportDialog::on_key_released));

        this->dialog->show_now();
    }
}

/*************************************************************
 * PRIVATE METHODS
 *************************************************************/
void ImportDialog::on_button_accept () {
    std::list<std::string> listInterfaces;

    if (this->checkbutton_laser->get_active())
        listInterfaces.push_back(std::string("<jderobot/laser.h>"));

    if (this->checkbutton_sonar->get_active())
        listInterfaces.push_back(std::string("<jderobot/sonar.h>"));

    if (this->checkbutton_camera->get_active())
        listInterfaces.push_back(std::string("<jderobot/camera.h>"));

    if (this->checkbutton_pose3dencoders->get_active())
        listInterfaces.push_back(std::string("<jderobot/pose3dencoders.h>"));

    if (this->checkbutton_pose3dmotors->get_active())
        listInterfaces.push_back(std::string("<jderobot/pose3dmotors.h>"));

    this->gsubautomata->setInterfaces(listInterfaces);

	delete this->dialog;
}

void ImportDialog::on_button_cancel () {
    delete this->dialog;
}

bool ImportDialog::on_key_released ( GdkEventKey* event ) {
    if (event->keyval == GDK_KEY_Return)
        this->on_button_accept();

    return true;
}
/*
 *
 *  Copyright (C) 1997-2013 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Authors :
 *						Francisco Rivas <franciscomiguel.rivas@urjc.es>
 *
 */
#ifndef RECORDERGUI_H_
#define RECORDERGUI_H_

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include "control.h"

namespace replayer {

class replayergui {
public:
	replayergui(control *c);
	virtual ~replayergui();
	void update();

private:
	Glib::RefPtr<Gnome::Glade::Xml> refXml;
	Gtk::Main gtkmain;
	Gtk::Window* mainwindow;
	Gtk::Button* w_stop;
	Gtk::Button* w_resume;
	Gtk::Button* w_step;
	Gtk::ToggleButton* w_repeat;

	control *controller;

	//gtk functions
	void on_clicked_stop();
	void on_clicked_resume();
	void on_change_repeat();
	void on_clicked_step();


};

} /* namespace recorder */
#endif /* RECORDERGUI_H_ */

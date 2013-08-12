/*
 * recordergui.h
 *
 *  Created on: 14/05/2013
 *      Author: frivas
 */

#ifndef RECORDERGUI_H_
#define RECORDERGUI_H_

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <jderobot/replayControl.h>

namespace replayController {

class replayControllergui {
public:
	replayControllergui(jderobot::replayControlPrx c);
	virtual ~replayControllergui();
	bool update();

private:
	Glib::RefPtr<Gnome::Glade::Xml> refXml;
	Gtk::Main gtkmain;
	Gtk::Window* mainwindow;
	Gtk::Button* w_stop;
	Gtk::Button* w_resume;
	Gtk::Button* w_step;
	Gtk::Button* w_exit;
	Gtk::ToggleButton* w_repeat;

	jderobot::replayControlPrx controller;
	bool exit;

	//gtk functions
	void on_clicked_stop();
	void on_clicked_resume();
	void on_change_repeat();
	void on_clicked_step();
	void on_clicked_exit();



};

} /* namespace recorder */
#endif /* RECORDERGUI_H_ */


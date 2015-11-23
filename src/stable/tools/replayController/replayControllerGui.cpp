/*
 * recordergui.cpp
 *
 *  Created on: 14/05/2013
 *      Author: frivas
 */

#include "replayControllerGui.h"


namespace replayController {

replayControllergui::replayControllergui(jderobot::replayControlPrx c): gtkmain(0,0) {
	// TODO Auto-generated constructor stub

	this->controller=c;
	this->exit=false;
	std::cout << "Loading glade\n";
	refXml = Gnome::Glade::Xml::create("./replayControllerGui.glade");

		/*Get widgets*/
	refXml->get_widget("window1",mainwindow);
	refXml->get_widget("button1", w_stop);
	refXml->get_widget("button2", w_resume);
	refXml->get_widget("button3", w_step);
	refXml->get_widget("button4", w_exit);
	refXml->get_widget("togglebutton1", w_repeat);

	w_repeat->set_active(true);

	w_stop->signal_clicked().connect(sigc::mem_fun(this,&replayControllergui::on_clicked_stop));
	w_resume->signal_clicked().connect(sigc::mem_fun(this,&replayControllergui::on_clicked_resume));
	w_repeat->signal_clicked().connect(sigc::mem_fun(this,&replayControllergui::on_change_repeat));
	w_step->signal_clicked().connect(sigc::mem_fun(this,&replayControllergui::on_clicked_step));
	w_exit->signal_clicked().connect(sigc::mem_fun(this,&replayControllergui::on_clicked_exit));


	mainwindow->show();

}

replayControllergui::~replayControllergui() {
	// TODO Auto-generated destructor stub
}

bool replayControllergui::update(){
	while (gtkmain.events_pending())
		gtkmain.iteration();
	return !(this->exit);
}

void replayControllergui::on_clicked_stop(){
	this->controller->pause();
}

void replayControllergui::on_clicked_resume(){
	this->controller->resume();
}

void replayControllergui::on_change_repeat(){
	this->controller->setReplay(this->w_repeat->get_active());
}

void replayControllergui::on_clicked_step(){
	this->controller->setStep(100);
}

void replayControllergui::on_clicked_exit(){
	this->exit=true;
}


} /* namespace recorder */


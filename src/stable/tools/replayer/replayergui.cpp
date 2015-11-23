/*
 * recordergui.cpp
 *
 *  Created on: 14/05/2013
 *      Author: frivas
 */

#include "replayergui.h"


namespace replayer {

replayergui::replayergui(control* c): gtkmain(0,0) {
	// TODO Auto-generated constructor stub

	this->controller=c;
			std::cout << "Loading glade\n";
		refXml = Gnome::Glade::Xml::create("./replayergui.glade");

			/*Get widgets*/
	    refXml->get_widget("window1",mainwindow);
	    refXml->get_widget("button1", w_stop);
	    refXml->get_widget("button2", w_resume);
	    refXml->get_widget("button3", w_step);
	    refXml->get_widget("togglebutton1", w_repeat);

	    w_repeat->set_active(true);

	    w_stop->signal_clicked().connect(sigc::mem_fun(this,&replayergui::on_clicked_stop));
	    w_resume->signal_clicked().connect(sigc::mem_fun(this,&replayergui::on_clicked_resume));
	    w_repeat->signal_clicked().connect(sigc::mem_fun(this,&replayergui::on_change_repeat));
	    w_step->signal_clicked().connect(sigc::mem_fun(this,&replayergui::on_clicked_step));


	    mainwindow->show();

}

replayergui::~replayergui() {
	// TODO Auto-generated destructor stub
}

void replayergui::update(){
	while (gtkmain.events_pending())
		gtkmain.iteration();
}

void replayergui::on_clicked_stop(){
	std::cout << "paro" << std::endl;
	this->controller->stop();
}

void replayergui::on_clicked_resume(){
	std::cout << "reinicio" << std::endl;
	this->controller->resume();
}

void replayergui::on_change_repeat(){
	std::cout << "repito" << std::endl;
	this->controller->setRepeat(this->w_repeat->get_active());
}

void replayergui::on_clicked_step(){
	std::cout << "step" << std::endl;
	this->controller->setStep(100);
}

} /* namespace recorder */

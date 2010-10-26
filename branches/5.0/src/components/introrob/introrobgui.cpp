/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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
 *  Authors : Javier Vázquez Pereda <javiervazper@yahoo.es>
 *
 */

#include "introrobgui.h" 
#include <iostream>
#include <cmath>

namespace introrob{
  const std::string gladepath = std::string(GLADE_DIR) + 
    std::string("/introrobgui.glade");

  introrobgui::introrobgui() 
    : gtkmain(0,0),frameCount(0) {

    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(gladepath);
    refXml->get_widget("image", gtkimage);
    refXml->get_widget("mainwindow",mainwindow);
    refXml->get_widget("fpslabel",fpslabel);
    refXml->get_widget("stopbutton",stopbutton);
    refXml->get_widget("hscalev",hscalev);
    refXml->get_widget("hscalew",hscalew);
//    refXml->get_widget("hscalemap",hscalemap);
    refXml->get_widget("resetv",resetv);
    refXml->get_widget("resetw",resetw);
    refXml->get_widget("yourcode",yourcode);
    refXml->get_widget("manual",manual);
    refXml->get_widget("hbox2",hbox2);
	//refXml->get_widget("canvasmap",canvasmap);

	yourcode_cond = false;
	stop_cond = false;
	v=0;
	w=0;
	MAX_V=1000;
	MAX_W=2.38;
	close=false;

        
    // start the timer for calculating the number of frames per second
    // the images are being displayed at
    oldFrameTime = IceUtil::Time::now();
	
	mainwindow->signal_delete_event().connect(sigc::mem_fun(this,&introrobgui::on_delete_event_mainwindow));	
	hscalev->signal_change_value().connect(sigc::mem_fun(this,&introrobgui::on_change_value_hscalev));
	hscalew->signal_change_value().connect(sigc::mem_fun(this,&introrobgui::on_change_value_hscalew));
	resetv->signal_clicked().connect(sigc::mem_fun(this,&introrobgui::on_clicked_resetv));
	resetw->signal_clicked().connect(sigc::mem_fun(this,&introrobgui::on_clicked_resetw));
	yourcode->signal_clicked().connect(sigc::mem_fun(this,&introrobgui::on_clicked_yourcode));
	manual->signal_clicked().connect(sigc::mem_fun(this,&introrobgui::on_clicked_manual));
//	yourcode->signal_toggled().connect(sigc::mem_fun(this,&introrobgui::on_clicked_yourcode));
//	manual->signal_toggled().connect(sigc::mem_fun(this,&introrobgui::on_clicked_manual));
	stopbutton->signal_clicked().connect(sigc::mem_fun(this,&introrobgui::on_toggled_stopbutton));

  }
    

  introrobgui::~introrobgui() {	

   }


  float introrobgui::getV(){
	return v;
  }
    
  float introrobgui::getW(){
	return w;
  }

	
  bool introrobgui::isClosed(){
	return close;
  }

  //  return mainwindow->is_visible();
  bool introrobgui::isVisible(){
  }

  void introrobgui::updatecamera( const colorspaces::Image& image )
  {
    colorspaces::ImageRGB8 img_rgb8(image);//conversion will happen if needed
    Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
      Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_rgb8.width,
				    img_rgb8.height,
				    img_rgb8.step);
    
    gtkimage->clear();
    gtkimage->set(imgBuff);
    displayFrameRate();
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }
    
  void
  introrobgui::displayFrameRate()
  {
    double diff;
    IceUtil::Time diffT;

    currentFrameTime = IceUtil::Time::now();
    diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
    if (diff < 1000.0)
      frameCount++;
    else{
      oldFrameTime = currentFrameTime;
      fps = frameCount*1000.0/diff;
      frameCount=0;
      // Display the frame rate
      std::stringstream fpsString;
      fpsString << "fps = " << int(fps);
      fpslabel->set_label(fpsString.str());
    }
  }


	bool introrobgui::on_change_value_hscalev(Gtk::ScrollType scroll, double new_value)
	{

		if (new_value>MAX_V)
			v=MAX_V;
		else if (new_value<-MAX_V)
			v=-MAX_V;
		else
			v=new_value;
		printf("v=%.2f\n",v);
	}


	bool introrobgui::on_change_value_hscalew(Gtk::ScrollType scroll, double new_value)
	{

		if (new_value>MAX_W)
			w=MAX_W;
		else if (new_value<-MAX_W)
			w=-MAX_W;
		else
			w=new_value;
		printf("w=%.2f\n",w);
	}

	bool introrobgui::on_delete_event_mainwindow(GdkEventAny* event){
		close=true;
	}

	void introrobgui::on_clicked_resetv()
	{
		//bool val=resetv->get_focus_on_click();
		//printf("on_clicked_resetv = %d\n",(int)val);
		//adj_hscalev.set_value((double)0);
//Da Segmentation fault		hscalev->set_adjustment(*adj_hscalev);
		v=0;
		hscalev->set_value(0);

	}
	void introrobgui::on_clicked_resetw(){
		//printf("on_clicked_resetw\n");		
		w=0;
		hscalew->set_value(0);
	}
	void introrobgui::on_clicked_yourcode(){
		if (yourcode->get_active()) {
		//	printf("active yourcode\n");
			hbox2->hide();
			yourcode_cond=true;
		}
	}
	void introrobgui::on_clicked_manual(){
		if (manual->get_active()) {
		//	printf("active manual\n");
			hbox2->show();
			yourcode_cond=false;
		}
	}
	void introrobgui::on_toggled_stopbutton(){
		if (stopbutton->get_active()) {
		//	printf("on_toggled_stopbutton\n");
			stopbutton->set_label("start");
			stop_cond=true;
		}
		else {
			stopbutton->set_label("stop");
			stop_cond=false;
		}

	}

	bool introrobgui::isYourcode(){
		return yourcode_cond;
	}

	bool introrobgui::isStopped(){
		return stop_cond;
	}

}//namespace

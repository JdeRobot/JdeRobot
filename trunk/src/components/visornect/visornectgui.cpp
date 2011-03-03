

#include "visornectgui.h"

namespace visornect {

	const std::string gladepath = std::string(GLADE_DIR) + std::string("/visornectgui.glade");

	visornectgui::visornectgui(jderobot::PTMotorsPrx m, jderobot::KinectLedsPrx l, std::string path): gtkmain(0,0) {
		/*Create controller*/
		this->mprx=m;
		this->leds=l;

		/*Init OpenGL*/
		if(!Gtk::GL::init_check(NULL, NULL))	{
			std::cerr << "Couldn't initialize GL\n";
			std::exit(1);
		}

    std::cout << "Loading glade\n";
    refXml = Gnome::Glade::Xml::create(gladepath);

		/*Get widgets*/
    refXml->get_widget("VisorNect", mainwindow);
	refXml->get_widget("imageRGB", w_imageRGB);
	refXml->get_widget("imageDEPTH", w_imageDEPTH);
	refXml->get_widget("button_ir_rgb", w_change_ir_rgb);
	refXml->get_widget("button_up", w_up);
	refXml->get_widget("button_down", w_down);
	refXml->get_widget("buttonled_off", w_led_off);
	refXml->get_widget("buttonled_green", w_led_green);
	refXml->get_widget("buttonled_red", w_led_red);
	refXml->get_widget("buttonled_yellow", w_led_yellow);
	refXml->get_widget("buttonled_bgreen", w_led_bgreen);
	refXml->get_widget("buttonled_bred", w_led_bred);
	
	

	if (m!=NULL){
		w_up->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_up));
		w_down->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_down));
	}
	if (l!=NULL){
		w_led_off->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_led_off));
		w_led_green->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_led_green));
		w_led_red->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_led_red));
		w_led_yellow->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_led_yellow));
		w_led_bgreen->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_led_bgreen));
		w_led_bred->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_led_bred));
	}
	else{
		std::cout << "null" << std::endl;
	}
	w_change_ir_rgb->signal_clicked().connect(sigc::mem_fun(this,&visornectgui::on_clicked_change_ir_rgb));


		// Imagen de la cámara

		// Mundo OpenGL
    refXml->get_widget_derived("gl_world",world);

		/*Show window. Note: Set window visibility to false in Glade, otherwise opengl won't work*/
		world->readFile(path);
		mainwindow->show();
			

	}

	visornectgui::~visornectgui() {
		//delete this->controller;
	}


void 
visornectgui::update( const colorspaces::Image& imageRGB, const colorspaces::Image& imageDEPTH )
{
			if (1){
				colorspaces::ImageRGB8 img_rgb888(imageRGB);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
	    		w_imageRGB->clear();
	    		w_imageRGB->set(imgBuff);
	    		displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			}
			if (1){
				colorspaces::ImageRGB8 img_rgb888(imageDEPTH);//conversion will happen if needed
				Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) img_rgb888.data,Gdk::COLORSPACE_RGB,false,8,img_rgb888.width,img_rgb888.height,img_rgb888.step);    
	    		w_imageDEPTH->clear();
	    		w_imageDEPTH->set(imgBuff);
	    		displayFrameRate();
	    		while (gtkmain.events_pending())
	      		gtkmain.iteration();
			}
			


		//}
		//mainwindow->resize(1,1);
    	while (gtkmain.events_pending())
      	gtkmain.iteration();
  	}


	bool visornectgui::isClosed(){
		return false;
	}

	  bool visornectgui::isVisible() {
    return mainwindow->is_visible();
  }

void visornectgui::displayFrameRate()
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
		//fpslabel->set_label(fpsString.str());
		}
	}


void visornectgui::on_clicked_up(){
	jderobot::PTMotorsDataPtr p;

	p=mprx->getPTMotorsData();
	p->latitude=p->latitude+1;
	
	mprx->setPTMotorsData(p);
}

void visornectgui::on_clicked_down(){
	jderobot::PTMotorsDataPtr p;

	p=mprx->getPTMotorsData();
	p->latitude=p->latitude-1;
	
	mprx->setPTMotorsData(p);
}

void visornectgui::on_clicked_change_ir_rgb(){
	cam->changeCamera();
}

void visornectgui::setCameraPrx(jderobot::KinectPrx c){
	cam=c;
}

void visornectgui::on_clicked_led_off(){
	leds->setLedActive(jderobot::OFF);
}

void visornectgui::on_clicked_led_green(){
	leds->setLedActive(jderobot::GREEN);
}

void visornectgui::on_clicked_led_red(){
	leds->setLedActive(jderobot::RED);
}

void visornectgui::on_clicked_led_yellow(){
	leds->setLedActive(jderobot::YELLOW);
}

void visornectgui::on_clicked_led_bgreen(){
	leds->setLedActive(jderobot::BLINKGREEN);
}

void visornectgui::on_clicked_led_bred(){
	leds->setLedActive(jderobot::BLINKREDYELLOW);
}

} // namespace

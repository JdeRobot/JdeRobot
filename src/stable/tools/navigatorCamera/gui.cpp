#include "gui.h"

namespace navigatorCamera {

	Gui::Gui(Sharer *sharer,const std::string& gladeFile) : gtkmain(0, 0)
	{

		this->sharer = sharer;

		std::cout << "Loading glade." << std::endl;

		const std::string gladepath = std::string(GLADE_DIR) + gladeFile;
		refXml = Gnome::Glade::Xml::create(gladepath);

		refXml->get_widget("showWindow", showWindow);
		refXml->get_widget("RGB", RGB);
	        refXml->get_widget("teleopAreaTrl", teleopAreaTrl);
	        refXml->get_widget("teleopAreaRtt", teleopAreaRtt);


		refXml->get_widget("txtInfoX", txtInfoX);
		refXml->get_widget("txtInfoY", txtInfoY);
		refXml->get_widget("txtInfoZ", txtInfoZ);
		refXml->get_widget("txtInfoH", txtInfoH);
		refXml->get_widget("txtInfoQ0", txtInfoQ0);
		refXml->get_widget("txtInfoQ1", txtInfoQ1);
		refXml->get_widget("txtInfoQ2", txtInfoQ2);
		refXml->get_widget("txtInfoQ3", txtInfoQ3);

		refXml->get_widget("controlWindow", controlWindow);

		refXml->get_widget("bttnTrlnRight", bttnTrlnRight);
		refXml->get_widget("bttnTrlnLeft", bttnTrlnLeft);
		refXml->get_widget("bttnTrlnFront", bttnTrlnFront);
		refXml->get_widget("bttnTrlnBack", bttnTrlnBack);
		refXml->get_widget("bttnTrlnUp", bttnTrlnUp);
		refXml->get_widget("bttnTrlnDown", bttnTrlnDown);

		refXml->get_widget("bttnRttnRight", bttnRttnRight);
		refXml->get_widget("bttnRttnLeft", bttnRttnLeft);
		refXml->get_widget("bttnRttnUp", bttnRttnUp);
		refXml->get_widget("bttnRttnDown", bttnRttnDown);
		refXml->get_widget("bttnRttnCW", bttnRttnCW);
		refXml->get_widget("bttnRttnACW", bttnRttnACW);

		refXml->get_widget("spnbTrlnStp", spnbTrlnStp);
		refXml->get_widget("spnbRttnStp", spnbRttnStp);

		refXml->get_widget("bttnRstr", bttnRstr);

		teleopAreaTrl->signal_event().connect(sigc::mem_fun(this, &Gui::on_press_teleopAreaTrl));
		teleopAreaRtt->signal_event().connect(sigc::mem_fun(this, &Gui::on_press_teleopAreaRtt));

		bttnTrlnRight->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnTrlnRight_onButtonClick));
		bttnTrlnLeft->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnTrlnLeft_onButtonClick));
		bttnTrlnFront->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnTrlnFront_onButtonClick));
		bttnTrlnBack->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnTrlnBack_onButtonClick));
		bttnTrlnUp->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnTrlnUp_onButtonClick));
		bttnTrlnDown->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnTrlnDown_onButtonClick));

		bttnRttnRight->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnRttnRight_onButtonClick));
		bttnRttnLeft->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnRttnLeft_onButtonClick));
		bttnRttnUp->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnRttnUp_onButtonClick));
		bttnRttnDown->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnRttnDown_onButtonClick));
		bttnRttnCW->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnRttnCW_onButtonClick));
		bttnRttnACW->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_bttnRttnACW_onButtonClick));

		spnbTrlnStp->signal_value_changed().connect(sigc::mem_fun(this,&Gui::gtk_spnbTrlnStp_valueChanged));
		spnbRttnStp->signal_value_changed().connect(sigc::mem_fun(this,&Gui::gtk_spnbRttnStp_valueChanged));

		bttnRstr->signal_clicked().connect(sigc::mem_fun(this,&Gui::gtk_restart_onButtonClick));

		std::cout << "Glade done." << std::endl;

		spnbTrlnStp->set_value(sharer->getTranslationStep());
		spnbRttnStp->set_value(sharer->getRotationStep());

		controlWindow->add_events(Gdk::KEY_PRESS_MASK);
		controlWindow->signal_key_press_event().connect(sigc::mem_fun(this,&Gui::on_key_press_event));

		/*Show window. Note: Set window visibility to false in Glade, otherwise opengl won't work*/
		showWindow->show();
		controlWindow->show();

		teleopAreaTrl->signal_unrealize();
		teleopAreaTrl->signal_realize();
		teleopAreaTrl->set_child_visible(TRUE);
		teleopAreaTrl->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::VISIBILITY_NOTIFY_MASK | Gdk::BUTTON1_MOTION_MASK);
		gc_teleoperateTrl = Gdk::GC::create(teleopAreaTrl->get_window());

		teleopAreaRtt->signal_unrealize();
		teleopAreaRtt->signal_realize();
		teleopAreaRtt->set_child_visible(TRUE);
		teleopAreaRtt->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::VISIBILITY_NOTIFY_MASK | Gdk::BUTTON1_MOTION_MASK);
		gc_teleoperateRtt = Gdk::GC::create(teleopAreaRtt->get_window());
		

		//Colors
		colormapTrl = teleopAreaTrl->get_colormap();
		colormapRtt = teleopAreaRtt->get_colormap();
		color_white = Gdk::Color("#FFFFFF");
		color_black = Gdk::Color("#000000");
		color_red = Gdk::Color("#FF0000");
		colormapTrl->alloc_color(color_white);
		colormapTrl->alloc_color(color_black);
		colormapTrl->alloc_color(color_red);

		colormapRtt->alloc_color(color_white);
		colormapRtt->alloc_color(color_black);
		colormapRtt->alloc_color(color_red);

		m_imageTrl = Gdk::Pixbuf::create_from_file("myimage.png");
		m_imageRtt = Gdk::Pixbuf::create_from_file("myimage.png");
		this->previous_event_x = 100;
		this->previous_event_y = 100;
		this->prev_x = 0.0;
		this->prev_y = 0.0;
		this->previous_event_yaw = 100;
		this->previous_event_pitch = 100;
		prev_yaw = 0.0;
		prev_pitch = 0.0;

	}

	Gui::~Gui()
	{
		delete this->showWindow;
		delete this->controlWindow;
	}

	bool Gui::isVisible()
	{
		return showWindow->get_visible() && controlWindow->get_visible();
	}

	void Gui::display()
	{
		cv::Mat image = sharer->getImage();
		Glib::RefPtr<Gdk::Pixbuf> imgBuff =  Gdk::Pixbuf::create_from_data((const guint8*) image.data, Gdk::COLORSPACE_RGB, false, 8, image.cols, image.rows, image.step);
		RGB->clear();
		RGB->set(imgBuff);
        	this->teleoperateTrl();
        	this->teleoperateRtt();
		this->showPose3d();



		while ( gtkmain.events_pending() )
			gtkmain.iteration();

		this->showWindow->queue_draw();
	}

	bool Gui::on_press_teleopAreaTrl(GdkEvent * event) {
		float event_x = event->button.x;
		float event_y = event->button.y;
		float k = 0.01;
		float p = -1;
		float x_normalized, y_normalized;
		static gboolean dragging = FALSE;

		switch (event->type) {
		    case GDK_BUTTON_PRESS:
		        if (event->button.button == 3) {
		            this->previous_event_x = event->button.x;
		            this->previous_event_y = event->button.y;
		        }
		        if (event->button.button == 1) {
		            GdkCursor *cursor;
		            cursor = gdk_cursor_new(GDK_FLEUR);
		            gdk_cursor_destroy(cursor);

		            dragging = true;
		        }
		        break;

		    case GDK_MOTION_NOTIFY:
		        if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) {
		            this->previous_event_x = event_x;
		            this->previous_event_y = event_y;
		            this->teleoperateTrl();
		        }
		        break;

		    case GDK_BUTTON_RELEASE:
		        dragging = FALSE;
		        break;

		    default:
		        break;
		}
		x_normalized = 20 * (k * previous_event_y + p)*(-1);
       		y_normalized = 20 * (k * previous_event_x + p)*(-1);

		sharer->setSpeedX(x_normalized/100.0);
		sharer->setSpeedY(y_normalized/100.0);


		/*if (x_normalized > prev_x)
			sharer->changePose3dTranslation(1., 0., 0.);
		else if (x_normalized < prev_x)
			sharer->changePose3dTranslation(-1., 0., 0.);

		if (y_normalized > prev_y)
			sharer->changePose3dTranslation(0., -1., 0.);
		else if (y_normalized < prev_y)
			sharer->changePose3dTranslation(0., 1., 0.);
		this->prev_x = x_normalized;
		this->prev_y = y_normalized;
		//Set to API
		//pthread_mutex_lock(&api->controlGui);
		//api->setYawValue(yaw_normalized);
		//api->setPitchValue(pitch_normalized);
		//pthread_mutex_unlock(&api->controlGui);*/
	    }

	void Gui::teleoperateTrl() {
		gc_teleoperateTrl->set_foreground(color_black);
		teleopAreaTrl->get_window()->draw_rectangle(gc_teleoperateTrl, true, 0, 0, 200, 200);


		teleopAreaTrl->get_window()->draw_pixbuf(m_imageTrl,
		        0, 0, this->previous_event_x-12, this->previous_event_y-12 ,
		        m_imageTrl->get_width(),
		        m_imageTrl->get_height(),
		        Gdk::RGB_DITHER_NONE,
		        -1, -1);
		
		gc_teleoperateTrl->set_foreground(color_red);
		teleopAreaTrl->get_window()->draw_line(gc_teleoperateTrl, 0, previous_event_y, 200, previous_event_y);
		teleopAreaTrl->get_window()->draw_line(gc_teleoperateTrl, previous_event_x, 0, previous_event_x, 200);


		gc_teleoperateTrl->set_foreground(color_white);
		teleopAreaTrl->get_window()->draw_line(gc_teleoperateTrl, 100, 0, 100, 200);
		teleopAreaTrl->get_window()->draw_line(gc_teleoperateTrl, 0, 100, 200, 100);
	}

	bool Gui::on_press_teleopAreaRtt(GdkEvent * event) {
		float event_yaw = event->button.x;
		float event_pitch = event->button.y;
		float k = 0.01;
		float p = -1;
		float yaw_normalized, pitch_normalized;
		static gboolean dragging = FALSE;

		switch (event->type) {
		    case GDK_BUTTON_PRESS:
		        if (event->button.button == 3) {
		            this->previous_event_yaw = event->button.x;
		            this->previous_event_pitch = event->button.y;
		        }
		        if (event->button.button == 1) {
		            GdkCursor *cursor;
		            cursor = gdk_cursor_new(GDK_FLEUR);
		            gdk_cursor_destroy(cursor);

		            dragging = true;
		        }
		        break;

		    case GDK_MOTION_NOTIFY:
		        if (dragging && (event->motion.state & GDK_BUTTON1_MASK)) {
		            this->previous_event_yaw = event_yaw;
		            this->previous_event_pitch = event_pitch;
		            this->teleoperateRtt();
		        }
		        break;

		    case GDK_BUTTON_RELEASE:
		        dragging = FALSE;
		        break;

		    default:
		        break;
		}
		yaw_normalized = pi * (k * previous_event_yaw + p)*(-1);
		pitch_normalized = pi  * (k * previous_event_pitch + p);

		if (yaw_normalized > prev_yaw)
			sharer->changePose3dRotation(-1., 0., 0.);
		else if (yaw_normalized < prev_yaw)
			sharer->changePose3dRotation(1., 0., 0.);

		if (pitch_normalized > prev_pitch)
			sharer->changePose3dRotation(0., -1., 0.);
		else if (pitch_normalized < prev_pitch)
			sharer->changePose3dRotation(0., 1., 0.);
		this->prev_yaw = yaw_normalized;
		this->prev_pitch = pitch_normalized;
		//Set to API
		//pthread_mutex_lock(&api->controlGui);
		//api->setYawValue(yaw_normalized);
		//api->setPitchValue(pitch_normalized);
		//pthread_mutex_unlock(&api->controlGui);
	}

    	void Gui::teleoperateRtt() {
		gc_teleoperateRtt->set_foreground(color_black);
		teleopAreaRtt->get_window()->draw_rectangle(gc_teleoperateRtt, true, 0, 0, 200, 200);


		teleopAreaRtt->get_window()->draw_pixbuf(m_imageRtt,
			0, 0, this->previous_event_yaw-12, this->previous_event_pitch-12 ,
			m_imageRtt->get_width(),
			m_imageRtt->get_height(),
			Gdk::RGB_DITHER_NONE,
			-1, -1);
	
		gc_teleoperateRtt->set_foreground(color_red);
		teleopAreaRtt->get_window()->draw_line(gc_teleoperateRtt, 0, previous_event_pitch, 200, previous_event_pitch);
		teleopAreaRtt->get_window()->draw_line(gc_teleoperateRtt, previous_event_yaw, 0, previous_event_yaw, 200);


		gc_teleoperateRtt->set_foreground(color_white);
		teleopAreaRtt->get_window()->draw_line(gc_teleoperateRtt, 100, 0, 100, 200);
		teleopAreaRtt->get_window()->draw_line(gc_teleoperateRtt, 0, 100, 200, 100);
    	}

	void Gui::showPose3d()
	{
		std::string output;			// String for text information widgets
		std::stringstream sstream;	// Stream for conversion to text

		jderobot::Pose3DDataPtr p3d = this->sharer->getPose3D();

		sstream << p3d->x;
		sstream >> output;	sstream.flush();
		txtInfoX->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();

		sstream << p3d->y;
		sstream >> output;	sstream.flush();
		txtInfoY->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();

		sstream << p3d->z;
		sstream >> output;	sstream.flush();
		txtInfoZ->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();

		sstream << p3d->h;
		sstream >> output;	sstream.flush();
		txtInfoH->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();

		sstream << p3d->q0;
		sstream >> output;	sstream.flush();
		txtInfoQ0->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();

		sstream << p3d->q1;
		sstream >> output;	sstream.flush();
		txtInfoQ1->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();

		sstream << p3d->q2;
		sstream >> output;	sstream.flush();
		txtInfoQ2->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();

		sstream << p3d->q3;
		sstream >> output;	sstream.flush();
		txtInfoQ3->set_text(Glib::ustring(output));
		output.clear();
		sstream.clear();
	}

	void Gui::gtk_bttnTrlnRight_onButtonClick()
	{
		sharer->changePose3dTranslation(0., -1., 0.);
	}

	void Gui::gtk_bttnTrlnLeft_onButtonClick()
	{
		sharer->changePose3dTranslation(0., 1., 0.);
	}

	void Gui::gtk_bttnTrlnFront_onButtonClick()
	{
		sharer->changePose3dTranslation(1., 0., 0.);
	}

	void Gui::gtk_bttnTrlnBack_onButtonClick()
	{
		sharer->changePose3dTranslation(-1., 0., 0.);
	}

	void Gui::gtk_bttnTrlnUp_onButtonClick()
	{
		sharer->changePose3dTranslation(0., 0., 1.);
	}

	void Gui::gtk_bttnTrlnDown_onButtonClick()
	{
		sharer->changePose3dTranslation(0., 0., -1.);
	}

	void Gui::gtk_bttnRttnRight_onButtonClick()
	{
		sharer->changePose3dRotation(-1., 0., 0.);
	}

	void Gui::gtk_bttnRttnLeft_onButtonClick()
	{
		sharer->changePose3dRotation(1., 0., 0.);
	}

	void Gui::gtk_bttnRttnUp_onButtonClick()
	{
		sharer->changePose3dRotation(0., -1., 0.);
	}

	void Gui::gtk_bttnRttnDown_onButtonClick()
	{
		sharer->changePose3dRotation(0., 1., 0.);
	}

	void Gui::gtk_bttnRttnCW_onButtonClick()
	{
		sharer->changePose3dRotation(0., 0., 1.);
	}

	void Gui::gtk_bttnRttnACW_onButtonClick()
	{
		sharer->changePose3dRotation(0., 0., -1.);
	}

	void Gui::gtk_spnbTrlnStp_valueChanged()
	{
		sharer->setTranslationStep(spnbTrlnStp->get_value());
	}

	void Gui::gtk_spnbRttnStp_valueChanged()
	{
		sharer->setRotationStep(spnbRttnStp->get_value());
	}

	void Gui::gtk_restart_onButtonClick()
	{
		this->previous_event_x = 100;
		this->previous_event_y = 100;
		this->previous_event_yaw = 100;
		this->previous_event_pitch = 100;
		sharer->setSpeedX(0.0);
		sharer->setSpeedY(0.0);		
		sharer->restartPose3D();
	}

	bool Gui::on_key_press_event(GdkEventKey* event)
	{
		switch ( event->keyval )
		{
			case 'W':
			case 'w':
				gtk_bttnTrlnFront_onButtonClick();
				break;
			case 'S':
			case 's':
				gtk_bttnTrlnBack_onButtonClick();
				break;
			case 'D':
			case 'd':
				gtk_bttnTrlnRight_onButtonClick();
				break;
			case 'A':
			case 'a':
				gtk_bttnTrlnLeft_onButtonClick();
				break;
			case 'R':
			case 'r':
				gtk_bttnTrlnUp_onButtonClick();
				break;
			case 'F':
			case 'f':
				gtk_bttnTrlnDown_onButtonClick();
				break;
			case 'E':
			case 'e':
				gtk_bttnRttnRight_onButtonClick();
				break;
			case 'Q':
			case 'q':
				gtk_bttnRttnLeft_onButtonClick();
				break;
			case 'T':
			case 't':
				gtk_bttnRttnUp_onButtonClick();
				break;
			case 'G':
			case 'g':
				gtk_bttnRttnDown_onButtonClick();
				break;
			case 'C':
			case 'c':
				gtk_bttnRttnCW_onButtonClick();
				break;
			case 'Z':
			case 'z':
				gtk_bttnRttnACW_onButtonClick();
				break;
		}

		return false;
	}


} /* namespace navigatorCamera */

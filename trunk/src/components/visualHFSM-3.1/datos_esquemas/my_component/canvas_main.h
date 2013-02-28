#ifndef INTROROB_CANVASLASERWIN_H
#define INTROROB_CANVASLASERWIN_H

#include <string>
#include <iostream>
#include <gtkmm.h>
#include <libglademm.h>
#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <jderobot/camera.h>
#include <colorspaces/colorspacesmm.h>
//#include "controller.h"
//#include "drawarea.h"
//#include "navegacion.h"
#include <libgnomecanvasmm.h>
//#include "menudesplegable.h"


namespace mycomponent {

	//class Navega;
	//class MenuDesp;

	class CanvasWin : public Gnome::Canvas::CanvasAA//, public Gtk::Menu::Menu
		{
			public:
  				CanvasWin(BaseObjectType* cobject,/* BaseObjectType* cobject_menu,*/ const Glib::RefPtr<Gnome::Glade::Xml>& builder);
  				virtual ~CanvasWin();

				Gtk::Menu* m_pMenuPopup;

			protected:
  				bool on_canvas_event_state(GdkEvent * event, Gnome::Canvas::Item * item);
				bool on_canvas_event_transition(GdkEvent * event, Gnome::Canvas::Item * item);

				Gnome::Canvas::Group m_canvasgroup;
				Gnome::Canvas::Ellipse *m_ellipse;
  				//Gnome::Canvas::Rect *m_rect;
  				//  Gnome::Canvas::Image *m_image;
  				Gnome::Canvas::Text *m_text;
				Gnome::Canvas::Line *m_line;

				  void on_menu_file_popup_generic(Gnome::Canvas::Item * item);
				  void on_menu_file_popup_generic_2(Gnome::Canvas::Item * item);

				  //Child widgets:
				  //Gtk::VBox m_Box;
				  //Gtk::EventBox m_EventBox;
				  //Gtk::Label m_Label;

				  Glib::RefPtr<Gtk::UIManager> m_refUIManager;
				  Glib::RefPtr<Gtk::ActionGroup> m_refActionGroup;

				  

		};

	/*class CanvasLaserWin : public Gtk::Window
		{
			public:
  				CanvasLaserWin();
	
			protected:
  				//Member widgets:
  				CanvasLaser laser_canvas;
		};*/

} // namespace

#endif /*INTROROB_CANVASLASERWIN_H*/

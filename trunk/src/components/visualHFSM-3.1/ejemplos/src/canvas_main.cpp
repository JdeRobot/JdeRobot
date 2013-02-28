#include "canvas_main.h"
#include <gtkmm.h>

namespace mycomponent {

	

	CanvasWin::CanvasWin(BaseObjectType* cobject, /*BaseObjectType* cobject_menu,*/ const Glib::RefPtr<Gnome::Glade::Xml>& builder)
  		: Gnome::Canvas::CanvasAA(cobject), /*Gtk::Menu::Menu(cobject_menu),*/ m_canvasgroup(*(root()), 0, 0)/*, m_pMenuPopup*/
	{ 		
		// create some elements there
  		Gnome::Canvas::Points m_points;

  		m_points.push_back(Gnome::Art::Point(0, 0));
  		m_points.push_back(Gnome::Art::Point(100, 0));
  		m_points.push_back(Gnome::Art::Point(0, 100));
  		m_points.push_back(Gnome::Art::Point(100, 100));

  		// we want to use the stream like interface
  		using namespace Gnome::Canvas;

  		m_line = new Gnome::Canvas::Line(m_canvasgroup, m_points);
  		*m_line << Properties::fill_color("Green")
		  		<< Properties::width_pixels(5);

  		m_ellipse = new Gnome::Canvas::Ellipse(m_canvasgroup, 0, 0, 100, 100);
  		*m_ellipse << Properties::fill_color("red");

  		//m_rect = new Gnome::Canvas::Rect(m_canvasgroup, 10, 10, 50, 100);
  		//*m_rect << Properties::width_pixels(2)
  		//        << Properties::fill_color("white");

  		//m_image = new Gnome::Canvas::Image(m_canvasgroup, 0, 0, Gdk_Imlib::Image("example.png"));
  		//The width and height are set from the information in the image file.

  		m_text = new Gnome::Canvas::Text(m_canvasgroup, 0, 0, "Funciona!!");
  		*m_text << Properties::font("-Adobe-Helvetica-Medium-R-Normal--*-100-*-*-*-*-*-*")
          		<< Properties::fill_color("blue"); //Changes the color of the text.
		
		//Connecting events
		m_line->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_canvas_event_transition),m_line));
		m_ellipse->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_canvas_event_state),m_ellipse));
		m_text->signal_event().connect(sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_canvas_event_state),m_text));
		
				
		//Menu contextual
		/*m_pMenuPopup = new Gtk::Menu::Menu();
		
		m_refActionGroup = Gtk::ActionGroup::create();

		//File|New sub menu:
		  //These menu actions would normally already exist for a main menu, because a
		  //context menu should not normally contain menu items that are only available
		  //via a context menu.
		  m_refActionGroup->add(Gtk::Action::create("ContextMenu", "Context Menu"));

		  m_refActionGroup->add(Gtk::Action::create("ContextEdit", "Edit"),
				  sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic_2),m_ellipse));

		  m_refActionGroup->add(Gtk::Action::create("ContextProcess", "Process"),
				  sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic));

		  m_refActionGroup->add(Gtk::Action::create("ContextRemove", "Remove"),
				  sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic));


		  m_refUIManager = Gtk::UIManager::create();
		  m_refUIManager->insert_action_group(m_refActionGroup);


		  //Layout the actions in a menubar and toolbar:
		  Glib::ustring ui_info =
				"<ui>"
				"  <popup name='PopupMenu'>"
				"    <menuitem action='ContextEdit'/>"
				"    <menuitem action='ContextProcess'/>"
				"    <menuitem action='ContextRemove'/>"
				"  </popup>"
				"</ui>";

		  try
		  {
			m_refUIManager->add_ui_from_string(ui_info);
		  }
		  catch(const Glib::Error& ex)
		  {
			std::cerr << "building menus failed: " <<  ex.what();
		  }

		  //Get the menu:
		  m_pMenuPopup = dynamic_cast<Gtk::Menu*>(
				  m_refUIManager->get_widget("/PopupMenu")); 
		  if(!m_pMenuPopup)
			g_warning("menu not found");*/

	}

	CanvasWin::~CanvasWin()
	{
  		delete m_line;
  		delete m_ellipse;
  		//delete m_rect;
		//delete m_image;
  		delete m_text;

	}

	bool CanvasWin::on_canvas_event_state(GdkEvent * event, Gnome::Canvas::Item * item)
	{
		/* Static variables for drag-and-drop */
  		static gboolean dragging = FALSE;
  		static double previous_x, previous_y;

  		double event_x = event->button.x;
  		double event_y = event->button.y;

		switch (event->type)
  		{
  			case GDK_BUTTON_PRESS:
    			if (event->button.button == 1)
    			{
      				GdkCursor *cursor;

      				/* Store these coordinates for later... */
      				previous_x = event_x;
      				previous_y = event_y;

      				cursor = gdk_cursor_new(GDK_FLEUR);
      				item->grab(GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK, event->button.time);
      				gdk_cursor_destroy(cursor);
      				dragging = TRUE;
    			}
				if (event->button.button == 3)
    			{
      				
					/*********************************************/

					//Menu contextual
					m_pMenuPopup = new Gtk::Menu::Menu();
		
					m_refActionGroup = Gtk::ActionGroup::create();

					//File|New sub menu:
					  //These menu actions would normally already exist for a main menu, because a
					  //context menu should not normally contain menu items that are only available
					  //via a context menu.
					  m_refActionGroup->add(Gtk::Action::create("ContextMenu", "Context Menu"));

					  m_refActionGroup->add(Gtk::Action::create("ContextEdit", "Editar"),
							  sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic_2),item));

					  m_refActionGroup->add(Gtk::Action::create("ContextProcess", "Codigo"),
							  sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic_2),item));

					  m_refActionGroup->add(Gtk::Action::create("ContextRemove", "Eliminar"),
							  sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic_2),item));


					  m_refUIManager = Gtk::UIManager::create();
					  m_refUIManager->insert_action_group(m_refActionGroup);


					  //Layout the actions in a menubar and toolbar:
					  Glib::ustring ui_info =
							"<ui>"
							"  <popup name='PopupMenu'>"
							"    <menuitem action='ContextEdit'/>"
							"    <menuitem action='ContextProcess'/>"
							"    <menuitem action='ContextRemove'/>"
							"  </popup>"
							"</ui>";

					  try
					  {
						m_refUIManager->add_ui_from_string(ui_info);
					  }
					  catch(const Glib::Error& ex)
					  {
						std::cerr << "building menus failed: " <<  ex.what();
					  }

					  //Get the menu:
					  m_pMenuPopup = dynamic_cast<Gtk::Menu*>(
							  m_refUIManager->get_widget("/PopupMenu")); 
					  if(!m_pMenuPopup)
						g_warning("menu not found");

					/**********************************************/

					if(m_pMenuPopup)
      					m_pMenuPopup->popup(event->button.button, event->button.time);
				}
    			break;

  			case GDK_MOTION_NOTIFY:
    			if (dragging && (event->motion.state & GDK_BUTTON1_MASK))
    			{
      				item->move(event_x - previous_x,
        			event_y - previous_y);

      				/* Remember these for the next pass */
      				previous_x = event_x;
      				previous_y = event_y;
    			}
    			break;

  			case GDK_BUTTON_RELEASE:
    			item->ungrab(event->button.time);
    			dragging = FALSE;
    			break;

  			default:
    			break;
  		}

  		return FALSE;
	}

	bool CanvasWin::on_canvas_event_transition(GdkEvent * event, Gnome::Canvas::Item * item)
	{
		/* Static variables for drag-and-drop */
  		static gboolean dragging = FALSE;
  		static double previous_x, previous_y;

  		double event_x = event->button.x;
  		double event_y = event->button.y;

		switch (event->type)
  		{
  			case GDK_BUTTON_PRESS:
    			if (event->button.button == 1)
    			{
      				GdkCursor *cursor;

      				/* Store these coordinates for later... */
      				previous_x = event_x;
      				previous_y = event_y;

      				cursor = gdk_cursor_new(GDK_FLEUR);
      				item->grab(GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK, event->button.time);
      				gdk_cursor_destroy(cursor);
      				dragging = TRUE;
    			}
				if (event->button.button == 3)
    			{
      				
					/*********************************************/

					//Menu contextual
					m_pMenuPopup = new Gtk::Menu::Menu();
		
					m_refActionGroup = Gtk::ActionGroup::create();

					//File|New sub menu:
					  //These menu actions would normally already exist for a main menu, because a
					  //context menu should not normally contain menu items that are only available
					  //via a context menu.
					  m_refActionGroup->add(Gtk::Action::create("ContextMenu", "Context Menu"));

					  m_refActionGroup->add(Gtk::Action::create("ContextEdit", "Editar"),
							  sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic),item));

					  m_refActionGroup->add(Gtk::Action::create("ContextProcess", "Condicional"),
							  sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic),item));

					  m_refActionGroup->add(Gtk::Action::create("ContextRemove", "Eliminar"),
							  sigc::bind(sigc::mem_fun(*this, &CanvasWin::on_menu_file_popup_generic),item));


					  m_refUIManager = Gtk::UIManager::create();
					  m_refUIManager->insert_action_group(m_refActionGroup);


					  //Layout the actions in a menubar and toolbar:
					  Glib::ustring ui_info =
							"<ui>"
							"  <popup name='PopupMenu'>"
							"    <menuitem action='ContextEdit'/>"
							"    <menuitem action='ContextProcess'/>"
							"    <menuitem action='ContextRemove'/>"
							"  </popup>"
							"</ui>";

					  try
					  {
						m_refUIManager->add_ui_from_string(ui_info);
					  }
					  catch(const Glib::Error& ex)
					  {
						std::cerr << "building menus failed: " <<  ex.what();
					  }

					  //Get the menu:
					  m_pMenuPopup = dynamic_cast<Gtk::Menu*>(
							  m_refUIManager->get_widget("/PopupMenu")); 
					  if(!m_pMenuPopup)
						g_warning("menu not found");

					/**********************************************/

					if(m_pMenuPopup)
      					m_pMenuPopup->popup(event->button.button, event->button.time);
				}
    			break;

  			case GDK_MOTION_NOTIFY:
    			if (dragging && (event->motion.state & GDK_BUTTON1_MASK))
    			{
      				item->move(event_x - previous_x,
        			event_y - previous_y);

      				/* Remember these for the next pass */
      				previous_x = event_x;
      				previous_y = event_y;
    			}
    			break;

  			case GDK_BUTTON_RELEASE:
    			item->ungrab(event->button.time);
    			dragging = FALSE;
    			break;

  			default:
    			break;
  		}

  		return FALSE;
	}


	void CanvasWin::on_menu_file_popup_generic(Gnome::Canvas::Item * item)
	{
    	std::cout << "Opcion de transicion seleccionada." << std::endl;
	}

	void CanvasWin::on_menu_file_popup_generic_2(Gnome::Canvas::Item * item)
	{
    	std::cout << "Opcion de estado seleccionada." << std::endl;
	}

	/*CanvasLaserWin::CanvasLaserWin()
	{
  		set_title ("ventana canvas laser");
  		add(laser_canvas);

  		show_all();
	}
	/*-----FIN CANVAS LASER------*/

}//namespace

#ifndef CARCLASSIER_VIEW_H
#define CARCLASSIER_VIEW_H

#include <gtkmm.h>
#include <gtkmm/socket.h>
#include <gdkmm.h>
#include <libglademm.h>
#include <colorspacesmm.h>

#include "trafficmonitor_config.h"
#include "widget.h" 


namespace trafficmonitor
{
class ViewGtk;
typedef std::tr1::shared_ptr<ViewGtk> ViewGtkPtr;

class ViewGtk
{
public:
   ViewGtk(colorspaces::Image& current_frame) throw();
   virtual ~ViewGtk() throw() {}
   void iteration();
   void set_cfg(TrafficMonitorAlgorithmConfig* alg_cfg) {cfg = alg_cfg;};
   void read_cfg();
   void display( const colorspaces::Image& image);

private:
   void updateWidgets();
   void display_tracking_zone(Cairo::RefPtr<Cairo::Context> cr);
   void update_cfg();
   void apply_cfg(){cfg->writeProperties();};
   bool onMotionEvent(GdkEventMotion* const& event);
   bool onDrawingAreaMainExposeEvent(GdkEventExpose* event);
   bool onDrawingAreaButtonPressEvent(GdkEventButton* event);
   void findNearestPoint(int y, int x);

   
   void update_play_cfg()
      {
         cfg->play = play->get_active();
         cfg->writeProperties();
      };
   
   //main window

   //Glade and main window
   Gtk::Main gtkmain;
   Glib::RefPtr<Gnome::Glade::Xml> refXml;
   
   Widget<Gtk::Window> mainwindow;
   Widget<Gtk::Window> input_image_window;

   //Drawing areas
   Widget<Gtk::DrawingArea> drawingarea_input_image;

   // Model controls
   Widget<Gtk::ToggleButton> play;
   Widget<Gtk::CheckButton> use_static_road;
   Widget<Gtk::CheckButton> auto_calib;
   Widget<Gtk::CheckButton> classify;
   Widget<Gtk::CheckButton> klt_tracking;
   Widget<Gtk::CheckButton> switch_detection_zone;
   Widget<Gtk::ToggleButton> applyConfig;

   //Vertical/horizontal bars
   Widget<Gtk::Scrollbar> detection_zone_perc_bar;  
   
   //Check buttons
   Widget<Gtk::CheckButton> show_tracking_zone;
   Widget<Gtk::CheckButton> show_detection_zone;
   Widget<Gtk::CheckButton> show_tracking_info;
   Widget<Gtk::CheckButton> show_categories;
   Widget<Gtk::CheckButton> show_contours;
   Widget<Gtk::CheckButton> show_oclusion;
   Widget<Gtk::CheckButton> show_bounding_box;
   Widget<Gtk::CheckButton> show_klt_points;

   TrafficMonitorAlgorithmConfig* cfg;
   colorspaces::Image& m_currentFrame;
};
  
}//namespace

#endif /*CARCLASSIER_VIEW_H*/

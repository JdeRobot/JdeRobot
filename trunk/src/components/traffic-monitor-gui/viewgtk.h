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
   ViewGtk() throw();
   virtual ~ViewGtk() throw() {}
   void iteration();
   void set_cfg(TrafficMonitorAlgorithmConfig* alg_cfg) {cfg = alg_cfg;};
   void read_cfg();

private:
   void updateWidgets();
   void update_cfg();
   void apply_cfg(){cfg->writeProperties();};
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
};
  
}//namespace

#endif /*CARCLASSIER_VIEW_H*/

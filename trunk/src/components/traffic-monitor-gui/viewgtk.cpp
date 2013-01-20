#include <stdlib.h>
#include <colorspacesmm.h>
#include <libglademm.h>
#include <cairomm/context.h>
#include <pangomm/context.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <unistd.h>

#include "viewgtk.h"

using namespace std;

namespace trafficmonitor {

const std::string gladepath = std::string(".")+std::string("/trafficmonitor.glade"); 

ViewGtk::ViewGtk() throw ():
   gtkmain(0, 0), //FIXME: do we need gtk params?? maybe we can get them from config file
   refXml(Gnome::Glade::Xml::create(gladepath)), //FIXME: check for existence

   INIT_WIDGET(mainwindow, refXml),
   INIT_WIDGET(play, refXml),
   INIT_WIDGET(applyConfig, refXml),
   INIT_WIDGET(use_static_road, refXml),
   INIT_WIDGET(auto_calib, refXml),
   INIT_WIDGET(classify, refXml),
   INIT_WIDGET(klt_tracking, refXml),
   INIT_WIDGET(switch_detection_zone, refXml),
   INIT_WIDGET(detection_zone_perc_bar, refXml),  
   INIT_WIDGET(show_tracking_zone, refXml),
   INIT_WIDGET(show_detection_zone, refXml),
   INIT_WIDGET(show_tracking_info, refXml),
   INIT_WIDGET(show_categories, refXml),
   INIT_WIDGET(show_contours, refXml),
   INIT_WIDGET(show_oclusion, refXml),
   INIT_WIDGET(show_bounding_box, refXml),
   INIT_WIDGET(show_klt_points, refXml)
{
   
   /* initialize random seed: */
   srand ( time(NULL) );
   
   //Connect Buttons 
   play->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_play_cfg));
   classify->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));
   use_static_road->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));
   switch_detection_zone->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));
   klt_tracking->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));
   applyConfig->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::apply_cfg));

   //Connect Slide bars
   detection_zone_perc_bar->signal_value_changed().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));
   
   //Connect Toggle Buttons
   show_tracking_zone->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));        
   show_detection_zone->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));       
   show_tracking_info->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));        
   show_categories->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));           
   show_contours->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));             
   show_oclusion->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));             
   show_bounding_box->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));         
   show_klt_points->signal_toggled().connect(sigc::mem_fun(this, &ViewGtk::update_cfg));           
   
   //update widget values according with model
   mainwindow->show();
}

/**
 *
 */
void ViewGtk::update_cfg()
{
   // Init controls:
   cfg->useStaticRoad = use_static_road->get_active();
   cfg->cameraAutoCalibration = auto_calib->get_active();
   cfg->classify = classify->get_active();
   cfg->kltTrackingActive = klt_tracking->get_active();
   cfg->switchDetectionZone = switch_detection_zone->get_active();

   double percentage = detection_zone_perc_bar->get_value();
   std::stringstream ss;
   ss << percentage;
   cfg->detectionZonePercentage = percentage;
   
   printf("new ctl play=%s classify=%s track=%s static_road=%s switch_det_zone=%s \n",
          play->get_active()?"true":"false",
          classify->get_active()?"true":"false",
          use_static_road->get_active()?"true":"false",
          klt_tracking->get_active()?"true":"false",
          switch_detection_zone->get_active()?"true":"false");
}

/**
 *
 */
void ViewGtk::read_cfg()
{
   // Init controls:
   use_static_road->set_active(cfg->useStaticRoad);
   auto_calib->set_active(cfg->cameraAutoCalibration);
   classify->set_active(cfg->classify);
   klt_tracking->set_active(cfg->kltTrackingActive);
   switch_detection_zone->set_active(cfg->switchDetectionZone);
}

void ViewGtk::iteration()
{
   for(;;)
      while (gtkmain.events_pending())
      gtkmain.iteration();
}

} //namespace

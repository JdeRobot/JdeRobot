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
#include <unistd.h>

#include "viewgtk.h"

#define DISTANCE_2D(a,b) sqrt( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) )

using namespace std;

namespace trafficmonitor {

const std::string gladepath = std::string(".")+std::string("/trafficmonitor.glade"); 

ViewGtk::ViewGtk(colorspaces::Image& current_frame) throw ():
   gtkmain(0, 0), //FIXME: do we need gtk params?? maybe we can get them from config file
   refXml(Gnome::Glade::Xml::create(gladepath)), //FIXME: check for existence
   m_currentFrame(current_frame),   

   INIT_WIDGET(mainwindow, refXml),
   INIT_WIDGET(input_image_window, refXml),
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
   INIT_WIDGET(show_klt_points, refXml),
   INIT_WIDGET(drawingarea_input_image, refXml)
{
   
   /* initialize random seed: */
   srand ( time(NULL) );

   //Drawing areas
   drawingarea_input_image->signal_expose_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaMainExposeEvent));
   drawingarea_input_image->signal_button_press_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaButtonPressEvent));
   drawingarea_input_image->signal_button_release_event().connect(sigc::mem_fun(this, &ViewGtk::onDrawingAreaButtonPressEvent));
   drawingarea_input_image->signal_motion_notify_event().connect(sigc::mem_fun(this, &ViewGtk::onMotionEvent));
   
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

   drawingarea_input_image->set_size_request(320, 240);

   //update widget values according with model
   mainwindow->show();
   input_image_window->show();
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

/**
 *
 */
void ViewGtk::display_tracking_zone(Cairo::RefPtr<Cairo::Context> cr)
{
   int i;
   Gdk::Color color;
   const std::vector<Tpoint2D>& road_points = cfg->get_road_points();

   cr->save();
   cr->set_line_width(2.0);
   color = Gdk::Color("Red");
   cr->set_source_rgba(color.get_red_p(),
                       color.get_green_p(),
                       color.get_blue_p(),
                       1.0);

   if (road_points.size()>0)
   {
      cr->move_to(road_points[0].x, road_points[0].y);
      for (i=0; i<road_points.size(); i++)
      {
         cr->line_to(road_points[i].x, road_points[i].y);
      }
      cr->close_path();
   }
   
   cr->stroke();
   cr->restore();
      
}

/**
 *
 */
void ViewGtk::findNearestPoint(int y, int x)
{
   // Find the nearest road point
   Tpoint2D tmp;
   std::vector<Tpoint2D> road_points = cfg->get_road_points();
   float dist = INT_MAX;
   float tmp_dist=0;
   int pos=-1;

   tmp.y = y;
   tmp.x = x;
   
   /** Find the nearst point*/
   for(unsigned int i=0; i<road_points.size(); i++)
   {
      tmp_dist = DISTANCE_2D(road_points[i], tmp);
      if (tmp_dist<dist)
      {
         dist = tmp_dist;
         pos = i;
      }
   }

   /** If we find the nearst point then we move the current point to this location*/
   if (pos>=0)
   {
      road_points[pos] = tmp;

      cfg->set_road_points(road_points);
   }
}

/**
 *
 */
bool ViewGtk::onMotionEvent(GdkEventMotion* const& event)
{
   if (event->state & GDK_BUTTON1_MASK)
   {
      findNearestPoint(event->y,event->x);
   }

   return true;
}

/**
 *
 */
bool ViewGtk::onDrawingAreaMainExposeEvent(GdkEventExpose* event) {

   //FIXME:event has a window
   Glib::RefPtr<Gdk::Window> window = drawingarea_input_image->get_window();
   if (window)
   {
      Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
      display(m_currentFrame);
      display_tracking_zone(cr);
      // display_detection_zone(cr);
   }

   return true;
}

/**
 *
 */
void ViewGtk::iteration()
{
   drawingarea_input_image->queue_draw();
   mainwindow->resize(1,1);
   input_image_window->resize(1,1);

   while (gtkmain.events_pending())
   {
      gtkmain.iteration();
      // printf("processing events .. \n");
   }

}

/**
 *
 */
bool ViewGtk::onDrawingAreaButtonPressEvent(GdkEventButton* event)
{
   gint win_x, win_y;
   gdk_window_at_pointer(&win_x, &win_y);
   printf("\n\n == %d:%d -- \n",win_y, win_x);
   printf("Click on button %d (%.2f:%.2f)\n",event->button, event->y, event->x);
   return true;
}

/**
 *
 */
void ViewGtk::display( const colorspaces::Image& image)
{
   Glib::RefPtr<Gdk::Window> window = drawingarea_input_image->get_window();
      
   if (window)
   {
      /*convert to RGB*/
      colorspaces::ImageRGB8 img_rgb8(image);
      Glib::RefPtr<Gdk::Pixbuf> imgBuff = Gdk::Pixbuf::create_from_data((const guint8*) img_rgb8.data,
                                                                        Gdk::COLORSPACE_RGB,
                                                                        false,
                                                                        8,
                                                                        img_rgb8.width,
                                                                        img_rgb8.height,
                                                                        img_rgb8.step);

      /* Create an empty GC*/
      const Glib::RefPtr< const Gdk::GC > gc; 
      window->draw_pixbuf(gc,
                          imgBuff,
                          0, 0, /*starting point from imgBuff*/
                          0, 0, /*starting point into drawable*/
                          imgBuff->get_width(),
                          imgBuff->get_height(),
                          Gdk::RGB_DITHER_NONE,
                          0,0);
         
      drawingarea_input_image->set_size_request(img_rgb8.width, img_rgb8.height);
   }
}
    


} //namespace

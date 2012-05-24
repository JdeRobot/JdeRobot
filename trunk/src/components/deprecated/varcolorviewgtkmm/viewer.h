#ifndef MULTICAMERAVIEWGTKMM_VIEWER_H
#define MULTICAMERAVIEWGTKMM_VIEWER_H

#include <gtkmm.h>
#include <libglademm.h>

#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>

#include <string>

#include <colorspaces/colorspaces++.h>

class Viewer
{
public:
  Viewer();
  ~Viewer();
  
  bool isVisible();

  //! function that actually displays the image in a window
  void display( const colorspaces::Imagepp& image );

private:
  Glib::RefPtr<Gnome::Glade::Xml> refXml;
  Gtk::Image* gtkimage;
  Gtk::Window* mainwindow;
  Gtk::Label* fpslabel;
  Gtk::Main gtkmain;
  
  //! display the frame rate of the received images
  void displayFrameRate();
      
  //! time variables for calculating number of frames per second 
  IceUtil::Time currentFrameTime,oldFrameTime;
  double fps;
  int frameCount;
};

#endif

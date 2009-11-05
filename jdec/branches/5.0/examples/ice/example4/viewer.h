#ifndef MULTICAMERAVIEWGTKMM_VIEWER_H
#define MULTICAMERAVIEWGTKMM_VIEWER_H

#include <gtkmm.h>
#include <libglademm.h>

#include <IceUtil/Thread.h>
#include <IceUtil/Time.h>
#include <varcolor.h>

#include <string>

// class GtkMainThread: public IceUtil::Thread
// {
// public:
//   GtkMainThread();
//   virtual void run();
// private:
//   Gtk::Main gtkmain;
// };

class Viewer
{
public:
  Viewer();
  ~Viewer();
  
  bool isVisible();

  //! function that actually displays the image in a window
  void display( jde::ImageDataPtr& image );

private:
  Glib::RefPtr<Gnome::Glade::Xml> refXml;
  Gtk::Image* gtkimage;
  Gtk::Window* mainwindow;
  Gtk::Label* fpslabel;
  //GtkMainThread* gtkthread;
  //Glib::Dispatcher redraw;
  Gtk::Main gtkmain;
  
  //! display the frame rate of the received images
  void displayFrameRate();
      
  //! time variables for calculating number of frames per second 
  IceUtil::Time currentFrameTime,oldFrameTime;
  double fps;
  int frameCount;
  guint8 *img_grey;
};

#endif

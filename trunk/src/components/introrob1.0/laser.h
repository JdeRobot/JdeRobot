#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm/drawingarea.h>

class MyArea : public Gtk::DrawingArea
{
public:
  MyArea();
  virtual ~MyArea();
  
  void setLaserData(std::vector <float> distanceData);
    
  Cairo::RefPtr<Cairo::Context> cr;
  
  std::vector <float> data;
  
  bool ok;

protected:
  //Override default signal handler:
  virtual bool on_expose_event(GdkEventExpose* event);
};

#endif // GTKMM_EXAMPLE_MYAREA_H

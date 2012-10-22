#include "laser.h"
#include <cairomm/context.h>

MyArea::MyArea()
{

    ok = false;

}

MyArea::~MyArea()
{
}

void MyArea::setLaserData(std::vector <float> distanceData)
{
    data = distanceData;
    ok = true;
}


bool MyArea::on_expose_event(GdkEventExpose* event)
{
  // This is where we draw on the window
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window)
  {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    // clip to the area indicated by the expose event so that we only redraw
    // the portion of the window that needs to be redrawn
    cr->rectangle(event->area.x, event->area.y,
            event->area.width, event->area.height);
    cr->clip();

    // scale to unit square (0 to 1 with and height)
    cr->scale(width, height);

    cr->set_line_width(0.001);

    // show control points
    cr->set_source_rgba(1, 0.2, 0.2, 0.6);
    cr->stroke();
    
    for (int i = 180; i> 0; i--){
        cr->move_to(0.5, 1);
        if(ok){
            cr->line_to ( 0.5+cos(i*3.1416/180)*data[i]/8000, 1-sin(i*3.1416/180)*data[i]/8000 );
        }else{
            //cr->line_to (0.5+cos(i*2*3.1416/180), 1-sin(i*3.1416/180)*(rand()%5));
        }
    }
    cr->stroke();

  }
  return true;
}

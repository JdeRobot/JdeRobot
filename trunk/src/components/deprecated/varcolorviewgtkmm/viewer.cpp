#include "viewer.h" 
#include <colorspaces/colorspaces++.h>
#include <iostream>
#include <cmath>

/*FIXME: declare this with a config.h¿?*/
#define _STR(x) #x
#define STR(x) _STR(x) 

const std::string gladepath = std::string(STR(GLADE_DIR)) + std::string("/varcolorviewgtkmm.glade");

Viewer::Viewer() 
 : gtkmain(0,0),frameCount(0) {

  std::cout << "Loading glade\n";
  refXml = Gnome::Glade::Xml::create(gladepath);
  refXml->get_widget("image", gtkimage);
  refXml->get_widget("mainwindow",mainwindow);
  refXml->get_widget("fpslabel",fpslabel);
        
  // start the timer for calculating the number of frames per second
  // the images are being displayed at
  oldFrameTime = IceUtil::Time::now();
}
    

Viewer::~Viewer() {}

bool Viewer::isVisible(){
  return mainwindow->is_visible();
}

void Viewer::display( const colorspaces::Imagepp& image )
{
  colorspaces::ImageppPtr img_converted(image.toRGB8());
  
  if (img_converted.get() != 0){
    Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
      Gdk::Pixbuf::create_from_data((const guint8*)img_converted->imageData,
				    Gdk::COLORSPACE_RGB,
				    false,
				    8,
				    img_converted->description.width,
				    img_converted->description.height,
				    img_converted->description.width*3);
  
    gtkimage->clear();
    gtkimage->set(imgBuff);
    displayFrameRate();
  }
  mainwindow->resize(1,1);
  while (gtkmain.events_pending())
    gtkmain.iteration();
}
    
void
Viewer::displayFrameRate()
{
  double diff;
  IceUtil::Time diffT;

  currentFrameTime = IceUtil::Time::now();
  diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
  if (diff < 1000.0)
     frameCount++;
  else{
     oldFrameTime = currentFrameTime;
     fps = frameCount*1000.0/diff;
     frameCount=0;
     // Display the frame rate
     std::stringstream fpsString;
     fpsString << "fps = " << int(fps);
     fpslabel->set_label(fpsString.str());
  }
}


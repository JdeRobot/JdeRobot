#include "viewer.h" 
#include <iostream>
#include <cmath>

Viewer::Viewer() :
  //gtkthread(new GtkMainThread())
  gtkmain(0,0),frameCount(0),img_rgb(0)
{

  std::cout << "Loading glade\n";
  refXml = Gnome::Glade::Xml::create("varcolorviewgtkmm.glade");
  refXml->get_widget("image", gtkimage);
  refXml->get_widget("mainwindow",mainwindow);
  refXml->get_widget("fpslabel",fpslabel);
        
  // start the timer for calculating the number of frames per second
  // the images are being displayed at
  oldFrameTime = IceUtil::Time::now();
}
    

Viewer::~Viewer()
{
  free(img_rgb);
}

bool Viewer::isVisible(){
  return mainwindow->is_visible();
}

void Viewer::display( jde::ImageDataPtr& image )
{
  int img_size = image->description->width*image->description->height*4;/*RGB 32bit*/
  const guint8 *img_rgb32 = (const guint8*)(&image->pixelData[0]);
  int i,j;
  
  if (img_rgb==0)
    img_rgb = (guint8*)calloc(img_size,sizeof(guint8));
 
  for (i=0,j=0; i<img_size; i+=4,j+=3){
    img_rgb[j] = img_rgb32[i+1];
    img_rgb[j+1] = img_rgb32[i+2];
    img_rgb[j+2] = img_rgb32[i+3];
  }

  Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
    Gdk::Pixbuf::create_from_data(img_rgb,
				  Gdk::COLORSPACE_RGB,
				  false,
				  8,
				  image->description->width,
				  image->description->height,
				  image->description->width*3);
  
  gtkimage->clear();
  gtkimage->set(imgBuff);
  displayFrameRate();
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


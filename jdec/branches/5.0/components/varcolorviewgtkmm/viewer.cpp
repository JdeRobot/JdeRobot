#include "viewer.h" 
#include <formats.h>
#include <iostream>
#include <cmath>

/*FIXME: declare this with a config.h¿?*/
#define _STR(x) #x
#define STR(x) _STR(x) 

const std::string gladepath = std::string(STR(GLADE_DIR)) + std::string("/varcolorviewgtkmm.glade");

Viewer::Viewer() :
  //gtkthread(new GtkMainThread())
  gtkmain(0,0),frameCount(0),img_rgb(0)
{

  std::cout << "Loading glade\n";
  refXml = Gnome::Glade::Xml::create(gladepath);
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

void Viewer::display( jderobot::ImageDataPtr& image )
{
  int img_size = image->description->width*image->description->height*3;/*RGB*/
  const Format *fmt = searchPixelFormat(image->description->format.c_str());
  if (img_rgb==0)
    img_rgb = (guint8*)calloc(img_size,sizeof(guint8));
  
  if (image->description->format.find("RGB") == 0 && fmt->size == 4){/*RGB 32-bits*/
    const int *img_rgb32 = (const int*)(&image->pixelData[0]);
    int i;
    for (i=0; i<img_size; i++){
      img_rgb[i*3] = (guint8)((fmt->componetsMask[INDEX_RED] & img_rgb32[i])>>16);/*FIXME*/
      img_rgb[i*3+1] = (guint8)((fmt->componetsMask[INDEX_GREEN] & img_rgb32[i])>>8);/*FIXME*/
      img_rgb[i*3+2] = (guint8)((fmt->componetsMask[INDEX_BLUE] & img_rgb32[i]));/*FIXME*/
    }
  }else if (image->description->format.find("RGB") == 0 && fmt->size == 3)/*RGB 24-bits*/
    memmove(img_rgb,(const guint8*)(&image->pixelData[0]),img_size);
  else if (image->description->format.find("YUV") == 0){
    const guint8 *img_yuv2 = (const guint8*)(&image->pixelData[0]);
    int i;
    /*yuv->grey scale*/
    for (i=0; i<img_size; i+=3){
      guint8 p = img_yuv2[(i/3)*2];
      img_rgb[i] = img_rgb[i+1] = img_rgb[i+2] = p;
    }
  }/*else-> FIXME:format not allowed*/

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


#include "view.h"
#include "model.h"

#include <formats.h>

#include <gtkmm.h>
#include <libglademm.h>
#include <string>
#include <iostream>
#include <cmath>

namespace opencvdemo {
  const std::string gladepath = std::string(GLADE_DIR) + 
    std::string("/varcolorviewgtkmm.glade");

  class privImpl {
  public:
    privImpl()
      :gtkmain(0,0),displayFps(0.0),displayCount(0),img_rgb(0) {
      
      std::cout << "Loading glade\n";
      refXml = Gnome::Glade::Xml::create(gladepath);
      refXml->get_widget("image", gtkimage);
      refXml->get_widget("mainwindow",mainwindow);
      refXml->get_widget("fpslabel",fpslabel);
    }

    ~privImpl(){
      if (img_rgb != 0)
	free(img_rgb);
    }

    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Image* gtkimage;
    Gtk::Window* mainwindow;
    Gtk::Label* fpslabel;
    Gtk::Main gtkmain;
    
    //! time variables for calculating number of frames per second 
    IceUtil::Time currentFrameTime,oldFrameTime;
    double displayFps;
    int updateCount;

    guint8 *img_rgb;
  };

  

  View::View() :
    gtkmain(0,0),displayFps(0.0),displayCount(0),img_rgb(0)
  {
    
  }
  
  
  View::~View()
  {
  }

  void View::update(jderobotutil::Subject *o, jderobotutil::ObserverArg *arg){
    Model *model = dynamic_cast<Model*>(o);/*downcast*/

    if (model==0)
      throw jderobot::JdeRobotException("Can't get model");
    jderobot::ImageDataPtr image = model->getImage();
    const Format *fmt = searchPixelFormat(image->description->format.c_str());
    if (fmt==0)
      throw jderobot::JdeRobotException("Unknown format");
    if (img_rgb==0){
      int img_size = image->description->width*image->description->height*3;
      img_rgb = (guint8*)calloc(img_size,sizeof(guint8));
    }
    
    if (fmt->components == RGBA){/*RGB 32-bits*/
      const guint8 *img_rgb32 = (const guint8*)(&image->pixelData[0]);
      int npixels = image->description->width*image->description->height;
      int i;
      for (i=0; i<npixels; i++){
	int j = i*3;/*byte index for rgb24*/
	int k = i*4;/*byte index for rgb32*/
	img_rgb[j] = img_rgb32[k+1];/*R*/
	img_rgb[j+1] = img_rgb32[k+2];/*G*/
	img_rgb[j+2] = img_rgb32[k+3];/*B*/
      }
    }else if (fmt->components == RGB)/*RGB 24-bits*/
      memmove(img_rgb,(const guint8*)(&image->pixelData[0]),image->description->size);
    else if (fmt->components == YUV){
      const guint8 *img_yuv2 = (const guint8*)(&image->pixelData[0]);
      int npixels = image->description->width*image->description->height;
      int i;
      /*yuv->grey scale*/
      for (i=0; i<npixels; i++){
	int j = i*3;/*byte index for rgb24*/
	int k = i*2;/*byte index for YUV2*/
	img_rgb[j] = img_rgb[j+1] = img_rgb[j+2] = img_yuv2[k];/*Y*/
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
    
    Glib::RefPtr<Gdk::Pixmap> imgPixmap;
    Glib::RefPtr<Gdk::Bitmap> imgBitmap;
    Glib::RefPtr<Gdk::GC> gc;

    imgBuff->render_pixmap_and_mask(imgPixmap,imgBitmap,0);

    gc = Gdk::GC::create(imgPixmap);
    gc->set_foreground(Gdk::Color("red"));

    OpencvdemoModel::OptFlowSeq fs = model->getOptFlow();
    OpencvdemoModel::OptFlowSeq::iterator fs_it;
    for (fs_it=fs.begin();fs_it!=fs.end();fs_it++){
      int x,y,radius;

      radius = (int)fs_it->flow;
      x = fs_it->p.x-radius;
      y = fs_it->p.y+radius;
      imgPixmap->draw_arc(gc,true,x,y,radius*2,radius*2,0,360*64);
    }

    gtkimage->clear();
    gtkimage->set(imgPixmap,imgBitmap);
    displayFrameRate();
    mainwindow->resize(1,1);
    while (gtkmain.events_pending())
      gtkmain.iteration();
  }
  
  void
  View::displayFrameRate()
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
  
}//namespace

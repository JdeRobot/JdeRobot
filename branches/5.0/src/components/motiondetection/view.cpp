#include "view.h"
#include "model.h"
#include <colorspaces/colorspaces++.h>
#include <gtkmm.h>
#include <libglademm.h>
#include <cairomm/context.h>
#include <string>
#include <iostream>
#include <cmath>
#include <gbxsickacfr/gbxiceutilacfr/timer.h>

/*FIXME: check this http://svn.drobilla.net/lad/trunk/eugene/gui/Widget.hpp*/

namespace motiondetection {
  const std::string gladepath = std::string(GLADE_DIR) + 
    std::string("/motiondetection.glade");

  class View::PImpl {
  public:
    PImpl(Controller& controller)
      :gtkmain(0,0),refXml(0),mainwindow(0),
       drawingArea(0), 
       hscaleMotionThreshold(0),hscaleSecsBtwAlarm(0),
       hscaleOpticalFlowNPoints(0),hscalePixelDifferenceThreshold(0),
       hscalePixelDifferenceXStep(0),hscalePixelDifferenceYStep(0),
       comboboxAlgorithm(0),
       fpsLabel(0),
       controller(controller),
       frameCount(0), timer(){
      
      std::cout << "Loading glade\n";
      refXml = Gnome::Glade::Xml::create(gladepath);
      refXml->get_widget("mainwindow",mainwindow);
      refXml->get_widget("drawingarea", drawingArea);
      refXml->get_widget("hscaleMotionThreshold", hscaleMotionThreshold);
      hscaleMotionThreshold->set_value((double)controller.getModel()->getMotionThreshold());
      hscaleMotionThreshold->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onMotionThresholdChanged));
      refXml->get_widget("hscaleSecsBtwAlarm", hscaleSecsBtwAlarm);
      hscaleSecsBtwAlarm->set_value((double)controller.getSecsBtwAlarm());
      hscaleSecsBtwAlarm->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onSecsBtwAlarmChanged));
      refXml->get_widget("comboboxAlgorithm", comboboxAlgorithm);
      comboboxAlgorithm->set_active(controller.getModel()->getMotionDetectionAlgorithm());
      comboboxAlgorithm->signal_changed().connect(sigc::mem_fun(this,&PImpl::onMotionDetectionAlgorithmChanged));
      refXml->get_widget("hscaleOpticalFlowNPoints", hscaleOpticalFlowNPoints);
      hscaleOpticalFlowNPoints->set_value((double)controller.getModel()->getOpticalFlowNPoints());
      hscaleOpticalFlowNPoints->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onOpticalFlowNPointsChanged));
      refXml->get_widget("hscalePixelDifferenceThreshold", hscalePixelDifferenceThreshold);
      hscalePixelDifferenceThreshold->set_value(controller.getModel()->getPixelDifferenceThreshold());
      hscalePixelDifferenceThreshold->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onPixelDifferenceThresholdChanged));
      refXml->get_widget("hscalePixelDifferenceXStep", hscalePixelDifferenceXStep);
      hscalePixelDifferenceXStep->set_value(controller.getModel()->getPixelDifferenceXStep());
      hscalePixelDifferenceXStep->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onPixelDifferenceXStepChanged));
      refXml->get_widget("hscalePixelDifferenceYStep", hscalePixelDifferenceYStep);
      hscalePixelDifferenceYStep->set_value(controller.getModel()->getPixelDifferenceYStep());
      hscalePixelDifferenceYStep->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onPixelDifferenceYStepChanged));
      refXml->get_widget("fpsLabel",fpsLabel);
    }

    ~PImpl(){}
    
    void drawImage(const colorspaces::ImageppPtr image){
      frameCount++;
      /*convert to RGB*/
      imageRGB888 = image->toRGB888();
      if (imageRGB888.get() == 0)
	throw gbxutilacfr::Exception(ERROR_INFO, "Can't convert image to RGB888");
    
    
      Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
	Gdk::Pixbuf::create_from_data((const guint8*)imageRGB888->imageData,
				      Gdk::COLORSPACE_RGB,
				      false,
				      8,
				      imageRGB888->description.width,
				      imageRGB888->description.height,
				      imageRGB888->description.width*3);
      
      Glib::RefPtr<Gdk::Window> window = drawingArea->get_window();
      const Glib::RefPtr< const Gdk::GC > gc;/*empty*/
      
      window->draw_pixbuf(gc,
			  imgBuff,
			  0,0,/*starting point from imgBuff*/
			  0,0,/*starting point into drawable*/
			  imgBuff->get_width(),
			  imgBuff->get_height(),
			  Gdk::RGB_DITHER_NONE, 0, 0);
      drawingArea->set_size_request(imageRGB888->description.width,
				    imageRGB888->description.height);
      mainwindow->resize(1,1);
    }
  
    void drawMotionGrid(const Model::MotionGrid2DPtr mGrid){
      Glib::RefPtr<Gdk::Window> window = drawingArea->get_window();
      if(window){
	int width;
	int height;
	
	drawingArea->get_size_request(width,height);
	
	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
      
	// Store context
	//cr->save();
      
	// Draw the source image on the widget context
	//cr->set_source(pImpl->image_surface, 0.0, 0.0);
	//cr->rectangle(0.0, 0.0, imgBuff->get_width(), imgBuff->get_height());
	//cr->clip();
	//cr->paint();
      
      
	//cr->set_line_width(2.0);
      
	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
	// cr->rectangle(event->area.x, event->area.y,
	// 		    event->area.width, event->area.height);
	//cr->clip();
      
      
	cr->scale(width/mGrid->cols,
		  height/mGrid->rows);/*scale to grid dimensions*/
	int i,j,k;
	for (i=0; i<mGrid->rows; i++){
	  for (j=0; j<mGrid->cols; j++){
	    k = i+(j*mGrid->cols);/*grid offset*/
	    if (mGrid->grid[k].motion > 0){
	      cr->set_source_rgba(1.0, 0.0, 0.0,0.5);
	      cr->rectangle(i, j, 1.0, 1.0);
	      cr->fill();
	    }
	  }
	}
	// Restore context
	//cr->restore();
      }
    }

    void displayFPS(){
      double fps,elapsed = timer.elapsedMs();
      if (elapsed > 1000.0){
	fps = frameCount*1000.0/elapsed;
	frameCount=0;
	timer.restart();
	// Display the frame rate
	std::stringstream fpsString;
	fpsString << "fps = " << int(fps);
	fpsLabel->set_label(fpsString.str());
      }
    }
  
    void onMotionThresholdChanged(){
      controller.setMotionThreshold((int)hscaleMotionThreshold->get_value());
    }
  
    void onOpticalFlowNPointsChanged(){
      controller.setOpticalFlowNPoints((int)hscaleOpticalFlowNPoints->get_value());
    }
  
    void onSecsBtwAlarmChanged(){
      controller.setSecsBtwAlarm((int)hscaleSecsBtwAlarm->get_value());
    }

    void onMotionDetectionAlgorithmChanged(){
      controller.setMotionDetectionAlgorithm((Model::MotionDetectionAlgorithm)comboboxAlgorithm->get_active_row_number());
    }

    void onPixelDifferenceThresholdChanged(){
      controller.setPixelDifferenceThreshold((int)hscalePixelDifferenceThreshold->get_value());
    }

    void onPixelDifferenceXStepChanged(){
      controller.setPixelDifferenceXStep((int)hscalePixelDifferenceXStep->get_value());
    }
    
    void onPixelDifferenceYStepChanged(){
      controller.setPixelDifferenceYStep((int)hscalePixelDifferenceYStep->get_value());
    }

    Gtk::Main gtkmain;
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Window *mainwindow;
    Gtk::DrawingArea* drawingArea;
    Gtk::HScale *hscaleMotionThreshold,*hscaleSecsBtwAlarm,
      *hscaleOpticalFlowNPoints,*hscalePixelDifferenceThreshold,
      *hscalePixelDifferenceXStep,*hscalePixelDifferenceYStep;
    Gtk::ComboBox *comboboxAlgorithm;
    Gtk::Label* fpsLabel;
    colorspaces::ImageppPtr imageRGB888;
    Controller& controller;

    int frameCount;
    gbxiceutilacfr::Timer timer;
  };

  

  View::View(Controller& controller) throw()
    : pImpl(new PImpl(controller)){
  }
  
  
  View::~View() throw() {}

  void View::update(const jderobotutil::SubjectPtr o, jderobotutil::ObserverArgPtr arg)
    throw(gbxutilacfr::Exception) {
    const Model* model = dynamic_cast<const Model*>(o.get());/*downcast*/
    if (model==0)
      throw gbxutilacfr::Exception(ERROR_INFO, "Can't get model");

    pImpl->drawImage(model->getImage()->toRGB888());
    const Model::MotionGrid2DPtr mGrid = model->getMotionGrid();
    if (mGrid)
      pImpl->drawMotionGrid(mGrid);
    pImpl->displayFPS();
    
    while (pImpl->gtkmain.events_pending())
      pImpl->gtkmain.iteration();
  }

  bool View::isVisible(){
    return pImpl->mainwindow->is_visible();
  }
  
// //   void
// //   View::displayFrameRate()
// //   {
//     // double diff;
// //     IceUtil::Time diffT;
    
// //     currentFrameTime = IceUtil::Time::now();
// //     diff = (currentFrameTime - oldFrameTime).toMilliSecondsDouble();
// //     if (diff < 1000.0)
// //       frameCount++;
// //     else{
// //       oldFrameTime = currentFrameTime;
// //       
// //     }
  //}
  
}//namespace

#include "view.h"
#include "model.h"
#include <colorspaces/colorspacesmm.h>
#include <gtkmm.h>
#include <libglademm.h>
#include <cairomm/context.h>
#include <string>
#include <iostream>
#include <cmath>
#include <jderobotutil/time.h>

/*FIXME: check this http://svn.drobilla.net/lad/trunk/eugene/gui/Widget.hpp*/

namespace motiondetection {
  const std::string gladepath = std::string(GLADE_DIR) + 
    std::string("/motiondetection.glade");

  class View::PImpl {
  public:
    PImpl(Controller& controller)
      :gtkmain(0,0),refXml(0),mainwindow(0),
       drawingAreaMain(0),drawingAreaDebug1(0), drawingAreaDebug2(0),
       hscaleMotionThreshold(0),hscaleSecsBtwAlarm(0),
       hscaleOpticalFlowNPoints(0),hscalePixelDifferenceThreshold(0),
       hscalePixelDifferenceXStep(0),hscalePixelDifferenceYStep(0),
       comboboxAlgorithm(0),
       checkbuttonBackground(0),checkbuttonDifference(0),
       controller(controller),
       selectedAlgorithm(0),drawBackground(true),drawDifference(true),
       fpsCounter(){
      
      //getting widgets
      refXml = Gnome::Glade::Xml::create(gladepath);
      refXml->get_widget("mainwindow",mainwindow);
      refXml->get_widget("drawingareaMain", drawingAreaMain);
      refXml->get_widget("drawingareaDebug1", drawingAreaDebug1);
      refXml->get_widget("drawingareaDebug2", drawingAreaDebug2);
      refXml->get_widget("hscaleMotionThreshold", hscaleMotionThreshold);
      refXml->get_widget("hscaleSecsBtwAlarm", hscaleSecsBtwAlarm);      
      refXml->get_widget("comboboxAlgorithm", comboboxAlgorithm);      
      refXml->get_widget("hscaleOpticalFlowNPoints", hscaleOpticalFlowNPoints);      
      refXml->get_widget("hscaleOpticalFlowThreshold", hscaleOpticalFlowThreshold);      
      refXml->get_widget("hscalePixelDifferenceThreshold", hscalePixelDifferenceThreshold);      
      refXml->get_widget("hscalePixelDifferenceXStep", hscalePixelDifferenceXStep);      
      refXml->get_widget("hscalePixelDifferenceYStep", hscalePixelDifferenceYStep);      
      refXml->get_widget("fpsLabel",fpsLabel);
      refXml->get_widget("checkbuttonBackground",checkbuttonBackground);
      refXml->get_widget("checkbuttonDifference",checkbuttonDifference);

      //update widget values according with model
      updateWidgets();

      //connect signals
      hscaleMotionThreshold->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onMotionThresholdChanged));
      hscaleSecsBtwAlarm->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onSecsBtwAlarmChanged));
      comboboxAlgorithm->signal_changed().connect(sigc::mem_fun(this,&PImpl::onMotionDetectionAlgorithmChanged));
      hscaleOpticalFlowNPoints->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onMotionDetectionAlgorithmChanged));
      hscaleOpticalFlowThreshold->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onMotionDetectionAlgorithmChanged));
      hscalePixelDifferenceThreshold->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onMotionDetectionAlgorithmChanged));
      hscalePixelDifferenceXStep->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onMotionDetectionAlgorithmChanged));
      hscalePixelDifferenceYStep->signal_value_changed().connect(sigc::mem_fun(this,&PImpl::onMotionDetectionAlgorithmChanged));
    }

    ~PImpl(){}
    
    void drawImage(const colorspaces::Image& image, Gtk::DrawingArea* drawingArea){
      /*convert to RGB*/
      colorspaces::ImageRGB8 img_rgb8(image);
      Glib::RefPtr<Gdk::Pixbuf> imgBuff = 
	Gdk::Pixbuf::create_from_data((const guint8*)img_rgb8.data,
				      Gdk::COLORSPACE_RGB,
				      false,
				      8,
				      img_rgb8.width,
				      img_rgb8.height,
				      img_rgb8.step);
      
      Glib::RefPtr<Gdk::Window> window = drawingArea->get_window();
      const Glib::RefPtr< const Gdk::GC > gc;/*empty*/
      window->draw_pixbuf(gc,
			  imgBuff,
			  0,0,/*starting point from imgBuff*/
			  0,0,/*starting point into drawable*/
			  imgBuff->get_width(),
			  imgBuff->get_height(),
			  Gdk::RGB_DITHER_NONE, 0, 0);
      drawingArea->set_size_request(img_rgb8.width,
				    img_rgb8.height);
    }
    
    void drawImages(const Model* model){
      fpsCounter.inc();
      //draw main image
      drawImage(model->getMotionDetectionAlgorithm()->state().currentImage,drawingAreaMain);

      //if debug images selected draw them
      if (selectedAlgorithm == 1){//opt flow
	//nothing yet
	return;
      }else if (selectedAlgorithm == 2){//pixel diff
	const PixelDifferenceAlgorithm* alg = dynamic_cast<const PixelDifferenceAlgorithm*>(model->getMotionDetectionAlgorithm().get());
	assert(alg!=0);
	if (drawBackground)
	  drawImage(alg->pixelDifferenceState().background,drawingAreaDebug1);
	if (drawDifference)
	  drawImage(alg->pixelDifferenceState().difference,drawingAreaDebug2);
      }
      mainwindow->resize(1,1);
    }

    void drawMotionItem(const MotionItem2D& item){
      Glib::RefPtr<Gdk::Window> window = drawingAreaMain->get_window();
      if(window){
	int width;
	int height;
	
	drawingAreaMain->get_size_request(width,height);
	
	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
      
	if (item.motion > 0){
	  cr->set_source_rgba(1.0, 0.0, 0.0,1.0);
	  cr->rectangle(item.area.x, item.area.y, item.area.width, item.area.height);
	  cr->stroke();
	}
      }
    }

    void drawMotion(const MotionItem2DSeq& motionSeq){
      Glib::RefPtr<Gdk::Window> window = drawingAreaMain->get_window();
      if(window){
	int width;
	int height;
	
	drawingAreaMain->get_size_request(width,height);
	
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
      
      
	MotionItem2DSeq::const_iterator m_it = motionSeq.begin();
	for (;m_it!=motionSeq.end(); m_it++){
	  if (m_it->motion > 0){
	    cr->set_source_rgba(1.0, 0.0, 0.0,1.0);
	    cr->rectangle(m_it->area.x, m_it->area.y, m_it->area.width, m_it->area.height);
	    cr->stroke();
	  }
	}
	// Restore context
	//cr->restore();
      }
    }

    void displayFPS(){
      double fps = fpsCounter.ips();
      std::stringstream fpsString;
      fpsString << "fps = " << int(fps);
      fpsLabel->set_label(fpsString.str());
    }

    void updateWidgets(){
      const ModelPtr m = controller.getModel();
      hscaleMotionThreshold->set_value((double)m->getMotionThreshold());
      hscaleSecsBtwAlarm->set_value((double)controller.getSecsBtwAlarm());
      const NullAlgorithm *nAlg;
      const OpticalFlowAlgorithm *ofAlg;
      const PixelDifferenceAlgorithm *pdAlg;
      
      if ((nAlg = dynamic_cast<const NullAlgorithm*>(controller.getModel()->getMotionDetectionAlgorithm().get()))){
	comboboxAlgorithm->set_active(0);
      }else if ((ofAlg = dynamic_cast<const OpticalFlowAlgorithm*>(controller.getModel()->getMotionDetectionAlgorithm().get()))){
	hscaleOpticalFlowNPoints->set_value(ofAlg->nPoints);
	hscaleOpticalFlowThreshold->set_value(ofAlg->opticalFlowThreshold);
	comboboxAlgorithm->set_active(1);
      }else if ((pdAlg = dynamic_cast<const PixelDifferenceAlgorithm*>(controller.getModel()->getMotionDetectionAlgorithm().get()))){
	hscalePixelDifferenceThreshold->set_value(pdAlg->pixelDiffThreshold);
	hscalePixelDifferenceXStep->set_value(pdAlg->winSize.width);
	hscalePixelDifferenceYStep->set_value(pdAlg->winSize.height);
	comboboxAlgorithm->set_active(2);
      }
    }

    // void updateAlgorithmWidgets(const OpticalFlowAlgorithmPtr alg){

//     }

//     void updateAlgorithmWidgets(const PixelDifferenceAlgorithmPtr alg){
//     }
  
    void onMotionThresholdChanged(){
      controller.setMotionThreshold((int)hscaleMotionThreshold->get_value());
    }
  
    void onSecsBtwAlarmChanged(){
      controller.setSecsBtwAlarm((int)hscaleSecsBtwAlarm->get_value());
    }

    void onMotionDetectionAlgorithmChanged(){
      colorspaces::Image initialImg = controller.getModel()->getMotionDetectionAlgorithm()->state().currentImage;

      selectedAlgorithm = comboboxAlgorithm->get_active_row_number();
      if (selectedAlgorithm == 0){//null
	controller.tracer().info("Instantiating Null");
	drawingAreaDebug1->hide();
	drawingAreaDebug2->hide();
	controller.setMotionDetectionAlgorithm(MotionDetectionAlgorithmPtr(new NullAlgorithm(controller.tracer(),initialImg)));
      }else if (selectedAlgorithm == 1){//optical flow
	int nPoints = hscaleOpticalFlowNPoints->get_value();
	int opticalFlowThreshold = hscaleOpticalFlowThreshold->get_value();
	std::stringstream ss;
	ss << "Instantiating Optical Flow with params: nPoints=" << nPoints << ",opticalFlowThreshold=" << opticalFlowThreshold << "\n";
	controller.tracer().info(ss.str());
	drawingAreaDebug1->hide();
	drawingAreaDebug2->hide();
	controller.setMotionDetectionAlgorithm(MotionDetectionAlgorithmPtr(new OpticalFlowAlgorithm(controller.tracer(),initialImg,nPoints,opticalFlowThreshold)));
      }else if (selectedAlgorithm == 2){//pixel diff
	int pixelDiffThreshold = hscalePixelDifferenceThreshold->get_value();
	cv::Size winSize(hscalePixelDifferenceXStep->get_value(),hscalePixelDifferenceYStep->get_value());
	std::stringstream ss;
	ss << "Instantiating Pixel Diff with params: pixelDiffThreshold=" << pixelDiffThreshold
	   << ",winsize=" << winSize.width << "x" << winSize.height << "\n";
	controller.tracer().info(ss.str());
	drawingAreaDebug1->show();
	drawingAreaDebug2->show();
	controller.setMotionDetectionAlgorithm(MotionDetectionAlgorithmPtr(new PixelDifferenceAlgorithm(controller.tracer(),initialImg,pixelDiffThreshold,winSize)));
      }
    }

    Gtk::Main gtkmain;
    Glib::RefPtr<Gnome::Glade::Xml> refXml;
    Gtk::Window *mainwindow;
    Gtk::DrawingArea *drawingAreaMain,*drawingAreaDebug1,*drawingAreaDebug2;
    Gtk::HScale *hscaleMotionThreshold,*hscaleSecsBtwAlarm,
      *hscaleOpticalFlowNPoints,*hscaleOpticalFlowThreshold,
      *hscalePixelDifferenceThreshold,
      *hscalePixelDifferenceXStep,*hscalePixelDifferenceYStep;
    Gtk::ComboBox *comboboxAlgorithm;
    Gtk::Label* fpsLabel;
    Gtk::CheckButton *checkbuttonBackground, *checkbuttonDifference;
    Controller& controller;
    int selectedAlgorithm;
    bool drawBackground,drawDifference;
    jderobotutil::IpsCounter fpsCounter;
  };

  

  View::View(Controller& controller) throw()
    : pImpl(new PImpl(controller)){
  }
  
  
  View::~View() throw() {}

  void View::update(const jderobotutil::Subject* o, jderobotutil::ObserverArg* arg)
    throw(gbxutilacfr::Exception) {
    const Model* model = dynamic_cast<const Model*>(o);/*downcast*/
    if (model==0)
      throw gbxutilacfr::Exception(ERROR_INFO, "Can't get model");

    pImpl->drawImages(model);
    MotionItem2D m;
    if (model->isMotionDetected(m))
      //pImpl->drawMotion(model->getMotionDetected());
      pImpl->drawMotionItem(m);
      
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

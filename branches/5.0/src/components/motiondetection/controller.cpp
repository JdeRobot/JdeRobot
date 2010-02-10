#include "controller.h"
#include "view.h"
#include <gbxsickacfr/gbxiceutilacfr/timer.h>

namespace motiondetection {
  class Controller::PImpl{
  public:
    PImpl()
      : running(true),secsBtwAlarms(10),model(),view(),timer(IceUtil::Time::seconds(secsBtwAlarms)) {}
    bool running;
    int secsBtwAlarms;
    ModelPtr model;
    ViewPtr view;
    gbxiceutilacfr::Timer timer;
  };

  Controller::Controller(ModelPtr m)
    : pImpl(new PImpl()) {
    pImpl->model = m;
    pImpl->view.reset(new View(*this));
    pImpl->model->addObserver(pImpl->view);
  }

  Controller::~Controller() {
    pImpl->model->deleteObserver(pImpl->view);//jderobotutil::ObserverPtr(view));
    pImpl->view.reset();
  }

  
  bool Controller::isRunning() const throw(){
    return pImpl->view->isVisible();
  }

  void Controller::exit() throw(){
    pImpl->running = false;
  }

  void Controller::setImage(const colorspaces::ImageppPtr img) throw(gbxutilacfr::Exception){
    pImpl->model->setImage(img);
  }

  const colorspaces::ImageppPtr Controller::getImage() const throw(){
    return pImpl->model->getImage();
  }

  void Controller::setMotionGridSize(const int rows, const int cols) throw(){
    pImpl->model->setMotionGridSize(rows,cols);
  }
  
  const Model::MotionGrid2DPtr Controller::getMotionGrid(const int rows, const int cols) const throw(){
    return pImpl->model->getMotionGrid();
  }

  bool Controller::isMotionDetected(CvRect *area, Model::MotionGridItem2D *value) const throw(){
    if (pImpl->model->isMotionDetected(area,value) &&
	(pImpl->timer.elapsedSec() > pImpl->secsBtwAlarms)){
      pImpl->timer.restart();
      return true;
    }
    return false;
  }

  void Controller::setMotionDetectionAlgorithm(const Model::MotionDetectionAlgorithm a) throw(){
    pImpl->model->setMotionDetectionAlgorithm(a);
  }

  void Controller::setMotionThreshold(const int threshold) throw(){
    pImpl->model->setMotionThreshold(threshold);
  }

  void Controller::setOpticalFlowNPoints(const int nPoints) throw(){
    pImpl->model->setOpticalFlowNPoints(nPoints);
  }
  
  void Controller::setPixelDifferenceThreshold(const int threshold) throw(){
    pImpl->model->setPixelDifferenceThreshold(threshold);
  }

  void Controller::setPixelDifferenceXStep(const int xStep) throw(){
    pImpl->model->setPixelDifferenceXStep(xStep);
  }

  void Controller::setPixelDifferenceYStep(const int xStep) throw(){
    pImpl->model->setPixelDifferenceYStep(xStep);
  }
  
  int Controller::getSecsBtwAlarm() const throw(){
    return pImpl->secsBtwAlarms;
  }
  
  void Controller::setSecsBtwAlarm(const int secs) throw(){
    pImpl->secsBtwAlarms = secs;
    pImpl->timer.restart();
  }

  const ModelPtr Controller::getModel() const throw(){
    return pImpl->model;
  }

} /*namespace*/


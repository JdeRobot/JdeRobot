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

  Controller::Controller(gbxutilacfr::Tracer& tracer, ModelPtr m, bool createView )
    : _tracer(tracer), pImpl(new PImpl()) {
    pImpl->model = m;
    if (createView){
      pImpl->view.reset(new View(*this));
      pImpl->model->addObserver(pImpl->view);
    }
  }

  Controller::~Controller() {
    if (pImpl->view){
      pImpl->model->deleteObserver(pImpl->view);//jderobotutil::ObserverPtr(view));
      pImpl->view.reset();
    }
  }

  
  bool Controller::isRunning() const throw(){
    if (pImpl->view)
      return pImpl->view->isVisible();
    return true;
  }

  void Controller::exit() throw(){
    pImpl->running = false;
  }

  void Controller::setImage(const colorspaces::Image& img) throw(){
    pImpl->model->setImage(img);
  }

  bool Controller::isMotionDetected(MotionItem2D& max) const throw(){
    if (pImpl->model->isMotionDetected(max) &&
	(pImpl->timer.elapsedSec() > pImpl->secsBtwAlarms)){
      pImpl->timer.restart();
      return true;
    }
    return false;
  }

  void Controller::setMotionDetectionAlgorithm(const MotionDetectionAlgorithmPtr a) throw(){
    pImpl->model->setMotionDetectionAlgorithm(a);
  }

  void Controller::setMotionThreshold(const int threshold) throw(){
    pImpl->model->setMotionThreshold(threshold);
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


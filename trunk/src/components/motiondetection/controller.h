#ifndef MOTIONDETECTION_CONTROLLER_H
#define MOTIONDETECTION_CONTROLLER_H
#include <tr1/memory>
#include <gbxutilacfr/tracer.h>
#include "model.h"

namespace motiondetection {
  class Controller {
  public:
    Controller(gbxutilacfr::Tracer& tracer, ModelPtr m, bool createView = true);
    virtual ~Controller();
    
    void exit() throw();
    bool isRunning() const throw();
    
    void setImage(const colorspaces::Image& img) throw();

    bool isMotionDetected(MotionItem2D& max) const throw();

    /*set properties*/
    void setMotionDetectionAlgorithm(const MotionDetectionAlgorithmPtr a) throw();
    void setMotionThreshold(const int threshold) throw();

    int getSecsBtwAlarm() const throw();
    void setSecsBtwAlarm(const int secs) throw();

    /*get model*/
    const ModelPtr getModel() const throw();

    gbxutilacfr::Tracer& tracer() { return _tracer; };
  private:
    gbxutilacfr::Tracer& _tracer;
    class PImpl;
    std::tr1::shared_ptr<PImpl> pImpl;
  };
  typedef std::tr1::shared_ptr<Controller> ControllerPtr;
} /*namespace*/

#endif /*MOTIONDETECTION_CONTROLLER_H*/

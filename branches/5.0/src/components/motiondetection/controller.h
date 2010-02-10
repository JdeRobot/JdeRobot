#ifndef MOTIONDETECTION_CONTROLLER_H
#define MOTIONDETECTION_CONTROLLER_H
#include <tr1/memory>
#include "model.h"

namespace motiondetection {
  class Controller {
  public:
    Controller(ModelPtr m);
    virtual ~Controller();
    
    void exit() throw();
    bool isRunning() const throw();
    
    void setImage(const colorspaces::ImageppPtr img) throw(gbxutilacfr::Exception);
    const colorspaces::ImageppPtr getImage() const throw();

    const Model::MotionGrid2DPtr getMotionGrid(const int rows, const int cols) const throw();
    bool isMotionDetected(CvRect *area = 0, Model::MotionGridItem2D *value = 0) const throw();

    /*set properties*/
    void setMotionDetectionAlgorithm(const Model::MotionDetectionAlgorithm a = Model::OpticalFlow) throw();
    void setMotionGridSize(const int rows, const int cols) throw();
    void setMotionThreshold(const int threshold) throw();
    void setOpticalFlowNPoints(const int nPoints) throw();

    void setPixelDifferenceThreshold(const int threshold) throw();
    void setPixelDifferenceXStep(const int xStep) throw();
    void setPixelDifferenceYStep(const int xStep) throw();

    int getSecsBtwAlarm() const throw();
    void setSecsBtwAlarm(const int secs) throw();

    /*get model*/
    const ModelPtr getModel() const throw();
  private:
    class PImpl;
    std::tr1::shared_ptr<PImpl> pImpl;
  };
  typedef std::tr1::shared_ptr<Controller> ControllerPtr;
} /*namespace*/

#endif /*MOTIONDETECTION_CONTROLLER_H*/

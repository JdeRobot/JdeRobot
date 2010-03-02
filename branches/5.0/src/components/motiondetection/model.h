#ifndef MOTIONDETECTION_MODEL_H
#define MOTIONDETECTION_MODEL_H

#include <jderobotutil/observer.h>
#include <colorspaces/colorspacesmm.h>
#include <opencv/cv.h>
#include <vector>
#include <tr1/memory>
#include <gbxutilacfr/tracer.h>
#include "motiondetection.h"

namespace motiondetection {
  class Model : public jderobotutil::Subject{
  public:
    Model(gbxutilacfr::Tracer& tracer, const colorspaces::Image& initialImg) throw();

    /*data insertion/access*/
    void setImage(const colorspaces::Image& img) throw();
    const colorspaces::Image& getImage() const throw() { return algorithm->state().currentImage; }

    /**
     * Get detected motion using selected algorithm.
     * Returned sequence has every motion detected, crossing or not the threshold
     */
    const MotionItem2DSeq& getMotionDetected() const throw ();

    /**
     * Return true if motion detected crossing the threshold
     * \param max will contain max motion detected
     */
    bool isMotionDetected(MotionItem2D& max) const throw();

    /*properties*/
    void setMotionDetectionAlgorithm(const MotionDetectionAlgorithmPtr a) throw() { algorithm = a; notifyObservers(); }
    MotionDetectionAlgorithmPtr getMotionDetectionAlgorithm() const throw() { return algorithm; }
    
    void setMotionThreshold(const int t) throw() { motionThreshold = t; notifyObservers(); }
    int getMotionThreshold() const throw() { return motionThreshold; }

    gbxutilacfr::Tracer& tracer() { return _tracer; };
  private:
    int motionThreshold;/**< range between 0..10. \sa MotionItem2D*/
    MotionDetectionAlgorithmPtr algorithm;
    gbxutilacfr::Tracer& _tracer;
  };
  typedef std::tr1::shared_ptr<Model> ModelPtr;

}//namespace
#endif /*MOTIONDETECTION_MODEL_H*/

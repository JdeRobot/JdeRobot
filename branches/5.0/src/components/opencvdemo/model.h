#ifndef OPENCVDEMO_MODEL_H
#define OPENCVDEMO_MODEL_H

#include <jderobot/varcolor.h>
#include <jderobotutil/observer.h>
#include <gbxutilacfr/exception.h>
#include <gbxsickacfr/gbxiceutilacfr/store.h>
#include <opencv/cv.h>
#include <vector>

namespace opencvdemo {
  class Model : public virtual jderobotutil::Subject{
  public:
    /*thrown if bad data is given on any set method*/
    class BadData: public gbxutilacfr::Exception {};

    class UpdatedField : public ObserverArg {
    public:
      enum {
	Image = 0,
	State = 2
      }
      UpdatedField(int mask);
      int mask;
    };
    
    class OptFlowItem {
    public:
      OptFlowItem(CvPoint p, double flow)
	:p(p),flow(flow) {}
      CvPoint p;
      double flow;
    };
    typedef std::vector<OptFlowItem> OptFlowSeq;

    Model();

    jderobot::ImageDataPtr getImage();
    void setImage(jderobot::ImageDataPtr img);

    jderobot::ImageDataPtr getHoughTransform();

    /*optflow methods*/
    void setOptFlowThreshold(int t);
    int getOptFlowThreshold() const;
    OptFlowSeq getOptFlow();

  private:
    State state;
    jderobot::ImageDataPtr previousImage;
    jderobot::ImageDataPtr image;

    void initRawState(State s);
    void initOptFlowState(State s);
    void initHoughTransformState(State s);

  };

}//namespace
#endif /*OPENCVDEMO_MODEL_H*/

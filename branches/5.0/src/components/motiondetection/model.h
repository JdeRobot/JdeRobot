#ifndef MOTIONDETECTION_MODEL_H
#define MOTIONDETECTION_MODEL_H

#include <jderobotutil/observer.h>
#include <colorspaces/colorspaces++.h>
#include <gbxutilacfr/exceptions.h>
#include <opencv/cv.h>
#include <vector>
#include <tr1/memory>

namespace motiondetection {
  class Model : public jderobotutil::Subject{
  public:
    class MotionGridItem2D {
    public:
      MotionGridItem2D(const int motion = 0, const int count = 0)
	: motion(motion),count(count) {}
      bool operator<(const MotionGridItem2D &a){
	return (motion*count) < (a.motion*a.count);
      }
      bool operator>(const MotionGridItem2D &a){
	return (motion*count) > (a.motion*a.count);
      }
      int motion;
      int count;
    };

    class MotionGrid2D {
    public:
      MotionGrid2D(const int rows = 0, const int cols = 0)
	:rows(rows),cols(cols),grid(rows*cols,MotionGridItem2D()) {}
      int rows;
      int cols;
      std::vector<MotionGridItem2D> grid;
    };
    typedef std::tr1::shared_ptr<MotionGrid2D> MotionGrid2DPtr;
	
    class OpticalFlowState {
    public:
      OpticalFlowState(const int nPoints = 100)
	: nPoints(nPoints),nPreviousPoints(0),
	  previousPoints(nPoints),currentPoints(nPoints),status(nPoints){}
    
      ~OpticalFlowState(){}

      int nPoints;
      int nPreviousPoints;
      std::vector<CvPoint2D32f> previousPoints;
      std::vector<CvPoint2D32f> currentPoints;
      std::vector<char> status;
    };
    typedef std::tr1::shared_ptr<OpticalFlowState> OpticalFlowStatePtr;

    class PixelDifferenceState {
    public:
      PixelDifferenceState(colorspaces::ImageppPtr background)
	: backgroundL8(background->toL8()), differenceL8(backgroundL8),
	  backgroundCount(0),
	  threshold(30),xStep(8),yStep(8) {}
      colorspaces::ImageppPtr backgroundL8;
      colorspaces::ImageppPtr differenceL8;
      int backgroundCount;
      int threshold;
      int xStep;
      int yStep;
    };
    typedef std::tr1::shared_ptr<PixelDifferenceState> PixelDifferenceStatePtr;

    enum MotionDetectionAlgorithm{
      OpticalFlow = 0,
      PixelDifference,
      LAST_ALGORITHM
    };

    Model() throw();

    /*data insertion/access*/
    void setImage(const colorspaces::ImageppPtr img) throw(gbxutilacfr::Exception);
    const colorspaces::ImageppPtr getImage() const throw();

    const MotionGrid2DPtr getMotionGrid() const throw();
    bool isMotionDetected(CvRect *area = 0, MotionGridItem2D *value = 0) const throw();

    /*properties*/
    void setMotionDetectionAlgorithm(const MotionDetectionAlgorithm a = OpticalFlow) throw();
    MotionDetectionAlgorithm getMotionDetectionAlgorithm() const throw();
    void setMotionGridSize(const int rows, const int cols) throw();
    void getMotionGridSize(int *rows, int *cols) const throw();
    void setMotionThreshold(const int t) throw();
    int getMotionThreshold() const throw();

    void setOpticalFlowNPoints(const int n) throw();
    int getOpticalFlowNPoints() const throw();

    const OpticalFlowStatePtr getOpticalFlowState() const throw();

    void setPixelDifferenceThreshold(const int threshold) throw();
    int getPixelDifferenceThreshold() const throw();

    void setPixelDifferenceXStep(const int xStep) throw();
    int getPixelDifferenceXStep() const throw();

    void setPixelDifferenceYStep(const int yStep) throw();
    int getPixelDifferenceYStep() const throw();

    const PixelDifferenceStatePtr getPixelDifferenceState() const throw();
  private:
    void calcMotionWithOpticalFlow();
    void calcMotionWithPixelDifference();
    class PImpl;
    std::tr1::shared_ptr<PImpl> pImpl;
  };
  typedef std::tr1::shared_ptr<Model> ModelPtr;

}//namespace
#endif /*MOTIONDETECTION_MODEL_H*/

/*
 *
 *  Copyright (C) 1997-2010 JDERobot Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *
 *  Authors : David Lobato Bravo <dav.lobato@gmail.com>
 *
 */

#ifndef MOTIONDETECTION_MOTIONDETECTION_H
#define MOTIONDETECTION_MOTIONDETECTION_H

#include <opencv/cv.h>
#include <colorspaces/colorspacesmm.h>
#include <gbxutilacfr/tracer.h>
#include <jderobotutil/time.h>


namespace motiondetection {
  /**
   * Represent a 2D area where motion has been detected
   */
  class MotionItem2D {
  public:
    MotionItem2D(const int motion = 0, const int count = 0, const cv::Rect area = cv::Rect())
      : motion(motion),count(count),area(area) {}
    bool operator<(const MotionItem2D &a) const{
      return (motion*count) < (a.motion*a.count);
    }
    bool operator>(const MotionItem2D &a) const{
      return (motion*count) > (a.motion*a.count);
    }
    int motion;/**< value in the range 0(no motion detected)..10(maximun motion detected)*/
    int count;/**< times motion has been detected in this area. With values >1, motion value will be the mean of detected motion*/
    cv::Rect area;/**< rectangle area where motion has been detected*/
  };

  /**
   * A sequence of areas with motion
   */
  typedef std::vector<MotionItem2D> MotionItem2DSeq;

  
  /**
   * Generic motion detection algorithm
   */
  class MotionDetectionAlgorithm{
  public:
    class AlgorithmState {
    public:
      AlgorithmState(const colorspaces::Image& initialImg)
	: previousImage(initialImg),currentImage(initialImg), ipsCounter() {}
      colorspaces::ImageRGB8 previousImage;
      colorspaces::ImageRGB8 currentImage;
      MotionItem2DSeq motionDetected;
      MotionItem2D maxMotionDetected;
      jderobotutil::IpsCounter ipsCounter;
    };

    MotionDetectionAlgorithm(gbxutilacfr::Tracer& tracer)
      : _tracer(tracer) { }
    virtual ~MotionDetectionAlgorithm() { };
    
    virtual const MotionItem2DSeq& calcMotion(const colorspaces::Image& currentImg) throw () = 0;
    virtual const MotionItem2DSeq& getMotionDetected() const throw () { return state().motionDetected; }
    virtual const MotionItem2D& getMaxMotionDetected() const throw () { return state().maxMotionDetected; }

    /**
     * Get algorithm state for debug purpouses.
     * Only useful after a call to getMotion.
     */
    virtual const AlgorithmState& state() const throw() = 0;
    gbxutilacfr::Tracer& tracer() { return _tracer; };
  private:
    gbxutilacfr::Tracer& _tracer;
  };
  typedef std::tr1::shared_ptr<MotionDetectionAlgorithm> MotionDetectionAlgorithmPtr;
  
  class NullAlgorithm: public MotionDetectionAlgorithm {
  public:
    NullAlgorithm(gbxutilacfr::Tracer& tracer,
		  const colorspaces::Image& initialImg)
      : MotionDetectionAlgorithm(tracer),_state(initialImg) {}
    virtual const MotionItem2DSeq& calcMotion(const colorspaces::Image& currentImg) throw ();
    virtual const AlgorithmState& state() const throw() { return _state; }
  private:
    AlgorithmState _state;
  };

  class OpticalFlowAlgorithm: public MotionDetectionAlgorithm {
  public:
    class OpticalFlowAlgorithmState: public MotionDetectionAlgorithm::AlgorithmState {
    public:
      OpticalFlowAlgorithmState(const colorspaces::Image& initialImg,
				const int nPoints)
	: MotionDetectionAlgorithm::AlgorithmState(initialImg),
	  currentImageGRAY8(initialImg),
	  previousImageGRAY8(initialImg),
	  nPreviousPoints(0),previousPoints(nPoints),currentPoints(nPoints),status(nPoints) {}
      colorspaces::ImageGRAY8 currentImageGRAY8;
      colorspaces::ImageGRAY8 previousImageGRAY8;
      int nPreviousPoints;
      std::vector<cv::Point2f> previousPoints;
      std::vector<cv::Point2f> currentPoints;
      std::vector<uchar> status;
      std::vector<float> error;
    };

    OpticalFlowAlgorithm(gbxutilacfr::Tracer& tracer,
			 const colorspaces::Image& initialImg,
			 const int nPoints = 100,
			 const int opticalFlowThreshold = 5)
      : MotionDetectionAlgorithm(tracer),
	nPoints(nPoints),opticalFlowThreshold(opticalFlowThreshold),
      _state(initialImg,nPoints) {}

    virtual const MotionItem2DSeq& calcMotion(const colorspaces::Image& currentImg) throw ();
    virtual const AlgorithmState& state() const throw() { return _state; }
    const OpticalFlowAlgorithmState& opticalFlowState() const throw() { return _state; }
    static const int opticalFlowMaxMotion = 50;//value used to normalize calculated value to motion range 0..10
    //parameters
    const int nPoints;
    const int opticalFlowThreshold;
  private:
    OpticalFlowAlgorithmState _state;
  };
  typedef std::tr1::shared_ptr<OpticalFlowAlgorithm> OpticalFlowAlgorithmPtr;
  
  class PixelDifferenceAlgorithm: public MotionDetectionAlgorithm {
  public:
    class PixelDifferenceAlgorithmState: public MotionDetectionAlgorithm::AlgorithmState {
    public:
      PixelDifferenceAlgorithmState(const colorspaces::Image& initialImg)
	: MotionDetectionAlgorithm::AlgorithmState(initialImg),
	  background(initialImg.clone()),
	  difference(colorspaces::Image(cv::Mat::zeros(initialImg.rows,
						       initialImg.cols,
						       colorspaces::ImageRGB8::FORMAT_RGB8->cvType),
					colorspaces::ImageRGB8::FORMAT_RGB8)),
	  backgroundCount(0) {}
      colorspaces::ImageRGB8 background;
      colorspaces::ImageRGB8 difference;
      int backgroundCount;
    };
    
    PixelDifferenceAlgorithm(gbxutilacfr::Tracer& tracer,
			     const colorspaces::Image& initialImg,
			     const int pixelDiffThreshold = 128,
			     const cv::Size winSize = cv::Size(8,8))
      : MotionDetectionAlgorithm(tracer),
	pixelDiffThreshold(pixelDiffThreshold),winSize(winSize),
	_state(initialImg) {}
    
    virtual const MotionItem2DSeq& calcMotion(const colorspaces::Image& currentImg) throw ();
    virtual const AlgorithmState& state() const throw() { return _state; }
    const PixelDifferenceAlgorithmState& pixelDifferenceState() const throw() { return _state; }
    static const int pixelDiffMaxDiff = 255;//value used to normalize calculated value to motion range 0..10
    //parameters
    const int pixelDiffThreshold;
    const cv::Size winSize;
  private:
    //state
    PixelDifferenceAlgorithmState _state;
  };
  typedef std::tr1::shared_ptr<PixelDifferenceAlgorithm> PixelDifferenceAlgorithmPtr;
}//namespace

#endif //MOTIONDETECTION_MOTIONDETECTION_H

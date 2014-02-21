
#ifndef IMAGE_ICE
#define IMAGE_ICE

#include <jderobot/common.ice>

module jderobot {

  enum FeaturesType { Detected, Filtered, Ball, BlueNet, YellowNet, Field, Lines, Regions, Segments, HSV };
  enum ObjectsType { BallObj, BlueNetObj, YellowNetObj, FieldObj, LinesObj };
  enum CameraType { UPPERCAMERA, LOWERCAMERA };
    
  /**
   *  Static description of the image source.
   */
  class ImageDescription 
  {
    int width; /**< %Image width [pixels] */
    int height;/**< %Image height [pixels] */
    int size;/**< %Image size [bytes] */
    string format; /**< %Image format string */
  };
  
  /**
  * A single image served as a sequence of bytes
  */
  class ImageData
  { 
    Time timeStamp; /**< TimeStamp of Data */
    ImageDescription description; /**< ImageDescription of Data, for convienence purposes */
    ByteSeq pixelData; /**< The image data itself. The structure of this byte sequence
			  depends on the image format and compression. */
  };
  
  class HSVFilter
  {
  	float hmin;
  	float hmax;
  	float smin;
  	float smax;
  	float vmin;
  	float vmax;
  };
  
  class CalibrationParams
  {
  	float robotx;
	float roboty;
	float robott;
	float u0;
	float v0;
	float fdistx;
	float fdisty;
  };
  
    
  /** 
   * Interface to the image provider.
   */
  interface ImageProvider
  {
    /** 
     * Returns the image source description.
     */
    idempotent ImageDescription getImageDescription();

    /**
     * Returns the latest data.
     */
    ["amd"] idempotent ImageData getImageData()
      throws DataNotExistException, HardwareFailedException;
  };
  
  /** 
  * Interface to the image selector.
  */
  interface ImageSelector extends ImageProvider
  {
    /** 
     * Changes the type of image sent (filtered, segmented, ...)
     */
    idempotent ImageData getImageDataWithFeatures(FeaturesType type)
      throws DataNotExistException, HardwareFailedException;
      
    idempotent HSVFilter getHSVFilter (CameraType cam, ObjectsType obj);
    idempotent void setHSVFilter (CameraType cam, ObjectsType obj, HSVFilter newFilter);
    
    idempotent void setCam (CameraType cam);
  };
  
  /** 
  * Interface to the calibration.
  */
  interface CalibrationProvider
  {
    /** 
     * 
     */
    idempotent ImageData getCalibrationImg()
      throws DataNotExistException, HardwareFailedException;
    idempotent CalibrationParams getCalibrationParams();
    idempotent void setCalibrationParams (CalibrationParams newParams);
  };
  
};

#endif //IMAGE_ICE

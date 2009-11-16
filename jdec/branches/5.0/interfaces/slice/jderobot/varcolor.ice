#ifndef VARCOLOR_ICE
#define VARCOLOR_ICE


#include <jderobot/common.ice>

module jderobot{  

  //! Static description of the image source.
  class ImageDescription 
  {
    //! %Image width [pixels]
    int width; 
    
    //! %Image height [pixels]
    int height;

    //! %Image size [bytes]
    int size;
    
    //! %Image format string
    string format;
  };
  
  //! A single image served as a sequence of bytes
  class ImageDataByteSeq
  { 
    //! TimeStamp of Data
    Time timeStamp;   
    
    //! ImageDescription of Data, for convienence purposes
    ImageDescription description;
    
    //! The image data itself. The structure of this byte sequence
    //! depends on the image format and compression.
    ByteSeq pixelData;
  };

  //! A sequence of pixels. The pixel will be packed inside a int.
  sequence<int> PixelSeq;


  //! A single image served as packed pixels
  class ImageDataPixelSeq
  { 
    //! TimeStamp of Data
    Time timeStamp;   
    
    //! ImageDescription of Data, for convienence purposes
    ImageDescription description;
    
    //! The image data itself. The structure of this pixel sequence
    //! depends on the image format and compression.
    PixelSeq pixelData;
  };

  //! Interface to the image provider.
  interface VarColor
  {
    //! Returns the image source description.
    idempotent ImageDescription getDescription();

    //! Returns the latest data.
    idempotent ImageDataByteSeq getDataAsByteSeq()
      throws DataNotExistException, HardwareFailedException;

    //! Returns the latest data as a pixel sequence.
    idempotent ImageDataPixelSeq getDataAsPixelSeq()
      throws DataNotExistException, HardwareFailedException;
  };

}; /*module*/

#endif /*VARCOLOR_ICE*/

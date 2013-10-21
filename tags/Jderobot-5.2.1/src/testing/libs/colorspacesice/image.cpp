#include "image.h"
#include <string>

namespace colorspacesice{
  Image::Image(colorspaces::Image& i)
    : colorspaces::Image(i),imageData(0) {}
  
  Image::Image(const jderobot::ImageDataPtr& img)
    : Image(img->description->height,
	    img->description->width,
	    colorspaces::Image::Format::searchFormat(img->description->format),//it can return a null pointer!?
	    (char*)&(img->pixelData[0])),
      imageData(img) {}

  Image::operator jderobot::ImageDataPtr() const{
    if (!imageData){
      imageData = new jderobot::ImageData();
      imageData->description = (jderobot::ImageDescriptionPtr)(imgDesc);/*cast*/
      imageData->pixelData.resize(imgDesc.size);
      memmove( &(imageData->pixelData[0]), imageData, imgDesc.size );
      
    IceUtil::Time t = IceUtil::Time::now();
    ImageDescription imgDesc(description);/*copy and cast*/

    imageData->timeStamp.seconds = (long)t.toSeconds();
    imageData->timeStamp.useconds = (long)t.toMicroSeconds() - 
      imageData->timeStamp.seconds*1000000;
    
    return imageData;
  }

} //colorspacesice

#include "image.h"
#include <string>

namespace colorspacesice{
  ImageDescription::ImageDescription(const int width,
				     const int height,
				     const int size,
				     const Format *fmt)
    :colorspaces::ImageDescriptionpp(width,height,size,fmt) {}

  ImageDescription::ImageDescription(const ::ImageDescription& i)
    :colorspaces::ImageDescriptionpp(i) {}

  ImageDescription::ImageDescription(const jderobot::ImageDescriptionPtr& iDesc)
    :colorspaces::ImageDescriptionpp(iDesc->width,iDesc->height,iDesc->size,0) {
    const Format *fmt;

    fmt = searchFormat(iDesc->format.c_str());
    if (fmt==0)
      fmt = &getFormatTable(0)[FORMAT_UNKNOWN];
    this->fmt = fmt;
  }

  ImageDescription::operator jderobot::ImageDescriptionPtr() const{
    jderobot::ImageDescriptionPtr iPtr = new jderobot::ImageDescription();
    iPtr->width = this->width;
    iPtr->height = this->height;
    iPtr->size = this->size;
    iPtr->format = std::string(this->fmt->format_name);
    return iPtr;
  }

  Image::Image(const ::ImageDescription &desc, char *const data)
    :Imagepp(desc,data),imgPtr(0) {}

  Image::Image(::Image& i)
    :Imagepp(i),imgPtr(0) {}
  
  Image::Image(const jderobot::ImageDataPtr& img)
    :Imagepp(colorspacesice::ImageDescription(img->description),(char*)&(img->pixelData[0])),
     imgPtr(img) {}

  Image::operator jderobot::ImageDataPtr() const{
    jderobot::ImageDataPtr imgDataPtr = new jderobot::ImageData();
    IceUtil::Time t = IceUtil::Time::now();
    ImageDescription imgDesc(description);/*copy and cast*/

    imgDataPtr->timeStamp.seconds = (long)t.toSeconds();
    imgDataPtr->timeStamp.useconds = (long)t.toMicroSeconds() - 
      imgDataPtr->timeStamp.seconds*1000000;
    imgDataPtr->description = (jderobot::ImageDescriptionPtr)(imgDesc);/*cast*/
    imgDataPtr->pixelData.resize(imgDesc.size);
    memmove( &(imgDataPtr->pixelData[0]), imageData, imgDesc.size );
    return imgDataPtr;
  }

} //colorspacesice

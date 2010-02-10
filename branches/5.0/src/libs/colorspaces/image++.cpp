#include "image++.h"
#include <cstdlib>
#include <formats++.h>

namespace colorspaces {
  ImageDescriptionpp::ImageDescriptionpp(const ImageDescription& i){
    ImageDescription_initialize(this,i.width,i.height,i.size,i.fmt);
  }

  ImageDescriptionpp::ImageDescriptionpp(const int width,
					 const int height,
					 const int size,
					 const Format *fmt){
    ImageDescription_initialize(this,width,height,size,fmt);
  }

  ImageDescriptionpp::~ImageDescriptionpp(){
    ImageDescription_deinitialize(this);
  }

  Imagepp::Imagepp(const ImageDescription& desc, char * const data) {
    Image_initialize(this,&desc,data);
  }

  Imagepp::Imagepp(Image& i){
    Image *copy = Image_copy(&i);
    Image_initialize(this,&copy->description,copy->imageData);
    Image_swapDataOwner(copy,this);
    delete_Image(copy);
  }

  Imagepp::~Imagepp(){
    Image_deinitialize(this);
  }

  ImageppPtr Imagepp::toL8() const{
    Image *i = Image_toL8(this);
    if (i!=0){
      ImageppPtr ii(new Imagepp(i->description,i->imageData));
      Image_swapDataOwner(i,ii.get());
      delete_Image(i);
      return ii;
    }else
      return ImageppPtr();
  }

  ImageppPtr Imagepp::toRGB888() const{
    Image *i = Image_toRGB888(this);
    if (i!=0){
      ImageppPtr ii(new Imagepp(i->description,i->imageData));
      Image_swapDataOwner(i,ii.get());
      delete_Image(i);
      return ii;
    }else
      return ImageppPtr();
  }

  ImageppPtr Imagepp::toYUY2() const{
   Image *i = Image_toYUY2(this);
    if (i!=0){
      ImageppPtr ii(new Imagepp(i->description,i->imageData));
      Image_swapDataOwner(i,ii.get());
      delete_Image(i);
      return ii;
    }else
      return ImageppPtr();
  }

  ImageppPtr Imagepp::convert(const Format *outFmt) const{
    Image *i = Image_convert(this,outFmt);
    if (i!=0){
      ImageppPtr ii(new Imagepp(i->description,i->imageData));
      Image_swapDataOwner(i,ii.get());
      delete_Image(i);
      return ii;
    }else
      return ImageppPtr();
  }

  ImageppPtr Imagepp::createTestHline(const int width,
				      const int height,
				      const int lineWidth,
				      const int startRow,
				      const RGBColor *bgColor,
				      const RGBColor *fgColor){
    Image *i = Image_createTestHline(width,height,lineWidth,startRow,bgColor,fgColor);
    if (i!=0){
      ImageppPtr ii(new Imagepp(i->description,i->imageData));
      Image_swapDataOwner(i,ii.get());
      delete_Image(i);
      return ii;
    }else
      return ImageppPtr();
  }

  ImageppPtr Imagepp::createTestVline(const int width,
				      const int height,
				      const int lineWidth,
				      const int startCol,
				      const RGBColor *bgColor,
				      const RGBColor *fgColor){
    Image *i = Image_createTestVline(width,height,lineWidth,startCol,bgColor,fgColor);
    if (i!=0){
      ImageppPtr ii(new Imagepp(i->description,i->imageData));
      Image_swapDataOwner(i,ii.get());
      delete_Image(i);
      return ii;
    }else
      return ImageppPtr();
  }

  ImageppPtr Imagepp::createTestSquare(const int width,
				       const int height,
				       const int sideLength,
				       const int xStartCorner,
				       const int yStartCorner,
				       const RGBColor *bgColor,
				       const RGBColor *fgColor){
    Image *i = Image_createTestSquare(width,height,
				      sideLength,
				      xStartCorner,yStartCorner,
				      bgColor,bgColor);
    if (i!=0){
      ImageppPtr ii(new Imagepp(i->description,i->imageData));
      Image_swapDataOwner(i,ii.get());
      delete_Image(i);
      return ii;
    }else
      return ImageppPtr();
  }


}//namespace

std::ostream &operator<<(std::ostream &stream, ImageDescription& desc){
  char str[512];

  ImageDescription_snprintf(str,512,&desc);
  stream << str;
  return stream;
}

std::ostream &operator<<(std::ostream &stream, Image& img){
  char str[1024];

  Image_snprintf(str,1024,&img);
  stream << str;
  return stream;
}

#include "imagecv.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>

namespace colorspaces {
  Image::Format::Format(const std::string name, const int id, const int cvType, imageCtor ctor, imageCvt cvt)
    : name(name), id(id), cvType(cvType),ctor(ctor),cvt(cvt) {}

  std::vector<Image::FormatPtr>& Image::Format::formatTable(){
    static std::vector<FormatPtr> formatTable;
    return formatTable;
  }

  const Image::FormatPtr Image::Format::searchFormat(const std::string name){
    std::vector<FormatPtr>::iterator it;

    for (it = formatTable().begin(); it != formatTable().end(); it++){
      if (name.compare((*it)->name) == 0)
	return *it;
    }
    return FormatPtr();
  }

  const Image::FormatPtr  Image::Format::createFormat(const std::string name, const int cvType, imageCtor ctor, imageCvt cvt){
    int id = formatTable().size();
    FormatPtr nFmt(new Format(name,id,cvType,ctor,cvt));
    formatTable().push_back(nFmt);
    return nFmt;
  }

  Image* Image::Format::createInstance(const int width, const int height, void *const data){
    if (ctor)
      return ctor(width,height,data);
    return 0;
  }
   
  Image& Image::convert(Image& dst) const throw(NoConversion){
    return _format->cvt(*this,dst);
  }

  Image Image::clone() const{
    Image copy(cv::Mat::clone(),_format);
    return copy;
  }
 

  //static definitions
  const Image::FormatPtr Image::FORMAT_NONE = Image::Format::createFormat("NONE",0,0,0);
  const Image::FormatPtr  ImageRGB888::FORMAT_RGB888 = Image::Format::createFormat("RGB888",CV_8UC3,&ImageRGB888::createInstance,&ImageRGB888::imageCvt);
  const Image::FormatPtr  ImageYUY2::FORMAT_YUY2 = Image::Format::createFormat("YUY2",CV_8UC2,&ImageYUY2::createInstance,&ImageYUY2::imageCvt);
  const Image::FormatPtr  ImageGRAY8::FORMAT_GRAY8 = Image::Format::createFormat("GRAY8",CV_8UC1,&ImageGRAY8::createInstance,&ImageGRAY8::imageCvt);

  Image::Image(const int width, const int height, const FormatPtr fmt)
    : cv::Mat(height,width,fmt->cvType),width(width),height(height),_format(fmt) {}

  Image::Image(const int width, const int height, const FormatPtr fmt, void *const data)
    : cv::Mat(height,width,fmt->cvType,data),width(width),height(height),_format(fmt) {}

  Image::Image(const Image& i)
    : cv::Mat(i),width(i.width),height(i.height),_format(i.format()) {}

  Image::Image(const cv::Mat& m, const FormatPtr fmt)
    : cv::Mat(m),width(m.cols),height(m.rows),_format(fmt) {}

  ImageRGB888::ImageRGB888(const int width, const int height)
    : Image(width,height,FORMAT_RGB888) {}
    
  ImageRGB888::ImageRGB888(const int width, const int height, void *const data)
    : Image(width,height,FORMAT_RGB888,data) {}

  ImageRGB888::ImageRGB888(const Image& i)
    : Image(i.width,i.height,FORMAT_RGB888) {
    i.convert(*this);
  }

  Image& ImageRGB888::imageCvt(const Image& src, Image& dst) throw(NoConversion){
    assert(src.format() == FORMAT_RGB888 && "src is not a RGB888 image");
    if (dst.format() == FORMAT_RGB888)
      dst = src;
    else {
      const ImageRGB888 srcRgb888(src);//cast src to rgb image
      if (dst.format() == ImageYUY2::FORMAT_YUY2)
	srcRgb888.toYUY2(dst);
      else if (dst.format() == ImageGRAY8::FORMAT_GRAY8)
	srcRgb888.toGRAY8(dst);
      else
	throw Image::NoConversion();
    }
    return dst;
  }

  void ImageRGB888::toGRAY8(Image& dst) const throw(Image::FormatMismatch){
    if (dst.format() != ImageGRAY8::FORMAT_GRAY8)
      throw Image::FormatMismatch("FORMAT_GRAY8 required for dst");
    cv::cvtColor(*this,dst,CV_RGB2GRAY);
  }

  void ImageRGB888::toYUY2(Image& dst) const throw(Image::FormatMismatch){
    if (dst.format() != ImageYUY2::FORMAT_YUY2)
      throw Image::FormatMismatch("FORMAT_YUY2 required for dst");
    if ((dst.width % 2 != 0) || (this->width % 2 != 0))
      throw Image::FormatMismatch("src and dst images have to have even number of columns");

    cv::Mat_<cv::Vec3b> ycrcb(dst.height,dst.width,dst.type());//YUV444 previous conversion
    cv::Mat_<cv::Vec2b> yuy2(dst);
    cv::cvtColor(*this,ycrcb,CV_RGB2YCrCb);

    for (int i=0; i < height; i++){
      for (int j=0; j < width; j+=2){//two pixels each loop
	yuy2(i,j)[0] = ycrcb(i,j)[0];//Y0
	yuy2(i,j)[1] = ycrcb(i,j)[2];//U0
	yuy2(i,j+1)[0] = ycrcb(i,j+1)[0];//Y1
	yuy2(i,j+1)[1] = ycrcb(i,j)[1];//V0
      }
    }
  }

  Image* ImageRGB888::createInstance(const int width, const int height, void *const data){
    if (data)
      return new ImageRGB888(width,height,data);
    else
      return new ImageRGB888(width,height);
  }

  ImageYUY2::ImageYUY2(const int width, const int height)
    : Image(width,height,FORMAT_YUY2) {}
    
  ImageYUY2::ImageYUY2(const int width, const int height, void *const data)
    : Image(width,height,FORMAT_YUY2,data) {}

  ImageYUY2::ImageYUY2(const Image& i)
    : Image(i.width,i.height,FORMAT_YUY2) {
    i.convert(*this);
  }

  Image& ImageYUY2::imageCvt(const Image& src, Image& dst) throw(NoConversion){
    assert(src.format() == FORMAT_YUY2 && "src is not a YUY2 image");
    if (dst.format() == FORMAT_YUY2)
      dst = src;
    else {
      const ImageYUY2 srcYuy2(src);
      if (dst.format() == ImageRGB888::FORMAT_RGB888)
	srcYuy2.toRGB888(dst);
      else if (dst.format() == ImageGRAY8::FORMAT_GRAY8)
	srcYuy2.toGRAY8(dst);
      else
	throw Image::NoConversion();
    }
    return dst;
  }

  void ImageYUY2::toGRAY8(Image& dst) const throw(Image::FormatMismatch){
    if (dst.format() != ImageGRAY8::FORMAT_GRAY8)
      throw Image::FormatMismatch("FORMAT_GRAY8 required for dst");

    int fromTo[] = {0,0};//first channel of YUY2 have the luminance information
    cv::mixChannels(this,1,&dst,1,fromTo,1);
  }

  void ImageYUY2::toRGB888(Image& dst) const throw(Image::FormatMismatch){
    if (dst.format() != ImageRGB888::FORMAT_RGB888)
      throw Image::FormatMismatch("FORMAT_RGB888 required for dst");
    if ((dst.width % 2 != 0) || (this->width % 2 != 0))
      throw Image::FormatMismatch("src and dst images have to have even number of columns");

    cv::Mat_<cv::Vec3b> ycrcb(dst.height,dst.width,dst.type());//YUV444 previous conversion
    cv::Mat_<cv::Vec2b> yuy2(*this);

    for (int i=0; i < height; i++){
      for (int j=0; j < width; j+=2){//two pixels each loop
	ycrcb(i,j)[0] = yuy2(i,j)[0];//Y0<-Y0
	ycrcb(i,j)[1] = yuy2(i,j+1)[1];//V0<-V0
	ycrcb(i,j)[2] = yuy2(i,j)[1];//U0<-U0
	ycrcb(i,j+1)[0] = yuy2(i,j+1)[0];//Y1<-Y1
	ycrcb(i,j+1)[1] = yuy2(i,j+1)[1];//V1<-V0
	ycrcb(i,j+1)[2] = yuy2(i,j)[1];//U1<-U0
      }
    }
    cv::cvtColor(ycrcb,dst,CV_YCrCb2RGB);
  }

  Image* ImageYUY2::createInstance(const int width, const int height, void *const data){
    if (data)
      return new ImageYUY2(width,height,data);
    else
      return new ImageYUY2(width,height);
  }
  
  ImageGRAY8::ImageGRAY8(const int width, const int height)
    : Image(width,height,FORMAT_GRAY8) {}
    
  ImageGRAY8::ImageGRAY8(const int width, const int height, void *const data)
    : Image(width,height,FORMAT_GRAY8,data) {}

  ImageGRAY8::ImageGRAY8(const Image& i)
    : Image(i.width,i.height,FORMAT_GRAY8) {
    i.convert(*this);
  }

  Image& ImageGRAY8::imageCvt(const Image& src, Image& dst) throw(NoConversion){
    assert(src.format() == FORMAT_GRAY8 && "src is not a GRAY8 image");
    if (dst.format() == FORMAT_GRAY8)
      dst = src;
    else {
      const ImageGRAY8 srcGray8(src);
      if (dst.format() == ImageYUY2::FORMAT_YUY2)
	srcGray8.toYUY2(dst);
      else if (dst.format() == ImageRGB888::FORMAT_RGB888)
	srcGray8.toRGB888(dst);
      else
	throw Image::NoConversion();
    }
    return dst;
  }

  void ImageGRAY8::toRGB888(Image& dst) const throw(Image::FormatMismatch){
    if (dst.format() != ImageRGB888::FORMAT_RGB888)
      throw Image::FormatMismatch("FORMAT_RGB888 required for dst");

    cv::cvtColor(*this,dst,CV_GRAY2RGB);
  }

  void ImageGRAY8::toYUY2(Image& dst) const throw(Image::FormatMismatch){
    if (dst.format() != ImageYUY2::FORMAT_YUY2)
      throw Image::FormatMismatch("FORMAT_YUY2 required for dst");
    //U and V will be 0
    cv::Mat uv(cv::Mat::zeros(width,height,FORMAT_GRAY8->cvType));

    int fromTo[] = {0,0 , 1,1};//GRAY to Y channel, 0->U/V
    cv::Mat src[] = {*this,uv};
    cv::mixChannels(src,2,&dst,1,fromTo,1);
  }

  Image* ImageGRAY8::createInstance(const int width, const int height, void *const data){
    if (data)
      return new ImageGRAY8(width,height,data);
    else
      return new ImageGRAY8(width,height);
  }

//   ImageppPtr Imagepp::createTestHline(const int width,
// 				      const int height,
// 				      const int lineWidth,
// 				      const int startRow,
// 				      const RGBColor *bgColor,
// 				      const RGBColor *fgColor){
//     Image *i = Image_createTestHline(width,height,lineWidth,startRow,bgColor,fgColor);
//     if (i!=0){
//       ImageppPtr ii(new Imagepp(i->description,i->imageData));
//       Image_swapDataOwner(i,ii.get());
//       delete_Image(i);
//       return ii;
//     }else
//       return ImageppPtr();
//   }

//   ImageppPtr Imagepp::createTestVline(const int width,
// 				      const int height,
// 				      const int lineWidth,
// 				      const int startCol,
// 				      const RGBColor *bgColor,
// 				      const RGBColor *fgColor){
//     Image *i = Image_createTestVline(width,height,lineWidth,startCol,bgColor,fgColor);
//     if (i!=0){
//       ImageppPtr ii(new Imagepp(i->description,i->imageData));
//       Image_swapDataOwner(i,ii.get());
//       delete_Image(i);
//       return ii;
//     }else
//       return ImageppPtr();
//   }

//   ImageppPtr Imagepp::createTestSquare(const int width,
// 				       const int height,
// 				       const int sideLength,
// 				       const int xStartCorner,
// 				       const int yStartCorner,
// 				       const RGBColor *bgColor,
// 				       const RGBColor *fgColor){
//     Image *i = Image_createTestSquare(width,height,
// 				      sideLength,
// 				      xStartCorner,yStartCorner,
// 				      bgColor,bgColor);
//     if (i!=0){
//       ImageppPtr ii(new Imagepp(i->description,i->imageData));
//       Image_swapDataOwner(i,ii.get());
//       delete_Image(i);
//       return ii;
//     }else
//       return ImageppPtr();
//   }


}//namespace

/**
 * Insert a format in an output stream. Only debugging, output could be truncated
 */
std::ostream &operator<<(std::ostream &stream, const colorspaces::Image::Format& fmt){
  stream << "FMT("<< fmt.name << ";channels:" << CV_MAT_CN(fmt.cvType) << ";depth:" << CV_MAT_DEPTH(fmt.cvType) << ")";
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const colorspaces::Image& img){
  stream << "IMG(" << *(img.format()) << ";" << img.width << "x" << img.height << ")";
  return stream;
}

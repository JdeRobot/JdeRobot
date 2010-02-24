/*
 *
 *  Copyright (C) 1997-2009 JDERobot Developers Team
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

#ifndef IMAGECV_COLORSPACES_H
#define IMAGECV_COLORSPACES_H

#include <opencv/cv.h>
#include <exception>
#include <vector>
#include <iostream>
#include <tr1/memory>
#include "uncopyable.h"

namespace colorspaces {
  /**
   * An image
   */
  class Image;
  typedef Image* (*imageCtor)(const int height, const int width, void *const data);

  class Image: public  cv::Mat {
  public:
    class Format;
    typedef std::tr1::shared_ptr<Format> FormatPtr;

    class Format: public Uncopyable{
    public:
      static const FormatPtr createFormat(const std::string name, const int cvType, imageCtor ctor);
      static const FormatPtr getFormat(const int fmtId);
      static const FormatPtr searchFormat(const std::string name);
      Image* createInstance(const int width, const int height, void *const data);
      int bytesPerPixel() { return CV_ELEM_SIZE(cvType); }
      std::string name;/**< String that represents this format*/ 
      int id;/**< Format identification*/
      int cvType;/**< Opencv data type used for pixel data*/
      imageCtor ctor;
    private:
      Format(const std::string name, const int id, const int cvType, imageCtor ctor);
      static std::vector<FormatPtr>& formatTable();
    };

    class FormatMismatch: public std::exception{
    public:
      FormatMismatch(const std::string msg)
	: message(msg) {}
      ~FormatMismatch() throw() {}
      virtual const char* what() const throw()
      {
	return message.c_str();
      }
    private:
      const std::string message;
    };

    class NoConversion: public std::exception{
      virtual const char* what() const throw()
      {
	return "Can't convert image to requested format";
      }
    };
      
    /**
     * Constructor
     */
    Image(const int width, const int height, const FormatPtr fmt);
    
    /**
     * Constructor from user data
     */
    Image(const int width, const int height, const FormatPtr fmt, void *const data);

    /**
     * Copy onstructor
     */
    Image(const Image& i);

    /**
     * Copy onstructor from cv::Mat
     */
    Image(const cv::Mat& m, const FormatPtr fmt);

    /**
     * Image destructor
     */
    virtual ~Image() {}

    /**
     * Get image's format
     */
    const FormatPtr  format() const { return _format; }

    virtual void convert(Image& dst) const throw(NoConversion) = 0;

    int width;
    int height;

    static const FormatPtr FORMAT_NONE;
  private:
    FormatPtr _format;
  };
  typedef std::tr1::shared_ptr<Image> ImagePtr;

  class ImageRGB888: public Image {
  public:
    /**
     * Constructor
     */
    ImageRGB888(const int width, const int height);
    
    /**
     * Constructor from user data
     */
    ImageRGB888(const int width, const int height, void *const data);

    /**
     * Copy constructor from Image, conversion will happen if needed
     */
    ImageRGB888(const Image& i);
    
    /**
     * Conversion methods.
     */
    virtual void convert(Image& dst) const throw(NoConversion);
    void toGRAY8(Image& dst) const throw(FormatMismatch);
    void toYUY2(Image& dst) const throw(FormatMismatch);
    
    /**
     * Factory method
     */
    static Image* createInstance(const int width, const int height, void *const data);
    static const FormatPtr FORMAT_RGB888;
  };
  typedef std::tr1::shared_ptr<ImageRGB888> ImageRGB888Ptr;

  class ImageYUY2: public  Image {
  public:
    /**
     * Constructor
     * Width have to be an even number.
     */
    ImageYUY2(const int width, const int height);
    
    /**
     * Constructor from user data
     * Width have to be an even number.
     */
    ImageYUY2(const int width, const int height, void *const data);

    /**
     * Copy constructor.
     * if \param i doesn't match format a conversion will happen.
     */
    ImageYUY2(const Image& i);

    
    /**
     * Conversion methods.
     * Returns a copy
     */
    virtual void convert(Image& dst) const throw(NoConversion);
    void toGRAY8(Image& dst) const throw(FormatMismatch);
    void toRGB888(Image& dst) const throw(FormatMismatch);
    
    /**
     * Factory method
     */
    static Image* createInstance(const int width, const int height, void *const data);
    static const FormatPtr FORMAT_YUY2;
  };
  typedef std::tr1::shared_ptr<ImageYUY2> ImageYUY2Ptr;


  class ImageGRAY8: public  Image {
  public:
    /**
     * Constructor
     */
    ImageGRAY8(const int width, const int height);
    
    /**
     * Constructor from user data
     */
    ImageGRAY8(const int width, const int height, void *const data);

    /**
     * Copy constructor.
     * if \param i doesn't match format a conversion will happen.
     */
    ImageGRAY8(const Image& i);

    
    /**
     * Conversion methods.
     * Returns a copy
     */
    virtual void convert(Image& dst) const throw(NoConversion);
    void toRGB888(Image& dst) const throw(FormatMismatch);
    void toYUY2(Image& dst) const throw(FormatMismatch);

    /**
     * Factory method
     */
    static Image* createInstance(const int width, const int height, void *const data);
    static const FormatPtr FORMAT_GRAY8;
  };
  typedef std::tr1::shared_ptr<ImageGRAY8> ImageGRAY8Ptr;

    /* /\** */
/*      * Creates a synthetic rgb88 image with an horizontal line */
/*      *\/ */
/*     static Image& createTestHline(const int width, */
/* 				  const int height, */
/* 				  const int lineWidth, */
/* 				  const int startRow, */
/* 				  const RGBColor *bgColor = 0, */
/* 				  const RGBColor *fgColor = 0); */
    
/*     /\** */
/*      * Creates a synthetic rgb888 image with an vertical line */
/*      *\/ */
/*     static Image& createTestVline(const int width, */
/* 				  const int height, */
/* 				  const int lineWidth, */
/* 				  const int startCol, */
/* 				  const RGBColor *bgColor = 0, */
/* 				  const RGBColor *fgColor = 0); */
    
/*     /\** */
/*      * Creates a synthetic rgb888 image with a square */
/*      * \sa Image_createTestSquare */
/*      *\/ */
/*     static Image& createTestSquare(const int width, */
/* 				   const int height, */
/* 				   const int sideLength, */
/* 				   const int xStartCorner, */
/* 				   const int yStartCorner, */
/* 				   const RGBColor *bgColor = 0, */
/* 				   const RGBColor *fgColor = 0); */
//  };
} //namespace

//declarations outside the namespace

/**
 * Insert a format in an output stream. Only debugging, output could be truncated
 */
std::ostream &operator<<(std::ostream &stream, const colorspaces::Image::Format& fmt);

/**
 * Insert an image in an output stream. Only debugging, output could be truncated
 */
std::ostream &operator<<(std::ostream &stream, const colorspaces::Image& img);

#endif //IMAGECV_COLORSPACES_H

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

#ifndef IMAGEPP_COLORSPACES_H
#define IMAGEPP_COLORSPACES_H

#include <iostream>
#include <colorspaces/image.h>
#include <colorspaces/imagetest.h>
#include <tr1/memory>

namespace colorspaces {
  
  /**
   * An image description
   */
  class ImageDescriptionpp: public ImageDescription{
  public:
    ImageDescriptionpp(const ImageDescription& i);
    ImageDescriptionpp(const int width,
		       const int height,
		       const int size,
		       const Format *fmt);
    ~ImageDescriptionpp();
  };

  class Imagepp; /*forward declaration*/
  typedef std::tr1::shared_ptr<Imagepp> ImageppPtr;


  /**
   * An image
   */
  class Imagepp : public Image {
  public:
    /**
     * Image constructor from params
     */
    Imagepp(const ImageDescription &desc, char *const data = 0);

    /**
     * Image copy constructor from
     * \param i reference to Image
     */
    Imagepp(Image& i);

    /**
     * Image destructor
     */
    ~Imagepp();

    /**
     * Convert image to luminance 8bit (greyscale)
     * \sa Image_toL8
     */
    ImageppPtr toL8() const;

    /**
     * Convert image to RGB888
     * \sa Image_toRGB888
     */
    ImageppPtr toRGB888() const;

    /**
     * Convert image to YUY2/YUYV
     * \sa Image_toYUY2
     */
    ImageppPtr toYUY2() const;

    /**
     * Convert image to outFmt
     * \sa Image_convert
     */
    ImageppPtr convert(const Format *outFmt) const;

    /**
     * Creates a synthetic rgb88 image with an horizontal line
     * \sa Image_createTestHline
     */
    static ImageppPtr createTestHline(const int width,
				      const int height,
				      const int lineWidth,
				      const int startRow,
				      const RGBColor *bgColor = 0,
				      const RGBColor *fgColor = 0);
    
    /**
     * Creates a synthetic rgb888 image with an vertical line
     * \sa Image_createTestVline
     */
    static ImageppPtr createTestVline(const int width,
				      const int height,
				      const int lineWidth,
				      const int startCol,
				      const RGBColor *bgColor = 0,
				      const RGBColor *fgColor = 0);

    /**
     * Creates a synthetic rgb888 image with a square
     * \sa Image_createTestSquare
     */
    static ImageppPtr createTestSquare(const int width,
				       const int height,
				       const int sideLength,
				       const int xStartCorner,
				       const int yStartCorner,
				       const RGBColor *bgColor = 0,
				       const RGBColor *fgColor = 0);
  };
} //namespace

//declarations outside the namespace

/**
 * Insert an image description in an output stream
 */
std::ostream &operator<<(std::ostream &stream, ImageDescription& desc);

/**
 * Insert an image in an output stream. Only debugging, output could be truncated
 */
std::ostream &operator<<(std::ostream &stream, Image& img);

#endif //IMAGEPP_COLORSPACES_H

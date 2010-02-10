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

#ifndef IMAGE_COLORSPACES_H
#define IMAGE_COLORSPACES_H

#include <stdio.h>
#include <colorspaces/formats.h>
/**
 * An image description
 */
struct ImageDescription {
  int width;/**< Image width*/
  int height;/**< Image height*/
  int size;/**< Image size in bytes*/
  const Format *fmt;/**< Image fmt*/
};
typedef struct ImageDescription ImageDescription;

/**
 * An image
 */
struct Image {
  ImageDescription description;/**< The image description*/
  char *imageData;/**< The image data pixels*/
  int imageDataOwned;/**< If 1 data is owned so it will be destroyed on destructor*/
};
typedef struct Image Image;

#ifdef __cplusplus
extern "C" {
#endif
  /**
   * Creates a new image description from params
   * \return A new allocated ImageDescription
   */
  ImageDescription* new_ImageDescription(const int width,
					 const int height,
					 const int size,
					 const Format *fmt);
  /**
   * Initializes the image description with params.
   * \param self must be a valid ImageDescription pointer.
   */
  void ImageDescription_initialize(ImageDescription* self,
				   const int width,
				   const int height,
				   const int size,
				   const Format *fmt);
  /**
   * Deinitialize the image description with params
   * \param self must be a valid ImageDescription pointer.
   */
  void ImageDescription_deinitialize(ImageDescription* self);
  
  /**
   * Destroy an image description if self /= 0
   */
  void delete_ImageDescription(ImageDescription * const self);
  
  /**
   * Print image description to string
   */
  int ImageDescription_snprintf(char *str, size_t size, const ImageDescription * const self);

  /**
   * Print image description to file stream
   */
  int ImageDescription_fprintf(FILE *stream, const ImageDescription * const self);

  /**
   * Print image description to stdout
   */
  int ImageDescription_printf(const ImageDescription * const self);


  /**
   * Creates a new Image from params
   * \param desc the image description
   * \param data allows user to define image data from a pointer. 
   * In this case data is not managed, so no copy or deallocation will happen.
   * If 0 data is allocated and destroyed when needed.
   * \return a new allocated Image
   */
  Image* new_Image(const ImageDescription* desc,
		   char * const data);

  /**
   * Initialize an Image structure with params
   * \param self must be a valid Image pointer
   */
  void Image_initialize(Image* const self,
			const ImageDescription* desc,
			char * const data);
  /**
   * Deinitialize
   * \param self must be a valid Image pointer
   */
  void Image_deinitialize(Image* const self);

  /**
   * Copy image
   * returns a copy of \param self
   */
  Image* Image_copy(Image* const self);


  /**
   * Swap data ownership if oldOwner owns data
   */
  void Image_swapDataOwner(Image* const oldOwner, Image* const newOwner);

  void Image_ownData(Image* const self);
  void Image_disownData(Image* const self);

  /**
   * Destroy an Image structure if self /=0
   */
  void delete_Image(Image * const self);

  /**
   * Convert image to L8 format
   * \param self image to be converted
   * \return new allocated image with conversion or 0 if image couldn't be converted
   * if image is already in the requested conversion format it returns a copy
   */
  Image* Image_toL8(const Image *const self);

  /**
   * Convert image to YUY2/YUYV
   * \param self image to be converted
   * \return new allocated image with conversion or 0 if image couldn't be converted
   * if image is already in the requested conversion format it returns a copy
   */
  Image* Image_toYUY2(const Image *const self);

  /**
   * Convert image to RGB888
   * \param self image to be converted
   * \return new allocated image with conversion or 0 if image couldn't be converted
   * if image is already in the requested conversion format it returns a copy
   */
  Image* Image_toRGB888(const Image *const self);

  /**
   * Convert image
   * \param self image to be converted
   * \param outFmt requested conversion format
   * \return new allocated image with conversion or 0 if image couldn't be converted
   * if image is already in the requested conversion format it returns a copy
   */
  Image* Image_convert(const Image *const self, const Format *outFmt);

  /**
   * Print image to string
   */
  int Image_snprintf(char *str, size_t size, const Image * const self);

  /**
   * Print image to file stream
   */
  int Image_fprintf(FILE *stream, const Image * const self);

  /**
   * Print image to stdout
   */
  int Image_printf(const Image * const self);
  
#ifdef __cplusplus
}//extern
#endif

#endif //IMAGE_COLORSPACES_H

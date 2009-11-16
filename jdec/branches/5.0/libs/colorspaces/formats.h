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

#ifndef _FORMATS_H
#define _FORMATS_SPACES_H
#include <stdlib.h>

enum PixelFormat {
  PIXEL_FORMAT_UNKNOWN = 0,
  PIXEL_FORMAT_NONE = 0,
  /*RGB*/
  PIXEL_FORMAT_RGBA_8888 = 1,/*32-bit ARGB*/
  PIXEL_FORMAT_RGBX_8888 = 2, /*24-bit RGB packed in 32-bit chuncks*/
  PIXEL_FORMAT_RGB_888 = 3, /*24-bit RGB*/
  PIXEL_FORMAT_RGB_565 = 4, /*16-bit RGB*/
  PIXEL_FORMAT_RGB_332 = 5, /*8-bit RGB*/
  
  /*Luminance (grayscale)*/
  PIXEL_FORMAT_Y_8 = 6,/*8-bit Luma, grayscale*/

  /*YUV*/
  PIXEL_FORMAT_YUV2 = 7,/*16-bit YUV 4:2:2 YUYV*/
  PIXEL_FORMAT_YUYV = PIXEL_FORMAT_YUV2
  
  /*Others*/
};

enum FormatComponents {
  RGB,
  RGBA,
  LUMINANCE,
  YUV
};

enum FormatComponentIndex {
  INDEX_ALPHA = 0,
  INDEX_RED = 1,
  INDEX_GREEN = 2,
  INDEX_BLUE = 3,
  INDEX_Y = 0,
  INDEX_U = 1,
  INDEX_V = 2
};

/*FIXME: keep bytes index*/
typedef struct Format{
  unsigned char size;/*bytes*/
  unsigned char bitsPerPixel;
  unsigned int componetsMask[4];
  enum FormatComponents components;
} Format;

typedef struct FormatEntry{
  char *format_name;
  Format format;
} FormatEntry;

#ifdef __cplusplus
extern "C" {
#endif

const FormatEntry* getPixelFormatTable(size_t *nEntries);
const Format* searchPixelFormat(const char *format_name);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*_FORMATS_H*/

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

#ifndef FORMATS_COLORSPACES_H
#define FORMATS_COLORSPACES_H

#include <stdio.h>

/**
 * Enumeration of formats
 */
enum FormatEnum {
  FORMAT_UNKNOWN = 0, /**< Uknown format*/
  FORMAT_NONE = 0, /**< None or void format*/
  /*RGB*/
  FORMAT_RGB_888 = 1, /**< 24-bit RGB*/
  
  /*YUV*/
  FORMAT_YUY2 = 2,/**< 16-bit YUV 4:2:2 YUYV*/
  FORMAT_YUYV = FORMAT_YUY2,

  /*Luminance*/
  FORMAT_L_8 = 3 /**< 8-bit luminance*/
  
  /*Others*/
};


/**
 * Defines a format*/
struct Format{
  char *format_name;/**< String that represents this format*/
  enum FormatEnum format;/**< Format identification*/
  unsigned char pixelSize;/**< Size of each pixel in bytes*/
  unsigned char bitsPerPixel;/**< Bits used for each pixel*/
};
typedef struct Format Format;

#ifdef __cplusplus
extern "C" {
#endif
  /**
   * Get the format table that contains all the known formats
   * \param nEntries if /=0 returns with the number of entries in the table
   * \return format table
   */
  const Format* getFormatTable(int *nEntries);

  /**
   * Search a format in the table given a format name
   * \return the format entry if found, 0 otherwise.
   */
  const Format* searchFormat(const char *format_name);

  /**
   * Print format to string
   */
  int Format_snprintf(char *str, size_t size, const Format * const self);

  /**
   * Print format to file stream
   */
  int Format_fprintf(FILE *stream, const Format * const self);

  /**
   * Print format to stdout
   */
  int Format_printf(const Format * const self);
  
#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*FORMATS_COLORSPACES_H*/

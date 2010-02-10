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

#ifndef COLOR_COLORSPACES_H
#define COLOR_COLORSPACES_H

struct RGBColor{
  unsigned char r,g,b;
};
typedef struct RGBColor RGBColor;

#ifdef __cplusplus
extern "C" {
#endif

  RGBColor* new_RGBColor(const unsigned char a,
			 const unsigned char r,
			 const unsigned char g,
			 const unsigned char b);

  void delete_RGBColor(RGBColor * const self);

  const RGBColor* RGBColor_get_predefined(const char * name);

#ifdef __cplusplus
}//extern
#endif

#endif //COLOR_COLORSPACES_H

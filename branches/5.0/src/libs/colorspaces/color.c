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

#include "color.h"
#include <stdlib.h>
#include <string.h>


//predefined colors
static const char * const predefinedColors[] = {
  "white",
  "red",
  "green",
  "blue",
  "magenta",
  "cyan",
  "yellow",
  "black"
};


static const RGBColor const predefinedRGBColors[] = {
  {255,255,255}, /*white*/
  {255,0,0}, /*red*/
  {0,255,0}, /*green*/
  {0,0,255}, /*blue*/
  {255,0,255}, /*magenta*/
  {0,255,255}, /*cyan*/
  {255,255,0}, /*yellow*/
  {0,0,0} /*black*/
};

RGBColor* new_RGBColor(const unsigned char a,
		       const unsigned char r,
		       const unsigned char g,
		       const unsigned char b){
  return (RGBColor*)calloc(1,sizeof(RGBColor));
}

void delete_RGBColor(RGBColor * const self){
  free(self);
}

const RGBColor* RGBColor_get_predefined(const char * name){
  int i,nColors = sizeof(predefinedColors)/sizeof(predefinedColors[0]);

  for (i=0; i<nColors; i++){
    if (strncmp(predefinedColors[i],name,255) == 0)
      return &predefinedRGBColors[i];
  }
  return 0;
}

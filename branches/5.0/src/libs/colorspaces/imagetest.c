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

#include "imagetest.h"
#include "formats.h"

Image* Image_createTestHline(const int width,
			     const int height,
			     const int lineWidth,
			     const int startRow,
			     const RGBColor *bgColor,
			     const RGBColor *fgColor){
  int i,j,k;
  int endRow = startRow+lineWidth;
  const Format *rgbFmt = &getFormatTable(0)[FORMAT_RGB_888];
  ImageDescription *rgbImageDesc = new_ImageDescription(width,
							height,
							width*height*3,
							rgbFmt);
  Image *rgbImage = new_Image(rgbImageDesc,0);

  delete_ImageDescription(rgbImageDesc);/*not used anymore after create rgbImage*/
  if (bgColor==0)
    bgColor = RGBColor_get_predefined("black");
  if (fgColor==0)
    fgColor = RGBColor_get_predefined("white");

  for (i=0,k=0; i<height; i++){
    for (j=0; j<width; j++,k+=3){
      if ((i>=startRow) && (i<endRow)){/*pixel inside line*/
	rgbImage->imageData[k] = fgColor->r;
	rgbImage->imageData[k+1] = fgColor->g;
	rgbImage->imageData[k+2] = fgColor->b;
      }else{
	rgbImage->imageData[k] = bgColor->r;
	rgbImage->imageData[k+1] = bgColor->g;
	rgbImage->imageData[k+2] = bgColor->b;
      }
    }
  }
  return rgbImage;
}
  

Image* Image_createTestVline(const int width,
			     const int height,
			     const int lineWidth,
			     const int startCol,
			     const RGBColor *bgColor,
			     const RGBColor *fgColor){
  int i,j,k;
  int endCol = startCol+lineWidth;
  const Format *rgbFmt = &getFormatTable(0)[FORMAT_RGB_888];
  ImageDescription *rgbImageDesc = new_ImageDescription(width,
							height,
							width*height*3,
							rgbFmt);
  Image *rgbImage = new_Image(rgbImageDesc,0);
  
  delete_ImageDescription(rgbImageDesc);/*not used anymore after create rgbImage*/
  if (bgColor==0)
    bgColor = RGBColor_get_predefined("black");
  if (fgColor==0)
    fgColor = RGBColor_get_predefined("white");

  for (i=0,k=0; i<height; i++){
    for (j=0; j<width; j++,k+=3){
      if ((j>=startCol) && (j<endCol)){/*pixel inside column*/
	rgbImage->imageData[k] = fgColor->r;
	rgbImage->imageData[k+1] = fgColor->g;
	rgbImage->imageData[k+2] = fgColor->b;
      }else{
	rgbImage->imageData[k] = bgColor->r;
	rgbImage->imageData[k+1] = bgColor->g;
	rgbImage->imageData[k+2] = bgColor->b;
      }
    }
  }
  return rgbImage;
}


Image* Image_createTestSquare(const int width,
			      const int height,
			      const int sideLength,
			      const int xStartCorner,
			      const int yStartCorner,
			      const RGBColor *bgColor,
			      const RGBColor *fgColor){
  int i,j,k;
  int xRightTopCorner = xStartCorner+sideLength;
  int yLeftBottomCorner = yStartCorner+sideLength;
  const Format *rgbFmt = &getFormatTable(0)[FORMAT_RGB_888];
  ImageDescription *rgbImageDesc = new_ImageDescription(width,
							height,
							width*height*3,
							rgbFmt);
  Image *rgbImage = new_Image(rgbImageDesc,0);
  
  delete_ImageDescription(rgbImageDesc);/*not used anymore after create rgbImage*/
  if (bgColor==0)
    bgColor = RGBColor_get_predefined("black");
  if (fgColor==0)
    fgColor = RGBColor_get_predefined("white");

  for (i=0,k=0; i<height; i++){
    for (j=0; j<width; j++,k+=3){
      if ((i>=yStartCorner) && (i<yLeftBottomCorner) &&
	  (j>=xStartCorner) && (j<xRightTopCorner)) {/*pixel inside square*/
	rgbImage->imageData[k] = fgColor->r;
	rgbImage->imageData[k+1] = fgColor->g;
	rgbImage->imageData[k+2] = fgColor->b;
      }else{
	rgbImage->imageData[k] = bgColor->r;
	rgbImage->imageData[k+1] = bgColor->g;
	rgbImage->imageData[k+2] = bgColor->b;
      }
    }
  }
  return rgbImage;
}

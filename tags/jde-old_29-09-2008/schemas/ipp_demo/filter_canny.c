/*
 *  Copyright (C) 2006 José María Cañas Plaza 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : José Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <ipp.h>

void filter_canny(char* input , char* output, int width, int height, float low, float high)
{
   IppStatus sts;
   int size, size1, srcStep=width*sizeof(Ipp8u), dxStep=width*sizeof(Ipp16s), dyStep=width*sizeof(Ipp16s), dstStep=width*sizeof(Ipp8u);
   IppiSize roi;
   Ipp8u *src, *dst, *buffer;
   Ipp16s *dx, *dy;

   roi.width=width;
   roi.height=height;

   src=ippsMalloc_8u(width*height);
   dst=ippsMalloc_8u(width*height);
   dx=ippsMalloc_16s(width*height);
   dy=ippsMalloc_16s(width*height);

   ippiRGBToGray_8u_C3C1R ((Ipp8u *)input, width*3, src, width, roi);
   
   sts = ippiFilterSobelNegVertGetBufferSize_8u16s_C1R(roi, ippMskSize3x3, &size);
   if (sts!=ippStsNoErr){
      fprintf (stderr,"Error en ipp\n");
      exit(-1);
   }
   sts = ippiFilterSobelHorizGetBufferSize_8u16s_C1R(roi, ippMskSize3x3, &size1);
   if (sts!=ippStsNoErr){
      fprintf (stderr,"Error en ipp\n");
      exit(-1);
   }
   if (size<size1)
      size=size1;
   ippiCannyGetSize(roi, &size1);
   if (size<size1)
      size=size1;
   buffer = ippsMalloc_8u(size);
   sts = ippiFilterSobelNegVertBorder_8u16s_C1R (src, srcStep, dx, dxStep, roi,
         ippMskSize3x3, ippBorderRepl, 0, buffer);
   if (sts!=ippStsNoErr){
      fprintf (stderr,"Error en ipp\n");
      exit(-1);
   }
   sts = ippiFilterSobelHorizBorder_8u16s_C1R(src, srcStep, dy, dyStep, roi,
                                              ippMskSize3x3, ippBorderRepl, 0, buffer);
   if (sts!=ippStsNoErr){
      fprintf (stderr,"Error en ipp\n");
      exit(-1);
   }
   sts = ippiCanny_16s8u_C1R(dx, dxStep, dy, dyStep, dst, dstStep, roi, low,
                             high, buffer);
   if (sts!=ippStsNoErr){
      fprintf (stderr,"Error en ipp\n");
      exit(-1);
   }

   ippiCopy_8u_C1C3R(dst, width, (Ipp8u*)output, width*3, roi);
   ippiCopy_8u_C1C3R(dst, width, (Ipp8u*)output+sizeof(Ipp8u), width*3, roi);
   ippiCopy_8u_C1C3R(dst, width, (Ipp8u*)output+sizeof(Ipp8u)*2, width*3, roi);

   ippsFree(buffer);
   ippsFree(dx);
   ippsFree(dy);
   ippsFree(src);
   ippsFree(dst);

}

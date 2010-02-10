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
#include <ipp.h>

void filter_conv(char* input , char* output, int width, int height, int* conv,
                  int conv_width, int conv_height, int divisor)
{
   IppiSize imgsize;
   IppiSize ksize;
   IppiPoint anchor;
   Ipp8u *input_bw;
   Ipp8u *output_bw;

   imgsize.width=width;
   imgsize.height=height;
   ksize.width=conv_width;
   ksize.height=conv_height;

   anchor.x=(int)(ksize.width/2);
   anchor.y=(int)(ksize.height/2);

   //Use grayscale images
   input_bw=malloc(width*height*sizeof(Ipp8u));
   output_bw=malloc(width*height*sizeof(Ipp8u));

   ippiRGBToGray_8u_C3C1R ((Ipp8u *)input, width*3, input_bw, width, imgsize);
   
   ippiFilter_8u_C3R((Ipp8u*) input_bw, imgsize.width, (Ipp8u *) output_bw,
                      imgsize.width, imgsize, (Ipp32s*) conv, ksize, anchor,
                      divisor);

//    ippiCopy_8u_C1R(input_bw, width, output_bw, width, imgsize);

   ippiCopy_8u_C1C3R(output_bw, width, (Ipp8u*)output, width*3, imgsize);
   ippiCopy_8u_C1C3R(output_bw, width, (Ipp8u*)output+sizeof(Ipp8u), width*3, imgsize);
   ippiCopy_8u_C1C3R(output_bw, width, (Ipp8u*)output+sizeof(Ipp8u)*2, width*3, imgsize);

   free(input_bw);
   free(output_bw);
}

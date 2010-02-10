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

void filter_corner(char* input , char* output, int width, int height, float low, float high)
{
   int i;
   IppiSize imgsize;
   Ipp8u *input_bw;

   Ipp32f *eig_val=NULL;
   static Ipp8u* eig_buffer=NULL;
   int eig_val_s;
   int eigen_buff_tam;

   imgsize.width=width;
   imgsize.height=height;


   //Use grayscale images
   input_bw=malloc(width*height*sizeof(Ipp8u));

   ippiRGBToGray_8u_C3C1R ((Ipp8u *)input, width*3, input_bw, width, imgsize);

   if (ippiMinEigenValGetBufferSize_8u32f_C1R(imgsize, 3, 3, &eigen_buff_tam)!=
       ippStsNoErr)
   {
      fprintf (stderr, "Error calculating size of temp space for calculate eigen values\n");
      exit(-1);
   }

   eig_buffer=(Ipp8u*)malloc(sizeof (Ipp8u)*eigen_buff_tam);
   eig_val=ippiMalloc_32f_C1 (imgsize.width, imgsize.height, &eig_val_s);
   
   ippiMinEigenVal_8u32f_C1R((Ipp8u*)input_bw, width,
                              eig_val, width*sizeof(Ipp32f),
                                    imgsize, ippKernelScharr, 3, 3, eig_buffer);

   ippiCopy_8u_C3R((Ipp8u *)input, width*3, (Ipp8u *)output, width*3, imgsize);

   //Draw high spacial derivate points
   for (i=0; i<width*height; i++){
      Ipp32f value= eig_val[i];
      if (value>low && value<high){
         output[i*3]=0;
         output[i*3+1]=0;
         output[i*3+2]=255;
      }
   }


   ippiFree(eig_val);
   free(eig_buffer);
   free(input_bw);
}

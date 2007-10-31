/*
 *  Copyright (C) 2007 Roberto Calvo Palomino
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
 *  Authors : Roberto Calvo Palomino <rocapal@gsyc.es> , Jose Maria Cañas Plaza <jmplaza@gsyc.es>
 */

#define NAME     "libRGB2HSI"
#define VERSION  "0.1"

struct HSI
{
  double H;
  double S;
  double I;
};

struct HSI * RGB2HSI[64][64][64];

/// \brief Init the libRGB2HSI
void libRGB2HSI_init();

/// \brief Create a translate RGB2HSI table with resolution of 6bits (64x64x64)
void libRGB2HSI_createTable();

/// \brief Free de memory of RGB2HSI
void libRGB2HSI_destroyTable();

/// \brief Print the struct HSI 
void libRGB2HSI_printHSI (struct HSI*);

/// \brief Test
void libRGB2HSI_test();

/// \brief Return the translate RGB to HSI
static inline struct HSI* libRGB2HSI_getHSI (int R, int G, int B)  { return RGB2HSI[R>>2][G>>2][B>>2]; };



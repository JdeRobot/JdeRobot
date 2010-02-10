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
 *  Authors : Roberto Calvo Palomino <rocapal@gsyc.es> ,
 *  	      Jose Maria Caas Plaza <jmplaza@gsyc.es>
 */

#ifndef _COLOR_SPACES_H
#define _COLOR_SPACES_H

#define NAME     "colorspaces"
#define VERSION  "1.3.0"

/// *** RGB to HSI  *** ///

struct HSV
{
	double H;
	double S;
	double V;
};

extern struct HSV * LUT_RGB2HSV [64][64][64];

extern int isInitTableHSV;

extern pthread_mutex_t mutex;
 
/// \brief Init the RGB2HSV
void RGB2HSV_init();

/// \brief Create a translate RGB2HSV table with resolution of 6bits (64x64x64)
void RGB2HSV_createTable();

/// \brief Free de memory of RGB2HSV
void RGB2HSV_destroyTable();

/// \brief Print the struct HSV
void RGB2HSV_printHSI (struct HSV*);

/// \brief Test
void RGB2HSV_test();

/// \brief Returns the translation from RGB to HSV
static inline const struct HSV* RGB2HSV_getHSV (int R, int G, int B)  { return LUT_RGB2HSV[R>>2][G>>2][B>>2]; };

/// \brief Returns the translation from HSV to RGB
void hsv2rgb(double H, double S, double V, double *r, double *g, double *b);

#endif

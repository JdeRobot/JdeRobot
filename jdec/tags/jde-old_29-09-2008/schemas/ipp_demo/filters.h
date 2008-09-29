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

extern void filter_pass(char* input , char* output, int width, int height);
extern void filter_opflow(char* input , char* output, int width, int height,
                           int show_interesting, int show_flow);
extern void filter_conv(char* input , char* output, int width, int height,
                        int* conv_matrix, int conv_width, int conv_height,
                        int divisor);
extern void filter_canny(char* input , char* output, int width, int height,
                          float low, float high);
extern void filter_corner(char* input , char* output, int width, int height,
                         float low, float high);


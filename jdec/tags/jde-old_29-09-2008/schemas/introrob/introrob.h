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
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 */
#include "pioneer.h"

extern void introrob_startup(char *configfile);
extern void introrob_stop();
extern void introrob_suspend();
extern void introrob_resume(int father, int *brothers, arbitration fn);
extern void introrob_guiresume();
extern void introrob_guisuspend();

#define MOUSELEFT 1
#define MOUSEMIDDLE 2
#define MOUSERIGHT 3
#define MOUSEWHEELUP 4
#define MOUSEWHEELDOWN 5

#define PI 3.14159265

extern float introrob_mouse_x, introrob_mouse_y;
/* absolute position of the point clicked with the central mouse button */

int absolutas2relativas(Tvoxel in, Tvoxel *out);
int relativas2absolutas(Tvoxel in, Tvoxel *out);
int pintaSegmento(Tvoxel a, Tvoxel b, int color);
 
float v;
float w;
float robot[5];
float laser[NUM_LASER];

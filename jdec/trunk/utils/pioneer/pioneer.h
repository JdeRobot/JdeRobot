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


#ifndef tvoxel
#define tvoxel
typedef struct voxel{
  float x;
  float y;}Tvoxel;
#endif

extern void us2xy(int numsensor, float d,float phi, Tvoxel *point, float *jderobot);
extern void laser2xy(int reading, float d, Tvoxel *point, float *jderobot);
extern void init_pioneer(void);

/***************** Robot Configuration ***************/
#define NUM_LASER 180
#define NUM_SONARS 16
#define NUM_BUMPERS 10
#define MAX_VEL 1000 /* mm/sec, hardware limit: 1800 */
#define MAX_RVEL 180 /* deg/sec, hardware limit: 360 */
/* SIF image size */
#define SIFNTSC_ROWS 240
#define SIFNTSC_COLUMNS 320
/* directed perception pantilt limits */
#define MAX_PAN_ANGLE 159.13 /* degrees */
#define MIN_PAN_ANGLE -159.13 /* degrees */
#define MAX_TILT_ANGLE 30. /* degrees */
#define MIN_TILT_ANGLE -46. /* degrees */
#define MAX_SPEED_PANTILT 205.89

/** Conversion from deg to rad*/
#define DEGTORAD     (3.14159264 / 180.0)

extern float laser_coord[5]; /* laser sensor position */
extern float us_coord[NUM_SONARS][5];/* us sensor positions */
extern float camera_coord[5]; /* camera position */


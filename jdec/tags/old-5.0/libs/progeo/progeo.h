/*
 *
 *  Copyright (C) 1997-2008 JDE Developers Team
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
 *  Authors : Jose María Cañas Plaza <jmplaza@gsyc.es>
 *
 */

#ifndef progeo_types
#define progeo_types

/* geometric distances */
#define DIST2D(p1,p2) sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y))
#define DIST3D(p1,p2) sqrt((p1.X-p2.X)*(p1.X-p2.X)+(p1.Y-p2.Y)*(p1.Y-p2.Y)+(p1.Z-p2.Z)*(p1.Z-p2.Z))

typedef struct {
  float x;
  float y;
  float h;
} HPoint2D;

typedef struct {
  float X;
  float Y;
  float Z;
  float H;
} HPoint3D;

#define ISIGHT_PINHOLE_FDIST 405.4
#define ISIGHT_PINHOLE_U0 142.6
#define ISIGHT_PINHOLE_V0 150.4

typedef struct {
  HPoint3D position; /* camera 3d position in mm */
  HPoint3D foa; /* camera 3d focus of attention in mm */
  float roll; /* camera roll position angle in rads */
  float fdistx; /* focus x distance in mm*/
  float fdisty; /* focus y distance in mm*/
  float u0,v0; /* pixels */
  float skew; /*angle between the x and y pixel axes in rads*/
  int rows; /* image height in pixels */
  int columns; /* image width in pixels */
  /* camera K matrix */
  float k11,k12,k13,k14,k21,k22,k23,k24,k31,k32,k33,k34;
  /* camera rotation + translation matrix */
  float rt11,rt12,rt13,rt14,rt21,rt22,rt23,rt24,rt31,rt32,rt33,rt34,rt41,rt42,rt43,rt44;
  /* distortion parameters */
  float d1,d2,d3,d4,d5,d6;
  float dx,dy;
  /* top right and bottom left points */
  HPoint3D tr, bl;
  /* name */
  char name[256];
}TPinHoleCamera;

typedef struct {
	HPoint3D position; /* stereocamera 3d position in mm */
	HPoint3D foa; /* stereocamera 3d focus of attention in mm */
	float roll; /* stereocamera roll position angle in rads */
	/* single cameras */
	TPinHoleCamera camera1;
	TPinHoleCamera camera2;
	/* baseline */
	float baseline;
	/* name */
	char name[256];	
} TPinHoleStereocamera;
#endif

extern void update_camera_matrix(TPinHoleCamera *camera);
extern void update_stereocamera_matrix(TPinHoleStereocamera *stereo);
extern int project(HPoint3D in, HPoint2D *out, TPinHoleCamera camera);
extern int backproject(HPoint3D *out, HPoint2D in, TPinHoleCamera camera);
extern int displayline(HPoint2D p1, HPoint2D p2, HPoint2D *a, HPoint2D *b, TPinHoleCamera camera);
extern void display_camerainfo(TPinHoleCamera camera);

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
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.es>, Antonio Pineda <apineda@gsyc.es>
	      Sara Marugán Alonso <smarugan@gsyc.es>
 *
 */

/* image size */
#define SIFNTSC_COLUMNS 320
#define SIFNTSC_ROWS 240
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
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.es>, Antonio Pineda <apineda@gsyc.es>
	      Sara Marugán Alonso <smarugan@gsyc.es>
 *
 */

/* PI definition */
#define PI 3.141592654

/** Conversion from deg to rad*/
#define DEGTORAD     (3.14159264 / 180.0)

/* button states */
#define PUSHED 1
#define RELEASED 0

/*grid*/
#define SLOTS 3
#define DELTA 333 /* mm */

/*limits*/
#define MAX_Z 30000. /* mm */
#define FDIST_MAX 750.
#define FDIST_MIN 1.
#define MAX_LINES_IN_ROOM 500

#define ROOM_MAX_X 79250.
#define ROOM_MIN_X -79250.
#define ROOM_MAX_Y 47890.
#define ROOM_MIN_Y -47890.
#define ROOM_MAX_Z 30000.
#define ROOM_MIN_Z -30000.

extern void libcalibrator_init(const char *worldfile,const char *camIn,const char *camOut);
extern void libcalibrator_initgui();
extern void libcalibrator_guibuttons();
extern void libcalibrator_guidisplay(unsigned char *image);
extern void libcalibrator_terminate();




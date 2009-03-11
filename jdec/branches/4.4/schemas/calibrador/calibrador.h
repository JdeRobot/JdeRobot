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

extern void calibrador_init(char *configfile);
extern void calibrador_terminate();

extern void calibrador_stop();
extern void calibrador_run(int father, int *brothers, arbitration fn);

extern void calibrador_show();
extern void calibrador_hide();

extern int calibrador_id; /* schema identifier */
extern int calibrador_cycle; /* ms */

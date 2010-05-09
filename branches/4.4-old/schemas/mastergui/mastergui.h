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
 *            José Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

extern void mastergui_init(char *configfile);
extern void mastergui_terminate();

extern void mastergui_stop();
extern void mastergui_run(int father, int *brothers, arbitration fn);

extern void mastergui_show();
extern void mastergui_hide();

extern int mastergui_id; /* schema identifier */
extern int mastergui_cycle; /* ms */

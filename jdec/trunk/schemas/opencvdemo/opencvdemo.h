/*
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
 *  Authors : Eduardo Perdices García <edupergar@gmail.com>
 */


extern void opencvdemo_init(char *configfile);
extern void opencvdemo_terminate();

extern void opencvdemo_stop();
extern void opencvdemo_run(int father, int *brothers, arbitration fn);

extern void opencvdemo_show();
extern void opencvdemo_hide();

extern int opencvdemo_id; /* schema identifier */
extern int opencvdemo_cycle; /* ms */

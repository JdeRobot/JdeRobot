/*
 *  Copyright (C) 1997-2009 JDE Developers Team
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
 *  Authors : Francisco Miguel Rivas Montero <fm.rivas@alumnos.urjc.es>
 */


extern void naooperator_init(char *configfile);
extern void naooperator_stop();
extern void naooperator_terminate();
extern void naooperator_run(int father, int *brothers, arbitration fn);
extern void naooperator_show();
extern void naooperator_hide();
 
extern int naooperator_id; /* schema identifier */
extern int naooperator_cycle; /* ms */



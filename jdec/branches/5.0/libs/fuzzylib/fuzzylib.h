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

/* No tiene limite en el numero de reglas, ni en el numero de etiquetas por variable */

#ifndef FUZZYLIB_H
#define FUZZYLIB_H

#define MAX_LINE_LENGTH 255
#define MAX_NOMBRE 	25
#define PARAM_PER_ETIQ	 4

int fc_open(char *filename);
void fc_close(int controlador);
void fc_save(int controlador, char *filename);
int fc_output(int controlador, char *varname, float *output);
int fc_link(int controlador, char *varname, float *varpointer);


#endif 











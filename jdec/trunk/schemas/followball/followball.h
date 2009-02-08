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
 *            Roberto Calvo Palomino <rocapal@gsyc.escet.urjc.es>
 *            Jose Antonio Santos Cadena <santoscadenas@gmail.com>
 *
 */

extern void followball_init(char *configfile);
extern void followball_terminate();

extern void followball_stop();
extern void followball_run(int father, int *brothers, arbitration fn);

extern void followball_show();
extern void followball_hide();

extern int followball_id; /* schema identifier */
extern int followball_cycle; /* ms */


struct dfilter {
  int x;                /* Coordenada X del centro de masas */
  int y;                /* Coordenada Y del centro de masas */
  int pixeles;          /* Numero de pixeles que pasan el filtro */
  int cuadrante;        /* Cuadrante en el que se encuentra el centro de masas */
  int distancia;        /* Distancia del centro de la imagen al centro de masas */
  int lineas;           /* Numero de lineas horizontales en las que al menos
                           un pixel pasa el filtro */
};

enum movement_pantilt {up,down,left,right};


/* Filtro Pelota ROJA 

H_MIN 5.52
H_MAX 6.28

S_MIN 0.33
S_MAX 0.52

*/

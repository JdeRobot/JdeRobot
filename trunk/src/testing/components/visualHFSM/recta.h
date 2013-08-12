#ifndef RECTA_H
#define RECTA_H

#include "tipos.h"
#include <math.h>


punto crear_punto (double x, double y);
punto punto_medio (punto p1, punto p2);
void punto_get_values (punto p, double *x, double *y);

/*Ax + By + C = 0
A=y2-y1 B=x2-x1  C=((y2-y1)*x1)+((x2-x1)*y1) */
recta crear_recta (punto p1, punto p2);

/* Recta perpendicular a otra y que pase por un punto
 A'=-B B=A C=-((A*x)+(B*y));*/
recta recta_perpendicular (recta r1, punto p1);

/* Recta paralela a otra a una distancia 
 d(p,R)=|Ax+By+C|/((A^2)+(B^2))^1/2 // A'=A B'=B*/
recta recta_paralela (recta r1, punto p1, double distancia);

punto interseccion_rectas (recta r1, recta r2);


#endif

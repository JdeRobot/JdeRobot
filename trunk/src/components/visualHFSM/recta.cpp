#include <stdlib.h>
#include "recta.h"
#include <math.h>

punto 
crear_punto (double x, double y)
{
	punto p;
	p.x = x;
	p.y = y;

	return p;
}

punto punto_medio (punto p1, punto p2)
{
	punto p;
	p.x = (p1.x + p2.x)/2;
	p.y = (p1.y + p2.y)/2;

	return p;
}

void 
punto_get_values (punto p, double *x, double *y)
{
	
	*x = p.x;
	*y = p.y;

}

recta crear_recta (punto p1, punto p2)
{	
	recta r; 

	r.A = p2.y - p1.y;
	r.B = -(p2.x - p1.x);
	r.C = -(r.A * p1.x) - (r.B * p1.y);

	return r;
}
recta recta_perpendicular (recta r1, punto p1)
{
	recta r;
	
	r.A = -r1.B;
	r.B = r1.A;
	r.C = -((r.A * p1.x)+(r.B * p1.y));

	return r;
	
}
recta recta_paralela (recta r1, punto p, double distancia)
{	
	recta r;

	r.A = r1.A;
	r.B = r1.B;
	r.C = (distancia*(sqrt((r1.A*r1.A)+(r1.B*r1.B))))-((r1.A*p.x)+ (r1.B * p.y));
	if (r1.A == 0)
	{
		r.B = 1;
		r.C = r1.C/r1.B + 10;
	}else if (r1.B == 0)
	{	
		r.A = 1;
		r.C = r1.C/r1.A + 10;
	} 
	
	return r;
}
punto interseccion_rectas (recta r1, recta r2)
{
	punto p;
	
	if (r1.A == 0)
	{
		p.y = -r1.C/r1.B;
		p.x = -r2.C;
	}
	else if (r1.B == 0)
	{
		p.y = -r2.C;
		p.x = -r1.C/r1.A;
	}
	else
	{
		p.y = (-(r2.A * r1.C - r1.A * r2.C))/ (r2.A * r1.B - r1.A * r2.B);
		p.x = ((-r2.B * p.y)-r2.C)/r2.A;
	}
	
	return p;
}

#ifndef Recta_H
#define Recta_H

#include <math.h>	

#define infinito 9.9e9

class Recta{
	public:
		Recta();
		Recta(float m, float c);
		~Recta();
		
		Recta Perpendicular (float PuntoX, float PuntoY);
		
        Recta Paralela_Der_Dist (float distancia, float x);
        Recta Paralela_Izq_Dist (float distancia, float x);
		
		float m;
		float c;
	private:
};

#endif

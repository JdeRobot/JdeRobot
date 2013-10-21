#ifndef Segmento_H
#define Segmento_H

#include <math.h>	

#include "recta.h"
#include <eigen3/Eigen/Dense>

#define infinito 9.9e9

class Segmento{
	public:
		Segmento();
        Segmento(float x1, float y1, float z1, float x2, float y2, float z2);
        Segmento(Eigen::Vector3d p, Eigen::Vector3d q);
        ~Segmento();
		
		Recta SegmentoARecta();

		float x1;
		float y1;
		float z1;
		
		float x2;
		float y2;
		float z2;
	private:
};

#endif

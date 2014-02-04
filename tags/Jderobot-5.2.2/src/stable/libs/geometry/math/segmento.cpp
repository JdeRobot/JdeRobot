#include "segmento.h"

Segmento::Segmento()
{
}

Segmento::Segmento(float x1, float y1, float z1, float x2, float y2, float z2)
{
	this->x1 = x1;
	this->y1 = y1;
	this->z1 = z1;
	this->x2 = x2;
	this->y2 = y2;
	this->z2 = z2;
}

Segmento::Segmento(Eigen::Vector3d p, Eigen::Vector3d q)
{
    this->x1 = p(0);
    this->y1 = p(1);
    this->z1 = p(2);
    this->x2 = q(0);
    this->y2 = q(1);
    this->z2 = q(2);
}


Segmento::~Segmento()
{
}

Recta Segmento::SegmentoARecta( )
{
        Recta rx;

    if( fabs( x2 - x1 ) < ( 0.00001 )){//abs// //son rectas paralelas, intersectan en el infinito//
		rx.m = infinito;
		rx.c = x1;	
	}else{
		rx.m = (y2 - y1) / (x2 - x1); // (m = (y2- y1)/ (x2-x1)) Pendiente
		rx.c = y1 - ( rx.m* x1 );
	}
	
	return rx;
}





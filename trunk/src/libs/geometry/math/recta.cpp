#include "recta.h"

Recta::Recta()
{
	this->m = 0;
	this->c = 0;
}

Recta::Recta(float m, float c)
{
	this->m = m;
	this->c = c;
}


Recta::~Recta()
{
}

Recta Recta::Perpendicular (float PuntoX, float PuntoY)
{
	Recta Recta_Perp;
  
    if ( fabs(this->m) < 0.001 ){
        Recta_Perp.m = infinito /*( 1 / ( Recta.m * ( -1 ) ) );*/ ;

		Recta_Perp.c =  PuntoX;  
	}else{
        Recta_Perp.m = -( 1 / ( this->m ) );

        Recta_Perp.c = ( ( -Recta_Perp.m *  PuntoX  ) + PuntoY );
	}
	return Recta_Perp;
  
}

Recta Recta::Paralela_Der_Dist (float  distancia, float x)
{
  Recta recta_salida;

   if( fabs(this->m) < 1000 ){
      recta_salida.m = this->m;
      recta_salida.c = this->c;

      recta_salida.c = recta_salida.c - distancia;

   }else{
       recta_salida.m = infinito;
       recta_salida.c = x - distancia;
   }
  return recta_salida;

}
Recta Recta::Paralela_Izq_Dist ( float  distancia, float x)
{
  Recta recta_salida;

  if( fabs(this->m) < 1000 ){
      recta_salida.m = this->m;
      recta_salida.c = this->c;

      recta_salida.c = recta_salida.c + distancia;

  }else{
      recta_salida.m = infinito;
      recta_salida.c = x + distancia;
  }



  return recta_salida;

}

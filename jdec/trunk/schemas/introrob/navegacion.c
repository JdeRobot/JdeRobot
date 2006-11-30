#include "jde.h"
#include "jdegui.h"
#include "utilidades.h"

void vff_iteration(void)
{
  v=0;
  w=40;
}

void deliberative_iteration(void)
{
  v=300;
  w=60;
}

void hybrid_iteration(void)
{
  v=600;
  w=60;
}

void visualizacion(void)     
{ 
  Tvoxel kaka;
  static Tvoxel a,b;

  pintaSegmento(a,b,FL_WHITE);
  kaka.x=500.; kaka.y=500.;
  relativas2absolutas(kaka,&a);
  kaka.x=0.; kaka.y=0.;
  relativas2absolutas(kaka,&b);
  pintaSegmento(a,b,FL_RED);
}

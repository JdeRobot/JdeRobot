#include "jde.h"
#include "pioneer.h"
#include "forms.h"
#include "introrob.h"

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
  Tvoxel aa;
  static Tvoxel a,b;
  static Tvoxel c,d;

  /* example of relative segment */
  /* borra la el segmento pintado en la ultima iteracion */
  pintaSegmento(a,b,FL_WHITE);

  /* calcula nueva posicion absoluta del segmento */
  aa.x=500.; aa.y=500.; /* en mm */
  relativas2absolutas(aa,&a);
  aa.x=0.; aa.y=0.;
  relativas2absolutas(aa,&b);
  /* pinta la nueva posicion del segmento */
  pintaSegmento(a,b,FL_RED);

  /* ejemplo segmento absoluto */
  /* borra la el segmento pintado en la ultima iteracion */
  pintaSegmento(c,d,FL_WHITE); 
  c.x=0.; c.y=0.; /* en mm */
  d.x=robot[0]; d.y=robot[1]; /* en mm */
  pintaSegmento(c,d,FL_BLUE);
}

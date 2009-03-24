#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "motors.h"


Motors* new_Motors(const char* father, 
		   const char* interface_name,
		   const int owned){
  Interface* i = new_Interface(father,interface_name,owned);
  if (owned)
    i->datap = calloc(1,sizeof(Motors_data));
  return (Motors*)i;
}

void delete_Motors(Motors* m){
  if (m->owned)
    free(m->datap);
  delete_Interface((Interface*)m);
}

float Motors_v_get(const Motors* m){
  float* vp;

  assert(m!=0);
  if (m->owned)
    return ((Motors_data*)m->datap)->v;
  else{
    vp=(float *)myimport(m->interface_name,"v");
    return (vp?*vp:0.0);
  }
}

float Motors_w_get(const Motors* m){
  float* wp;

  assert(m!=0);
  if (m->owned)
    return ((Motors_data*)m->datap)->w;
  else{
    wp=(float *)myimport(m->interface_name,"w");
    return (wp?*wp:0);
  }
}

int Motors_cycle_get(const Motors* m){
  int* cyclep;

  assert(m!=0);
  if (m->owned)
    return ((Motors_data*)m->datap)->cycle;
  else{
    cyclep=(int *)myimport(m->interface_name,"cycle");
    return (cyclep?*cyclep:0);
  }
}

void Motors_v_set(Motors* m, const float new_v){
  float* vp;

  assert(m!=0);
  if (m->owned)
    ((Motors_data*)m->datap)->v=new_v;
  else{  
    vp=(float *)myimport(m->interface_name,"v");
    if (vp)
      *vp = new_v;
  }
}

void Motors_w_set(Motors* m, const float new_w){
  float* wp;

  assert(m!=0);
  if (m->owned)
    ((Motors_data*)m->datap)->w=new_w;
  else{
    wp=(float *)myimport(m->interface_name,"w");
    if (wp)
      *wp = new_w;
  }
}

void Motors_cycle_set(Motors* m, const int new_cycle){
  int* cyclep;

  assert(m!=0);
  if (m->owned)
    ((Motors_data*)m->datap)->cycle=new_cycle;
  else{
    cyclep=(int *)myimport(m->interface_name,"cycle");
    if (cyclep)
      *cyclep = new_cycle;
  }
}

#ifndef MOTORS_H
#define MOTORS_H
#include <jde.h>

typedef struct{
  /*modulations*/
  float v; /* mm/s */
  float w; /* deg/s */
  int cycle;
  char interface_name[MAX_NAME];
} Motors;

Motors* new_Motors(const char* interface_name);
void delete_Motors(Motors* m);

float Motors_v_get(const Motors* m);
float Motors_w_get(const Motors* m);
int Motors_cycle_get(const Motors* m);

void Motors_v_set(Motors* m, const float v);
void Motors_w_set(Motors* m, const float w);
void Motors_cycle_set(Motors* m, const int cycle);

#endif /*MOTORS_H*/

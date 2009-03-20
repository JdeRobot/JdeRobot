#ifndef ENCODERS_H
#define ENCODERS_H
#include <jde.h>

typedef struct{
  /*perceptions*/
  float x;
  float y;
  float theta;
  float cos;
  float sin;
  unsigned long int clock;
  /*modulations*/
  int cycle;
  char interface_name[MAX_NAME];
} Encoders;

Encoders* new_Encoders(const char* interface_name);
void delete_Encoders(Encoders* e);

float Encoders_x_get(const Encoders* e);
float Encoders_y_get(const Encoders* e);
float Encoders_theta_get(const Encoders* e);
float Encoders_cos_get(const Encoders* e);
float Encoders_sin_get(const Encoders* e);
unsigned long int Encoders_clock_get(const Encoders* e);
int Encoders_cycle_get(const Encoders* e);

void Encoders_cycle_set(Encoders* e, const int cycle);


#endif /*ENCODERS_H*/

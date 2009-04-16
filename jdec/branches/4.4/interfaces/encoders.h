#ifndef ENCODERS_H
#define ENCODERS_H
#include <jde.h>
#include <interface.h>

#ifdef __cplusplus
extern "C" {
#endif

enum robot_enum {ROBOT_X,ROBOT_Y,ROBOT_THETA,ROBOT_COS,ROBOT_SIN,ROBOT_NELEM};
typedef struct{
  /*perceptions*/
  float robot[ROBOT_NELEM];
  unsigned long int clock;
  /*modulations*/
  int cycle;
} Encoders_data;

typedef Interface Encoders;

/*constructor & destructor*/
Encoders* new_Encoders(const char* father,
		       const char* interface_name,
		       JDESchema* owner);
void delete_Encoders(Encoders* e);

/*interface methods*/
void Encoders_run(Encoders* e);
void Encoders_stop(Encoders* e);

/*get methods*/
float* Encoders_robot_get(const Encoders* e);
float Encoders_x_get(const Encoders* e);
float Encoders_y_get(const Encoders* e);
float Encoders_theta_get(const Encoders* e);
float Encoders_cos_get(const Encoders* e);
float Encoders_sin_get(const Encoders* e);
unsigned long int Encoders_clock_get(const Encoders* e);
int Encoders_cycle_get(const Encoders* e);

/*set methods*/
void Encoders_robot_set(const Encoders* e, const float* new_robot);
void Encoders_x_set(const Encoders* e, const float new_x);
void Encoders_y_set(const Encoders* e, const float new_y);
void Encoders_theta_set(const Encoders* e, const float new_theta);
void Encoders_cos_set(const Encoders* e, const float new_cos);
void Encoders_sin_set(const Encoders* e, const float new_sin);
void Encoders_clock_set(const Encoders* e, const unsigned long int new_clock);
void Encoders_cycle_set(Encoders* e, const int new_cycle);

#ifdef __cplusplus
}
#endif
#endif /*ENCODERS_H*/

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
Encoders* new_Encoders(JDESchema* const owner,
		       const char* interface_name,
		       const int implemented);
void delete_Encoders(Encoders* const e);

/*interface methods*/
void Encoders_run(const Encoders* e);
void Encoders_stop(const Encoders* e);

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
void Encoders_robot_set(Encoders* const e, const float* new_robot);
void Encoders_x_set(Encoders* const e, const float new_x);
void Encoders_y_set(Encoders* const e, const float new_y);
void Encoders_theta_set(Encoders* const e, const float new_theta);
void Encoders_cos_set(Encoders* const e, const float new_cos);
void Encoders_sin_set(Encoders* const e, const float new_sin);
void Encoders_clock_set(Encoders* const e, const unsigned long int new_clock);
void Encoders_cycle_set(Encoders* const e, const int new_cycle);

#ifdef __cplusplus
}
#endif
#endif /*ENCODERS_H*/

#ifndef MOTORS_H
#define MOTORS_H
#include <jde.h>
#include <interface.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
  /*modulations*/
  float v; /* mm/s */
  float w; /* deg/s */
  int cycle;
} Motors_data;

typedef Interface Motors;

/*constructor & destructor*/
Motors* new_Motors(const char* father,
		   const char* interface_name,
		   JDESchema* owner);
void delete_Motors(Motors* m);

/*interface methods*/
void Motors_run(Motors* m);
void Motors_stop(Motors* m);

/*get methods*/
float Motors_v_get(const Motors* m);
float Motors_w_get(const Motors* m);
int Motors_cycle_get(const Motors* m);

/*set methods*/
void Motors_v_set(Motors* m, const float new_v);
void Motors_w_set(Motors* m, const float new_w);
void Motors_cycle_set(Motors* m, const int new_cycle);

#ifdef __cplusplus
}
#endif
#endif /*MOTORS_H*/

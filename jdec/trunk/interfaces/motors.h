#ifndef MOTORS_H
#define MOTORS_H
#include <jde.h>
#include <interface.h>

#ifdef __cplusplus
extern "C" {
#endif

INTERFACEDEF(Motors,
	     /*modulations*/
	     float v; /* mm/s */
	     float w; /* deg/s */
	     int cycle;)

IFPROXYDEF(Motors,MotorsPrx)

/*constructors & destructors*/
Motors* new_Motors(const char* interface_name,
		       JDESchema* const supplier);
void delete_Motors(Motors* const this);

MotorsPrx* new_MotorsPrx(const char* interface_name,
			     JDESchema* const user,
			     Motors* const refers_to);
void delete_MotorsPrx(MotorsPrx* const this);

/* /\*interface methods*\/ */
/* void MotorsPrx_run(const MotorsPrx* this); */
/* void MotorsPrx_stop(const MotorsPrx* this); */

/*get methods*/
float MotorsPrx_v_get(const MotorsPrx* this);
float MotorsPrx_w_get(const MotorsPrx* this);
/* int MotorsPrx_cycle_get(const MotorsPrx* this); */

/*set methods*/
void MotorsPrx_v_set(MotorsPrx* const this, const float new_v);
void MotorsPrx_w_set(MotorsPrx* const this, const float new_w);
/* void MotorsPrx_cycle_set(MotorsPrx* const this, const int new_cycle); */

#ifdef __cplusplus
}
#endif
#endif /*MOTORS_H*/

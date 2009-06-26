#ifndef ENCODERS_H
#define ENCODERS_H
#include <jde.h>
#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

enum robot_enum {ROBOT_X,ROBOT_Y,ROBOT_THETA,ROBOT_COS,ROBOT_SIN,ROBOT_NELEM};
INTERFACEDEF(Encoders,
	     /*perceptions*/
	     float robot[ROBOT_NELEM];
	     unsigned long int clock;)
IFPROXYDEF(Encoders,EncodersPrx)

/*constructors & destructors*/
Encoders* new_Encoders(const char* interface_name,
		       JDESchema* const supplier);
void delete_Encoders(Encoders* const this);

EncodersPrx* new_EncodersPrx(const char* interface_name,
			     JDESchema* const user,
			     Encoders* const refers_to);
void delete_EncodersPrx(EncodersPrx* const this);

/*get methods*/
float* EncodersPrx_robot_get(const EncodersPrx* this);
float EncodersPrx_x_get(const EncodersPrx* this);
float EncodersPrx_y_get(const EncodersPrx* this);
float EncodersPrx_theta_get(const EncodersPrx* this);
float EncodersPrx_cos_get(const EncodersPrx* this);
float EncodersPrx_sin_get(const EncodersPrx* this);
unsigned long int EncodersPrx_clock_get(const EncodersPrx* this);

/*set methods*/
void EncodersPrx_robot_set(EncodersPrx* const this, 
			   const float* new_robot);
void EncodersPrx_x_set(EncodersPrx* const this, 
		       const float new_x);
void EncodersPrx_y_set(EncodersPrx* const this, 
		       const float new_y);
void EncodersPrx_theta_set(EncodersPrx* const this, 
			   const float new_theta);
void EncodersPrx_cos_set(EncodersPrx* const this, 
			 const float new_cos);
void EncodersPrx_sin_set(EncodersPrx* const this, 
			 const float new_sin);
void EncodersPrx_clock_set(EncodersPrx* const this,
			   const unsigned long int new_clock);
#ifdef __cplusplus
}
#endif
#endif /*ENCODERS_H*/

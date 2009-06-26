#ifndef LASER_H
#define LASER_H
#include <jde.h>
#include <interface.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LASER 720

INTERFACEDEF(Laser,
	     /*perceptions*/
	     int laser[MAX_LASER];
	     int number;
	     int resolution;
	     unsigned long int clock;
	     /*modulations*/
	     int cycle;)

IFPROXYDEF(Laser,LaserPrx)

/*constructors & destructors*/
Laser* new_Laser(const char* interface_name,
		 JDESchema* const supplier);
void delete_Laser(Laser* const this);

LaserPrx* new_LaserPrx(const char* interface_name,
		       JDESchema* const user,
		       Laser* const refers_to);
void delete_LaserPrx(LaserPrx* const this);


/*get methods*/
int* LaserPrx_laser_get(const LaserPrx* this);
int LaserPrx_number_get(const LaserPrx* this);
int LaserPrx_resolution_get(const LaserPrx* this);
unsigned long int LaserPrx_clock_get(const LaserPrx* this);

/*set methods*/
void LaserPrx_laser_set(LaserPrx* const this, const int* new_laser);
void LaserPrx_number_set(LaserPrx* const this, const int new_number);
void LaserPrx_resolution_set(LaserPrx* const this, const int new_resolution);
void LaserPrx_clock_set(LaserPrx* const this, const unsigned long int new_clock);

#ifdef __cplusplus
}
#endif
#endif /*LASER_H*/

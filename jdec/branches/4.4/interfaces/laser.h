#ifndef LASER_H
#define LASER_H
#include <jde.h>
#include <interface.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_LASER 720

typedef struct{
  /*perceptions*/
  int laser[MAX_LASER];
  int number;
  int resolution;
  unsigned long int clock;
  /*modulations*/
  int cycle;
} Laser_data;

typedef Interface Laser;

/*constructor & destructor*/
Laser* new_Laser(const char* father,
		 const char* interface_name,
		 const int owned);
void delete_Laser(Laser* l);

/*get methods*/
int* Laser_laser_get(const Laser* l);
int Laser_number_get(const Laser* l);
int Laser_resolution_get(const Laser* l);
unsigned long int Laser_clock_get(const Laser* l);
int Laser_cycle_get(const Laser* l);

/*set methods*/
void Laser_laser_set(const Laser* l, const int* new_laser);
void Laser_number_set(const Laser* l, const int new_number);
void Laser_resolution_set(const Laser* l, const int new_resolution);
void Laser_clock_set(const Laser* l, const unsigned long int new_clock);
void Laser_cycle_set(Laser* l, const int new_cycle);

#ifdef __cplusplus
}
#endif
#endif /*LASER_H*/

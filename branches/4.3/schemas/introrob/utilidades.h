#include "pioneer.h"

extern float introrob_mouse_x, introrob_mouse_y;
/* absolute position of the point clicked with the central mouse button */

int absolutas2relativas(Tvoxel in, Tvoxel *out);
int relativas2absolutas(Tvoxel in, Tvoxel *out);
int pintaSegmento(Tvoxel a, Tvoxel b, int color);
 
float v;
float w;
float jde_robot[5];
float jde_laser[NUM_LASER];

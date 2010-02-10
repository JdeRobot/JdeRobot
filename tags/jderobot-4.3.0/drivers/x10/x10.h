
#ifndef __T_IFACE
#define __T_IFACE
typedef struct iface{
   int (*set_property)(char *unit, char *property, ...);
}t_iface;
#endif

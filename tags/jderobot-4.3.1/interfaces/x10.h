// generic x10 interface
typedef struct iface{
   int (*set_property)(char *unit, char *property, ...);
   int (*start_monitor)();
   int (*stop_monitor)();
   int (*get_status)(char *unit);
} X10Iface;

// monitor events data interface 
typedef struct event{
   char unit[MAX_BUFFER];
   int *status;
   unsigned long int *clock;
} X10Events;

//* unit = id device (example: A1,C2...)

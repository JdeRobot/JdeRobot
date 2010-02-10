#include <jde+/util.h>
#include <stdexcept>

#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
	using ost::SysTime;
#endif

double timeval2sec(struct timeval* tv) {
   return (double)tv->tv_sec + ((double)tv->tv_usec)/1000000.0;
}

double gettimeofdaysec() {
   struct timeval tv;

   SysTime::getTimeOfDay(&tv);
   return timeval2sec(&tv);
}

#ifndef UTIL_H
#define UTIL_H

struct timeval;

/**
   Translate timeval struct to secs
*/
double timeval2sec(struct timeval* tv);

/**
   Get timeofday in secs
*/
double gettimeofdaysec();


#endif /*UTIL_H*/

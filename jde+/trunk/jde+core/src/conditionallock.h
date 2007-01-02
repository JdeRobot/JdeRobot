#ifndef CONDITIONALLOCK_H
#define CONDITIONALLOCK_H

#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::Conditional;
#endif

/**
   Class to make more easy the usage of Conditional variables.
*/
class ConditionalLock {
public:
  /**
     Constructor. Locks conditional variable
  */
  ConditionalLock(Conditional& cond)
    :mycond(cond){ mycond.lock(); }
	
  /**
     Destructor. Unlocks conditional variable
  */
  ~ConditionalLock() { mycond.unlock(); }
private:
  Conditional& mycond;
};

#endif /*CONDITIONALLOCK_H*/

#ifndef SCHEDULER_MANAGER_ICE
#define SCHEDULER_MANAGER_ICE

#include "common.ice"

module bica {
  
  /** 
  * Interface to the component scheduler.
  */
  interface SchedulerManager
  {
    /** 
     * Runs a BICA component
     */
    void run( string component );

    /**
     * Stops a BICA component
     */    
    void stop( string component );    
  };
};

#endif //SCHEDULER_MANAGER_ICE

#ifndef SCHEMAIMPLEMENTATION_H
#define SCHEMAIMPLEMENTATION_H
#include <string>
#include <vector>
#include <map>
#include <jde+/schematypes.h>
#include <jde+/data.h>

/*forward declaration*/
class Schemainstance;
//class Schemainterface;
//class Schemaimplementation_Pimpl;

/*
  Schemaimplementation class is abstract base class 
  of any schema implementation.
*/
class Schemaimplementation {
public:
  /**
     Destructor.
  */
  virtual ~Schemaimplementation() {}

  /**
     Initialize schema instance (data,private data,cycle_time,...).
     Called once per instance.
  */
  virtual void init_instance(Schemainstance* instance) = 0;

  /**
     Schema iteration code.
  */
  virtual void iteration(Schemainstance* instance) = 0;

  /**
     Schema arbitration code. A child requesting arbitration locks
     until a response is sended, so all request must be replied.
     \param request_nick source name from child requesting arbitration
  */
  virtual void arbitrate(Schemainstance* instance, const std::string& request_nick) = 0;

  /**
     Precondition code. It Only makes sense when implementing a motor
     schema.
     \return true if preconditions meets success
  */
  virtual bool check_preconditions(Schemainstance* instance) = 0;

  /**
     Sleep_handler, called before change to SLEPT state
     Default handler send SLEPT state to children.
  */
  virtual void sleep_handler(Schemainstance* instance);

  /**
     Wakeup_handler, called after leave SLEPT state
  */
  virtual void wakeup_handler(Schemainstance* instance) = 0;
};

#endif /*SCHEMAIMPLEMENTATION_H*/

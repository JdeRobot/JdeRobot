#ifndef ISC_HPP
#define ISC_HPP
#include <jde+/messageprocessor.h>

#include <string>

/*forward declaration*/
class Hierarchy;
class Isc_Pimpl;

class Isc : public Message_Processor {
public:
  /**
     Constructor.
     \param owner hierarchy that owns this isc
     \param debugflag if true enable debug messages
  */
  Isc(Hierarchy* owner, bool debugflag = false) throw();

  /**
     Destructor.
  */
  virtual ~Isc() throw();
  
  /**
     Handles perception messages.
     \param p perception message
  */
  virtual void process(Pointer<Perception> p) throw();

  /**
     Handles modulation messages.
     \param m modulation message
  */
  virtual void process(Pointer<Modulation> m) throw();

  /**
     Handles arbitrate request messages.
     \param a arbitrate request message
  */
  virtual void process(Pointer<Arbitrate_Req> a) throw();

  /**
     Handles state change messages.
     \param s 'go to this state' message
  */
  virtual void process(Pointer<State_Change> s) throw();

  /**
     Handles new state messages.
     \param n 'this is my new state' message
  */
  virtual void process(Pointer<New_State> n) throw();

  /**
     Add a new processor to message receiver's list.
     On message process all processors are executed secuencially.
     \param mp message processor to be added
  */
  void append_processor(Message_Processor* mp) throw();
private:
  const bool debug; /**< If true methods print debug messages*/
  Isc_Pimpl* pimpl; /**< Private implementation*/
  friend class Isc_Pimpl;
};
#endif /*ISC_HPP*/

#ifndef SCHEMAINSTANCE_H
#define SCHEMAINSTANCE_H
#include <jde+/schematypes.h>
#include <jde+/pointer.h>
#include <jde+/messageprocessor.h>
#include <string>
#include <vector>
#include <map>

/*forward declarations*/
class Hierarchy;
class Schemastate;
class Schemaimplementation;
class Schemainstance_Pimpl;
class Schemainstancefacade;

/**
   Schemainstance class is abstract base class of instances.
*/
class Schemainstance : public Message_Processor {
public:
  /**
     Constructor.
     \param sid schema identifier
     \param owner hierarchy than contains this instance
     \param debugflag if true enable debug messages
  */
  Schemainstance(const sid_t sid,
		 Hierarchy* owner,
		 const bool debugflag=false) throw();

  /** Destructor */
  virtual ~Schemainstance() throw();

  /**
     Get schema id.
  */
  sid_t get_sid() const throw();

  /**
     Get actual schema state.
  */
  state_enum get_state() const throw();

  /**
     Get schema iteration cycle time in ms.
  */
  int get_cycle_time() const throw();

  /**
     Set schema iteration cycle time in ms.
     \param c cycle time in ms
  */
  void set_cycle_time(const int c) throw();

  /**
     Send request for arbitration to father.
     Arbitration request are auto generated when a control conflict
     apears.
  */
  void send_arbitrate_request() const throw();

  /**
     Count active and ready brothers.
     FIXME: de donde cogemos el estado de los hermanos??
     de hierarchy?? hacemos que nos envien los cambios de
     estado de los hermanos??
  */
  int get_ready_or_active_brothers() throw();


  /**
     Handles perception messages.
  */
  virtual void process(Pointer<Perception> p) throw();

  /**
     Handles modulation messages.
  */
  virtual void process(Pointer<Modulation> m) throw();

  /**
     Handles arbitrate request messages.
  */
  virtual void process(Pointer<Arbitrate_Req> a) throw();

  /**
     Handles state change messages.
  */
  virtual void process(Pointer<State_Change> s) throw();

  /**
     Handles new state messages.
  */
  virtual void process(Pointer<New_State> n) throw();

  /**
     Change state
  */
  void change_state(Schemastate* s) throw();

  /**
     Promote state unless newstate received
  */
  void promote_state(Schemastate* s) throw();

  /**
     Wait until a new state arrives or timeout expires.
     \param timeout time to wait the state change in ms
     \return non 0 value if timedout
  */
  int wait_state_change(const timeout_t timeout = 0) throw();

  /**
     Adquire the implementation
  */
  Schemaimplementation* adquire_impl() throw();

  /**
     System start time.
  */
  static const double& tstart;

  /**
     Debug flag.
  */
  bool debug;

 private:
  void change_state_i(Schemastate* s) throw();

  Schemainstance_Pimpl* pimpl;

  friend class Schemainstancefacade;
};

#endif /*SCHEMAINSTANCE_H*/

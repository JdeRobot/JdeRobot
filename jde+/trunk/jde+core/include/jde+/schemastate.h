#ifndef SCHEMASTATE_H
#define SCHEMASTATE_H
#include <string>

#include <jde+/data.h>
#include <jde+/schematypes.h>
#include <jde+/singleton.h>


/*forward declaration*/
class Schemainstance;
class Schemaimplementation;

/**
   Base class for states. Each non abstract class represent a schema
   state, and define the actions tha occurs on it for each event.
   This class is a proxy for other state classes, allowing they to use
   some non public members of Schemainstance and Schemaimplementation.
*/
class Schemastate {
public:
  /**
     Get state name as an enum
  */
  virtual state_enum get_state_name() const = 0;

  /**
     Event executed for each iteration.
  */
  virtual void exec_actions(Schemainstance* i) {}

  /**
     Event executed when receiving a perception.
     FIXME: borrar
  */
  virtual void receive_perception(Schemainstance* i,
				  const std::string& child_nick,
				  const std::string& name,
				  const Data& perception) {}

  /**
     Event executed when receiving a modulation.
     FIXME: borrar
  */
  virtual void receive_modulation(Schemainstance* i,
				  const std::string& name,
				  const Data& modulation) {}

  /**
     Event executed when receiving a state change.
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate) {}

  /**
     Event executed when receiving a child new state.
     FIXME: borrar
  */
  virtual void receive_child_new_state(Schemainstance* i,
				       const std::string& child_nick,
				       const state_enum newstate) {}

  /**
     Event executed when receiving an arbitration request.
  */
  virtual void receive_arbitrate_request(Schemainstance* i,
					 const std::string& child_nick) {}
protected:
  /**
     Derived state classes will be singletons, so putting ctor and
     dtor on protected avoid they use
  */
  Schemastate() {}
  Schemastate(Schemastate& st) {}
  virtual ~Schemastate() {}

  /**
     Change the state on an instance.
     \sa Schemainstance::change_state()
  */
  void change_state(Schemainstance* i, Schemastate* s);

  /**
     Promote state on an instance.
     \sa Schemainstance::promote_state()
  */
  void promote_state(Schemainstance* i, Schemastate* s);

  /**
     Wait until a new state arrives.
     \sa Schemainstance::wait_state_change()
  */
  int wait_state_change(Schemainstance* i, const timeout_t timeout = 0);

  /**
     Adquire impl.
     \sa Schemainstance::adquire_impl()
  */
  Schemaimplementation* adquire_impl(Schemainstance* i);

  /**
     Release impl.
     \sa Schemainstance::release_impl()
  */
  void release_impl(Schemainstance* i);

  
  /**
     Set a new received perception from child nick
     \sa Schemaimplementation::set_child_perception()
     FIXME: borrar
  */
  void set_child_perception(Schemainstance* i,
			    const std::string& child_nick,
			    const std::string& name, const Data& perception);

  /**
     Set a new received modulation
     \sa Schemaimplementation::set_modulation()
     FIXME: borrar
  */
  void set_modulation(Schemainstance* i,
		      const std::string& name, const Data& modulation);
};

/*GROUP STATES*/
/**
   Groups slept states
*/
class Schemaslept : public Schemastate {
public:
  /**
     Wait until a new state arrives
  */
  virtual void exec_actions(Schemainstance* i);

  /**
     Set modulation, allowing a slept instance to be modulated before
     activation.
  */
  virtual void receive_modulation(Schemainstance* i,
				  const std::string& name, const Data& modulation);

  virtual state_enum get_state_name() const { return SLEPT; }
};

/**
   Groups active states(alert,ready,active) with some common actions
*/
class Schemanoslept : public Schemastate {
public:
  /**
     Set perception.
  */
  virtual void receive_perception(Schemainstance* i,
				  const std::string& child_nick,
				  const std::string& name, const Data& perception);

  /**
     Set modulation.
  */
  virtual void receive_modulation(Schemainstance* i,
				  const std::string& name, const Data& modulation);

  /**
     Exec arbitration.
  */
  virtual void receive_arbitrate_request(Schemainstance* i,
					 const std::string& child_nick);
};

/*PSCHEMAS STATES*/
/**
   Perceptive schemas slept state
*/
class Pschemaslept : public Singleton<Pschemaslept>, public Schemaslept{
public:
  /**
     Active instance if newstate isn't SLEPT. Executes wakeup_handler
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate);
  friend class Singleton<Pschemaslept>;
};

/**
   Perceptive schemas active state
*/
class Pschemaactive : public Singleton<Pschemaactive>, public Schemanoslept {
public:
  /**
     Exec iteration.
  */
  virtual void exec_actions(Schemainstance* i);

  /**
     Sleep instance if newstate is SLEPT. Release implementation
     allowing reuse it.
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate);

  
  virtual state_enum get_state_name() const { return ACTIVE; }
  friend class Singleton<Pschemaactive>;
};


/*MSCHEMAS STATES*/
/**
   Motor schema slept state
*/
class Mschemaslept : public Singleton<Mschemaslept>, public Schemaslept {
public:
  /**
     Active instance if newstate isn't SLEPT. Executes wakeup_handler
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate);
};

/**
   Motor schema alert state
*/
class Mschemaalert : public Singleton<Mschemaalert>, public Schemanoslept {
public:
  /**
     Check preconditions. If success promote to ready state, else
     check how many brothers are ready or active. If 0 (control
     absence) send an arbitration request to father and wait a 
     new state indefinitely.
   */
  virtual void exec_actions(Schemainstance* i);

  /**
     Receive state. If slept release implementation to reuse it.
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate);
  virtual state_enum get_state_name() const { return ALERT; }
  friend class Singleton<Mschemaalert>;
};


/**
   Motor schema ready state
*/
class Mschemaready : public Singleton<Mschemaready>, public Schemanoslept {
public:
  /**
     Check how many brothers are ready or active. If >= 1 (control
     overlap) send an arbitration request to father and wait a 
     new state indefinitely. Else promote to active state.
  */
  virtual void exec_actions(Schemainstance* i);

  /**
     Receive state. If slept release implementation to reuse it.
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate);
  virtual state_enum get_state_name() const { return READY; }
  friend class Singleton<Mschemaready>;
};

/**
   Motor schema active state
*/
class Mschemaactive : public Singleton<Mschemaactive>, public Schemanoslept {
public:
  /**
     Exec iteration. Unless a new state received, promote to alert state.
  */
  virtual void exec_actions(Schemainstance* i);

  /**
     Receive state. If slept release implementation to reuse it.
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate);
  virtual state_enum get_state_name() const { return ACTIVE; }
  friend class Singleton<Mschemaactive>;
};

#endif /*SCHEMASTATE_H*/

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
  */
  virtual void receive_perception(Schemainstance* i,
				  const std::string& child_nick,
				  const std::string& name,
				  const Data& perception) {}

  /**
     Event executed when receiving a modulation.
     Schema can receive modulation on all states.
  */
  virtual void receive_modulation(Schemainstance* i,
				  const std::string& name,
				  const Data& modulation);

  /**
     Event executed when receiving a state change.
  */
  virtual void receive_state_change(Schemainstance* i,
				    const state_enum newstate) {}

  /**
     Event executed when receiving a child new state.
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
     Exec arbitration.
  */
  virtual void receive_arbitrate_request(Schemainstance* i,
					 const std::string& child_nick);

  /*
    Receive new states from children
  */
  virtual void receive_child_new_state(Schemainstance* i,
				       const std::string& child_nick,
				       const state_enum newstate);
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

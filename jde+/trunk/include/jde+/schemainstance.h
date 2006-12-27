#ifndef SCHEMAINSTANCE_H
#define SCHEMAINSTANCE_H
#include <jde+/schematypes.h>
#include <jde+/pointer.h>
#include <jde+/messageprocessor.h>
#include <jde+/data.h>
#include <jde+/privatedata.h>
#include <string>
#include <vector>

/*forward declaration*/
class Hierarchy;
class Schemafactory;
class Schemastate;
class Schemaimplementation;
class Schemainstance_Pimpl;
class PrivateData;

/**
   Schemainstance class is abstract base class of instances.
*/
class Schemainstance : private Message_Processor {
public:
  /**
     Constructor.
     \param sid schema identifier
     \param owner hierarchy than contains this instance
     \param debugflag if true enable debug messages
  */
  Schemainstance(const sid_t sid,
		 Hierarchy* owner,
		 const bool debugflag=false);

  /** Destructor */
  virtual ~Schemainstance();

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
     Get last modulation received
     \param name modulation name
     \return modulation in Data object
     \exception std::logic_error if modulation name doesn't exist in the interface
  */
  Data& get_modulation(const std::string& name) throw(std::logic_error);

  /**
     Set modulation. Allow configure schema on initialization.
     \param name modulation name
     \param modulation Data object
     \exception std::logic_error if modulation name doesn't exist in the interface
  */
  void set_modulation(const std::string& name,
		      const Data& modulation) throw(std::logic_error);

  /**
     Get schema children nicks.
     \return vector with children nicks
  */
  std::vector<std::string>& get_children() const throw();

  /**
     Get child last seen state.
     \param child_nick child schema nick
     \return child state
     \exception std::logic_error if instance hasn't got this child
  */
  state_enum get_child_state(const std::string& child_nick) const throw(std::logic_error);
   
  /**
     Get child last received perception
     \param child_nick source child
     \param name perception name
     \return perception on Data object
     \exception std::logic_error if instance hasn't got this child or
     child interface hasn't got perception name 
  */
  Data& get_child_perception(const std::string& child_nick,
			     const std::string& name) throw(std::logic_error);

  /**
     Send perception to father.
     \param name perception name
     \param perception data object containing perception data
     \exception std::logic_error if instance hasn't got perception name
  */
  void send_perception(const std::string& name,
		       const Data& perception) const throw(std::logic_error);

  /**
     Send modulation to child.
     \param child_nick destination child schema nick
     \param name modulation name
     \param modulation data object containing modulation data
     \exception std::logic_error if child_nick doesn't exists or
     child interface hasn't got modulation name
  */
  void send_modulation(const std::string& child_nick, const std::string& name,
		       const Data& modulation) const throw(std::logic_error);
  
  /**
     Send state change to child.
     \param child_nick destination child schema nick
     \param newstate state that dst schema must change
     \exception std::logic_error if child_nick doesn't exists
  */
  void send_state_change(const std::string& child_nick,
			 const state_enum newstate) const throw(std::logic_error);

  /**
     Send request for arbitration to father.
     Arbitration request are auto generated when a control conflict
     apears. Use this method only if you know what are you doing.
     \param src source schema id
     \param dst destination schema id
  */
  void send_arbitrate_request() const throw();

  /**
     Wait child state change. Lock iteration on exec_actions so
     messages can be delivered.
     \param child_nick child name
     \param wait_state state that unlock this call
     \param timeout time in ms waited before unlock this call
     automatically. A 0 timeout lock indefinitely
     \return non 0 value if timedout
  */
  int wait_child_state(const std::string& child_nick,
		       const state_enum wait_state,
		       const timeout_t timeout = 0) throw();

  /**
     Count active and ready brothers.
     FIXME: de donde cogemos el estado de los hermanos??
     de hierarchy?? hacemos que nos envien los cambios de
     estado de los hermanos??
  */
  int get_ready_or_active_brothers() throw();

  /**
     Instance private data.
  */
  Pointer<PrivateData> private_data;

  /**
     System start time.
  */
  static const double& tstart;

  /**
     Debug flag.
  */
  bool debug;
private:
  /**
     Handles perception messages.
  */
  virtual void process(Pointer<Perception> p);

  /**
     Handles modulation messages.
  */
  virtual void process(Pointer<Modulation> m);

  /**
     Handles arbitrate request messages.
  */
  virtual void process(Pointer<Arbitrate_Req> a);

  /**
     Handles state change messages.
  */
  virtual void process(Pointer<State_Change> s);

  /**
     Handles new state messages.
  */
  virtual void process(Pointer<New_State> n);

  /**
     Set child received perception
     \param child_nick source child
     \param name perception name
     \param perception on Data object
     \exception std::logic_error if instance hasn't got this child or
     child interface hasn't got perception name 
  */
  void set_child_perception(const std::string& child_nick,
			    const std::string& name,
			    const Data& perception) throw(std::logic_error);

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

  /*internal implementations for thread-safe interface*/
  state_enum get_child_state_i(const std::string& child_nick) const throw(std::logic_error);

  Data& get_modulation_i(const std::string& name) throw(std::logic_error);

  void set_modulation_i(const std::string& name,
			const Data& modulation) throw(std::logic_error);

  Data& get_child_perception_i(const std::string& child_nick,
			       const std::string& name) throw(std::logic_error);

  void set_child_perception_i(const std::string& child_nick,
			      const std::string& name,
			      const Data& perception) throw(std::logic_error);

  Schemainstance_Pimpl* pimpl;

  friend class Hierarchy;
  friend class Isc;
  friend class Schemastate;
};


#endif /*SCHEMAINSTANCE_H*/

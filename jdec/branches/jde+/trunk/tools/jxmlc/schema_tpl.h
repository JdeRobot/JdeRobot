#include <jde+/schemaimplementation.h>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

class $schemaname: public Schemaimplementation{
 public:
  $schemaname();

  //virtual ~$schemaname();

  virtual void init_instance(Schemainstancefacade* instance) throw();

  virtual void iteration(Schemainstancefacade* instance) throw();

  virtual void arbitrate(Schemainstancefacade* instance, const std::string& request_nick) throw();

  virtual bool check_preconditions(Schemainstancefacade* instance) throw();

  virtual void receive_child_new_state(Schemainstancefacade* instance,
				       const std::string& child_nick,
				       const state_enum newstate) throw();

  virtual void receive_modulation(Schemainstancefacade* instance,
				  const std::string& name, 
				  const Data& modulation) throw();

  virtual void receive_perception(Schemainstancefacade* instance,
				  const std::string& child_nick, 
				  const std::string& name, 
				  const Data& perception) throw();

  virtual void sleep_handler(Schemainstancefacade* instance) throw();

  virtual void wakeup_handler(Schemainstancefacade* instance) throw();
};

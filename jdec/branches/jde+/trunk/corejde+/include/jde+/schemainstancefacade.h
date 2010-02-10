#ifndef SCHEMAINSTANCEFACADE_H
#define SCHEMAINSTANCEFACADE_H
#include <string>
#include <jde+/schematypes.h>
#include <jde+/pointer.h>
#include <jde+/data.h>
#include <jde+/privatedata.h>

/*forward declaration*/
class Schemainstance;
class PrivateData;

class Schemainstancefacade {
 public:
  Schemainstancefacade(Schemainstance* instance) throw();

  sid_t get_sid() const throw();
  state_enum get_state() const throw();

  int get_cycle_time() const throw();
  void set_cycle_time(const int c) throw();

  void set_child_state(const std::string& child_nick,
		       const state_enum newstate) throw(std::logic_error);
  
  void send_perception(const std::string& name,
		       const Data& perception) const throw(std::logic_error);
  void send_modulation(const std::string& child_nick, const std::string& name,
		       const Data& modulation) const throw(std::logic_error);

  Pointer<PrivateData> private_data;
 private:
  Schemainstance* instance;
};  

#endif /*SCHEMAINSTANCEFACADE_H*/

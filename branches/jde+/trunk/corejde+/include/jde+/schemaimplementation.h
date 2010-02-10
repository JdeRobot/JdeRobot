#ifndef SCHEMAIMPLEMENTATION_H
#define SCHEMAIMPLEMENTATION_H
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <jde+/schematypes.h>
#include <jde+/data.h>

/*forward declaration*/
class Schemainstancefacade;
class Schemaimplementation;

/*
  Schemaimplementation class is abstract base class 
  of any schema implementation.
*/
class Schemaimplementation {
public:
  typedef Schemaimplementation* simplmaker_t();

  struct child_info_t {
    std::string interface_name;
    std::vector<std::string> prefered_branches;
  };

  struct schemaimplementation_info_t {
    std::string class_name;
    std::string interface_name;
    std::string implementation_branch;
    std::map<std::string, child_info_t> children_infos;
    simplmaker_t* constructor;
  };
  
  Schemaimplementation(const schemaimplementation_info_t* implementation_info);

  /**
     Destructor.
  */
  virtual ~Schemaimplementation() throw() {}

  /**
     Initialize schema instance (data,private data,cycle_time,...).
     Called once per instance.
  */
  virtual void init_instance(Schemainstancefacade* instance) throw() = 0;

  /**
     Schema iteration code.
  */
  virtual void iteration(Schemainstancefacade* instance) throw() = 0;

  /**
     Schema arbitration code. A child requesting arbitration locks
     until a response is sended, so all request must be replied.
     \param request_nick source name from child requesting arbitration
  */
  virtual void arbitrate(Schemainstancefacade* instance, const std::string& request_nick) throw() = 0;

  /**
     Precondition code. It Only makes sense when implementing a motor
     schema.
     \return true if preconditions meets success
  */
  virtual bool check_preconditions(Schemainstancefacade* instance) throw() = 0;


  virtual void receive_child_new_state(Schemainstancefacade* instance,
				       const std::string& child_nick,
				       const state_enum newstate) throw() = 0;
  virtual void receive_modulation(Schemainstancefacade* instance,
				  const std::string& name, 
				  const Data& modulation) throw() = 0;
  virtual void receive_perception(Schemainstancefacade* instance,
				  const std::string& child_nick, 
				  const std::string& name, 
				  const Data& perception) throw() = 0;

  /**
     Sleep_handler, called before change to SLEPT state
     Default handler send SLEPT state to children.
  */
  virtual void sleep_handler(Schemainstancefacade* instance) throw() = 0;

  /**
     Wakeup_handler, called after leave SLEPT state
  */
  virtual void wakeup_handler(Schemainstancefacade* instance) throw() = 0;

  /**
     Auto load mechanisms
  */
  static Schemaimplementation* instance(const std::string& class_name) throw();

  static const schemaimplementation_info_t* class_info(const std::string& class_name) throw();
  static const schemaimplementation_info_t* class_info_search(const std::string& interface_name,
							      const std::vector<std::string>& branch_preferences =
							      std::vector<std::string>::vector(0) ) throw();

  static void register_class(const schemaimplementation_info_t& class_info) throw();

  static schemaimplementation_info_t parse_class_info(const std::string& class_name,
						      const std::string& file_name) throw(std::runtime_error);

  /**
     Introspection info
  */
  const schemaimplementation_info_t* implementation_info;

private:
  static const schemaimplementation_info_t* class_info_i(const std::string& class_name) throw();
  static const schemaimplementation_info_t* class_info_search_i(const std::string& interface_name,
								const std::vector<std::string>& branch_preferences =
								std::vector<std::string>::vector(0) ) throw();

  static void register_class_i(const schemaimplementation_info_t& class_info) throw();

  static std::map<std::string, int > info_by_classname;
  static std::map<std::string, int > info_by_inteface;
  static std::vector<schemaimplementation_info_t> class_infos;
};

#define REGISTER_SCHEMA(X) \
namespace _X { \
   static Schemaimplementation::schemaimplementation_info_t si; \
   static Schemaimplementation* smaker() { return new X(); } \
   class reg_proxy { \
   public: \
     reg_proxy() { \
       si = Schemaimplementation::parse_class_info("X","X.xml"); \
       si.constructor = smaker; \
       Schemaimplementation::register_class(si); \
     } \
   }; \
   static reg_proxy rp; \
}

#define SCHEMA_INFO(X) _X::si

#endif /*SCHEMAIMPLEMENTATION_H*/

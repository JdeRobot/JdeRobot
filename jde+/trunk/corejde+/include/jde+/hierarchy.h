#ifndef HIERARCHY_HPP
#define HIERARCHY_HPP
#include <string>
#include <map>
#include <vector>
#include <set>
#include <utility>
#include <jde+/pointer.h>
#include <jde+/schematypes.h>

/*forward declarations*/
class Isc;
class Schemainstance;
class Schemafactory;
class Message_Processor;
class Hierarchy_Pimpl;

class Hierarchy {
public:
  class Instanceinfo { /*FIXME: hacer monitor*/
  public:
    /**
       Constructor. Interface specification is known
       \param sid Instance identifier
       \param father_sid Father instance identifier 
       \param interface_name Interface name used by this instance
       \param branch_preferences Instance branch preferences used to
       choose implementation
    */
    Instanceinfo(const sid_t sid, const sid_t father_sid,
		 const std::string& interface_name,
		 const std::vector<std::string>& branch_preferences);

    /**
       Link  child to this instance.
       \param child_sid child to link
       \exception std::logic_error if child sid is already linked
    */
    void link_child(const sid_t child_sid) throw(std::logic_error);

    /**
       Get a set with instance's children sids.
    */
    const std::set<sid_t>& get_children() const throw();

    /**
       Question about if a sid is a child
       \param sid sid to test
       \return true if sid identify a child of mine
    */
    bool is_child(const sid_t sid) const throw();

    sid_t sid;
    sid_t father_sid;
    std::string interface_name;
    std::vector<std::string> branch_preferences;
  private:
    std::set<sid_t> children;
  };

  /**
     Constructor.
     \param paths search paths for locate schemas
     \param debugflag if true enable debug messages
  */
  //Hierarchy(const std::vector<std::string>& paths, const bool debugflag=false);

  /**
     Constructor.
     \param root_interface_name the hierarchy's root schema
     \param root_branch the hierarchy's prefered branch
     \param debugflag if true enable debug messages
  */
  Hierarchy(const std::string& root_interface_name,
	    const std::string& root_branch = "",
	    const bool debugflag=false) throw();

  /** Destructor. */
  ~Hierarchy();

  /**
     Activate hierarchy.
     Active Hierarchy with 'root_interface_name' schema as root
     \exception std::runtime_error if hierarchy can't locate root factory
  */
  void start() throw(std::runtime_error);

  /**
      Stop Hierarchy.
  */
  void stop() throw();

  /**
     Get instance info copy. Info could be incomplete if instance
     hasn't been created yet. At least sid and
     father contain right values. FIXME!!!
     \param sid instance id to find
     \exception std::logic_error if sid is unknown on this hierarchy
     \return instance info if found. 
  */
  const Instanceinfo* get_instanceinfo(const sid_t sid) const throw(std::logic_error);

  /**
    Get brother count of schema identified by sid.
    \param sid A valid schema identifier
    \return brother count.
    \exception std::logic_error if sid is unknown on this hierarchy
  */
  int get_nbrothers(const sid_t sid) const throw(std::logic_error);

  /**
    Get brothers of schema identified by sid.
    \param sid a valid schema identifier
    \return set with brother sids.
    \exception std::logic_error if sid is unknown on this hierarchy
  */
  std::set<sid_t> get_brothers(const sid_t sid) const throw(std::logic_error);

  /**
    Get children count of schema identified by sid.
    Info only available if instance has been refered sometime,
    in other case return 0.
    \param sid a valid schema identifier
    \return children count.
    \exception std::logic_error if sid is unknown on this hierarchy
  */
  int get_nchildren(const sid_t sid) const throw(std::logic_error);

  /**
    Get children of schema identified by sid.
    Info only available if instance has been refered sometime,
    in other case return vector(0)
    \param sid a valid schema identifier
    \return set with children sids.
    \exception std::logic_error if sid is unknown on this hierarchy
  */
  std::set<sid_t> const& get_children(const sid_t sid) const throw(std::logic_error);

  /**
    Get state of schema identified by sid.
    \param sid a valid schema identifier
    \return schema state
    \exception std::logic_error if sid is unknown on this hierarchy
  */
  state_enum get_state(const sid_t sid) const throw(std::logic_error);

  /**
    Get schema instance identified by sid.
    \param sid a valid schema identifier
    \return a pointer to schema instance
    \exception std::logic_error if sid is unknown on this hierarchy
    \return instance or null if can't create it because factory is unavailable.
  */
  Pointer<Schemainstance> get_instance(const sid_t sid) throw(std::logic_error);

  /**
     Get the isc an instance must use to 'talk' with other schemas.
     Actual implementation return always the same isc
     \param sid a valid schema identifier
     \exception std::logic_error if sid is unknown on this hierarchy
     \return a pointer to isc
  */
  Pointer<Isc> get_isc(const sid_t sid) throw(std::logic_error);

  /**
     Add a child to instance identified by sid.
     \param sid father instance
     \param interface_name interface name for this child
     \param branch_preferences implementation branch preferences
     \exception std::logic_error if sid is unknown on this hierarchy
     \return sid assigned for this child
  */
  sid_t add_child(const sid_t sid,
		  const std::string& interface_name,
		  const std::vector<std::string>& branch_preferences) throw(std::logic_error);

  /**
     Load dynamic library
  */
  int load_library(const std::string file_path) throw();

  /**
     Load dynamic libraries in dir
  */
  int load_libraries(const std::string dir_path,
		     const std::string ext = ".so") throw();

  /**
     Get loaded libraries
  */
  std::vector<std::string> get_loaded_libraries() const throw();

  /**
     Append a processor to all system isc.
  */
  void append_processor(Message_Processor* mp) throw();
  
  /**
    Start time of system
  */
  static const double tstart;

  /**
    debug mode enable if true
  */
  bool debug;
private:
  Hierarchy_Pimpl* pimpl;

  /*internal implementations for thread-safe interface*/
  void start_i() throw(std::runtime_error);

  void stop_i() throw();
  
  Instanceinfo* get_instanceinfo_i(const sid_t sid) const throw(std::logic_error);

  std::set<sid_t> get_brothers_i(const sid_t sid) const throw(std::logic_error);

  std::set<sid_t> const& get_children_i(const sid_t sid) const throw(std::logic_error);

  state_enum get_state_i(const sid_t sid) const throw(std::logic_error);

  Pointer<Schemainstance> get_instance_i(const sid_t sid) throw(std::logic_error);

  Pointer<Isc> get_isc_i(const sid_t sid) throw(std::logic_error);

  sid_t add_child_i(const sid_t sid,
		    const std::string& interface_name,
		    const std::vector<std::string>& branch_preferences) throw(std::logic_error);
};

#endif /*HIERARCHY_HPP*/


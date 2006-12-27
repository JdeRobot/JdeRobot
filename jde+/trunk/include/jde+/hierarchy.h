#ifndef HIERARCHY_HPP
#define HIERARCHY_HPP
#include <string>
#include <map>
#include <vector>
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
  class Instanceinfo;
  class Instanceinfo {
  public:
    typedef std::map<sid_t,string> sid2nick_map_t;
    typedef std::map<string,sid_t> nick2sid_map_t;
    
    /**
       Constructor. Interface specification is known
       \param sid Instance identifier
       \param father pointer to father info
       \param factory than can create this instance
    */
    Instanceinfo(const sid_t sid, const Pointer<Instanceinfo> father,
		 const Pointer<Schemafactory> factory) throw();

    /**
       Link  child to this instance.
       \param child_instanceinfo child to link
       \exception std::logic_error if child sid or child nick is
       already linked
    */
    void link_child(const sid_t child_sid, const std::string child_nick) throw(std::logic_error);

    /*
      Get child nick from sid. 
      \param child_sid child sit to translate
      \exception std::logic_error if instance hasn't got this child
    */
    std::string get_child_nick(const sid_t child_sid) const throw(std::logic_error);

    /*
      Get child sid from nick. 
      \param child_nick child nick to translate
      \exception std::logic_error if instance hasn't got this child
    */
    sid_t get_child_sid(const string child_nick) const throw(std::logic_error);

    /**
       Get a vector with instance's children sids.
    */
    const std::vector<sid_t>& get_children() const throw();

    /**
       Get a vector with instance's children nicks.
    */
    const std::vector<std::string>& get_children_nicks() const throw();

    /**
       Question about if a sid is a child
       \param sid sid to test
       \return true if sid identify a child of mine
    */
    bool is_child(const sid_t sid) const throw();

    /**
       Question about if a nick is a child
       \param sid sid to test
       \return true if sid identify a child of mine
    */
    bool is_child(const string child_nick) const throw();

    /**
       Get a vector with instance brother's sids.
    */
    std::vector<sid_t> get_brothers() const throw();

    const sid_t sid;
    const Pointer<Instanceinfo> father;
    Pointer<Schemafactory> factory;
  private:
    std::vector<sid_t> children;
    std::vector<std::string> children_nicks;
    sid2nick_map_t sid2nick;
    nick2sid_map_t nick2sid;
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
     \exception std::run_time if hierarchy can't locate root factory
  */
  void start() throw(std::run_time);

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
     \return instance info if found. instanceinfo->factory can be null
     if no suitable factory is found. In this case only sid and father
     info will be available.
  */
  const Pointer<Instanceinfo> get_instanceinfo(const sid_t sid) throw(std::logic_error);

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
    \return vector with brother sids.
    \exception std::logic_error if sid is unknown on this hierarchy
  */
  std::vector<sid_t> const& get_brothers(const sid_t sid) const throw(std::logic_error);

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
    \return vector with children sids.
    \exception std::logic_error if sid is unknown on this hierarchy
  */
  std::vector<sid_t> const& get_children(const sid_t sid) const throw(std::logic_error);

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
    \exception std::logic_error
    \exception std::logic_error if sid is unknown on this hierarchy
    \return instance or null if can't create it because factory is unavailable.
  */
  Pointer<Schemainstance> get_instance(const sid_t sid) throw(std::logic_error);

  /**
     Get the isc an instance must use to 'talk' with other schemas.
     Actual implementation return always the same isc
     \param sid a valid schema identifier
     \return a pointer to isc
  */
  Pointer<Isc> get_isc(const sid_t sid) throw();

  /**
     Load schema factory in file
  */
  Pointer<Schemafactory> load_factory(const std::string file_path) throw();

  /**
     Load schema factories in dir
  */
  std::vector<Pointer<Schemafactory> > load_factories(const std::string dir_path,
						      const std::string ext = ".so") throw();

  /**
     Get factory by name
  */
  Pointer<Schemafactory> const get_factory(const std::string interface_name, 
					   const std::vector<std::string>& branch_preferences 
					   = std::vector<std::string>::vector()) const throw();
   
  /**
     Get hierarchy registered factories
  */
  std::map<std::string,std::map<std::string,Pointer<Schemafactory> > > const&
    get_factories() const throw();

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
  
  Pointer<Instanceinfo> get_instanceinfo_i(const sid_t sid) throw(std::logic_error);

  std::vector<sid_t> const& get_brothers_i(const sid_t sid) const throw(std::logic_error);

  std::vector<sid_t> const& get_children_i(const sid_t sid) const throw(std::logic_error);

  state_enum get_state_i(const sid_t sid) const throw(std::logic_error);

  Pointer<Schemainstance> get_instance_i(const sid_t sid) throw(std::logic_error);

  Pointer<Isc> get_isc_i(const sid_t sid) throw();
};

#endif /*HIERARCHY_HPP*/


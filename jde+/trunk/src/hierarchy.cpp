#include <jde+/hierarchy.h>
#include <jde+/isc.h>
#include <jde+/message.h>
#include <jde+/schemainstance.h>
#include <jde+/schemafactory.h>
#include <jde+/schemainterface.h>
#include "util.h"
#include "loader.h"
#include <iostream>
#include <stdexcept>
#include <cc++/thread.h>
#include <cc++/file.h>
#ifdef CCXX_NAMESPACES
using ost::Mutex;
using ost::MutexLock;
using ost::Dir;
#endif

class Hierarchy_Pimpl {
public:
  Pointer<Isc> hierarchy_isc;
  std::string root_interface_name;
  std::string root_branch;

  /**
     Hierarchy owned instance infos
  */
  std::map<sid_t,Pointer<Instanceinfo> > instanceinfos;

  
  /**
     Hierarchy owned instances
  */
  std::map<sid_t,Pointer<Schemainstance> > instances;

  /**
    Map of known factories.
    Indexed first by interface_name, second by branch
  */
  std::map<std::string,std::map<std::string,Pointer<Schemafactory> > > factories;

  /**
    Loader stores dynamically loaded schemas code
  */
  Loader<Schemafactory> schemaloader;

  /**
    Next sid to be used
  */
  sid_t next_sid;

  
  /**
    Mutex and conditions
  */
  mutable Mutex monitor_lock;

  /**
     Extra message processors added
  */
  std::vector<Message_Processor*> processors;
};


Hierarchy::Instanceinfo::Instanceinfo(const sid_t sid, const Pointer<Instanceinfo> father,
				      const Pointer<Schemafactory> factory)
  :sid(sid),father(father),factory(factory) { }


Hierarchy::Instanceinfo::link_child(const sid_t child_sid, const std::string child_nick) {
  std::pair<sid2nick_map_t::iterator,bool> s2n_insert;
  std::pair<nick2sid_map_t::iterator,bool> n2s_insert;

  s2n_insert = 
    sid2nick.insert(sid2nick_map_t::value_type::pair(child_sid,child_nick));
  if ( ! s2n_insert.second ) /*child sid already exists*/
    throw std::logic_error("Hierarchy: child '" + sid_to_str(child_sid) + "' already linked");

  n2s_insert = 
    nick2sid.insert(nick2sid_map_t::value_type::pair(child_nick,
						     child_sid));
  if ( ! n2s_insert.second ) /*child nick exists*/
    throw std::logic_error("Hierarchy: child '" + child_nick + "' already linked");
  children.push_back(child_sid);
  children_nicks.push_back(child_nick);
}

std::string
Hierarchy::Instanceinfo::get_child_nick(const sid_t child_sid) const {
  sid2nick_map_t::const_iterator s2n_it = 
    sid2nick.find(child_sid);

  if ( s2n_it == sid2nick.end() )
    throw std::logic_error("Hierarchy: instance hasn't got this child sid:" + sid_to_str(child_sid));
  return s2n_it->second;
}

sid_t
Hierarchy::Instanceinfo::get_child_sid(const string child_nick) const {
  nick2sid_map_t::const_iterator n2s_it = 
    nick2sid.find(child_nick);

  if ( n2s_it == nick2sid.end() )
    throw std::logic_error("Hierarchy: instance hasn't got this child nick:" + child_nick);
  return n2s_it->second;
}

const std::vector<sid_t>&
Hierarchy::Instanceinfo::get_children() const {
  return children;
}

const std::vector<std::string>&
Hierarchy::Instanceinfo::get_children_nicks() const {
  return children_nicks;
}

bool Hierarchy::Instanceinfo::is_child(const sid_t sid) const {
  return (std::find(children.begin(),
		    children.end(),sid) != children.end());
}

bool Hierarchy::Instanceinfo::is_child(const string child_nick) const {
  nick2sid_map_t::const_iterator n2s_it = 
    nick2sid.find(child_nick);

  return (n2s_it != nick2sid.end());
}

std::vector<sid_t>
Hierarchy::Instanceinfo::get_brothers() const {
  if (sid == ROOT) /*root instance hasn't got brothers*/
    return std::vector<sid_t>::vector(0);
  
  std::vector<sid_t>::const_iterator f_c_it;
  std::vector<sid_t> brothers;

  /*delete my sid from father children list*/
  for (f_c_it = father->children.begin();
       f_c_it != father->children.end();
       f_c_it++) {
    if (*f_c_it != sid)
      brothers.push_back(*f_c_it);
  }
  return brothers;
}

const double Hierarchy::tstart = gettimeofdaysec();

Hierarchy::Hierarchy(const std::string& root_interface_name,
		     const std::string& root_branch,
		     const bool debugflag)
  : next_sid(ROOT+1),debug(debugflag),pimpl(new Hierarchy_Pimpl()) { 
  pimpl->root_interface_name = root_interface_name;
  pimpl->root_branch = root_branch;
}


/*FIXME: repasar parada*/
Hierarchy::~Hierarchy() {
  if (debug) std::cerr << "Hierarchy: stoping schemas\n";
  stop();/*stop schemas*/
  /*clear maps before loader dtor*/
  pimpl->instanceinfos.clear();
  pimpl->instances.clear();
  pimpl->factories.clear();
}

void Hierarchy::start() {
  MutexLock l(pimpl->monitor_lock);

  start_i();
}

void Hierarchy::stop() {
  MutexLock l(pimpl->monitor_lock);

  stop_i();
}

const Pointer<Instanceinfo> Hierarchy::get_instanceinfo(const sid_t sid) {
  MutexLock l(pimpl->monitor_lock);

  return get_instanceinfo_i(sid);
}

int Hierarchy::get_nbrothers(const sid_t sid) const {
  MutexLock l(pimpl->monitor_lock);

  return get_brothers_i(sid).size();
}

std::vector<sid_t> const&
Hierarchy::get_brothers(const sid_t sid) {
  MutexLock l(pimpl->monitor_lock);

  return get_brothers_i(sid);
}


int Hierarchy::get_nchildren(const sid_t sid) {
  MutexLock l(pimpl->monitor_lock);

  return get_children_i(sid).size();
}

std::vector<sid_t> const&
Hierarchy::get_children(const sid_t sid) const {
  MutexLock l(pimpl->monitor_lock);

  return get_children_i(sid);
}

state_enum
Hierarchy::get_state(const sid_t sid) const {
  MutexLock l(pimpl->monitor_lock);

  return get_state_i(sid);
}

Pointer<Schemainstance>
Hierarchy::get_instance(const sid_t sid) {
  MutexLock l(pimpl->monitor_lock);

  return get_instance_i(sid);
}

Pointer<Isc> Hierarchy::get_isc(const sid_t sid) {
  MutexLock l(pimpl->monitor_lock);
  
  return get_isc_i(sid);
}

void Hierarchy::start_i() {
  Pointer<Instanceinfo> root_instance(new Instanceinfo(ROOT,0,0));

  pimpl->instanceinfos[ROOT] = root_instance;
  root_instance = get_instanceinfo_i(ROOT);

  if (root_instance->factory.get() == NULL)
    throw std::runtime_error("Hierarchy: can't create root instance. No factories found!");

  /*change root_instance to alert/active state (motor/perceptive)*/
  /*src=dst ok because root_instance is its own father*/
  Pointer<Isc> isc = get_isc_i(ROOT);
  Pointer<State_Change> state_message;

  if (root_instance->factory->get_interface_info().type == PSCHEMA)
    state_message = Pointer<State_Change>::Pointer(new State_Change(ROOT,ROOT,ACTIVE));
  else
    state_message = Pointer<State_Change>::Pointer(new State_Change(ROOT,ROOT,ALERT));
  isc->process(state_message);/*send message*/
}

void Hierarchy::stop_i() {
  /*FIXME: implementar parada*/
}

/*get instance info and fills it*/
Pointer<Instanceinfo> Hierarchy::get_instanceinfo_i(const sid_t sid) {
  std::map<sid_t,Pointer<Instanceinfo> >::iterator i_it;
  i_it = pimpl->instanceinfos.find(sid);

  if ( i_it == pimpl->instanceinfos.end() )
    throw std::logic_error("Hierarchy: sid '" + sid_to_str(sid)
			   + "' unknown");

  Pointer<Instanceinfo> instanceinfo(*i_it);

  if (sid == ROOT){/*root instance must be initialized*/
    std::vector<std::string> schema_branch_prefs;

    schema_branch_prefs.push_back(pimpl->root_branch);
    instanceinfo->factory = get_factory(pimpl->root_interface_name,
					schema_branch_prefs);
  }

  /*factory not found. Return known info, caller must supose this
    instance is disabled*/
  if (instanceinfo->factory.get() == 0)
    return instanceinfo;

  /*create children instance infos*/
  std::vector<Schemafactory::interfaceinfo_t>::const_iterator ci_it;
  for (ci_it = instanceinfo->factory->get_children_interface_infos().begin();
       ci_it != instanceinfo->factory->get_children_interface_infos().end();
       ci_it++) {
    Pointer<Schemafactory> child_factory = 
      get_factory(ci_it->interfaceinfo.name,
		  instanceinfo->factory->get_child_branch_preferences(ci_it->interfaceinfo.nick));

    if (child_factory.get() == NULL)
      if (debug) std::cerr << "Hierarchy: can't get factory for child instance " 
			   << ci_it->interfaceinfo.name
			   << ". We can continue, but this child will be unavailable\n";
    Pointer<Instanceinfo> child_instanceinfo= new Instanceinfo(next_sid++,instanceinfo,child_factory);
    
    /*insert child*/
    pimpl->instanceinfos[child_instanceinfo->sid] = child_instanceinfo;
    /*link child*/
    instanceinfo->link_child(child_instanceinfo->sid,ci_it->nick);
  }
  return instanceinfo;
}

std::vector<sid_t> const&
Hierarchy::get_brothers_i(const sid_t sid) const{
  return get_instanceinfo_i(sid).get_brothers();
}

std::vector<sid_t> const&
Hierarchy::get_children_i(const sid_t sid) const{
  return get_instanceinfo_i(sid).get_children();
}

state_enum
Hierarchy::get_state_i(const sid_t sid) const{
  std::map<sid_t,Pointer<Schemainstance> >::const_iterator i_it =
    pimpl->instances.find(sid);

  if (i_it == pimpl->instances.end() || i_it->second.get() == 0)
    return SLEPT;
  else
    return i_it->second->get_state();
}

Pointer<Schemainstance>
Hierarchy::get_instance_i(const sid_t sid) {
  Pointer<Instanceinfo> instanceinfo(get_instanceinfo_i(sid));
  std::map<sid_t,Pointer<Schemainstance> >::iterator i_it =
    pimpl->instances.find(sid);

  if (i_it != pimpl->instances.end())
    return i_it->second;

  Pointer<Schemainstance> instance(0);

  if (instanceinfo->factory.get() != NULL) {
    /*create instance*/
    if (debug) std::cerr << "Hierarchy: creating instance("
			 << sid_to_str(sid) << "). Interface: '"
			 << instanceinfo->factory->get_interface_info().name << "', Branch: '" 
			 << instanceinfo->factory->get_implementation_branch() << "\n";
    instance = Pointer<Schemainstance>::Pointer(new Schemainstance(sid,
								   instanceinfo.father->sid,
								   this,debug));
    instances[sid] = instance;
  }
  return instance;
}

Pointer<Isc> Hierarchy::get_isc_i(const sid_t sid) {
  /*Implementation only use one isc*/
  if (hierarchy_isc.get() == NULL){
    hierarchy_isc = Pointer<Isc>::Pointer(new Isc(this,debug));
    std::vector<Message_Processor*>::iterator mp_it;
    /*append all registered processors*/
    for (mp_it = processors.begin();
	 mp_it != processors.end();
	 mp_it++)
      hierarchy_isc->append_processor(*mp_it);
  }
  return hierarchy_isc;
}

Pointer<Schemafactory> 
Hierarchy::load_factory(const std::string file_path) {
  Pointer<Schemafactory> factory_ptr;

  try {
    factory_ptr = schemaloader.get_factory(file_path);
    if (debug) std::cerr << "Hierarchy: loading " << file_path
			 << " .Interface '"
			 << factory_ptr->get_interface_name()
			 << "' interface, Branch '"
			 << factory_ptr->get_implementation_branch() << "'\n"; 
  } catch(std::runtime_error& e) {
    if (debug) std::cerr << "Hierarchy: error loading " << file_path << ":\n"
			 << "\t" << e.what() << "\n";
    return Pointer<Schemafactory>::Pointer(NULL);
  } catch(...) {
    if (debug) std::cerr << "Hierarchy: unexpected exception when trying to load "
			 << file_path << "\n";
    return Pointer<Schemafactory>::Pointer(NULL);
  }

  /*index factory. first by interface_name, second by branch*/
  std::map<std::string,Pointer<Schemafactory> >& fb = 
    factories[factory_ptr->get_interface_name()];
  std::map<std::string,Pointer<Schemafactory> >::const_iterator fb_it;
  
  fb_it = fb.find(factory_ptr->get_implementation_branch());
  /*interface factory not registered yet*/
  if (fb_it == fb.end()){/*new branch*/
    fb[factory_ptr->get_implementation_branch()] = factory_ptr;
  }else{/*else unload library*/
    if (debug) std::cerr << "Hierarchy: " << file_path
			 << "contains a factory already loaded. Unloaded\n";
    schemaloader.unload_dll(file_path);
    return Pointer<Schemafactory>::Pointer(NULL);
  }
  return factory_ptr;
}

std::vector<Pointer<Schemafactory> >
Hierarchy::load_factories(const std::string dir_path,
			  const std::string ext) {
  std::vector<Pointer<Schemafactory> > loaded_factories;
  try{
    Dir dir(dir_path.c_str());
    if (!dir) {
      if (debug) std::cerr << "Hierarchy: Can't open dir " << dir_path
			   << ". Ignored\n";
      return loaded_factories;//return empty vector
    }
      
    if (debug) std::cerr << "Hierarchy: Loading schemas from " << dir_path << "\n";

    const char* dname;
    while((dname = dir.getName()) != NULL) {
      std::string file_name(dname);
      std::string file_path(dir_path + "/" + file_name);
      std::string::size_type ppos;

      if (ext.size() > 0) { /* ext not empty, check file name*/
	/*only files with ext at the end are loaded*/
	ppos = file_name.find_last_of(".");
	if ((ppos == std::string::npos) or
	    ((file_name.substr(ppos)) != ext)) {
	  if (debug) std::cerr << "Hierarchy: ignoring file " << file_path << "\n";
	  continue;
	}
      }
      
      Pointer<Schemafactory> factory_ptr = load_factory(file_path);
      if (factory_ptr.get() != NULL)
	loaded_factories.push_back(factory_ptr);
    }
  } catch(...) {
    if (debug) std::cerr << "Hierarchy: unexpected exception when reading directory\n";
  }
  if (debug) std::cerr << "Hierarchy: " << loaded_factories.size() << " loaded successfully\n";

  return loaded_factories;
}

Pointer<Schemafactory> const
Hierarchy::get_factory(const std::string interface_name, 
		       const std::vector<std::string>& branch_preferences) const{
  std::map<std::string,std::map<std::string,
    Pointer<Schemafactory> > >::const_iterator fi_it;

  fi_it = factories.find(interface_name);
  if (fi_it == factories.end() or fi_it->second.empty())
    return Pointer<Schemafactory>::Pointer(NULL);
  
  std::vector<std::string>::const_iterator br_it;
  std::map<std::string,Pointer<Schemafactory> >::const_iterator fb_it;

  fb_it = fi_it->second.begin();/*default=first*/
  for (br_it = branch_preferences.begin();
       br_it != branch_preferences.end();
       br_it++){
    std::map<std::string,Pointer<Schemafactory> >::const_iterator tmp_it;
    
    if ((tmp_it = (fi_it->second).find(*br_it)) != fi_it->second.end()){
      fb_it = tmp_it;
      break;/*found it*/
    }
  }
  return fb_it->second;
}

std::map<std::string,std::map<std::string,Pointer<Schemafactory> > > const&
Hierarchy::get_factories() const{
  return factories;
}

void Hierarchy::append_processor(Message_Processor* mp){
  /*load a new processor*/
  processors.push_back(mp);
}

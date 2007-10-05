#include <jde+/hierarchy.h>
#include <jde+/isc.h>
#include <jde+/message.h>
#include <jde+/schemainstance.h>
#include <jde+/schemainterface.h>
#include <jde+/util.h>
#include <iostream>
#include <stdexcept>
#include <ltdl.h>
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
  std::map<sid_t,Pointer<Hierarchy::Instanceinfo> > instanceinfos;

  
  /**
     Hierarchy owned instances
  */
  std::map<sid_t,Pointer<Schemainstance> > instances;

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

  /*
    Flag for system running status
  */
  bool system_running;

  /*
    Open libraries
  */
  static Mutex loaded_libs_lock;
  static std::map<std::string,lt_dlhandle> loaded_libs;
};

/*static definitions*/
Mutex Hierarchy_Pimpl::loaded_libs_lock;
std::map<std::string,lt_dlhandle> Hierarchy_Pimpl::loaded_libs;

Hierarchy::Instanceinfo::Instanceinfo(const sid_t sid, const sid_t father_sid,
				      const std::string& interface_name,
				      const std::vector<std::string>& branch_preferences)
  :sid(sid),father_sid(father_sid),interface_name(interface_name),branch_preferences(branch_preferences) { }


void 
Hierarchy::Instanceinfo::link_child(const sid_t child_sid) throw(std::logic_error) {
  if (children.count(child_sid) != 0)
    throw std::logic_error("Hierarchy::Instanceinfo: sid already linked");
  children.insert(child_sid);
}

const std::set<sid_t>&
Hierarchy::Instanceinfo::get_children() const throw(){
  return children;
}

bool Hierarchy::Instanceinfo::is_child(const sid_t sid) const throw(){
  return (children.count(sid) != 0);
}


const double Hierarchy::tstart = gettimeofdaysec();

Hierarchy::Hierarchy(const std::string& root_interface_name,
		     const std::string& root_branch,
		     const bool debugflag) throw()
  : debug(debugflag),pimpl(new Hierarchy_Pimpl()) { 
  pimpl->root_interface_name = root_interface_name;
  pimpl->root_branch = root_branch;
  pimpl->next_sid = ROOT+1;
  pimpl->system_running = false;

  /*initialize dl*/
  if (lt_dlinit() != 0){
    std::cerr << "Hierarchy: can't init ltdl";
    std::unexpected();
  }
}


/*FIXME: repasar parada*/
Hierarchy::~Hierarchy() {
  if (debug) std::cerr << "Hierarchy: stoping schemas\n";
  stop();/*stop schemas*/
  /*clear maps before loader dtor*/
  pimpl->instanceinfos.clear();
  pimpl->instances.clear();

  std::map<std::string,lt_dlhandle>::iterator l_it;
  for (l_it = pimpl->loaded_libs.begin();
       l_it != pimpl->loaded_libs.end();
       l_it++)
    lt_dlclose(l_it->second);
  /*exit dl*/
  lt_dlexit();
}

void Hierarchy::start() throw(std::runtime_error){
  MutexLock l(pimpl->monitor_lock);

  start_i();
}

void Hierarchy::stop() throw(){
  MutexLock l(pimpl->monitor_lock);

  stop_i();
}

const Hierarchy::Instanceinfo*
Hierarchy::get_instanceinfo(const sid_t sid) const throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return get_instanceinfo_i(sid);
}

int Hierarchy::get_nbrothers(const sid_t sid) const throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return get_brothers_i(sid).size();
}

std::set<sid_t>
Hierarchy::get_brothers(const sid_t sid) const throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return get_brothers_i(sid);
}


int Hierarchy::get_nchildren(const sid_t sid) const throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return get_children_i(sid).size();
}

std::set<sid_t> const&
Hierarchy::get_children(const sid_t sid) const throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return get_children_i(sid);
}

state_enum
Hierarchy::get_state(const sid_t sid) const throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return get_state_i(sid);
}

Pointer<Schemainstance>
Hierarchy::get_instance(const sid_t sid) throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return get_instance_i(sid);
}

Pointer<Isc> Hierarchy::get_isc(const sid_t sid) throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);
  
  return get_isc_i(sid);
}

sid_t Hierarchy::add_child(const sid_t sid,
			   const std::string& interface_name,
			   const std::vector<std::string>& branch_preferences) throw(std::logic_error){
  MutexLock l(pimpl->monitor_lock);

  return add_child_i(sid,interface_name,branch_preferences);
}

void Hierarchy::start_i() throw(std::runtime_error){
  std::vector<std::string> schema_branch_prefs;
  schema_branch_prefs.push_back(pimpl->root_branch);

  Pointer<Instanceinfo> root_instance(new Instanceinfo(ROOT,ROOT,pimpl->root_interface_name,
						       schema_branch_prefs));

  pimpl->instanceinfos[ROOT] = root_instance;
  
  /*change root_instance to alert/active state (motor/perceptive)*/
  /*src=dst ok because root_instance is its own father*/
  Pointer<Isc> isc = get_isc_i(ROOT);
  Pointer<State_Change> state_message;
  const Schemainterface::schemainterface_info_t* root_interface_class_info =
    Schemainterface::class_info(pimpl->root_interface_name);

  if (!root_interface_class_info)
    throw std::runtime_error("Hierarchy::start_i: can't find root interface class");

  if (root_interface_class_info->type == PSCHEMA)
    state_message = Pointer<State_Change>::Pointer(new State_Change(ROOT,ROOT,ACTIVE));
  else
    state_message = Pointer<State_Change>::Pointer(new State_Change(ROOT,ROOT,ALERT));
  isc->process(state_message);/*send message*/
  pimpl->system_running = true;
}

void Hierarchy::stop_i() throw(){
  /*FIXME: implementar parada*/
  pimpl->system_running = false;
}

/*get instance info and fills it*/
Hierarchy::Instanceinfo*
Hierarchy::get_instanceinfo_i(const sid_t sid) const throw(std::logic_error){
  std::map<sid_t,Pointer<Instanceinfo> >::iterator i_it;
  i_it = pimpl->instanceinfos.find(sid);

  if ( i_it == pimpl->instanceinfos.end() )
    throw std::logic_error("Hierarchy::get_instanceinfo_i: schema instance " + sid_to_str(sid) + " unknown");

  return i_it->second.get();
}

std::set<sid_t>
Hierarchy::get_brothers_i(const sid_t sid) const throw(std::logic_error){
  if (sid == ROOT) /*root instance hasn't got brothers*/
    return std::set<sid_t>::set();
  
  std::set<sid_t>::const_iterator f_c_it;
  std::set<sid_t> brothers;

  const Instanceinfo* info = get_instanceinfo_i(sid);
  const Instanceinfo* f_info;
  try{
    f_info = get_instanceinfo_i(info->father_sid);
  }catch(std::logic_error& e){
    std::cerr << "Hierarchy::get_brothers_i: can't get father instance info.\n";
    std::unexpected();
  }

  /*delete my sid from father children list*/
  for (f_c_it = f_info->get_children().begin();
       f_c_it != f_info->get_children().end();
       f_c_it++) {
    if (*f_c_it != sid)
      brothers.insert(*f_c_it);
  }
  return brothers;
}

std::set<sid_t> const&
Hierarchy::get_children_i(const sid_t sid) const throw(std::logic_error){
  return get_instanceinfo_i(sid)->get_children();
}

state_enum
Hierarchy::get_state_i(const sid_t sid) const throw(std::logic_error){
  std::map<sid_t,Pointer<Schemainstance> >::const_iterator i_it =
    pimpl->instances.find(sid);

  if (i_it == pimpl->instances.end())
    throw std::logic_error("Hierarchy::get_state_i: schema instance " + sid_to_str(sid) + " unknown");

  if (i_it->second.get() == 0)
    return SLEPT;
  else
    return i_it->second->get_state();
}

Pointer<Schemainstance>
Hierarchy::get_instance_i(const sid_t sid) throw(std::logic_error){
  std::map<sid_t,Pointer<Schemainstance> >::iterator i_it =
    pimpl->instances.find(sid);

  if (i_it != pimpl->instances.end())
    return i_it->second;

  const Instanceinfo* instanceinfo = get_instanceinfo_i(sid);

  /*create instance*/
  if (debug) std::cerr << "Hierarchy::get_instance_i: creating instance("
		       << sid_to_str(sid) << "). Interface: '"
		       << instanceinfo->interface_name << "\n";
  Schemainstance* instance_p = new Schemainstance(sid,this,debug);
  Pointer<Schemainstance> instance(instance_p);
  pimpl->instances[sid] = instance;
  return instance;
}

Pointer<Isc> Hierarchy::get_isc_i(const sid_t sid) throw(std::logic_error){
  /*Implementation only use one isc*/
  if (pimpl->hierarchy_isc.get() == 0){
    pimpl->hierarchy_isc = Pointer<Isc>::Pointer(new Isc(this,debug));
    std::vector<Message_Processor*>::iterator mp_it;
    /*append all registered processors*/
    for (mp_it = pimpl->processors.begin();
	 mp_it != pimpl->processors.end();
	 mp_it++)
      pimpl->hierarchy_isc->append_processor(*mp_it);
  }
  return pimpl->hierarchy_isc;
}

int Hierarchy::load_library(const std::string file_path) throw() {
  if (pimpl->system_running) {/*FIXME: repasar condiciones de carrera*/
    if (debug) std::cerr << "Hierarchy::load_library: system running, can't load library.\n";
    return -1;
  }

  {
    MutexLock l(Hierarchy_Pimpl::loaded_libs_lock);
    if (Hierarchy_Pimpl::loaded_libs.count(file_path) == 0) {
      lt_dlhandle handle;

      handle = lt_dlopen(file_path.c_str());
      if (handle == NULL){
	if (debug) std::cerr << "Hierarchy::load_library: Can't open library.(" 
			     << std::string(lt_dlerror()) 
			     << ")";
	return -1;
      }
      Hierarchy_Pimpl::loaded_libs[file_path] = handle;
      lt_dlerror();/*clean error*/
      return 1;
    }
    return 0;
  }
}

int Hierarchy::load_libraries(const std::string dir_path,
			      const std::string ext) throw() {
  if (pimpl->system_running) {
    if (debug) std::cerr << "Hierarchy::load_libraries: system running, can't load libraries.\n";
    return -1;
  }

  int nloaded;
  try{
    Dir dir(dir_path.c_str());
    if (!dir) {
      if (debug) std::cerr << "Hierarchy::load_libraries: Can't open dir " << dir_path
			   << ". Ignored\n";
      return -1;
    }
      
    if (debug) std::cerr << "Hierarchy::load_libraries: Loading schemas from " << dir_path << "\n";

    const char* dname;
    while((dname = dir.getName()) != 0) {
      std::string file_name(dname);
      std::string file_path(dir_path + "/" + file_name);
      std::string::size_type ppos;

      if (ext.size() > 0) { /* ext not empty, check file name*/
	/*only files with ext at the end are loaded*/
	ppos = file_name.find_last_of(".");
	if ((ppos == std::string::npos) or
	    ((file_name.substr(ppos)) != ext)) {
	  if (debug) std::cerr << "Hierarchy::load_libraries: ignoring file " << file_path << "\n";
	  continue;
	}
      }
      if (load_library(file_path) > 0) {
	nloaded++;
	if (debug) std::cerr << "Hierarchy::load_libraries: " << file_path << " loaded successfully.\n";
      }
    }
  } catch(...) {
    if (debug) std::cerr << "Hierarchy::load_libraries: unexpected exception when reading directory\n";
  }
  if (debug) std::cerr << "Hierarchy::load_libraries: " << nloaded << " libraries loaded successfully\n";

  return nloaded;
}

std::vector<std::string>
Hierarchy::get_loaded_libraries() const throw() {
  std::map<std::string,lt_dlhandle>::const_iterator ll_it;
  std::vector<std::string> ll;

  {
    MutexLock l(Hierarchy_Pimpl::loaded_libs_lock);
  
    for (ll_it = Hierarchy_Pimpl::loaded_libs.begin();
	 ll_it != Hierarchy_Pimpl::loaded_libs.end();
	 ll_it++)
      ll.push_back(ll_it->first);
  }

  return ll;
}

void Hierarchy::append_processor(Message_Processor* mp) throw(){
  /*load a new processor*/
  pimpl->processors.push_back(mp);
}


sid_t Hierarchy::add_child_i(const sid_t sid,
			     const std::string& interface_name,
			     const std::vector<std::string>& branch_preferences) throw(std::logic_error){
  sid_t child_sid = pimpl->next_sid++;
  Instanceinfo* info = get_instanceinfo_i(sid);

  info->link_child(child_sid);

  /*create instance info record*/
  pimpl->instanceinfos[child_sid] = Pointer<Instanceinfo>::Pointer(new Instanceinfo(child_sid,sid,
										    interface_name,
										    branch_preferences));
  return child_sid;
}

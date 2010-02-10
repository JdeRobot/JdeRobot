#include <jde+/schemainstance.h>
#include <jde+/schemainstancefacade.h>
#include <jde+/schemainterface.h>
#include <jde+/hierarchy.h>
#include <jde+/isc.h>
#include <jde+/schemastate.h>
#include <jde+/schemaimplementation.h>
#include <jde+/conditionallock.h>

#include <stdexcept>
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <algorithm>

#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::Mutex;
using ost::MutexLock;
using ost::Conditional;
using ost::Thread;
#endif

class Schemainstance_Pimpl: public Thread {
public:
  Schemainstance_Pimpl(const sid_t sid, Hierarchy* owner, Schemainstance* instance) throw();
  ~Schemainstance_Pimpl() throw() { terminate();/*finish thread*/ }

  /*iteration thread function*/
  void run() throw();

  sid_t get_child_sid(const std::string& child_nick) throw(std::logic_error);
  const std::string& get_child_nick(const sid_t child_sid) const throw();

  typedef std::map<std::string,sid_t> nick2sid_map_t;
  
  struct state_cond_t{
    mutable Conditional cond;
    state_enum state;
  };
  
  struct child_t{
    const Schemainterface::schemainterface_info_t* interface_info;
    std::string nick;
  };

  const sid_t mysid;
  Hierarchy* const myowner;
  Schemainstance* const myinstance;
  int cycle_time;/*in ms*/

  /*Controls instance state*/
  state_cond_t state_change;
  Schemastate* state;

  /*instance interfaces (main and children)*/
  const Schemainterface::schemainterface_info_t* interface_info;/*interaface implemented by this
								    schema*/
  std::map<sid_t,child_t> children;/*child interfaces*/
  nick2sid_map_t nick2sid;/*cache nick->sidtranslation*/

  Pointer<PrivateData> private_data;
  
  Pointer<Schemaimplementation> implementation;
  
  mutable Mutex monitor_lock;

  /*rcv queues*/
  Message_Queue<Pointer<Perception> > perceptions_queue;
  Message_Queue<Pointer<Modulation> > modulations_queue;
  Message_Queue<Pointer<Arbitrate_Req> > arbitrate_req_queue;
  Message_Queue<Pointer<New_State> > new_state_queue;

  /*schemaimplementation reuse, classname indexed*/
  static std::map<std::string,Pointer<Schemaimplementation> > implementations;
  static Mutex reuse_lock;
};

/*schamainstance static definitions*/
std::map<std::string,Pointer<Schemaimplementation> > Schemainstance_Pimpl::implementations;
Mutex Schemainstance_Pimpl::reuse_lock;

Schemainstance_Pimpl::Schemainstance_Pimpl(const sid_t sid,
					   Hierarchy* owner, Schemainstance* instance) throw()
  :mysid(sid),myowner(owner),
   myinstance(instance),interface_info(0),implementation(0) { 
  /*don't call Hierarchy methods because deadlock*/ }

void Schemainstance_Pimpl::run() throw(){
  const Hierarchy::Instanceinfo* i_info;
  try{
    i_info = this->myowner->get_instanceinfo(mysid);
  }catch(std::logic_error& e){
    std::cerr << "Schemainstance_Pimpl::run: " << e.what() << "\n";
    std::unexpected();
  }

  /*get interface info*/
  interface_info = Schemainterface::class_info(i_info->interface_name);
  if (interface_info == 0) {
    std::cerr << "Schemainstance_Pimpl::run: can't get interface info: " 
	      << i_info->interface_name << "\n";
    std::unexpected();
  }

  if (interface_info->type == PSCHEMA)
    state = Pschemaslept::instance();
  else/*MSCHEMA*/
    state = Mschemaslept::instance();

  state_change.state = SLEPT;

  try {
    while(1){
      /*exec actions*/
      if (myinstance->debug) std::cerr << "Schemainstance::run: (" << sid_to_str(mysid)
				       << ") executing actions\n";
      state->exec_actions(myinstance);
      
      /*deliver state change*/
      {
	ConditionalLock cl(state_change.cond);
       
	if (myinstance->debug) std::cerr << "Schemainstance::run: (" << sid_to_str(mysid)
					 << ") delivering state changes\n";
	/*received new state??*/
	if (state_change.state != state->get_state_name())
	  state->receive_state_change(myinstance,state_change.state);
      }

      /*deliver child new states*/
      if (myinstance->debug) std::cerr << "Schemainstance::run: (" << sid_to_str(mysid)
				       << ") delivering children new states\n";
      int i,qsize = new_state_queue.size();
      for (i = 0; i < qsize; i++) {
	Pointer<New_State> n = new_state_queue.front();
	new_state_queue.pop();
	state->receive_child_new_state(myinstance, get_child_nick(n->src),n->state);
      }
	
      /*deliver arbitrate req*/
      if (myinstance->debug) std::cerr << "Schemainstance::run: (" << sid_to_str(mysid)
				       << ") delivering arbitrate requests\n";
      qsize = arbitrate_req_queue.size();
      for (i = 0; i < qsize; i++){
	Pointer<Arbitrate_Req> a_req = arbitrate_req_queue.front();
	arbitrate_req_queue.pop();
	state->receive_arbitrate_request(myinstance, get_child_nick(a_req->src));
      }

      /*deliver perceptions*/
      if (myinstance->debug) std::cerr << "Schemainstance::run: (" << sid_to_str(mysid)
				       << ") delivering perceptions\n";
      qsize = perceptions_queue.size();
      for (i = 0; i < qsize; i++){
	Pointer<Perception> p = perceptions_queue.front();
	perceptions_queue.pop();
	state->receive_perception(myinstance, get_child_nick(p->src), p->name, p->data);
      }

      /*deliver modulations*/
      if (myinstance->debug) std::cerr << "Schemainstance::run: (" << sid_to_str(mysid)
				       << ") delivering modulations\n";
      qsize = modulations_queue.size();
      for (i = 0; i < qsize; i++){
	Pointer<Modulation> m = modulations_queue.front();
	modulations_queue.pop();
	state->receive_modulation(myinstance, m->name, m->data);
      }
      yield();
    }
  } catch(std::exception& e) {
    if (myinstance->debug) std::cerr << "Schemainstance::run: exception raised on iteration thread: " 
				     << e.what() << "\n";
    std::unexpected();
  }catch(...) {
    if (myinstance->debug) std::cerr << "Schemainstance::run: unknown exception raised on iteration thread\n";
    std::unexpected();
  }
}

sid_t 
Schemainstance_Pimpl::get_child_sid(const std::string& child_nick)
  throw(std::logic_error) {
  nick2sid_map_t::const_iterator n2s_it;

  /*exists child?*/
  n2s_it = this->nick2sid.find(child_nick);
  if (n2s_it != this->nick2sid.end())
    return n2s_it->second;

  /*get implementation info*/
  const Schemaimplementation::schemaimplementation_info_t* impl_info;
  if (this->implementation.get() == 0){ /*impl not instanciated yet*/
    const Hierarchy::Instanceinfo* i_info;
    try{
      i_info = this->myowner->get_instanceinfo(this->mysid);
    }catch(std::logic_error& e){
      std::cerr << "Schemainstance_Pimpl::get_child_sid: " << e.what() << "\n";
      std::unexpected();
    }
    impl_info =
      Schemaimplementation::class_info_search(i_info->interface_name,
					     i_info->branch_preferences);
  }else
    impl_info = this->implementation->implementation_info;

  if (impl_info == 0){
    std::cerr << "Schemainstance_Pimpl::get_child_sid: implementation class info not found\n";
    std::unexpected();
  }

  /*have i got this child?*/
  std::map<std::string, Schemaimplementation::child_info_t>::const_iterator c_it =
    impl_info->children_infos.find(child_nick);
  if (c_it == impl_info->children_infos.end())
    throw std::logic_error("Schemainstance_Pimpl::get_child_sid: child '" + child_nick + "' unknown");

  /*ask hierarchy for child*/
  sid_t child_sid = this->myowner->add_child(this->mysid,
					     c_it->second.interface_name,
					     c_it->second.prefered_branches);

  const Schemainterface::schemainterface_info_t* child_interface_info = 
    Schemainterface::class_info(c_it->second.interface_name);
  if (child_interface_info == 0) {
    std::cerr << "Schemainstance_Pimpl::get_child_sid: can't get interface info for child: " 
	      << sid_to_str(child_sid) << " with interface_name: " << c_it->second.interface_name << ". Child disabled\n";
  }

  this->nick2sid[child_nick] = child_sid;
  child_t c = { child_interface_info, child_nick };
  this->children[child_sid] = c;

  return child_sid;
}

const std::string&
Schemainstance_Pimpl::get_child_nick(const sid_t child_sid) const throw(){
  std::map<sid_t,child_t>::const_iterator c_it =
    this->children.find(child_sid);

  if (c_it == this->children.end()) {
    std::cerr << "Schemainstance_Pimpl::get_child_nick: child unknown\n";
    std::unexpected();
  }

  return c_it->second.nick;
}

const double& Schemainstance::tstart = Hierarchy::tstart;

Schemainstance::Schemainstance(const sid_t sid,
                               Hierarchy* owner,
			       const bool debugflag) throw()
  :debug(debugflag),pimpl(new Schemainstance_Pimpl(sid,owner,this)) {
  
  /*set default cycle_time*/
  set_cycle_time(100);

  if (debug) std::cerr << "Schemainstance: starting thread\n";
  pimpl->start();
}

Schemainstance::~Schemainstance() throw() {
  if (debug) std::cerr << "Schemainstance: releasing pimpl\n";
  delete pimpl;
}

sid_t Schemainstance::get_sid() const throw(){
  return pimpl->mysid;
}


state_enum Schemainstance::get_state() const throw(){
  /*begin critical section*/
  ConditionalLock l(pimpl->state_change.cond);

  return pimpl->state->get_state_name();
  /*end critical section*/
}

int Schemainstance::get_cycle_time() const throw(){
  return pimpl->cycle_time;
}

void Schemainstance::set_cycle_time(const int c) throw(){
  pimpl->cycle_time = c;
}

void Schemainstance::send_arbitrate_request() const throw(){
  Pointer<Isc> myisc = pimpl->myowner->get_isc(pimpl->mysid);
  sid_t father_sid;
  try{
    father_sid = pimpl->myowner->get_instanceinfo(pimpl->mysid)->father_sid;
  }catch(std::logic_error& e){
    std::cerr << "Schemainstance::send_arbitrate_request: " << e.what() << "\n";
    std::unexpected();
  }
  Pointer<Arbitrate_Req> message(new Arbitrate_Req(pimpl->mysid,father_sid));
  
  myisc->process(message);
}

int Schemainstance::get_ready_or_active_brothers() throw(){
  std::set<sid_t> brothers;
  std::set<sid_t>::const_iterator b_it;
  int nready_or_active = 0;

  /*get brother states*/
  try{
    brothers = pimpl->myowner->get_brothers(pimpl->mysid);

    for (b_it = brothers.begin();
	 b_it != brothers.end(); b_it++) {
      state_enum b_state = pimpl->myowner->get_state(*b_it);
    
      if (b_state == READY or b_state == ACTIVE)
	nready_or_active++;
    }
  }catch(std::logic_error& e){
    std::cerr << "Schemainstance::get_ready_or_active_brothers: " << e.what() << "\n";
    std::unexpected();
  }
  return nready_or_active;
}

void Schemainstance::process(Pointer<Perception> p) throw(){
  /*asyncronous pereception*/
  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(pimpl->mysid) 
		       << ") receiving perception from (" 
		       << sid_to_str(p->src)<< ")\n";
  
  pimpl->perceptions_queue.push(p);
}

void Schemainstance::process(Pointer<Modulation> m) throw(){
  /*asyncronous modulation*/
  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(pimpl->mysid) 
		       << ") receiving modulation from (" 
		       << sid_to_str(m->src)<< ")\n";

  pimpl->modulations_queue.push(m);
}

void Schemainstance::process(Pointer<Arbitrate_Req> a) throw(){
  /*asyncronous arbitration*/
  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(pimpl->mysid) 
		       << ") receiving arbitrate req from (" 
		       << sid_to_str(a->src)<< ")\n";

  pimpl->arbitrate_req_queue.push(a);
}

void Schemainstance::process(Pointer<State_Change> s) throw(){
  /*asyncronous state change*/
  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(pimpl->mysid)
		       << ") receiving state change from ("
		       << sid_to_str(s->src)<< ")\n";
  {
    /*begin critical section*/
    ConditionalLock cl(pimpl->state_change.cond);
  
    if (s->state != pimpl->state->get_state_name()) {
      pimpl->state_change.state = s->state;
      pimpl->state_change.cond.signal(true);/*broadcast signal all threads
					      waiting*/
    }
    /*end critical section*/
  }
}

void Schemainstance::process(Pointer<New_State> n) throw() {
  /*syncronous receive*/
  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(pimpl->mysid) 
		       << ") receiving new state from (" << sid_to_str(n->src)<< "):"
		       << state_to_str(n->state) << "\n";
  
  pimpl->new_state_queue.push(n);
}

void Schemainstance::change_state(Schemastate* s) throw(){
  /*begin critical section*/
  ConditionalLock cl(pimpl->state_change.cond);
  
  change_state_i(s);
  /*end critical section*/
}

void Schemainstance::change_state_i(Schemastate* s) throw(){
  Pointer<Isc> myisc;
  sid_t father_sid;
  try{
    father_sid = pimpl->myowner->get_instanceinfo(pimpl->mysid)->father_sid;
    myisc = pimpl->myowner->get_isc(pimpl->mysid);
  }catch(std::logic_error& e){
    std::cerr << "Schemainstance::change_state_i: " << e.what() << "\n";
    std::unexpected();
  }
  
  pimpl->state = s;
  /*update to new state*/
  pimpl->state_change.state = pimpl->state->get_state_name();
  /*send father my new state*/
  Pointer<New_State> message(new New_State(pimpl->mysid,father_sid,
					   s->get_state_name()));

  myisc->process(message);
}

void Schemainstance::promote_state(Schemastate* s) throw(){
  /*begin critical section.*/
  ConditionalLock cl(pimpl->state_change.cond);

  /*if new state received ignore promotion*/
  if (pimpl->state_change.state != pimpl->state->get_state_name())
    change_state_i(s);
  /*end critical section*/
}

int Schemainstance::wait_state_change(const timeout_t timeout)  throw(){
  int res = 0;
  /*begin critical section.*/
  ConditionalLock cl(pimpl->state_change.cond);

  /*if new state received exit wait*/
  if(pimpl->state_change.state != pimpl->state->get_state_name())
    res = pimpl->state_change.cond.wait(timeout,true);/*wait until state
							change or timedout*/
  return res;
  /*end critical section*/
}

Schemaimplementation*
Schemainstance::adquire_impl() throw(){
  if (pimpl->implementation.get() != 0)
    return pimpl->implementation.get();


  const Hierarchy::Instanceinfo* i_info;
  try{
    i_info = pimpl->myowner->get_instanceinfo(pimpl->mysid);
  }catch(std::logic_error& e){
    std::cerr << "Schemainstance::adquire_impl: " << e.what() << "\n";
    std::unexpected();
  }
      
  /*search implementation info*/
  const Schemaimplementation::schemaimplementation_info_t* impl_info = 
    Schemaimplementation::class_info_search(i_info->interface_name,
					    i_info->branch_preferences);
  if (impl_info == 0){
    std::cerr << "Schemainstance::adquire_impl: implementation class info not found\n";
    std::unexpected();
  }


  {
    /*begin critical section*/
    MutexLock l(pimpl->reuse_lock);
    
    /*search class name because is unique*/
    Pointer<Schemaimplementation>& impl_ref = pimpl->implementations[impl_info->class_name];

    if (impl_ref.get() == 0) {/*implementation not created yet*/
      if (debug) std::cerr << "Schemainstance::adquire_impl: Creating new implementation\n";
      impl_ref = Pointer<Schemaimplementation>::Pointer(Schemaimplementation::instance(impl_info->class_name));
    }

    pimpl->implementation = impl_ref;
    /*end critical section*/
  }

  /*init instance from implementation*/
  Schemainstancefacade i_facade(this);

  pimpl->implementation->init_instance(&i_facade);
  return pimpl->implementation.get();
}


Schemainstancefacade::Schemainstancefacade(Schemainstance* instance) throw()
  :private_data(instance->pimpl->private_data),instance(instance) {}

sid_t Schemainstancefacade::get_sid() const throw() {
  return instance->get_sid();
}

state_enum Schemainstancefacade::get_state() const throw() {
  return instance->get_state();
}

int Schemainstancefacade::get_cycle_time() const throw() {
  MutexLock l(instance->pimpl->monitor_lock);

  return instance->pimpl->cycle_time;
}

void Schemainstancefacade::set_cycle_time(const int c) throw() {
  /*start critical section*/
  MutexLock l(instance->pimpl->monitor_lock);

  instance->pimpl->cycle_time = c;
  /*end critical section*/
}

void Schemainstancefacade::set_child_state(const std::string& child_nick,
					   const state_enum newstate) throw(std::logic_error) {
  Pointer<Isc> myisc = instance->pimpl->myowner->get_isc(get_sid());
  sid_t child_sid;
  {/*start critical section*/
    MutexLock l(instance->pimpl->monitor_lock);

    child_sid = instance->pimpl->get_child_sid(child_nick);
  }/*end critical section*/

  Pointer<State_Change> message(new State_Change(get_sid(),child_sid,newstate));
  myisc->process(message);
}

void Schemainstancefacade::send_perception(const std::string& name,
					   const Data& perception) const throw(std::logic_error) {
  if (instance->pimpl->interface_info->perceptions.count(name) == 0)
    throw std::logic_error("Schemainstancefacade: main interface hasn't got '"
			   + name + "' perception");

  Pointer<Isc> myisc = instance->pimpl->myowner->get_isc(get_sid());
  sid_t father_sid = instance->pimpl->myowner->get_instanceinfo(get_sid())->father_sid;
  Pointer<Perception> message(new Perception(get_sid(),father_sid,name,perception));

  myisc->process(message);
}

void Schemainstancefacade::send_modulation(const std::string& child_nick, const std::string& name,
					   const Data& modulation) const throw(std::logic_error) {
  Pointer<Isc> myisc = instance->pimpl->myowner->get_isc(get_sid());
  std::map<sid_t,Schemainstance_Pimpl::child_t>::const_iterator ci_it;
  sid_t child_sid;

  {/*start critical section*/
    MutexLock l(instance->pimpl->monitor_lock);

    child_sid = instance->pimpl->get_child_sid(child_nick);
    ci_it = instance->pimpl->children.find(child_sid);
    if (ci_it == instance->pimpl->children.end()) {
      std::cerr << "Schemainstancefacade: can't find " << child_nick << " interface.";
      std::unexpected();
    }
  }/*end critical section*/

  const Schemainterface::schemainterface_info_t* child_interface_info = ci_it->second.interface_info;
  if (child_interface_info == 0){ /*child disabled, do not send
					      anything*/
    if (instance->debug) std::cerr << "Schemainstancefacade: child " + child_nick + "disabled. No modulation sent";
    return;
  }

  if (child_interface_info->modulations.count(name) == 0)
    throw std::logic_error("Schemainstancefacade: child interface " + child_nick + " hasn't got '"
			   + name + "' modulation");

  Pointer<Modulation> message(new Modulation(get_sid(),child_sid,name,modulation));

  myisc->process(message);
}

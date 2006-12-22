#include <jde+/schemainstance.h>
#include <jde+/schemainterface.h>
#include <jde+/hierarchy.h>
#include <jde+/isc.h>
#include <jde+/schemafactory.h>
#include <jde+/schemastate.h>
#include <jde+/schemaimplementation.h>
#include <jde+/conditionallock.h>
#include <jde+/pointer.h>
#include <jde+/privatedata.h>
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
  Schemainstance_Pimpl(const Pointer<Hierarchy::Instanceinfo> instanceinfo,
		       Hierarchy* owner, Schemainstance* instance);
  ~Schemainstance_Pimpl() { terminate();/*finish thread*/ }

  /*iteration thread function*/
  void run();

  const Pointer<Hierarchy::Instanceinfo> myinstanceinfo;
  Hierarchy* const myowner;
  Schemainstance* const myinstance;

  int cycle_time;/*in ms*/

  mutable Mutex state_lock;
  Schemastate* state;

  /*instance data*/
  Pointer<Schemainterface> interface;/*interaface implemented by this schema*/
  std::map<std::string,Pointer<Schemainterface> > children_interfaces;/*child interfaces,
									indexed by interface nick*/

  /*receive Isc msg Data*/
  mutable Mutex receive_data_lock;
  std::map<sid_t,std::map<std::string,Data> > perceptions_rcv;
  std::map<std::string,Data> modulations_rcv;

  mutable Mutex receive_arbitrate_req_lock;
  std::set<sid_t> arbitrate_req_rcv;

  mutable Conditional state_change_cond;
  bool newstate_received;
  state_enum state_received;

  mutable Conditional child_new_state_cond;
  sid_t waited_child;
  state_enum waited_child_state;
  bool waiting_child_state;

  /*children last seen state*/
  std::map<std::string,state_enum> children_last_state;

  Pointer<Schemaimplementation> implementation;

  /*schemaimpementation reuse*/
  static std::map<std::string,Pointer<Schemaimplementation> > implementations;
  static Mutex reuse_lock;
};

/*schamainstance static definitions*/
std::vector<Pointer<Schemaimplementation> > Schemainstance_Pimpl::impl;
std::map<sid_t,int> Schemainstance_Pimpl::last_got;
std::map<std::string,std::list<int> > Schemainstance_Pimpl::implementations;
Mutex Schemainstance_Pimpl::reuse_lock;

Schemainstance_Pimpl::Schemainstance_Pimpl(const Pointer<Hierarchy::Instanceinfo> instanceinfo,
					   Hierarchy* owner, Schemainstance* instance)
  :myinstanceinfo(instanceinfo),myowner(owner),
   myinstance(instance),interface(0),implementation(0),
   newstate_received(false) {

  if (myinstanceinfo->factory.get() == NULL){
    std::cerr << "Schemainstance: unexpected error. Trying to create an instance without a null factory";
    std::unexpected();
  }

  /*create main interface and children interfaces*/
  interface = Pointer<Schemainterface>::Pointer(myinstanceinfo->factory->create_interface());

  std::vector<sid_t>::const_iterator cs_it;
  for (cs_it = myinstanceinfo->get_children().begin();
       cs_it != myinstanceinfo->get_children().end();
       cs_it++) {
    Pointer<Schemafactory> child_factory = owner->get_instanceinfo(*cs_it)->factory;
    if (child_factory.get() == NULL)/*if factory can't be loaded
				      disable thid child*/
      children_interfaces[myinstanceinfo->get_child_nick(*cs_it)] = Pointer<Schemainterface>::Pointer(0);
    else
      children_interfaces[myinstanceinfo->get_child_nick(*cs_it)] = 
	Pointer<Schemainterface>::Pointer(child_factory->create_interface());
  }
}

void Schemainstance_Pimpl::run(){
  try {
    /*assign initial state*/
    if (myinstanceinfo->factory->get_interface_info().type == PSCHEMA)
      state = Pschemaslept::instance();
    else/*MSCHEMA*/
      state = Mschemaslept::instance();

    while(1){
      /*exec actions*/
      if (myinstance->debug) std::cerr << "Schemainstance: (" << sid_to_str(mysid)
		      << ") executing actions\n";
      state->exec_actions(myinstance);
      
      /*deliver state change*/
      {
	ConditionalLock cl(state_change_cond);
       
	if (myinstance->debug) std::cerr << "Schemainstance: (" << sid_to_str(mysid)
			<< ") delivering state changes\n";
	if (newstate_received) {
	  state->receive_state_change(myinstance,state_received);
	  newstate_received = false;
	}
      }

      /*deliver arbitrate req*/
      {
	MutexLock l(receive_arbitrate_req_lock);
	std::set<sid_t>::iterator ar_it;

	if (myinstance->debug) std::cerr << "Schemainstance: (" << sid_to_str(mysid)
			<< ") delivering arbitrate reqs\n";
	for(ar_it = arbitrate_req_rcv.begin();
	    ar_it != arbitrate_req_rcv.end();
	    ar_it++)
	  state->receive_arbitrate_request(myinstance,
					   myinstanceinfo->get_child_nick(*ar_it));
	arbitrate_req_rcv.clear();
      }

      /*
	FIXME: Data can be delivered syncronous becaus doesn't change
	anything on the state ??
      */
      // /*deliver Data*/
//       {
// 	MutexLock l(receive_data_lock);
// 	std::map<sid_t,std::map<std::string,Data> >::iterator out_it;
// 	std::map<std::string,Data>::iterator in_it;
       

	// if (myinstance->debug) std::cerr << "Schemainstance: (" << sid_to_str(mysid)
// 			<< ") delivering data\n";
	// /*perceptions*/
// 	for(out_it = perceptions_rcv.begin();
// 	    out_it != perceptions_rcv.end();
// 	    out_it++) {
// 	  std::string child_nick = myinstanceinfo->get_child_nick(out_it->first);
// 	  for (in_it = out_it->second.begin();
// 	       in_it != out_it->second.end();
// 	       in_it++) {
// 	    /*FIXME: debug*/
// 	    /*std::cerr << "Schemainstance: delivering perception to "
// 	      << child_nick << ":" << in_it->second << "\n";*/
// 	    state->receive_perception(myinstance,child_nick,
// 				      in_it->first,in_it->second);
// 	  }
// 	}
// 	perceptions_rcv.clear();
       
// 	/*modulations*/
// 	for (in_it = modulations_rcv.begin();
// 	     in_it != modulations_rcv.end();
// 	     in_it++) {
// 	  state->receive_modulation(myinstance,in_it->first,in_it->second);
// 	}
// 	modulations_rcv.clear();
//       }
     
      
      yield();
    }
  } catch(std::exception& e) {
    if (myinstance->debug) std::cerr << "Schemainstance: exception raised on iteration thread: " 
		    << e.what() << "\n";
    std::unexpected();
  }catch(...) {
    if (myinstance->debug) std::cerr << "Schemainstance: unknown exception raised on iteration thread\n";
    std::unexpected();
  }
}

const double& Schemainstance::tstart = Hierarchy::tstart;

Schemainstance::Schemainstance(const sid_t sid,
                               Hierarchy* owner,
                               Schemafactory* factory,
			       const bool debugflag)
  :debug(debugflag),pimpl(new Schemainstance_Pimpl(owner->get_instanceinfo(sid),owner,this,factory)) {
  
  /*set default cycle_time*/
  set_cycle_time(100);

  if (debug) std::cerr << "Schemainstance: starting thread\n";
  pimpl->start();
}

Schemainstance::~Schemainstance() {
  if (debug) std::cerr << "Schemainstance: releasing pimpl\n";
  delete pimpl;
}

sid_t Schemainstance::get_sid() const {
  return pimpl->myinstanceinfo->sid;
}


state_enum Schemainstance::get_state() const {
  /*begin critical section*/
  MutexLock l(pimpl->state_lock);

  return pimpl->state->get_state_name();
  /*end critical section*/
}

int Schemainstance::get_cycle_time() const {
  return pimpl->cycle_time;
}

void Schemainstance::set_cycle_time(const int c) {
  pimpl->cycle_time = c;
}

Data& Schemainstance::get_modulation(const std::string& name) {
  /*begin critical section*/
  MutexLock l(pimpl->receive_data_lock);

  return get_modulation_i(name);
  /*end critical section*/
}

void Schemainstance::set_modulation(const std::string& name,
				    const Data& modulation) {
  /*begin critical section*/
  MutexLock l(pimpl->receive_data_lock);

  set_modulation_i(name,modulation);
  /*end critical section*/
}

std::vector<std::string>&
Schemainstance::get_children() const {
  return pimpl->myinstanceinfo->get_children_nicks();
}

state_enum
Schemainstance::get_child_state(const std::string& child_nick) const {
  /*begin critical section*/
  ConditionalLock cl(pimpl->child_new_state_cond);

  return get_child_state_i(child_nick);
  /*end critical section*/
}

Data& Schemainstance::get_child_perception(const std::string& child_nick,
					   const std::string& name) {
  /*begin critical section*/
  MutexLock l(pimpl->receive_data_lock);

  return get_child_perception_i(child_nick,name);
  /*end critical section*/
}

void Schemainstance::send_perception(const std::string& name,
                                     const Data& perception) const {
  Pointer<Isc> myisc = pimpl->myowner->get_isc(get_sid());
  Pointer<Perception> message(new Perception(get_sid(),get_father(),name,perception));

  if (std::find(pimpl->interface->get_perceptions_names().begin(),
		pimpl->interface->get_perceptions_names().end(),
		name))
    throw std::logic_error("Schemainstance: main interface hasn't got '"
			   + name + "' perception");

  myisc->process(message);
}

void Schemainstance::send_modulation(const std::string& child_nick, const std::string& name,
                                     const Data& modulation) const {
  Pointer<Isc> myisc = pimpl->myowner->get_isc(get_sid());

  if (!pimpl->myinstanceinfo->is_child(child_nick))
    throw std::logic_error("Schemainstance: instance(" + sid_to_str(get_sid())
			   + ") hasn't got child instance(" + child_nick
			   + ")");

  std::map<std::string,Pointer<Schemainterface> >::const_iterator ci_it;
  ci_it = pimpl->children_interfaces.find(child_nick);
  if (ci_it == pimpl->children_interfaces.end())
    throw std::logic_error("Schemainstance: can't find " + child_nick + " interface.");

  if (ci_it->second.get() == NULL){ /*child disabled, do not send
				      anything*/
    if (debug) std::cerr << "Schemainstance: child " + child_nick + "disabled. No modulation sent";
    return;
  }

  if (std::find(ci_it->second->get_perceptions_names().begin(),
		ci_it->second->get_perceptions_names().end(),
		name))
    throw std::logic_error("Schemainstance: child interface " + child_nick + " hasn't got '"
			   + name + "' modulation");

  sid_t child_sid = pimpl->myinstanceinfo->get_child_sid(child_nick);
  Pointer<Modulation> message(new Modulation(get_sid(),child_sid,name,modulation));

  myisc->process(message);
}

void Schemainstance::send_state_change(const std::string& child_nick,
                                       const state_enum newstate) const {
  Pointer<Isc> myisc = pimpl->myowner->get_isc(get_sid());

  if (!pimpl->myinstanceinfo->is_child(child_nick))
    throw std::logic_error("Schemainstance: instance(" + sid_to_str(get_sid())
			   + ") hasn't got child instance(" + child_nick
			   + ")");
  sid_t child_sid = pimpl->myinstanceinfo->get_child_sid(child_nick);
  Pointer<State_Change> message(new State_Change(get_sid(),child_sid,newstate));

  myisc->process(message);
}

void Schemainstance::send_arbitrate_request() const {
  Pointer<Isc> myisc = pimpl->myowner->get_isc(get_sid());
  Pointer<Arbitrate_Req> message(new Arbitrate_Req(get_sid(),get_father()));
  
  myisc->process(message);
}

int Schemainstance::wait_child_state(const std::string& child_nick,
				     const state_enum wait_state,
				     const timeout_t timeout) {
  int res = 0;
  /*begin critical section.*/
  ConditionalLock cl(pimpl->child_new_state_cond);

  /*no body else is waiting and child hasn't got the state yet*/
  if(! pimpl->waiting_child_state &&
     get_child_state_i(child_nick) != wait_state) {
    pimpl->waiting_child_state = true;
    pimpl->waited_child = child_nick;
    pimpl->waited_child_state = wait_state;
    /*wait until new child state received or timedout*/
    res = pimpl->child_new_state_cond.wait(timeout,true);
    pimpl->waiting_child_state = false;
  }
  return res;
  /*end critical section*/
}

int Schemainstance::get_ready_or_active_brothers() {
  std::vector<sid_t> brothers;
  std::vector<sid_t>::const_iterator b_it;
  int nready_or_active = 0;

  /*get brother states*/
  brothers = pimpl->myowner->get_brothers(get_sid());
  for (b_it = brothers.begin();
       b_it != brothers.end(); b_it++) {
    state_enum b_state = pimpl->myowner->get_state(*b_it);

    if (b_state == READY or b_state == ACTIVE)
      nready_or_active++;
  }
  return nready_or_active;
}

void Schemainstance::process(Pointer<Perception> p){
  /*syncronous perception*/
  /*begin critical section*/
  MutexLock l(pimpl->receive_data_lock);

  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(get_sid()) 
		       << ") receiving perception from (" 
		       << sid_to_str(p->src)<< ")\n";
  
  set_child_perception_i(pimpl->myinstanceinfo->get_child_nick(p->src),
			 p->perception_name,p->perception_data);

  /*asyncronous reception implementation*/
  /*push perception. Only newer perceptions are stored*/
  //pimpl->perceptions_rcv[p->src].insert(std::make_pair(p->perception_name,
  //					       p->perception_data));
  /*end critical section*/
}

void Schemainstance::process(Pointer<Modulation> m){
  /*asyncronous modulation*/
  /*begin critical section*/
  MutexLock l(pimpl->receive_data_lock);

  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(get_sid()) 
		       << ") receiving modulation from (" 
		       << sid_to_str(m->src)<< ")\n";
  set_modulation_i(m->modulation_name,m->modulation_data);

  /*asyncronous reception implementation*/
  // pimpl->modulations_rcv.insert(std::make_pair(m->modulation_name,
// 					       m->modulation_data));
  /*end critical section*/
}

void Schemainstance::process(Pointer<Arbitrate_Req> a){
  /*asyncronous arbitration*/
  /*begin critical section*/
  MutexLock l(pimpl->receive_arbitrate_req_lock);

  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(get_sid()) 
			 << ") receiving arbitrate req from (" 
		       << sid_to_str(a->src)<< ")\n";
  pimpl->arbitrate_req_rcv.insert(a->src);
  /*end critical section*/

  /*FIXME, syncronous arbitration*/
  // /*get actual state instance to exec arbitrate*/
  //   Schemastate* s;
  //   {
  //     /*begin critical section*/
  //     MutexLock l(state_lock);

  //     s = state;
  //   }
  //   s->receive_arbitrate_request(this,
  // 			       get_child_nick(src));
  //   /*end critical section*/
}

void Schemainstance::process(Pointer<State_Change> s){
  /*asyncronous state change*/
  /*begin critical section*/
  ConditionalLock cl(pimpl->state_change_cond);
   
  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(get_sid()) 
		       << ") receiving state change from (" 
		       << sid_to_str(s->src)<< ")\n";
  pimpl->state_received = s->change_state;
  pimpl->newstate_received = true;
  pimpl->state_change_cond.signal(false);/*false to avoid locking the
					   cond another time*/

  /*end critical section*/
}

void Schemainstance::process(Pointer<New_State> n){
  /*syncronous receive*/

  if (debug) std::cerr << "Schemainstance: (" << sid_to_str(get_sid()) 
		       << ") receiving new state from (" << sid_to_str(n->src)<< "):"
		       << state_to_str(n->new_state) << "\n";
  std::string child_nick = pimpl->myinstanceinfo->get_child_nick(n->src);

  /*begin critical section*/
  ConditionalLock cl(pimpl->child_new_state_cond);

  pimpl->children_last_state[child_nick] = n->new_state;
  if (pimpl->waiting_child_state &&
      pimpl->waited_child == n->src && 
      pimpl->waited_child_state == n->new_state)
    pimpl->child_new_state_cond.signal(false);/*false to avoid locking the
						cond another time*/
  /*end critical section*/
}

 void Schemainstance::set_child_perception(const std::string& child_nick,
					  const std::string& name,
					  const Data& perception) {
  /*begin critical section*/
  MutexLock l(pimpl->receive_data_lock);

  set_child_perception_i(child_nick,name,perception);
  /*end critical section*/
}


state_enum
Schemainstance::get_child_state_i(const std::string& child_nick) const {
  std::map<std::string,state_enum>::const_iterator cs_it;

  if (pimpl->myinstanceinfo->is_child(child_nick))
    throw std::logic_error("Schemainstance: child interface '"
			   + child_nick + "' unknown");

  cs_it = pimpl->children_last_state.find(child_nick);
  if ( cs_it == pimpl->children_last_state.end() )/*no state seen
						    yet.supose SLEEP*/
    return SLEPT;
  return *(cs_it->second);
}

Data& Schemainstance::get_modulation_i(const std::string& name) {
  return (*pimpl->interface)[name];
}

void Schemainstance::set_modulation_i(const std::string& name,
				      const Data& modulation) {
  (*pimpl->interface)[name] = modulation;
}

Data& Schemainstance::get_child_perception_i(const std::string& child_nick,
					     const std::string& name) {
  if (pimpl->myinstanceinfo->is_child(child_nick))
    throw std::logic_error("Schemainstance: child interface '"
			   + child_nick + "' unknown");
  
  std::map<std::string,Schemainterface*>::const_iterator c_it;
  c_it = pimpl->children_interfaces.find(child_nick);

  return (*(c_it->second))[name];
}

void Schemainstance::set_child_perception_i(const std::string& child_nick,
					    const std::string& name,
					    const Data& perception) {
  if (pimpl->myinstanceinfo->is_child(child_nick))
    throw std::logic_error("Schemainstance: child interface '"
			   + child_nick + "' unknown");

  std::map<std::string,Schemainterface*>::const_iterator c_it;
  c_it = pimpl->children_interfaces.find(child_nick);

  (*(c_it->second))[name] = perception;
}

void Schemainstance::change_state(Schemastate* s) {
  /*begin critical section*/
  MutexLock l(pimpl->state_lock);
  
  Pointer<Isc> myisc = pimpl->myowner->get_isc(get_sid());

  pimpl->state = s;
  Pointer<New_State> message(new New_State(get_sid(),get_father(),s->get_state_name()));

  myisc->process(message);
  /*end critical section*/
}

void Schemainstance::promote_state(Schemastate* s) {
  /*begin critical section.*/
  ConditionalLock cl(pimpl->state_change_cond);

  /*if new state received ignore promotion*/
  if (!pimpl->newstate_received)
    change_state(s);

  /*end critical section*/
}

int Schemainstance::wait_state_change(const timeout_t timeout) {
  int res = 0;
  /*begin critical section.*/
  ConditionalLock cl(pimpl->state_change_cond);

  /*if new state received exit wait*/
  if(!pimpl->newstate_received)
    res = pimpl->state_change_cond.wait(timeout,true);/*wait until state
							change or timedout*/
  return res;
  /*end critical section*/
}

Schemaimplementation*
Schemainstance::adquire_impl() {
  if (pimpl->implementation.get() == NULL) {
    {
      /*begin critical section*/
      MutexLock l(pimpl->reuse_lock);
      Pointer<Schemaimplementation>& impl_ref = pimpl->implementations[get_interface_name()];

      if (impl_ref.get() == NULL) {/*implementation not created yet*/
	if (debug) std::cerr << "Schemainstance: Creating new implementation\n";
	impl_ref = Pointer<Schemaimplementation>::Pointer(pimpl->myinstanceinfo->factory->create_implementation());
      }
      pimpl->implementation = impl_ref;
      /*end critical section*/
    }

    /*init instance from implementation*/
    pimpl->implementation->init_instance(this);
  }

  return pimpl->implementation.get();
}

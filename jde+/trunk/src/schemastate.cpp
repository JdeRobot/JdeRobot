#include <jde+/schemastate.h>
#include <jde+/schemainstance.h>
#include <jde+/schemaimplementation.h>
#include <iostream>

#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::SysTime;
#endif

void Schemastate::change_state(Schemainstance* i,
                               Schemastate* s) {
  i->change_state(s);
}

void Schemastate::promote_state(Schemainstance* i,
                                Schemastate* s) {
  i->promote_state(s);
}

/*timeout in mili-sec*/
int Schemastate::wait_state_change(Schemainstance* i,
                                   const timeout_t timeout) {
  return i->wait_state_change(timeout);
}

Schemaimplementation* Schemastate::adquire_impl(Schemainstance* i) {
  return i->adquire_impl();
}

void Schemastate::release_impl(Schemainstance* i) {
  i->release_impl();
}

void Schemastate::set_child_perception(Schemainstance* i,
                                       const std::string& child_nick,
                                       const std::string& name, 
				       const Data& perception) {
  i->set_child_perception(child_nick,
			  name,perception);
}

void Schemastate::set_modulation(Schemainstance* i,
                                 const std::string& name, 
				 const Data& modulation) {
  i->set_modulation(name,modulation);
}

/*GROUP STATES*/
void Schemaslept::exec_actions(Schemainstance* i) {
  wait_state_change(i);
}

void Schemaslept::receive_modulation(Schemainstance* i,
                                     const std::string& name,
                                     const Data& modulation) {
  set_modulation(i,name,modulation);
}

void Schemanoslept::receive_perception(Schemainstance* i,
                                       const std::string& child_nick,
                                       const std::string& name,
                                       const Data& perception) {
  set_child_perception(i,child_nick,name,perception);
}

void Schemanoslept::receive_modulation(Schemainstance* i,
                                       const std::string& name,
                                       const Data& modulation) {
  set_modulation(i,name,modulation);
}

void Schemanoslept::receive_arbitrate_request(Schemainstance* i,
					      const std::string& child_nick) {
  adquire_impl(i)->arbitrate(child_nick);
}

/*PSCHEMAS STATES*/
void Pschemaslept::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate != SLEPT){
    /*adquire impl before state change, to force exec init_schema
      before exit from slept state*/
    Schemaimplementation* impl = adquire_impl(i);
    change_state(i,Pschemaactive::instance());
    impl->wakeup_handler();
  }
}

void Pschemaactive::exec_actions(Schemainstance* i) {
  struct timeval tv1,tv2;
  timeout_t diff,next;

  SysTime::getTimeOfDay(&tv1);
  adquire_impl(i)->iteration();
  SysTime::getTimeOfDay(&tv2);

  diff = (tv2.tv_sec - tv1.tv_sec)*1000 + 
    (tv2.tv_usec - tv1.tv_usec)/1000;/*msec*/
  next = i->get_cycle_time();/*msec*/
  if (diff >= next)
    next = 0;
  else
    next -= diff;/*msec*/
  /*std::cerr << "Pschemaactive: waiting " << next << "msec\n";FIXME:debug*/
  if (next > 0)
    wait_state_change(i,next);/*cancellation point
				allow to remove esquema 
				from ACTIVE state*/
  else
    if (i->debug) std::cerr << "Pschemaactive: cycle_time violated on instance("
			    << sid_to_str(i->get_sid()) << "\n";
}

void Pschemaactive::receive_state_change(Schemainstance* i,
					 const state_enum newstate) {
  if (newstate == SLEPT) {
    adquire_impl(i)->sleep_handler();
    change_state(i,Pschemaslept::instance());
    release_impl(i);
  }
}


/*MSCHEMAS STATES*/
void Mschemaslept::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate != SLEPT){
    change_state(i,Mschemaalert::instance());
    adquire_impl(i)->wakeup_handler();
  }
}


void Mschemaalert::exec_actions(Schemainstance* i) {
  if (adquire_impl(i)->check_preconditions()) {
    promote_state(i,Mschemaready::instance());
  } else {
    int rab;

    rab = i->get_ready_or_active_brothers();
    if (i->debug) std::cerr << "Mschemaalert(" << sid_to_str(i->get_sid())
			    << "): ready_active " << rab << "\n";
    if (rab == 0) {
      if (i->debug) std::cerr << "Mschemaalert(" << sid_to_str(i->get_sid())
			      << "): Control absence\n";
      i->send_arbitrate_request();
      wait_state_change(i);/*wait until arbitration is resolved*/
    }
  }
}

void Mschemaalert::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate == READY)
    change_state(i,Mschemaready::instance());
  else if (newstate == ACTIVE)
    change_state(i,Mschemaactive::instance());
  else if (newstate == SLEPT) {
    adquire_impl(i)->sleep_handler();
    change_state(i,Mschemaslept::instance());
    release_impl(i);
  }
}

void Mschemaready::exec_actions(Schemainstance* i) {
  int rab;

  rab = i->get_ready_or_active_brothers();
  if (i->debug) std::cerr << "Mschemaready(" << sid_to_str(i->get_sid())
			  << "): ready_active " << rab << "\n";
  if (rab >= 1) {/*rab do not include my self*/
    if (i->debug) std::cerr << "Mschemaready(" << sid_to_str(i->get_sid())
			    << "): Control overlap\n";
    i->send_arbitrate_request();
    wait_state_change(i);/*wait until arbitration resolved*/
  } else {
    promote_state(i,Mschemaactive::instance());
  }
}

void Mschemaready::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate == ACTIVE)
    change_state(i,Mschemaactive::instance());
  else if (newstate == ALERT)
    change_state(i,Mschemaalert::instance());
  else if (newstate == SLEPT) {
    adquire_impl(i)->sleep_handler();
    change_state(i,Mschemaslept::instance());
    release_impl(i);
  }
}

void Mschemaactive::exec_actions(Schemainstance* i) {
  struct timeval tv1,tv2;
  long diff,next;

  SysTime::getTimeOfDay(&tv1);
  adquire_impl(i)->iteration();
  SysTime::getTimeOfDay(&tv2);
   
  diff = (tv2.tv_sec - tv1.tv_sec)*1000 + 
    (tv2.tv_usec - tv1.tv_usec)/1000;/*msec*/
  next = i->get_cycle_time();/*msec*/
  if (diff >= next)
    next = 0;
  else
    next -= diff;/*msec*/
  if (i->debug) std::cerr << "Mschemaactive: waiting " << next << "msec\n";
  if (next > 0) {
    /*cancellation point allow to remove esquema
      from ACTIVE state. If timedout go to ALERT state*/
    if (wait_state_change(i,next) != 0)
      promote_state(i,Mschemaalert::instance());
  } else {
    if (i->debug) std::cerr << "Mschemaactive: cycle_time violated on instance("
			    << sid_to_str(i->get_sid()) << "\n";
    promote_state(i,Mschemaalert::instance());
  }
}

void Mschemaactive::receive_state_change(Schemainstance* i,
					 const state_enum newstate) {
  if (newstate == ALERT)
    change_state(i,Mschemaalert::instance());
  else if (newstate == READY)
    change_state(i,Mschemaready::instance());
  else if (newstate == SLEPT) {
    adquire_impl(i)->sleep_handler();
    change_state(i,Mschemaslept::instance());
    release_impl(i);
  }
}

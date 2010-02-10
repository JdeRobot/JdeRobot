#include <jde+/schemastate.h>
#include <jde+/schemainstance.h>
#include <jde+/schemainstancefacade.h>
#include <jde+/schemaimplementation.h>
#include <iostream>

#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::SysTime;
#endif

/*ALL STATE ACTIONS*/
void Schemastate::receive_modulation(Schemainstance* i,
				     const std::string& name,
				     const Data& modulation) {
  Schemainstancefacade i_facade(i);
  i->adquire_impl()->receive_modulation(&i_facade,name,modulation);
}


/*GROUP STATES*/
void Schemaslept::exec_actions(Schemainstance* i) {
  i->wait_state_change();
}

void Schemanoslept::receive_perception(Schemainstance* i,
                                       const std::string& child_nick,
                                       const std::string& name,
                                       const Data& perception) {
  Schemainstancefacade i_facade(i);
  i->adquire_impl()->receive_perception(&i_facade,child_nick,name,perception);
}


void Schemanoslept::receive_arbitrate_request(Schemainstance* i,
					      const std::string& child_nick) {
  Schemainstancefacade i_facade(i);
  i->adquire_impl()->arbitrate(&i_facade,child_nick);
}

void Schemanoslept::receive_child_new_state(Schemainstance* i,
					    const std::string& child_nick,
					    const state_enum newstate){
  Schemainstancefacade i_facade(i);
  i->adquire_impl()->receive_child_new_state(&i_facade,child_nick,newstate);
}

/*PSCHEMAS STATES*/
void Pschemaslept::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate != SLEPT){
    /*adquire impl before state change, to force exec init_schema
      before exit from slept state*/
    //Schemaimplementation* impl = i->adquire_impl(); FIXME
    i->change_state(Pschemaactive::instance());
    Schemainstancefacade i_facade(i);
    i->adquire_impl()->wakeup_handler(&i_facade);
  }
}

void Pschemaactive::exec_actions(Schemainstance* i) {
  struct timeval tv1,tv2;
  timeout_t diff,next;
  Schemainstancefacade i_facade(i);

  SysTime::getTimeOfDay(&tv1);
  i->adquire_impl()->iteration(&i_facade);
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
    i->wait_state_change(next);/*cancellation point
				 allow to remove esquema 
				 from ACTIVE state*/
  else
    if (i->debug) std::cerr << "Pschemaactive: cycle_time violated on instance("
			    << sid_to_str(i->get_sid()) << "\n";
}

void Pschemaactive::receive_state_change(Schemainstance* i,
					 const state_enum newstate) {
  if (newstate == SLEPT) {
    Schemainstancefacade i_facade(i);
    i->adquire_impl()->sleep_handler(&i_facade);
    i->change_state(Pschemaslept::instance());
  }
}


/*MSCHEMAS STATES*/
void Mschemaslept::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate != SLEPT){
    i->change_state(Mschemaalert::instance());
    Schemainstancefacade i_facade(i);
    i->adquire_impl()->wakeup_handler(&i_facade);
  }
}


void Mschemaalert::exec_actions(Schemainstance* i) {
  Schemainstancefacade i_facade(i);

  if (i->adquire_impl()->check_preconditions(&i_facade)) {
    i->promote_state(Mschemaready::instance());
  } else {
    int rab;

    rab = i->get_ready_or_active_brothers();
    if (i->debug) std::cerr << "Mschemaalert(" << sid_to_str(i->get_sid())
			    << "): ready_active " << rab << "\n";
    if (rab == 0) {
      if (i->debug) std::cerr << "Mschemaalert(" << sid_to_str(i->get_sid())
			      << "): Control absence\n";
      i->send_arbitrate_request();
      i->wait_state_change();/*wait until arbitration is resolved
			       FIXME: timeout??*/
    }
  }
}

void Mschemaalert::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate == READY)
    i->change_state(Mschemaready::instance());
  else if (newstate == ACTIVE)
    i->change_state(Mschemaactive::instance());
  else if (newstate == SLEPT) {
    Schemainstancefacade i_facade(i);
    i->adquire_impl()->sleep_handler(&i_facade);
    i->change_state(Mschemaslept::instance());
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
    i->wait_state_change();/*wait until arbitration resolved FIXME:
			     timeout??*/
  } else {
    i->promote_state(Mschemaactive::instance());
  }
}

void Mschemaready::receive_state_change(Schemainstance* i,
                                        const state_enum newstate) {
  if (newstate == ACTIVE)
    i->change_state(Mschemaactive::instance());
  else if (newstate == ALERT)
    i->change_state(Mschemaalert::instance());
  else if (newstate == SLEPT) {
    Schemainstancefacade i_facade(i);
    i->adquire_impl()->sleep_handler(&i_facade);
    i->change_state(Mschemaslept::instance());
  }
}

void Mschemaactive::exec_actions(Schemainstance* i) {
  struct timeval tv1,tv2;
  long diff,next;
  Schemainstancefacade i_facade(i);

  SysTime::getTimeOfDay(&tv1);
  i->adquire_impl()->iteration(&i_facade);
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
    if (i->wait_state_change(next) != 0)
      i->promote_state(Mschemaalert::instance());
  } else {
    if (i->debug) std::cerr << "Mschemaactive: cycle_time violated on instance("
			    << sid_to_str(i->get_sid()) << "\n";
    i->promote_state(Mschemaalert::instance());
  }
}

void Mschemaactive::receive_state_change(Schemainstance* i,
					 const state_enum newstate) {
  if (newstate == ALERT)
    i->change_state(Mschemaalert::instance());
  else if (newstate == READY)
    i->change_state(Mschemaready::instance());
  else if (newstate == SLEPT) {
    Schemainstancefacade i_facade(i);
    i->adquire_impl()->sleep_handler(&i_facade);
    i->change_state(Mschemaslept::instance());
  }
}

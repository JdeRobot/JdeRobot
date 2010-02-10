#include <jde+/isc.h>
#include <jde+/pointer.h>
#include <jde+/hierarchy.h>
#include <jde+/schemainstance.h>

#include <vector>


/*isc private implementation*/
class Isc_Pimpl {
public:
  Isc_Pimpl(bool debugflag = false)
    :debug(debugflag) {}
  ~Isc_Pimpl() { }

  Hierarchy* owner;
  std::vector<Message_Processor*> processors;
  const bool debug;
};

/*isc definitions*/
Isc::Isc(Hierarchy* owner, bool debugflag) throw()
  : debug(debugflag) {
  pimpl = new Isc_Pimpl(debugflag);
  pimpl->owner = owner;
}

Isc::~Isc() throw(){
  if (debug)
    std::cerr << "Isc: terminating isc\n";
  delete pimpl;
}


void Isc::process(Pointer<Perception> p) throw(){
  /*deliver it*/
  Pointer<Schemainstance> dst_i;
  std::vector<Message_Processor*>::iterator p_it;

  dst_i = pimpl->owner->get_instance(p->dst);
  if (dst_i.get() == NULL){ /*child to father msg. This is a bug*/
    std::cerr << "Isc: unexpected error. Msg to null father.";
    std::unexpected();
  }

  dst_i->process(p);
  for (p_it = pimpl->processors.begin();
       p_it != pimpl->processors.end();
       p_it++){
    (*p_it)->process(p);
  }
}

void Isc::process(Pointer<Modulation> m) throw(){
  /*deliver it*/
  Pointer<Schemainstance> dst_i;
  std::vector<Message_Processor*>::iterator p_it;

  dst_i = pimpl->owner->get_instance(m->dst);
  if (dst_i.get() == NULL){
    if (debug) std::cerr << "Isc: destination instance "
			 << sid_to_str(m->dst)
			 << " disabled. Message ignored";
    return;
  }

  dst_i->process(m);
  for (p_it = pimpl->processors.begin();
       p_it != pimpl->processors.end();
       p_it++){
    (*p_it)->process(m);
  }
}

void Isc::process(Pointer<Arbitrate_Req> a) throw(){
  /*deliver it*/
  Pointer<Schemainstance> dst_i;
  std::vector<Message_Processor*>::iterator p_it;

  dst_i = pimpl->owner->get_instance(a->dst);
  if (dst_i.get() == NULL){ /*child to father msg. This is a bug*/
    std::cerr << "Isc: unexpected error. Msg to null father.";
    std::unexpected();
  }

  dst_i->process(a);
  for (p_it = pimpl->processors.begin();
       p_it != pimpl->processors.end();
       p_it++){
    (*p_it)->process(a);
  }
}

void Isc::process(Pointer<State_Change> s) throw(){
  /*deliver it*/
  Pointer<Schemainstance> dst_i;
  std::vector<Message_Processor*>::iterator p_it;

  dst_i = pimpl->owner->get_instance(s->dst);
  if (dst_i.get() == NULL){
    if (debug) std::cerr << "Isc: destination instance "
			 << sid_to_str(s->dst)
			 << " disabled. Message ignored";
    return;
  }

  dst_i->process(s);
  for (p_it = pimpl->processors.begin();
       p_it != pimpl->processors.end();
       p_it++){
    (*p_it)->process(s);
  }
}

void Isc::process(Pointer<New_State> n) throw(){
  /*deliver it*/
  Pointer<Schemainstance> dst_i;
  std::vector<Message_Processor*>::iterator p_it;

  dst_i = pimpl->owner->get_instance(n->dst);
  if (dst_i.get() == NULL){ /*child to father msg. This is a bug*/
    std::cerr << "Isc: unexpected error. Msg to null father.";
    std::unexpected();
  }

  dst_i->process(n);
  for (p_it = pimpl->processors.begin();
       p_it != pimpl->processors.end();
       p_it++){
    (*p_it)->process(n);
  }
}

void Isc::append_processor(Message_Processor* mp) throw(){
  /*FIXME: posible condicion de carrera si añadimos
    procesadores cuando la jerarquia esta en marcha*/
  pimpl->processors.push_back(mp);
}

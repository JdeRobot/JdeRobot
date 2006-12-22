#include <jde+/schemaimplementation.h>
#include <jde+/schemainstance.h>
#include <jde+/schemainterface.h>
#include <stdexcept>
#include <iostream>

#include <cc++/thread.h>
#ifdef CCXX_NAMESPACES
using ost::Mutex;
#endif


void Schemaimplementation::sleep_handler(Schemainstance* instance) {
  std::vector<sid_t>::const_iterator c_it;

  for(c_it = instance->get_children().begin();
      c_it != instance->get_children().end(); c_it++)
    instance->send_state_change(c_it,SLEPT);
}

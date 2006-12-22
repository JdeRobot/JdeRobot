#include <jde+/schemafactory.h>

Schemafactory::Schemafactory() {
  Schemainterface* interface(create_interface());

  interface_info = { interface->interface_name,
		     interface->nickname,
		     interface->type };
  delete interface;

  std::vector<Schemainterface*> children_interfaces =
    create_children_interfaces();
  std::vector<Schemainterface*>::iterator ci_it;
    
  for (ci_it = children_interfaces.begin();
       ci_it != children_interfaces.end();
       ci_it++) {
    interfaceinfo_t infotmp = {(*ci_it)->interface_name,
			       (*ci_it)->nickname,
			       (*ci_it)->type };
    children_interface_infos.push_back(infotmp);
    delete (*ci_it);
  }
}

Schemafactory::~Schemafactory() {}

virtual interfaceinfo_t const& Schemafactory::get_interface_info() const {
  return interface_info;
}

virtual std::vector<interfaceinfo_t> const& Schemafactory::get_children_interface_infos() const {
  return children_interface_infos;
}

#include <jde+/schemainstancefacade.h>
#include "$schemaname.h"

REGISTER_SCHEMA($schemaname)

namespace _$schemaname{
  /*Private data class*/
  class myPrivateData: public PrivateData {
  public:
    /*data class declaration*/
  };
}

$schemaname::$schemaname()
  :Schemaimplementation(SCHEMA_INFO($schemaname)) {}

//$schemaname::~$schemaname() {}

void $schemaname::init_instance(Schemainstancefacade* instance) throw(){
  instance->private_data = new _$schemaname::myPrivateData();

}

void $schemaname::iteration(Schemainstancefacade* instance) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);

  /*iteration code*/
}

void $schemaname::arbitrate(Schemainstancefacade* instance, 
				   const std::string& request_nick) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);

  /*arbitration code*/
}

bool $schemaname::check_preconditions(Schemainstancefacade* instance) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);

  /*check preconditions code*/
}

void $schemaname::receive_child_new_state(Schemainstancefacade* instance,
						 const std::string& child_nick,
						 const state_enum newstate) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);
  
  /*receive child new state code*/
}

void $schemaname::receive_modulation(Schemainstancefacade* instance,
					    const std::string& name, 
					    const Data& modulation) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);

  /*receive modulation code*/
}

void $schemaname::receive_perception(Schemainstancefacade* instance,
					    const std::string& child_nick, 
					    const std::string& name, 
					    const Data& perception) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);
  
  /*receive perception code*/
}

void $schemaname::sleep_handler(Schemainstancefacade* instance) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);

  /*sleep handler code*/
}

void $schemaname::wakeup_handler(Schemainstancefacade* instance) throw(){
  _$schemaname::myPrivateData* priv = 
     dynamic_cast<_$schemaname::myPrivateData*>(instance->private_data);

  /*wakeup handler*/
}

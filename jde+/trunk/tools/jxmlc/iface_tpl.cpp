#include "$ifacename.h"

REGISTER_INTERFACE($ifacename)

namespace _$ifacename{
  /*Private data class*/
  class myPrivateData: public PrivateData {
    /*data class declaration*/
  };
}

$ifacename::$ifacename()
  : Schemainterface(INTERFACE_INFO($ifacename)),
    private_data(new _$ifacename::myPrivateData()) {
  /*Constructor code*/
}

Data& $ifacename::get_data(const std::string& data_name) {
  _$ifacename::myPrivateData* priv = 
     dynamic_cast<_$ifacename::myPrivateData*>(self->private_data);
  /*data adquire code*/
}

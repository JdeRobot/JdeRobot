#include <jde+/schemainterface.h>
#include <jde+/privatedata.h>
#include <string>


class $ifacename: public Schemainterface {
 public:
  $ifacename();
 private:
  Pointer<PrivateData> private_data;
  virtual Data& get_data(const std::string& data_name);
};

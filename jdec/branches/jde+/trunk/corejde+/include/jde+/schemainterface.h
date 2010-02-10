#ifndef SCHEMAINTERFACE_H
#define SCHEMAINTERFACE_H
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>
#include <jde+/data.h>
#include <jde+/schematypes.h>

class Schemainterface;

/**
   Schemainterface class define the interface of a schema 
   implementation. This is a generic interface.
   This class is the abstract base class of any schema interface.
   Any schema implementation inherits from one Schemainterface 
   class that define what "it is".
   A schema implementation may have schemainterfaces as object 
   attributes that define what "it uses".
 
   Pschema inherited class is perceptual schemas interface.
   Mschema inherited class is motor schemas interface.
*/
class Schemainterface {
public:
  typedef Schemainterface* sifacemaker_t();

  struct schemainterface_info_t {
    std::string class_name;
    schema_type_enum type;
    std::set<std::string> perceptions;
    std::set<std::string> modulations;
    sifacemaker_t* constructor;
  };

  Schemainterface(const schemainterface_info_t* interface_info);

  /**
     Destructor.
  */
  virtual ~Schemainterface() {}

  /**
     Operator [] to access data by name like in a map.
     \exception std::logic_error if data_name doesn't exist in the interface
  */
  virtual Data& operator[](const std::string& data_name) throw(std::logic_error);

  /**
     Auto load mechanisms
  */
  static Schemainterface* instance(const std::string class_name) throw();
  static const schemainterface_info_t* class_info(const std::string class_name) throw();
  static void register_class(const schemainterface_info_t& class_info) throw();
  static schemainterface_info_t parse_class_info(const std::string class_name,
						 const std::string file_name) throw(std::runtime_error);
  /**
     Introspection info
  */
  const schemainterface_info_t* interface_info;

protected:
  /**
     Get Data refered by data_name
  */
  virtual Data& get_data(const std::string& data_name) = 0;
  
private:
  /**
     Auto load mechanisms
  */
  static const schemainterface_info_t* class_info_i(const std::string class_name) throw();
  static void register_class_i(const schemainterface_info_t& class_info) throw();
  
  static std::vector<schemainterface_info_t> class_infos;
  static std::map<std::string, int > info_by_classname;
};

#define REGISTER_INTERFACE(X) \
namespace _X { \
   static Schemainterface::schemainterface_info_t ii; \
   static Schemainterface* imaker() { return (Schemainterface*) new X(); } \
   class reg_proxy { \
   public: \
     reg_proxy() { \
       ii = Schemainterface::parse_class_info( "X", "X.xml"); \
       ii.constructor = imaker; \
       Schemainterface::register_class(ii); \
     } \
   }; \
   static reg_proxy rp; \
}

#define INTERFACE_INFO(X) _X::ii

#endif /*SCHEMAINTERFACE_H*/

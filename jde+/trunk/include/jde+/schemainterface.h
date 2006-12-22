#ifndef SCHEMAINTERFACE_H
#define SCHEMAINTERFACE_H
#include <string>
#include <vector>
#include <stdexcept>
#include <jde+/data.h>
#include <jde+/schematypes.h>

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
  const schema_type_enum type;/**< Interface type*/
  const std::string interface_name;/**< Interface name*/
  const std::string nickname;/**< Interface nick that a schema use to
				name it*/

  /**
     Constructor.
  */
  Schemainterface(const schema_type_enum type, const std::string& name, const std::string& nick)
    :type(type),interface_name(name),nickname(nick) throw() {}
  /**
     Destructor.
  */
  virtual ~Schemainterface() {}

  /**
     Test if interface has got data_name.
     \return true if interface has data_name
  */
  virtual bool has_data_name(const std::string& data_name) const throw() = 0;

  /**
     Get a vector with perceptions data names
  */
  virtual std::vector<std::string> const& get_perceptions_names() const throw() = 0;

  /**
     Get a vector with modulations data names
  */
  virtual std::vector<std::string> const& get_modulations_names() const throw() = 0;

  /**
     Operator [] to access data by name like in a map.
     \exception std::logic_error if data_name doesn't exist in the interface
  */
  virtual Data& operator[](const std::string& data_name) throw(std::logic_error){
    if (has_data_name(data_name))
      return get_data(data_name);
    else
      throw std::logic_error("Schemainterface: interface hasn't got '"
			     + data_name + "' data field");
  }
private:
  /**
     Get Data refered by data_name
  */
  virtual Data& get_data(const std::string& data_name) = 0;
};

/**
   Specialization of schemainterface for Pschemas
*/
class Pschema: public Schemainterface {
public:
  Pschema(const std::string& name, const std::string& nick)
    :Schemainterface(PSCHEMA,name,nick) throw() {}
}
;

/**
   Specialization of schemainterface for Mschemas
*/
class Mschema: public Schemainterface {
public:
  Mschema(const std::string& name, const std::string& nick)
    :Schemainterface(MSCHEMA,name,nick) throw() {}
}
;

#endif /*SCHEMAINTERFACE_H*/

#ifndef SCHEMAFACTORY_H
#define SCHEMAFACTORY_H
#include <jde+/classfactory.h>
#include <jde+/schematypes.h>
#include <string>
#include <vector>


class Schemainterface;
class Schemaimplementation;

/**
   Base class for schemas factories.
*/
class Schemafactory : public Classfactory {
public:
  typedef struct {
    std::string name;
    std::string nick;
    schema_type_enum type;
  } interfaceinfo_t;

  /**
     Constructor
  */
  Schemafactory();
  
  /**
     Destructor
  */
  virtual ~Schemafactory();

  /**
     Create a new instance.
     \return implementation instance
  */
  virtual Schemaimplementation* create_implementation() const = 0;

  /**
     Create main interface
     \return schema interface
  */
  virtual Schemainterface* create_interface() const = 0;

  /**
     Create children interfaces
     \return vector with children interfaces
  */
  //virtual std::vector<Schemainterface*> create_children_interfaces() const = 0;

  /**
     Get interface info from represented schema.
     \return info about schema main interface
  */
  virtual interfaceinfo_t const& get_interface_info() const;
					   

  /**
     Get children interface infos from represented schema.
     \return children interfaces infos
     
  */
  virtual std::vector<interfaceinfo_t> const& get_children_interface_infos() const;

  /**
     Get implementated branch
  */
  virtual std::string const& get_implementation_branch() const = 0;

  /**
     Get child prefered branches
  */
  virtual std::vector<std::string> const&
    get_child_branch_preferences(const std::string& child_nick) const = 0;

 protected:
  interfaceinfo_t interface_info;
  std::vector<interfaceinfo_t> children_interface_infos;
};


#endif /*SCHEMAFACTORY_H*/

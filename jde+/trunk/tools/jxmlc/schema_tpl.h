#include <jde+.h>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

namespace _<--!SCHEMA_NAME-->{
  Schemainterface* ci[] = {<--!CHILDREN_INTERFACES-->};

  std::vector<Schemainterface*> civ(ci,ci+(sizeof(ci)/sizeof(ci[0])));

  std::map<std::string,std::vector<std::string> > get_children_branches(){
    std::map<std::string,std::vector<std::string> > cb;
    
    <--!CHILDREN_BRANCH_PREF_PUSH-->
    return cb;
  }
}


class <--!SCHEMA_NAME-->: public Schemaimplementation{
 public:
  <--!SCHEMA_NAME-->()
     :Schemaimplementation(new <--!SCHEMA_INTERFACE-->(),_<--!SCHEMA_NAME-->::civ),priv(NULL) {}
  virtual ~<--!SCHEMA_NAME-->() {}
  virtual void init_schema();
  virtual void delete_schema();
  virtual void iteration();
  virtual void arbitrate(const std::string& request_nick);
  virtual bool check_preconditions();
  <--!USER_HANDLERS-->
  <--!USER_PRIVATE_MEMBERS-->
 private:
  void* priv;
};

namespace _<--!SCHEMA_NAME-->{
  class <--!SCHEMA_NAME-->_factory: public Schemafactory {
  public:
    virtual void create_instance(void** i) const{
      <--!SCHEMA_NAME-->* s;
      s = new <--!SCHEMA_NAME-->();
      *i = s;
    }

    virtual Schemaimplementation* create_schema() const {
      <--!SCHEMA_NAME-->* s = new <--!SCHEMA_NAME-->();
      return s;
    }

    virtual std::string const& get_interface_name() const{
      return interface.interface_name;
    }

    virtual schema_type_enum get_interface_type() const{
      return interface.type;
    }

    virtual std::vector<Schemainterface*> const& get_children_interfaces() const{
      return children;
    }

    virtual std::string const& get_implementation_branch() const{
      return implementation_branch;
    }

    virtual std::vector<std::string> const& 
      get_child_branch_preferences(const std::string& child_nick) const{
      std::map<std::string,std::vector<std::string> >::const_iterator cb_it;

      cb_it = children_branch_preferences.find(child_nick);
      if (cb_it != children_branch_preferences.end())
	return cb_it->second;
      throw std::logic_error("<--!SCHEMA_NAME-->_factory: unknown child\n");
    }
  private:
    static const <--!SCHEMA_INTERFACE--> interface;
    static const std::string implementation_branch;
    static const std::vector<Schemainterface*> children;
    static const std::map<std::string,std::vector<std::string> > children_branch_preferences;
  };

  const <--!SCHEMA_INTERFACE-->
    <--!SCHEMA_NAME-->_factory::interface;
  const std::string 
    <--!SCHEMA_NAME-->_factory::implementation_branch("<--!SCHEMA_BRANCH-->");
  const std::vector<Schemainterface*>
    <--!SCHEMA_NAME-->_factory::children(ci,ci+(sizeof(ci)/sizeof(ci[0])));
  const std::map<std::string,std::vector<std::string> >
    <--!SCHEMA_NAME-->_factory::children_branch_preferences = get_children_branches();
}

/*entry point to obtain an instance of the factory*/
extern "C"{
  _<--!SCHEMA_NAME-->::<--!SCHEMA_NAME-->_factory*
  get_<--!SCHEMA_NAME-->_factory(){
    return new _<--!SCHEMA_NAME-->::<--!SCHEMA_NAME-->_factory();
  }
}

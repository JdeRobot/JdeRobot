#ifndef LOADER_HPP
#define LOADER_HPP
#include <map>
#include <vector>
#include <string>
#include <exception>
#include <jde+/pointer.h>
#include <ltdl.h>
#include <iostream>
#include <cc++/file.h>
#ifdef CCXX_NAMESPACES
using ost::File;
#endif


template < typename F >
class Loader {
public:
  typedef F loaded_t;/**< Loaded type*/
  typedef Pointer<loaded_t> loaded_t_p;/**< Loaded type pointer*/
  typedef F* (* get_factory_t)();/**< Factory function type*/

  /**
     Constructor. Initialize lt_dlinit
  */
  Loader() throw(std::runtime_error) { 
    if (lt_dlinit() != 0)
      throw std::runtime_error("Loader: can't init ltdl"); 
  }

  /**
     Destructor. 
  */
  ~Loader() throw() { lt_dlexit(); }


  /**
     Get factory class from library.
     \param sopath library path
     \return factory pointer
  */
  loaded_t_p get_factory(const std::string sopath) throw(std::runtime_error){
    lt_dlhandle handle;
    std::map<std::string,lt_dlhandle>::iterator dll_it;

    dll_it = dlls.find(sopath);
    if (dll_it == dlls.end()) {/*hasn't been loaded yet*/
      handle = lt_dlopen(sopath.c_str());
      if (handle == NULL)
	throw std::runtime_error("Loader: Can't open library.(" + std::string(lt_dlerror()) + ")");
      else
	dlls[sopath] = handle;
    } else
      handle = dlls[sopath];

    lt_dlerror();/*clean error*/

    get_factory_t get_factoryp;
    /*factory name is soname without extension. FIXME: allow
      multiple factories in one so*/
    std::string soname(File::getFilename(sopath.c_str()));
    std::string::size_type ppos = soname.find_last_of(".");
    std::string factory_name = soname.substr(0,ppos);
    std::string get_factory_f_name = "get_" + factory_name + "_factory";
      
    get_factoryp = (get_factory_t)lt_dlsym(handle,get_factory_f_name.c_str());
    const char* err = lt_dlerror();
    if (err != NULL)
      throw std::runtime_error("Loader: Can't load 'get_factory' symbol (" + std::string(err) + ")");
    return loaded_t_p(get_factoryp());
  }

  /**
     Unload a previouly opened library.
     \param sopath library to unload
  */
  void unload_dll(const std::string& sopath) throw() {
    std::map<std::string,lt_dlhandle>::iterator dll_it;

    dll_it = dlls.find(sopath);
    if (dll_it != dlls.end()) {
      lt_dlclose(dll_it->second);
      dlls.erase(dll_it);
    }
  }
private:
  std::map<std::string,lt_dlhandle> dlls;/*open libraries*/
};

#endif /*LOADER_HPP*/

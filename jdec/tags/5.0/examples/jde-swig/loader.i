%module loader

%{
#include <loader.h>
%}

#ifdef SWIGPYTHON
%{

  
%}

%typemap(in) PyObject* schema_class {
  if (!PyCallable_Check($input)) {
      PyErr_SetString(PyExc_TypeError, "Need a callable object!");
      return NULL;
  }
  $1 = $input;
}

Schema_reg* new_pySchema_reg(const char* schema_name,
			     const char* interface_name,
			     PyObject* schema_class);
void delete_pySchema_reg(Schema_reg* const sr);
#endif /*SWIGPYTHON*/

int add_schema_reg(Schema_reg* const s);
Schema_reg* search_schema_reg(const char* interface_name);
void print_schema_regs();
void* load_so(const char* sopath);



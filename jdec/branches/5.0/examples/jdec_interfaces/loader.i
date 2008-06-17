%module loader

%{
#include <loader.h>
%}

#ifdef SWIGPYTHON
%{

  Schema* schema_ctor_py(Schema_reg_p sr,
			 const int sid, const int father_sid) {
    PyObject* pschema;
    PyObject* arglist;
    Schema* s = 0;

    if (sr && sr->cbdata) {
      arglist = Py_BuildValue("(ii)",sid,father_sid);
      pschema = PyObject_CallObject((PyObject *)sr->cbdata ,arglist);
      Py_DECREF(arglist);
      if (pschema) {
	  int res;
	  res = SWIG_ConvertPtr(pschema,(void**)&s,SWIGTYPE_p_Schema, SWIG_POINTER_DISOWN);
	  if (!SWIG_IsOK(res)) {
	    SWIG_exception_fail(SWIG_ArgError(res), 
				"in callback schema constructor"); 
	  }
	  s->private_data = (void*)pschema;
      } else {
	if (PyErr_Occurred())
	  PyErr_Print();
	fprintf(stderr, "Cannot create instance for schema %s\n",sr->schema_name);
      }
    }
    return s;
  fail:
    return 0;
  }

  void schema_dtor_py(Schema_reg_p sr, Schema* s) {
    if (sr && s) {
      PyObject* pschema = (PyObject*)s->private_data;
      Py_DECREF(pschema);
    }
  }
  
  Schema_reg* new_pySchema_reg(const char* schema_name,
			       const char* interface_name,
			       PyObject* schema_class) {
    Py_INCREF(schema_class);
    return new_Schema_reg(schema_name,interface_name,
			  schema_ctor_py,schema_dtor_py,
			  (void*)schema_class);
  }

  void delete_pySchema_reg(Schema_reg* const sr) {
    Py_XDECREF((PyObject*)sr->cbdata);
    delete_Schema_reg(sr);
  }
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

typedef struct Schema_reg {
  char schema_name[MAX_NAMES];
  char interface_name[MAX_NAMES];
  schema_ctor_f schema_ctor;
  schema_dtor_f schema_dtor;
  %extend {
    Schema_reg(const char* schema_name,
	       const char* interface_name,
	       schema_ctor_f const schema_ctor,
	       schema_dtor_f const schema_dtor,
	       void* cbdata = 0);
    ~Schema_reg();
    Schema* instance(const int sid,
		     const int father_sid);
    void delete_instance(Schema* s);
    %pythoncode %{
      __oldinit__ = __init__
      def __init__(self,*args):
          if len(args) == 3:
              this = _loader.new_pySchema_reg(*args)
          else:
              this = _loader.new_Schema_reg(*args)
          try: self.this.append(this)
          except: self.this = this
      __swig_destroy__ = _loader.delete_pySchema_reg
    %}
  }
}Schema_reg;

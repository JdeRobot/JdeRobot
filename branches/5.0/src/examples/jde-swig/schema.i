%module schema
%{
#include <schema.h>
%}

#ifdef SWIGPYTHON
%{
  Schema* schema_ctor_py(SFactory* sf,
			 const int sid) {
    PyObject* pschema;
    PyObject* arglist;
    Schema* s = 0;

    if (sf && sf->cbdata) {
      arglist = Py_BuildValue("(i)",sid);
      pschema = PyObject_CallObject((PyObject *)sf->cbdata ,arglist);
      Py_DECREF(arglist);
      fprintf(stderr, "despues de DECREF\n");
      if (pschema) {
	int res;
	fprintf(stderr, "pschema no es nulo\n");
	res = SWIG_ConvertPtr(pschema,(void**)&s,SWIGTYPE_p_Schema, SWIG_POINTER_DISOWN);
	if (!SWIG_IsOK(res)) {
	  SWIG_exception_fail(SWIG_ArgError(res), 
			      "in callback schema constructor"); 
	}
	s->private_data = (void*)pschema;
      } else {
	if (PyErr_Occurred())
	  PyErr_Print();
	fprintf(stderr, "Cannot create instance for schema %s\n",sf->schema_name);
      }
    }
    return s;
  fail:
    return 0;
  }

  void schema_dtor_py(SFactory* sf, Schema* s) {
    if (sf && s) {
      PyObject* pschema = (PyObject*)s->private_data;
      Py_DECREF(pschema);
    }
  }
  
  SFactory* new_pySFactory(PyObject* self,
			   const char* schema_name,
			   const char* interface_name) {
    Py_INCREF(self);
    return new_SFactory(schema_name,interface_name,
			schema_ctor_py,schema_dtor_py,
			(void*)schema_class);
  }

  void delete_pySFactory(SFactory* const sf) {
    Py_XDECREF((PyObject*)sf->cbdata);
    delete_SFactory(sf);
  }


  static int init_pycb(Schema* const s) {
    PyObject* result;
    int fres = 0;

    result = PyObject_CallObject((PyObject *) s->init_cbdata,0);
    if (result) {                                 // If no errors, return float
      fres = (int)PyInt_AsLong(result);
    } else {
      if (PyErr_Occurred())
	PyErr_Print();
      fprintf(stderr, "Cannot init schema\n");
    }
    Py_XDECREF(result);
    return fres;
  }

  static int iteration_pycb(Schema* const s) {
    PyObject* result;
    int fres = 0;

    result = PyObject_CallObject((PyObject *) s->iteration_cbdata,0);
    if (result) {                                 // If no errors, return float
      fres = (int)PyInt_AsLong(result);
    } else {
      if (PyErr_Occurred())
	PyErr_Print();
      fprintf(stderr, "Cannot exec iteration on schema\n");
    }
    Py_XDECREF(result);
    return fres;
  }

  static void* cast_pycb(Schema* const s, const char* interface_name) {
    PyObject* result;
    PyObject* arglist;
    int res;
    void *retval = 0;

    if (s) {
      arglist = Py_BuildValue("(s)",interface_name);
      result = PyObject_CallObject((PyObject *) s->cast_cbdata,arglist);
      Py_DECREF(arglist);
      if (result) {                                 // If no errors, return float
	res = SWIG_ConvertPtr(result,SWIG_as_voidptrptr(&retval),0,SWIG_POINTER_DISOWN);
	if (!SWIG_IsOK(res)) {
	  SWIG_exception_fail(SWIG_ArgError(res), 
			      "in callback '" "cast_pycb" "'"); 
	}
      } else {
	if (PyErr_Occurred())
	  PyErr_Print();
	fprintf(stderr, "Cannot get interface %s\n",interface_name);
      }
      Py_XDECREF(result);
    }
    return retval;
  fail:
    Py_XDECREF(result);
    return 0;
  }

  PyObject* get_pymethod(PyObject* o, const char* method_name) {
    PyObject* method_o;
    
    if ( method_o = PyObject_GetAttrString(o,method_name) == 0)
      return NULL;
    
    if ( PyCallable_Check(method_o) == 0)
      return NULL;
    return method_o;
  }

  Schema* new_pySchema(PyObject* self, const int sid) {
    PyObject *init_cbo,*cast_cbo,*iteration_cbo;

    if ( (init_cbo = get_pymethod(self,"init")) == 0) {
      PyErr_SetString(PyExc_RuntimeError, "Expected an object with init method.");
      return NULL;
    }
    Py_INCREF(init_cbo);

    if ( (iteration_cbo = get_pymethod(self,"iteration")) == 0) {
      PyErr_SetString(PyExc_RuntimeError, "Expected an object with iteration method.");
      return NULL;
    }
    Py_INCREF(iteration_cbo);

    if ( (cast_cbo = get_pymethod(self,"cast")) == 0) {
      PyErr_SetString(PyExc_RuntimeError, "Expected an object with cast method.");
      return NULL;
    }
    Py_INCREF(cast_cbo);
    
    return new_Schema(sid,
		      &init_pycb,(void*)init_cbo,
		      &iteration_pycb,(void*)iteration_cbo,
		      &cast_pycb,cast_cbo,
		      0);
  }

  void delete_pySchema(Schema* s) {
    Py_XDECREF((PyObject*)s->init_cbdata);
    Py_XDECREF((PyObject*)s->cast_cbdata);
    delete_Schema(s);
  }
%}

SFactory* new_pySFactory(PyObject* self,
			 const char* schema_name,
			 const char* interface_name);
void delete_pySFactory(SFactory* const sf);
Schema* new_pySchema(PyObject* self, const int sid);
void delete_pySchema(Schema* s);

#endif /*SWIGPYTHON*/

int add_schema_reg(SFactory* const s);
SFactory* search_schema_reg(const char* interface_name);

typedef struct SFactory {
  char schema_name[MAX_NAMES];
  char interface_name[MAX_NAMES];
  schema_ctor_f schema_ctor;
  schema_dtor_f schema_dtor;
  %extend {
    SFactory(const char* schema_name,
	       const char* interface_name,
	       schema_ctor_f const schema_ctor,
	       schema_dtor_f const schema_dtor,
	       void* cbdata = 0);
    ~SFactory();
    Schema* instance(const int sid,
		     const int father_sid);
    void delete_instance(Schema* s);
    %pythoncode %{
      __oldinit__ = __init__
      def __init__(self,*args):
          if len(args) == 3:
              this = _loader.new_pySFactory(*args)
          else:
              this = _loader.new_SFactory(*args)
          try: self.this.append(this)
          except: self.this = this
      __swig_destroy__ = _loader.delete_pySFactory
    %}
  }
}SFactory;


typedef struct Schema {
  int sid;
  int father_sid;
  void* private_data;
  %extend {
    Schema(const int sid,
	   schema_init_f const init_cb,void* const init_cbdata,
	   schema_iteration_f const iteration_cb,void* const iteration_cbdata,
	   schema_cast_f const cast_cb,void* const cast_cbdata,
	   void* const private_data = 0);
    ~Schema();
    int init();
    int iteration();
    void* cast(const char* interface_name);
    %pythoncode %{
      __oldinit__ = __init__
      def __init__(self,*args):
          if len(args) == 2:
              this = _schema.new_pySchema(self,*args)
          else:
              this = _schema.new_Schema(*args)
          try: self.this.append(this)
          except: self.this = this
      __swig_destroy__ = _schema.delete_pySchema
    %}
  }
}Schema;


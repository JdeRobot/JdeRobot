%module schema
%{
#include <schema.h>
%}

#ifdef SWIGPYTHON
%inline %{
typedef PyObject* cb_object;
%}

%typemap(in) cb_object {
  if (!PyCallable_Check($input)) {
      PyErr_SetString(PyExc_TypeError, "Need a callable object!");
      return NULL;
  }
  $1 = $input;
}

%{
  static int init_pycb(Schema* const s) {
    PyObject* result;
    //PyObject* arglist;
    int fres = 0;

    //arglist = Py_BuildValue("()");
    result = PyObject_CallObject((PyObject *) s->init_cbdata,0);
    //Py_DECREF(arglist);
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

  Schema* new_pySchema(const int sid, const int father_sid,
		       cb_object init_cbo, cb_object cast_cbo) {
    Py_INCREF(init_cbo);
    Py_INCREF(cast_cbo);
    return new_Schema(sid,father_sid,
		      &init_pycb,(void*)init_cbo,
		      &cast_pycb,cast_cbo);
  }

  void delete_pySchema(Schema* s) {
    Py_XDECREF((PyObject*)s->init_cbdata);
    Py_XDECREF((PyObject*)s->cast_cbdata);
    delete_Schema(s);
  }
%}

//%constant int init_pycb(Schema* const s);
//%constant void* cast_pycb(Schema* const s, const char* interface_name);


Schema* new_pySchema(const int sid, const int father_sid,
		     cb_object init_cbo, cb_object cast_cbo);
void delete_pySchema(Schema* s);
#endif /*SWIGPYTHON*/


typedef struct Schema {
  int sid;
  int father_sid;
  void* private_data;
  %extend {
    Schema(const int sid, const int father_sid,
	   Schema_init_f const init_cb,void* const init_cbdata,
	   Schema_cast_f const cast_cb,
	   void* const cast_cbdata,
	   void* const private_data = 0);
    ~Schema();
    int init();
    void* cast(const char* interface_name);
    %pythoncode %{
      __oldinit__ = __init__
      def __init__(self,*args):
          if len(args) == 4:
              this = _schema.new_pySchema(*args)
          else:
              this = _schema.new_Schema(*args)
          try: self.this.append(this)
          except: self.this = this
      __swig_destroy__ = _schema.delete_pySchema
    %}
  }
}Schema;


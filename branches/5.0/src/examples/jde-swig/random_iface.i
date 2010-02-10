%module random_iface
%{
#include <random_iface.h>
%}

// Grab a Python function object as a Python object.
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
  static float get_random_pywrap(Random* i) {
    PyObject* result;
    //PyObject* arglist;
    float fres = 0;

    //arglist = Py_BuildValue("()");
    result = PyObject_CallObject((PyObject *) i->get_random_cbdata,0);
    //Py_DECREF(arglist);
    if (result) {                                 // If no errors, return float
      fres = (float)PyFloat_AsDouble(result);
    } else {
      if (PyErr_Occurred())
	PyErr_Print();
      fprintf(stderr, "Cannot get_random from random\n");
    }
    Py_XDECREF(result);
    return fres;
  }

  static void set_seed_pywrap(Random* i, const int seed) {
    PyObject* arglist;

    arglist = Py_BuildValue("(i)",seed);
    PyObject_CallObject((PyObject *) i->set_seed_cbdata,arglist);
    Py_DECREF(arglist);
    if (PyErr_Occurred()) {
      PyErr_Print();
      fprintf(stderr, "Cannot set_seed for random\n");
    }
  }

  Random* new_pyRandom(cb_object get_random_cbo,
		       cb_object set_seed_cbo) {
    Py_INCREF(get_random_cbo);
    Py_INCREF(set_seed_cbo);
    return new_Random(&get_random_pywrap,(void*)get_random_cbo,
		      &set_seed_pywrap,(void*)set_seed_cbo);
  }

  void delete_pyRandom(Random* i) {
    Py_XDECREF((PyObject*)i->get_random_cbdata);
    Py_XDECREF((PyObject*)i->set_seed_cbdata);
    delete_Random(i);
  }
%}

Random* new_pyRandom(cb_object get_random_cbo,
		     cb_object set_seed_cbo);
void delete_pyRandom(Random* i);

Random* cast_Random(Schema* const s);
#endif /*SWIGPYTHON*/

%constant char* IFACE_NAME = RANDOM_IFACE_NAME;

typedef struct {
  %extend {
    Random(Random_get_random_f const get_random_cb,
	   void* const get_random_cbdata,
	   Random_set_seed_f const set_seed_cb,
	   void* const set_seed_cbdata);
    ~Random();
    float get_random();
    void set_seed(const int seed);
    %pythoncode %{
      __oldinit__ = __init__
      def __init__(self,*args):
          if len(args) == 2:
              this = _random_iface.new_pyRandom(*args)
          else:
              this = _random_iface.new_Random(*args)
          try: self.this.append(this)
          except: self.this = this
      __swig_destroy__ = _random_iface.delete_pyRandom
    %}
  }
}Random;

#include <loader.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>


#define MAX_SCHEMAS 10
static Schema_reg* s_regs[MAX_SCHEMAS];
static int ns = 0;

static void* handle[MAX_SCHEMAS];
static int n_handles = 0;

void init_Schema_reg(Schema_reg* sr,
		     const char* schema_name,
		     const char* interface_name,
		     schema_ctor_f const schema_ctor,
		     schema_dtor_f const schema_dtor,
		     void* cbdata) {
  strncpy(sr->schema_name,schema_name,MAX_NAMES-1);
  sr->schema_name[MAX_NAMES-1] = '\n';
  strncpy(sr->interface_name,interface_name,MAX_NAMES-1);
  sr->interface_name[MAX_NAMES-1] = '\n';
  sr->schema_ctor = schema_ctor;
  sr->schema_dtor = schema_dtor;
  sr->cbdata = cbdata;
}

Schema_reg* new_Schema_reg(const char* schema_name,
			   const char* interface_name,
			   schema_ctor_f const schema_ctor,
			   schema_dtor_f const schema_dtor,
			   void* cbdata) {
  Schema_reg* sr;
#ifdef __cplusplus
  sr = new Schema_reg(schema_name,interface_name,
		      schema_ctor,schema_dtor,cbdata);
#else
  sr = (Schema_reg*)malloc(sizeof(Schema_reg));
  init_Schema_reg(sr,schema_name,interface_name,
		  schema_ctor,schema_dtor,cbdata);
#endif /*__cplusplus*/
  return sr;
}

// Schema_reg* copy_Schema_reg(Schema_reg* dst, const Schema_reg* src) {
//   return new_Schema_reg(src->schema_name,
// 			src->interface_name,
// 			src->schema_ctor,src->schema_dtor,
// 			src->cbdata,dst);
// }

void delete_Schema_reg(Schema_reg* const sr) {
#ifdef __cplusplus
  delete sr;
#else
  free(sr);
#endif /*__cplusplus*/
}

Schema* Schema_reg_instance(Schema_reg* const sr,
			    const int sid,
			    const int father_sid) {
  return sr->schema_ctor(sr,sid,father_sid);
}

void Schema_reg_delete_instance(Schema_reg* const sr, Schema* s) {
  sr->schema_dtor(sr,s);
}

int add_schema_reg(Schema_reg* const sr) {
  if (ns >= MAX_SCHEMAS)
    return -1;

  fprintf(stderr, "%s: registering %s with id %d, implements %s\n",
	  __PRETTY_FUNCTION__,sr->schema_name,ns,sr->interface_name);
  
  s_regs[ns] = sr;
  return ns++;
}

Schema_reg* search_schema_reg(const char* interface_name) {
  int i;
  
  for (i=0; i < ns; i++) {
    if (strcmp(interface_name,s_regs[i]->interface_name) == 0)
      return s_regs[i];
  }
  fprintf(stderr, "%s: schema_reg for interface %s not found\n",
	  __PRETTY_FUNCTION__,interface_name);
  return 0;
}

void print_schema_regs() {
  int i;

  for (i=0; i < ns; i++)
    fprintf(stderr, "%d: %s->%s\n",
	    i, s_regs[i]->schema_name, s_regs[i]->interface_name);
}

void* load_so(const char* sopath) {
  void* h;
  if (n_handles < MAX_SCHEMAS && 
      (h = dlopen(sopath,RTLD_NOW)) != 0)
    handle[n_handles++] = h;
  if (!h)
    fprintf(stderr,"%s\n",dlerror());
  return h;
}

PyObject* load_py(const char* pypath, int argc, char** argv) {
  PyObject* py_name, *py_module = 0;

  if (! Py_IsInitialized()) {
    Py_Initialize();
    PySys_SetArgv(argc, argv);
  }

  py_name = PyString_FromString(pypath);
  if (py_name == 0) {
    PyErr_Print();
    return 0;
  }

  py_module = PyImport_Import(py_name);
  Py_DECREF(py_name);

  if (py_module == 0) {
    PyErr_Print();
    fprintf(stderr, "%s: Failed to load %s\n",__PRETTY_FUNCTION__,pypath);
    return 0;
  }
  return py_module;
}

  
#ifdef __cplusplus

Schema_reg::Schema_reg(const char* schema_name,
		       const char* interface_name,
		       schema_ctor_f const schema_ctor,
		       schema_dtor_f const schema_dtor,
		       void* cbdata) {
  init_Schema_reg(this,schema_name,interface_name,
		  schema_ctor,schema_dtor,cbdata);
}

Schema_reg::~Schema_reg() {}


Schema* Schema_reg::instance(const int sid,
			     const int father_sid) {
  return Schema_reg_instance(this,sid,father_sid);
}

void Schema_reg::delete_instance(Schema* s) {
  Schema_reg_delete_instance(this,s);
}

int Schema_reg::add(Schema_reg* const sr) {
  return add_schema_reg(sr);
}

Schema_reg* Schema_reg::search(const char* interface_name) {
  return search_schema_reg(interface_name);
}

void Schema_reg::print() {
  print_schema_regs();
}

// Schema_regWrap::Schema_regWrap(Schema_reg* const sr)
//   : wrapped_sr(sr) {}

// Schema_regWrap::~Schema_regWrap() {}

// SchemaWrap* Schema_regWrap::instance(const int sid,
// 				     const int father_sid) {
//   return new SchemaWrap(Schema_reg_instance(this->wrapped_sr,sid,father_sid));
// }

// void Schema_regWrap::del_instance(SchemaWrap* s) {
//   Schema& tmp_s = *s;

//   Schema_reg_del_instance(this->wrapped_sr,&tmp_s);
//   delete s;
// }

// Schema_regWrap::operator Schema_reg& () {
//   return *this->wrapped_sr;
// }

// Schema_regWrap* Schema_regWrap::search(char* const interface_name) {
//   Schema_regWrap* srb = 0;
//   Schema_reg* sr = search_schema_reg(interface_name);

//   if (sr != 0)
//     srb = new Schema_regWrap(sr);

//   return srb;
// }

// void Schema_regWrap::print() {
//   print_schema_regs();
// }


// Schema_regBase::Schema_regBase(char* const schema_name,
// 			       char* const interface_name,
// 			       schema_ctor_f const schema_ctor,
// 			       schema_dtor_f const schema_dtor,
// 			       void* cbdata)
//   :Schema_regWrap(new_Schema_reg(schema_name,interface_name,
// 				 schema_ctor,schema_dtor,cbdata)) {}

// Schema_regBase::~Schema_regBase() {
//   delete_Schema_reg(this->wrapped_sr);
// }


// int Schema_regBase::add(Schema_regBase* const sr) {
//   Schema_reg& tmp_sr = *sr;

//   return add_schema_reg(&tmp_sr);
// }
#endif /*__cplusplus*/

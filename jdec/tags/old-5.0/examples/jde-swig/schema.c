#include <schema.h>
#include <stdlib.h>
#include <assert.h>

/*Schema instances*/
static GList* instances = 0;

#ifdef __cplusplus
#include <iostream>
class SchemaC: public Schema {
public:
  SchemaC(const int sid,
	  schema_init_f const init_cb,void* const init_cbdata,
	  schema_iteration_f const iteration_cb,void* const iteration_cbdata,
	  schema_cast_f const cast_cb,
	  void* const cast_cbdata,
	  void* const private_data = 0)
    : Schema(sid,init_cb,init_cbdata,iteration_cb,iteration_cbdata,
	     cast_cb,cast_cbdata,private_data) {}
  virtual int init() {
    return Schema_init(this);
  }

  virtual int iteration() {
    return Schema_iteration(this);
  }

  virtual void* cast(const char* interface_name) {
    return Schema_cast(this,interface_name);
  }
};
#endif


Schema* new_Schema(const int sid,
		   schema_init_f const init_cb,void* const init_cbdata,
		   schema_iteration_f const iteration_cb,void* const iteration_cbdata,
		   schema_cast_f const cast_cb,
		   void* const cast_cbdata,
		   void* const private_data) {
  Schema* s;

#ifdef __cplusplus
  s = new SchemaC(sid,init_cb,init_cbdata,iteration_cb,iteration_cbdata,
		  cast_cb,cast_cbdata,private_data);
#else
  s = (Schema*)malloc(sizeof(Schema));
  s->sid = sid;
  s->init_cb = init_cb;
  s->init_cbdata = init_cbdata;
  s->iteration_cb = iteration_cb;
  s->iteration_cbdata = iteration_cbdata;
  s->cast_cb = cast_cb;
  s->cast_cbdata = cast_cbdata;
  s->private_data = private_data;
#endif /*__cplusplus*/
  return s;
}

// Schema* copy_Schema(Schema* dst, const Schema* src) {
//   return new_Schema(src->sid,src->father_sid,
// 		    src->init_cb,src->init_cbdata,
// 		    src->cast_cb,src->cast_cbdata,
// 		    src->private_data,dst);
// }

void delete_Schema(Schema* const s) {
#ifdef __cplusplus
  delete s;
#else
  free(s);
#endif /*__cplusplus*/
}

int Schema_init(Schema* const s) {
  assert(s != 0);
  assert(s->init_cb != 0);
  return s->init_cb(s);
}

int Schema_iteration(Schema* const s) {
  assert(s != 0);
  assert(s->iteration_cb != 0);
  return s->iteration_cb(s);
}

void* Schema_cast(Schema* const s,
		  const char* interface_name) {
  assert(s != 0);
  assert(s->cast_cb != 0);
  return s->cast_cb(s,interface_name);
}

int Schema_cmp(Schema* const a, Schema* const b) {
  if (a->sid < b->sid)
    return -1;
  else if (a->sid == b->sid)
    return 0;
  else
    return 1;
}

int Schema_add_instance(Schema* const s) {
  instances = g_list_append(instances,s);
  return g_list_length(instances);
}

int Schema_del_instance(Schema* const s) {
  instances = g_list_remove(instances,s);
  return g_list_length(instances);
}

const GList* Schema_get_instances() {
  return instances;
}

#ifdef __cplusplus
#include <iostream>
Schema::Schema(const int sid)
  :sid(sid),
   init_cb(Schema::sinit),init_cbdata(0),
   iteration_cb(Schema::siteration),iteration_cbdata(0),
   cast_cb(Schema::scast),cast_cbdata(0),
   private_data(0) {}
  
Schema::Schema(const int sid,
	       schema_init_f const init_cb,void* const init_cbdata,
	       schema_iteration_f const iteration_cb,void* const iteration_cbdata,
	       schema_cast_f const cast_cb,
	       void* const cast_cbdata,
	       void* const private_data)
  :sid(sid),
     init_cb(init_cb),init_cbdata(init_cbdata),
     iteration_cb(iteration_cb),iteration_cbdata(iteration_cbdata),
     cast_cb(cast_cb),cast_cbdata(cast_cbdata),
     private_data(private_data) {}

Schema::~Schema() {}

const GList* Schema::get_instances() {
  return Schema_get_instances();
}

int Schema::add_instance(Schema* const s) {
  return Schema_add_instance(s);
}

int Schema::del_instance(Schema* const s) {
  return Schema_del_instance(s);
}

int Schema::sinit(Schema* const s) {
  return s->init();
}

int Schema::siteration(Schema* const s) {
  return s->iteration();
}

void* Schema::scast(Schema* const s,
		    const char* interface_name) {
  return s->cast(interface_name);
}
#endif /*__cplusplus*/

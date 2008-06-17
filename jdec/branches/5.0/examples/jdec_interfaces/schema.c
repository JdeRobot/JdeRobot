#include <schema.h>
#include <stdlib.h>
#include <assert.h>
#ifdef __cplusplus
#include <iostream>

class SchemaC: public Schema {
public:
  SchemaC(const int sid, const int father_sid,
	  Schema_init_f const init_cb,void* const init_cbdata,
	  Schema_cast_f const cast_cb,
	  void* const cast_cbdata,
	  void* const private_data = 0)
    : Schema(sid,father_sid,init_cb,init_cbdata,
	     cast_cb,cast_cbdata,private_data) {}
  virtual int init() {
    return Schema_init(this);
  }

  virtual void* cast(const char* interface_name) {
    return Schema_cast(this,interface_name);
  }
};
#endif



Schema* new_Schema(const int sid, const int father_sid,
		   Schema_init_f const init_cb,void* const init_cbdata,
		   Schema_cast_f const cast_cb,
		   void* const cast_cbdata,
		   void* const private_data) {
  Schema* s;

#ifdef __cplusplus
  s = new SchemaC(sid,father_sid,init_cb,init_cbdata,
		  cast_cb,cast_cbdata,private_data);
#else
  s = (Schema*)malloc(sizeof(Schema));
  s->sid = sid;
  s->father_sid = father_sid;
  s->init_cb = init_cb;
  s->init_cbdata = init_cbdata;
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

void* Schema_cast(Schema* const s,
		  const char* interface_name) {
  assert(s != 0);
  assert(s->cast_cb != 0);
  return s->cast_cb(s,interface_name);
}

#ifdef __cplusplus
#include <iostream>
Schema::Schema(const int sid, const int father_sid)
  :sid(sid),father_sid(father_sid),
   init_cb(Schema::sinit),init_cbdata(0),
   cast_cb(Schema::scast),cast_cbdata(0),
   private_data(0) {}
  
Schema::Schema(const int sid, const int father_sid,
	       Schema_init_f const init_cb,void* const init_cbdata,
	       Schema_cast_f const cast_cb,
	       void* const cast_cbdata,
	       void* const private_data)
  :sid(sid),father_sid(father_sid),
   init_cb(init_cb),init_cbdata(init_cbdata),
   cast_cb(cast_cb),cast_cbdata(cast_cbdata),
   private_data(private_data) {}

Schema::~Schema() {}

/* int Schema::init() { */
/*   if (this->init_cb == Schema::sinit) { */
/*     std::cerr << "Schema::init you must overload this method in your subclass\n"; */
/*     return 0; */
/*   } */
/*   return Schema_init(this); */
/* } */

/* void* Schema::cast(const char* interface_name) { */
/*   if (this->cast_cb == Schema::scast) { */
/*     std::cerr << "Schema::cast you must overload this method in your subclass\n"; */
/*     return 0; */
/*   } */
/*   return Schema_cast(this,interface_name); */
/* } */

int Schema::sinit(Schema* const s) {
  return s->init();
}

void* Schema::scast(Schema* const s,
		    const char* interface_name) {
  return s->cast(interface_name);
}

// SchemaWrap::SchemaWrap(Schema* const s)
// : sid(s->sid), father_sid(s->father_sid), wrapped_s(s) {}

// SchemaWrap::~SchemaWrap() {}

// int SchemaWrap::init() {
//   return Schema_init(this->wrapped_s);
// }

// void* SchemaWrap::cast(char* const interface_name) {
//   return Schema_cast(this->wrapped_s,interface_name);
// }

// SchemaWrap::operator Schema& () {
//   return *(this->wrapped_s);
// }

// SchemaBase::SchemaBase(const int sid, const int father_sid)
//   : SchemaWrap(new_Schema(sid,father_sid,
// 			  SchemaBase::sinit,this,
// 			  SchemaBase::scast,this,
// 			  this)) {}

// SchemaBase::~SchemaBase() {
//   delete_Schema(this->wrapped_s);
// }

// /*int SchemaBase::init() {
//   std::cerr << "init() not defined";
//   return 0;
// }

// void* SchemaBase::cast(char* const interface_name) {
//   std::cerr << "cast() not defined";
//   return 0;
// }
// */

// int SchemaBase::sinit(Schema* const s) {
//   SchemaBase* sb_instance = static_cast<SchemaBase*>(s->init_cbdata);
//   return sb_instance->init();
// }

// void* SchemaBase::scast(Schema* const s,
// 				 char* const interface_name) {
//   SchemaBase* sb_instance = static_cast<SchemaBase*>(s->cast_cbdata);
//   return sb_instance->cast(interface_name);
// }


#endif /*__cplusplus*/

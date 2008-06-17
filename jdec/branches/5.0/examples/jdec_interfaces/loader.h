#ifndef LOADER_H
#define LOADER_H
#include <schema.h>
#include <Python.h>

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

typedef struct Schema_reg* Schema_reg_p;
typedef Schema* (*schema_ctor_f)(Schema_reg_p sr,
				 const int sid, const int father_sid);
typedef void (*schema_dtor_f)(Schema_reg_p sr, Schema* s);

typedef struct Schema_reg{
  char schema_name[MAX_NAMES];
  char interface_name[MAX_NAMES];/*FIXME: lista de interfaces implementados*/
  schema_ctor_f schema_ctor;
  schema_dtor_f schema_dtor;
  void* cbdata;
#ifdef __cplusplus
  Schema_reg(const char* schema_name,
	     const char* interface_name,
	     schema_ctor_f const schema_ctor,
	     schema_dtor_f const schema_dtor,
	     void* cbdata = 0);
  virtual ~Schema_reg();
  Schema* instance(const int sid,
		   const int father_sid);
  void delete_instance(Schema* s);
  static int add(Schema_reg* const sr);
  static Schema_reg* search(const char* interface_name);
  static void print();
#endif /*__cplusplus*/
}Schema_reg;

Schema_reg* new_Schema_reg(const char* schema_name,
			   const char* interface_name,
			   schema_ctor_f const schema_ctor,
			   schema_dtor_f const schema_dtor,
			   void* cbdata = 0);
  //Schema_reg* copy_Schema_reg(Schema_reg* dst, const Schema_reg* src);
void delete_Schema_reg(Schema_reg* const sr);
Schema* Schema_reg_instance(Schema_reg* const sr,
			    const int sid,
			    const int father_sid);
void Schema_reg_delete_instance(Schema_reg* const sr, Schema* s);

/*loader functions*/
/*
  add_schema_reg
  Adds sr schema_reg to the list.
  sr isn't copied so it must be not deallocated
*/
int add_schema_reg(Schema_reg* const sr);

/*
  search_schema_reg
  Search a schema_reg that implements interface_name.
  A copy is returned that must be deallocated.
*/
Schema_reg* search_schema_reg(const char* interface_name);


void print_schema_regs();

void* load_so(const char* sopath);
PyObject* load_py(const char* pypath, int argc = 0, char** argv = 0);

#ifdef __cplusplus
#define ADD_SCHEMA_REG(SNAME,SINTERFACE,CTOR,DTOR,PDATA)    \
static Schema_reg __reg(SNAME,SINTERFACE,CTOR,DTOR,PDATA); \
static int __loaded = Schema_reg::add(&__reg);
#else
#define ADD_SCHEMA_REG(SNAME,SINTERFACE,CTOR,DTOR,PDATA)    \
static Schema_reg __reg = {SNAME,SINTERFACE,CTOR,DTOR,PDATA}; \
__attribute__((constructor)) void __add_schema_reg() { \
   add_schema_reg(&__reg); \
}
#endif

#ifdef __cplusplus
} /*extern "C"*/

// class Schema_regWrap {
// public:
//   Schema_regWrap(Schema_reg* const sr);
//   virtual ~Schema_regWrap();
//   SchemaWrap* instance(const int sid,
// 		       const int father_sid);
//   void del_instance(SchemaWrap* s);
//   operator Schema_reg& ();
//   static Schema_regWrap* search(char* const interface_name);
//   static void print();
// protected:
//   Schema_reg* const wrapped_sr;
// };

// class Schema_regBase: public Schema_regWrap {
// public:
//   Schema_regBase(char* const schema_name,
// 		 char* const interface_name,
// 		 schema_ctor_f const schema_ctor,
// 		 schema_dtor_f const schema_dtor,
// 		 void* cbdata = 0);
//   virtual ~Schema_regBase();
//   static int add(Schema_regBase* const sr);
// };


#endif /*__cplusplus*/
#endif /*LOADER_H*/

#ifndef SCHEMA_H
#define SCHEMA_H
#include <defs.h>
#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif


/*forward declarations*/
typedef struct SFactory* SFactory_p;
typedef struct Schema* Schema_p;

/*function types*/
typedef Schema* (*schema_ctor_f)(SFactory_p sf,const int sid);
typedef void (*schema_dtor_f)(SFactory_p sf, Schema* s);
typedef int (*schema_init_f)(Schema_p const s);
typedef int (*schema_iteration_f)(Schema_p const s);
typedef void* (*schema_cast_f)(Schema_p const s,
			       const char* interface_name);

typedef struct SFactory{
  char schema_name[MAX_STRLEN];
  char interface_name[MAX_STRLEN];/*FIXME: lista de interfaces implementados*/
  schema_ctor_f schema_ctor;
  schema_dtor_f schema_dtor;
  void* cbdata;
  GList* instances;
#ifdef __cplusplus
  SFactory(const char* schema_name,
	   const char* interface_name,
	   schema_ctor_f const schema_ctor,
	   schema_dtor_f const schema_dtor,
	   void* cbdata = 0);
  virtual ~SFactory();
  Schema_p create();
  void destroy(Schema_p s);
  const GList* get_instances();
  static int add_factory(SFactory* const sf);
  //static int del_factory(SFactory* const sf);
  static SFactory* search_factory(const char* interface_name);
  static const GList* get_factories();
#endif /*__cplusplus*/
}SFactory;

typedef struct Schema{
  int sid;
  schema_init_f init_cb;
  void* init_cbdata;
  schema_iteration_f iteration_cb;
  void* iteration_cbdata;
  schema_cast_f cast_cb;
  void* cast_cbdata;
  void* private_data;
#ifdef __cplusplus
  Schema(const int sid);
  Schema(const int sid,
	 schema_init_f const init_cb,void* const init_cbdata,
	 schema_iteration_f const iteration_cb,void* const iteration_cbdata,
	 schema_cast_f const cast_cb,void* const cast_cbdata,
	 void* const private_data = 0);
  virtual ~Schema();
  virtual int init() = 0;
  virtual int iteration() = 0;
  virtual void* cast(const char* interface_name) = 0;
  static const GList* get_instances();
  static int add_instance(Schema* const s);
  static int del_instance(Schema* const s);
private:
  static int sinit(Schema* const s);
  static int siteration(Schema* const s);
  static void* scast(Schema* const s, const char* interface_name);
#endif /*__cplusplus*/
}Schema;


/*SFactory methods*/
SFactory* new_SFactory(const char* schema_name,
		       const char* interface_name,
		       schema_ctor_f const schema_ctor,
		       schema_dtor_f const schema_dtor,
		       void* cbdata = 0);
void delete_SFactory(SFactory* const sf);
Schema* SFactory_create(SFactory* const sf);
void SFactory_destroy(SFactory* const sf, Schema* s);
const GList* SFactory_get_instances(SFactory* const sf);

/*SFactory static methods*/
/*
  SFactory_add_factory
  Adds sf sfactory to the list.
  sf isn't copied so it must be not deallocated
*/
int SFactory_add_factory(SFactory* const sf);


/*
  SFactory_del_factory
  Del sf sfactory from the list.
*/
//int SFactory_del_factory(SFactory* const sf);

/*
  SFactory_search_factory
  Search a sfactory that implements interface_name.
  A copy is returned that must be deallocated.
*/
SFactory* SFactory_search_factory(const char* interface_name);

/*
  SFactory_get_factories
  Get the list of factories loaded
*/
const GList* SFactory_get_factories();

#ifdef __cplusplus
#define ADD_SFACTORY(SNAME,SINTERFACE,CTOR,DTOR,PDATA)    \
static SFactory __reg(SNAME,SINTERFACE,CTOR,DTOR,PDATA); \
static int __loaded = SFactory::add_factory(&__reg);
#else
#define ADD_SFACTORY(SNAME,SINTERFACE,CTOR,DTOR,PDATA)    \
static SFactory __reg = {SNAME,SINTERFACE,CTOR,DTOR,PDATA}; \
__attribute__((constructor)) void __add_sfactory() { \
   SFactory_add_factory(&__reg); \
}
#endif

/*Schema functions*/
Schema* new_Schema(const int sid,
		   schema_init_f const init_cb,
		   void* const init_cbdata,
		   schema_iteration_f const iteration_cb,
		   void* const iteration_cbdata,
		   schema_cast_f const cast_cb,
		   void* const cast_cbdata,
		   void* const private_data = 0);
void delete_Schema(Schema* const s);


/*Schema API*/
int Schema_init(Schema* const s);
int Schema_iteration(Schema* const s);
void* Schema_cast(Schema* const s, const char* interface_name);
  //int Schema_cmp(Schema* const a, Schema* const b);

int Schema_add_instance(Schema* const s);
int Schema_del_instance(Schema* const s);
const GList* Schema_get_instances();



#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*SCHEMA_H*/

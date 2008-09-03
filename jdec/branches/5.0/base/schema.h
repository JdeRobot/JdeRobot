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
  static int add(SFactory* const sf);
  static SFactory* search(const char* interface_name);
  static void print();
#endif /*__cplusplus*/
}SFactory;

typedef struct Schema{
  int sid;
  schema_init_f init_cb;
  void* init_cbdata;
  schema_cast_f cast_cb;
  void* cast_cbdata;
  void* private_data;
#ifdef __cplusplus
  Schema(const int sid);
  Schema(const int sid,
	 schema_init_f const init_cb,void* const init_cbdata,
	 schema_cast_f const cast_cb,
	 void* const cast_cbdata,
	 void* const private_data = 0);
  virtual ~Schema();
  virtual int init() = 0;
  virtual void* cast(const char* interface_name) = 0;
private:
  static int sinit(Schema* const s);
  static void* scast(Schema* const s, const char* interface_name);
#endif /*__cplusplus*/
}Schema;


/*SFactory functions*/
SFactory* new_SFactory(const char* schema_name,
		       const char* interface_name,
		       schema_ctor_f const schema_ctor,
		       schema_dtor_f const schema_dtor,
		       void* cbdata = 0);
void delete_SFactory(SFactory* const sf);
Schema* SFactory_create(SFactory* const sf);
void SFactory_destroy(SFactory* const sf, Schema* s);

/*
  add_sfactory
  Adds sf sfactory to the list.
  sf isn't copied so it must be not deallocated
*/
int add_sfactory(SFactory* const sf);

/*
  search_sfactory
  Search a sfactory that implements interface_name.
  A copy is returned that must be deallocated.
*/
SFactory* search_sfactory(const char* interface_name);

void print_sfactories();

#ifdef __cplusplus
#define ADD_SFACTORY(SNAME,SINTERFACE,CTOR,DTOR,PDATA)    \
static SFactory __reg(SNAME,SINTERFACE,CTOR,DTOR,PDATA); \
static int __loaded = SFactory::add(&__reg);
#else
#define ADD_SFACTORY(SNAME,SINTERFACE,CTOR,DTOR,PDATA)    \
static SFactory __reg = {SNAME,SINTERFACE,CTOR,DTOR,PDATA}; \
__attribute__((constructor)) void __add_sfactory() { \
   add_sfactory(&__reg); \
}
#endif

/*Schema fucntions*/
Schema* new_Schema(const int sid,
		   schema_init_f const init_cb,void* const init_cbdata,
		   schema_cast_f const cast_cb,
		   void* const cast_cbdata,
		   void* const private_data = 0);
void delete_Schema(Schema* const s);


/*Schema API*/
int Schema_init(Schema* const s);
void* Schema_cast(Schema* const s, const char* interface_name);
int Schema_cmp(Schema* const a, Schema* const b);



#ifdef __cplusplus
} /*extern "C"*/
#endif /*__cplusplus*/
#endif /*SCHEMA_H*/

#include <schema.h>
#include <string.h>
#include <stdio.h>

/*Factories list*/
static GList* sfactories = 0;

/*Schema instances list*/
static GList* instances = 0;

/*Schema identification*/
static int sid = 0;

void init_SFactory(SFactory* sf,
		   const char* schema_name,
		   const char* interface_name,
		   schema_ctor_f const schema_ctor,
		   schema_dtor_f const schema_dtor,
		   void* cbdata) {
  strncpy(sf->schema_name,schema_name,MAX_STRLEN);
  sf->schema_name[MAX_STRLEN-1] = '\0';
  strncpy(sf->interface_name,interface_name,MAX_STRLEN);
  sf->interface_name[MAX_STRLEN-1] = '\0';
  sf->schema_ctor = schema_ctor;
  sf->schema_dtor = schema_dtor;
  sf->cbdata = cbdata;
  sf->instances = 0;
}

SFactory* new_SFactory(const char* schema_name,
		       const char* interface_name,
		       schema_ctor_f const schema_ctor,
		       schema_dtor_f const schema_dtor,
		       void* cbdata) {
  SFactory* sf;
#ifdef __cplusplus
  sf = new SFactory(schema_name,interface_name,
		    schema_ctor,schema_dtor,cbdata);
#else
  sf = (SFactory*)malloc(sizeof(SFactory));
  init_SFactory(sf,schema_name,interface_name,
		schema_ctor,schema_dtor,cbdata);
#endif /*__cplusplus*/
  return sf;
}

// SFactory* copy_SFactory(SFactory* dst, const SFactory* sfc) {
//   return new_SFactory(sfc->schema_name,
// 			sfc->interface_name,
// 			sfc->schema_ctor,sfc->schema_dtor,
// 			sfc->cbdata,dst);
// }

void delete_SFactory(SFactory* const sf) {
  g_list_free(sf->instances); /*free only list mem,not instances*/
#ifdef __cplusplus
  delete sf;
#else
  free(sf);
#endif /*__cplusplus*/
}

Schema* SFactory_create(SFactory* const sf) {
  Schema* s = sf->schema_ctor(sf,sid++);
  sf->instances = g_list_append(sf->instances,s);
  instances = g_list_append(instances,s);
  return s;
}

void SFactory_destroy(SFactory* const sf, Schema* s) {
  sf->instances = g_list_remove(sf->instances,s);
  instances = g_list_remove(instances,s);
  sf->schema_dtor(sf,s);
}

int add_sfactory(SFactory* const sf) {
  sfactories = g_list_append(sfactories,sf);
  fprintf(stderr, "%s: registering %s, implements %s\n",
	  __PRETTY_FUNCTION__,sf->schema_name,sf->interface_name);
  
  return g_list_length(sfactories);
}

int interface_cmp(const GList* a, const char* b) {
  return strcmp(((SFactory*)a->data)->interface_name,b);
}

SFactory* search_sfactory(const char* interface_name) {
  GList* e;

  e = g_list_find_custom(sfactories,(gpointer)interface_name,
			 (GCompareFunc)interface_cmp);
  if (e)
    return (SFactory*)e->data;

  fprintf(stderr, "%s: sfactory for interface %s not found\n",
	  __PRETTY_FUNCTION__,interface_name);
  return 0;
}

void print_sfactories() {
  GList* i;

  for (i = g_list_first(sfactories); i != 0; i = g_list_next(i))
    printf("%s->%s\n",
	   ((SFactory*)i->data)->schema_name,
	   ((SFactory*)i->data)->interface_name);
}
  
#ifdef __cplusplus
SFactory::SFactory(const char* schema_name,
		   const char* interface_name,
		   schema_ctor_f const schema_ctor,
		   schema_dtor_f const schema_dtor,
		   void* cbdata) {
  init_SFactory(this,schema_name,interface_name,
		schema_ctor,schema_dtor,cbdata);
}

SFactory::~SFactory() {}


Schema* SFactory::create() {
  return SFactory_create(this);
}

void SFactory::destroy(Schema* s) {
  SFactory_destroy(this,s);
}

int SFactory::add(SFactory* const sf) {
  return add_sfactory(sf);
}

SFactory* SFactory::search(const char* interface_name) {
  return search_sfactory(interface_name);
}

void SFactory::print() {
  print_sfactories();
}

#endif /*__cplusplus*/

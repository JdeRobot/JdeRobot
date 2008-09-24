#include <schema.h>
#include <string.h>
#include <stdio.h>

/*Factories list*/
static GList* sfactories = 0;

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
  Schema_add_instance(s);
  return s;
}

void SFactory_destroy(SFactory* const sf, Schema* s) {
  sf->instances = g_list_remove(sf->instances,s);
  Schema_del_instance(s);
  sf->schema_dtor(sf,s);
}

int SFactory_add_factory(SFactory* const sf) {
  sfactories = g_list_append(sfactories,sf);
  fprintf(stderr, "%s: registering %s, implements %s\n",
	  __PRETTY_FUNCTION__,sf->schema_name,sf->interface_name);
  
  return g_list_length(sfactories);
}

//int SFactory_del_factory(SFactory* const sf) {
//}

int interface_cmp(const GList* a, const char* b) {
  return strcmp(((SFactory*)a->data)->interface_name,b);
}

SFactory* SFactory_search_factory(const char* interface_name) {
  GList* e;

  e = g_list_find_custom(sfactories,(gpointer)interface_name,
			 (GCompareFunc)interface_cmp);
  if (e)
    return (SFactory*)e->data;

  fprintf(stderr, "%s: sfactory for interface %s not found\n",
	  __PRETTY_FUNCTION__,interface_name);
  return 0;
}

const GList* SFactory_get_factories() {
  return sfactories;
}

/* void print_sfactories() { */
/*   GList* i; */

/*   for (i = g_list_first(sfactories); i != 0; i = g_list_next(i)) */
/*     printf("%s->%s\n", */
/* 	   ((SFactory*)i->data)->schema_name, */
/* 	   ((SFactory*)i->data)->interface_name); */
/* } */

const GList* SFactory_get_instances(SFactory* const sf) {
  return sf->instances;
}

/* void print_instances() { */
/*   GList* i; */

/*   for (i = g_list_first(instances); i != 0; i = g_list_next(i)) */
/*     printf("%d\n",((Schema*)i->data)->sid); */
/* } */
  
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

const GList* SFactory::get_instances() {
  return SFactory_get_instances(this);
}

int SFactory::add_factory(SFactory* const sf) {
  return SFactory_add_factory(sf);
}

SFactory* SFactory::search_factory(const char* interface_name) {
  return SFactory_search_factory(interface_name);
}

const GList* SFactory::get_factories() {
  return SFactory_get_factories();
}

#endif /*__cplusplus*/

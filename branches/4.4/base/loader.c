#include "loader.h"
#include <schema.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

struct JDESchema *load_schema_so(const char* sopath, const char* cf_path){
  JDESchema* (*maker)();
  JDESchema *s;
  void* h = 0;

  dlerror();
  if ((h=dlopen(sopath,RTLD_LAZY)) == 0)
    fprintf(stderr,"%s\n",dlerror());
  else{
    maker = (JDESchema* (*)())dlsym(h,"createSchema");
    if (maker==0){
      dlclose(h);
      fprintf(stderr,"%s\n",dlerror());
    }else{
      s = maker();
      s->handle = h;
      return s;
    }
  }
  return 0;
}

struct JDESchema *load_schema(const char* schema_path, const char* cf_path){
  char *ppos;

  fprintf(stderr,"Loading %s...\n",schema_path);

  ppos=strrchr(schema_path,'.');
  if (strcmp(ppos, ".so") == 0)
    return load_schema_so(schema_path,cf_path);
  else {
    fprintf(stderr,"unknown module type %s\n",schema_path);
    return 0;
  }
}


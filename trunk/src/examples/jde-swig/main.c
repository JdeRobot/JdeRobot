#include <loader.h>
#include <schema.h>
#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

int main(int argc, char* argv[]) {
  int i;
  Schema_reg* sr;
  Schema* root;

  /*load so*/
  for (i=1; i<argc && i<10; i++) {
    char *ppos;

    fprintf(stderr,"Loading %s...\n",argv[i]);

    ppos=strrchr(argv[i],'.');
    if (strcmp(ppos, ".so") == 0) {
      if (load_so(argv[i]) == 0)
	fprintf(stderr,"%s\n",dlerror());
    } else if (strcmp(ppos, ".py") == 0) {
      char pymodule[32];
      int lpymodule = ppos-argv[i];

      strncpy(pymodule,argv[i],lpymodule);
      pymodule[lpymodule] = '\0';
      if (load_py(pymodule,argc,argv) == 0)
	fprintf(stderr,"error loading python module %s\n",argv[i]);
    } else
      fprintf(stderr,"unknown module type %s\n",argv[i]);
  }

  /*search root schema*/
  if ((sr = search_schema("interface1")) == 0) {
    fprintf(stderr,"Error: Can't find a schema implementing interface1\n");
    return 1;
  }

  /*create root schema instance*/
  root = Schema_reg_instance(sr,1,1);

  /*exec iteration*/
  for (i=1; i<5; i++) {
    Schema_init(root);
    usleep(1000000);
  }
  Schema_reg_del_instance(sr,root);
}

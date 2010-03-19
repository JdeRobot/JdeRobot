#include "hierarchy.h"
#include "schema.h"
#include "loader.h"
#include "jde.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <simclist.h>
#include <pthread.h>

/** Cronos thread it is going to iterate each 2 seconds*/
#define cronos_cycle 2000 /* ms, to compute fps*/


struct JDEHierarchy_p{
  JDESchema *root_schema;
  list_t schema_list;/*use mutex to access*/
  int shuttingdown;
  pthread_mutex_t mutex;
  pthread_t cronos_thread;
};


/*private functions*/
/*FIXME: config file parsing could be better done using flex*/
int JDEHierarchy_parse_configfile(JDEHierarchy * const self);

/**
 * Cronos thread, to measure the real rythm of different schemas and drivers,
 * in iterations per second
 */
void *cronos_threadf(void *args){
  JDEHierarchy* myhierarchy = (JDEHierarchy*)args;
  struct timeval tlast,tnow;
  long diff;
  
  /*initialize variables to avoid compilation warning*/
  gettimeofday(&tnow,NULL);
  tlast = tnow;
  
  /* frame rate computing */
  while(myhierarchy->priv->shuttingdown==0){
    /* printf("cronos iteration\n"); */
    gettimeofday(&tnow,NULL);
    diff = (tnow.tv_sec-tlast.tv_sec)*1000000+tnow.tv_usec-tlast.tv_usec;

    /*iterato over all hierarchy schemas*/
    pthread_mutex_lock(&(myhierarchy->priv->mutex));
    /*fprintf(stderr,"Schema list size %d\n",
      list_size(&(myhierarchy->schema_list)));*/
    list_iterator_start(&(myhierarchy->priv->schema_list));
    while(list_iterator_hasnext(&(myhierarchy->priv->schema_list))){
      JDESchema *s = 
	(JDESchema*)list_iterator_next(&(myhierarchy->priv->schema_list));
      /*fprintf(stderr,"Updating fps on %s\n",s->name);*/
      s->fps=(float)(s->k)*(float)1000000./(float)diff;
      s->k=0;
    }
    list_iterator_stop(&(myhierarchy->priv->schema_list));
    pthread_mutex_unlock(&(myhierarchy->priv->mutex));
    
    tlast=tnow;
    /* discounts 10ms taken by calling usleep itself */
    usleep(cronos_cycle*1000-10000);
  }
  return 0;
}

/*seeker function for lookups based on schema name*/
int seeker(const void *e, const void *key){
  JDESchema *s = (JDESchema*)e;
  assert(e!=0 && key!=0);

  if (strcmp((char*)key,s->name) == 0)
    return 1;
  return 0;
}

void root_schema_init(char *configfile){
}

void root_schema_terminate(void){
}

void root_schema_stop(void){
}

void root_schema_run(int father, int *brothers, arbitration fn){
}

void root_schema_show(void){
}

void root_schema_hide(void){
}

/*FIXME: parse args*/
JDEHierarchy* new_JDEHierarchy(int argc, char** argv, const char* cf){
  JDEHierarchy *h = (JDEHierarchy*)calloc(1,sizeof(JDEHierarchy));
  char str[MAX_BUFFER];
  int fd;

  h->name = 0;/*not used yet*/
  h->path = strdup("");
  h->priv = (JDEHierarchy_p*)calloc(1,sizeof(JDEHierarchy_p));
  assert(h->priv!=0);
  list_init(&(h->priv->schema_list));
  list_attributes_seeker(&(h->priv->schema_list),seeker);
  h->priv->shuttingdown = 0;
  pthread_mutex_init(&(h->priv->mutex),NULL);
  h->priv->root_schema = new_JDESchema("root_schema",
				       root_schema_init,
				       root_schema_terminate,
				       root_schema_stop,
				       root_schema_run,
				       root_schema_show,
				       root_schema_hide);
  JDEHierarchy_add_schema(h,h->priv->root_schema);
  
  fprintf(stdout,"Initializing python environment...\n");
  init_py(argc,argv);

  /*set config file*/
  if (cf == 0 || *cf == 0) {
    /*no configfile give,try default ones*/ 
    if ((fd=open("./jderobot.conf",O_RDONLY)) != -1){
      fprintf(stderr,"Configuration from ./jderobot.conf\n");
      h->config_file = strdup("./jderobot.conf");
      close(fd);
    }else{
      sprintf(str,"%s%s",CONFIGDIR,"/jderobot.conf");
      if ((fd=open(str,O_RDONLY)) != -1){
	fprintf(stderr,"Configuration from %s\n",str);
	h->config_file = strdup(str);
	close(fd);
      }else{
	fprintf(stderr,"Can not find any config file\n");
	h->config_file = strdup("");
      }
    }
  }else{
    if ((fd=open(cf,O_RDONLY)) != -1){
      fprintf(stdout,"Configuration from %s\n",cf);
      h->config_file = strdup(cf);
      close(fd);
    }else{
      fprintf(stderr,"Can not open config file: %s\n",cf);
      h->config_file = strdup("");
    }
  }

  /* read the configuration file: load drivers and schemas */
  if (strlen(h->config_file) > 0)
    if (JDEHierarchy_parse_configfile(h)!=1)
      fprintf(stderr,"Error parsing config file: %s\n",cf);

  /* start the cronos thread */
  fprintf(stdout,"Starting cronos...\n");
  pthread_create(&h->priv->cronos_thread,NULL,cronos_threadf,(void*)h);
  return h;
}


/*FIXME: delete Schemas??*/
void delete_JDEHierarchy(JDEHierarchy * const self){
  if (self==0)
    return;
    
  self->priv->shuttingdown=1;
  pthread_join(self->priv->cronos_thread,0);
  list_iterator_start(&(self->priv->schema_list));
  while(list_iterator_hasnext(&(self->priv->schema_list))){
    JDESchema *s = 
      (JDESchema*)list_iterator_next(&(self->priv->schema_list));
    delete_JDESchema(s);
  }
  list_iterator_stop(&(self->priv->schema_list));
  list_destroy(&(self->priv->schema_list));
  free(self->name);
  free(self->config_file);
  free(self->path);
  free(self->priv);
  free(self);
}


/**
 * Parse a config file
 * @param cf config file to parse
 * @return 1 if parsing ok
 */
int JDEHierarchy_parse_configfile(JDEHierarchy * const self) {
  FILE *config;
  int driver_configuration_section=0;
  int service_configuration_section=0;
  int schema_configuration_section=0;
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  int i=0,j=0,k=0,words=0;
  char buffer_file[MAX_BUFFER]; 
  JDESchema *s;
  JDEDriver *d;
  char *cf = 0;
  runFn r;

  assert(self!=0);
  assert(self->config_file!=0 && self->config_file[0]!='\0');

  if ((config=fopen(self->config_file,"r"))==NULL){
    fprintf(stderr,"Can't open configuration file: %s\n",self->config_file);
    return 0;
  }

  fprintf(stdout,"Reading configuration...\n");
  while ((buffer_file[0]=fgetc(config))!=EOF && 
	 buffer_file[0]!=(char)255){/*255??*/
    i = 0;j=0;
    /*discard comments*/
    if (buffer_file[0]=='#') {while(fgetc(config)!='\n'); continue;}

    /*strip whites*/
    if (buffer_file[0]==' ' || buffer_file[0]=='\t') {
      while(buffer_file[0]==' ' || buffer_file[0]=='\t')
	buffer_file[0]=fgetc(config);
    }


    /* Captura la linea y luego leeremos de la linea con sscanf, comprobando 
       en la linea que el ultimo caracter es \n. No lo podemos hacer
       directamente con fscanf porque esta funcion no distingue
       espacio en blanco de \n */
    while((buffer_file[i]!='\n') && 
	  (buffer_file[i] != (char)255) &&  
	  (i<MAX_BUFFER-1) ) {
      buffer_file[++i]=fgetc(config);
    }

    if (i >= MAX_BUFFER-1) { 
      fprintf(stderr,"%s...\n", buffer_file);
      fprintf(stderr,"Line too long in config file!\n");
      return 0;
    }
    buffer_file[++i]='\0';
    
    if (sscanf(buffer_file,"%s",word)!=1) continue; /*empty line, go
						      to next one*/
    else {
      if ((strcmp(word,"load")==0)||(strcmp(word,"load_schema")==0)){
	while((buffer_file[j]!='\n')&&
	      (buffer_file[j]!=' ')&&
	      (buffer_file[j]!='\0')&&
	      (buffer_file[j]!='\t')) j++;
	words=sscanf(&buffer_file[j],"%s %s",word,word2);
	
	if (words==1){
	  cf = self->config_file;
	}
	else if (words==2){
	  cf = word2;
	}else {
	  fprintf(stderr,"Bad line in configuration file %s, ignoring it. Load_schema only accepts one or two parameters: schema_name [schemaconfigfile]\n Offending line: '%s'\n", self->config_file, buffer_file);
	  continue;/*ignore line*/
	}
	s = jde_loadschema(word);
	if (s == 0){
	  fprintf(stderr,"Schema loading failed\n");
	}else{
	  JDEHierarchy_add_schema(self,s);
	  JDESchema_init(s,cf);
	}
      }else if (strcmp(word,"schema")==0){
	if(schema_configuration_section==0) schema_configuration_section=1; 
	else{
	  fprintf(stderr,"Error in configuration file %s. Schema's configuration section without 'end_schema' line\n", self->config_file);
	  return 0;
	}
      }else if(strcmp(word,"end_schema")==0){
	schema_configuration_section=0;

      }else if ((strcmp(word,"resume")==0) ||
		(strcmp(word,"run")==0)||
		(strcmp(word,"on")==0)){
	while((buffer_file[j]!='\n')&&
	      (buffer_file[j]!=' ')&&
	      (buffer_file[j]!='\0')&&
	      (buffer_file[j]!='\t')) j++;
	sscanf(&buffer_file[j],"%s",word);
	r=(runFn)myimport(word,"run");/*FIXME: use find_schema instead*/
	if (r!=NULL) r(SHELLHUMAN,NULL,NULL);
      }else if ((strcmp(word,"guiresume")==0)||
		(strcmp(word,"guion")==0)||
		(strcmp(word,"show")==0)){
	while((buffer_file[j]!='\n')&&
	      (buffer_file[j]!=' ')&&
	      (buffer_file[j]!='\0')&&
	      (buffer_file[j]!='\t')) j++;
	sscanf(&buffer_file[j],"%s",word);
	for(k=0;k<num_schemas;k++){
	  if (strcmp(all[k].name,word)==0) {
	    if (all[k].show!=NULL)
	      all[k].show();
	    all[k].guistate=on;
	    break;
	  }
	}
	if (k==num_schemas) 
	  fprintf(stderr,"Error in configuration file %s. Have you already loaded the schema before the offending line:' %s'\n", self->config_file, buffer_file);
      }else if ((strcmp(word,"load_driver")==0) ||
		(strcmp(word,"load_service")==0)){
	while((buffer_file[j]!='\n')&&
	      (buffer_file[j]!=' ')&&
	      (buffer_file[j]!='\0')&&
	      (buffer_file[j]!='\t')) j++;
	words=sscanf(&buffer_file[j],"%s %s",word,word2);

	if (words==1){
	  cf = self->config_file;
	}else if (words==2){
	  cf = word2;
	}else{ 
	  fprintf(stderr,"Bad line in configuration file %s, ignoring it. Load_driver/service only accepts one or two parameters: driver_name [driverconfigfile]\n Offending line: '%s'\n", self->config_file, buffer_file);
	  continue;
	}
	d = jde_loaddriver(word);
	if (d == 0){
	  fprintf(stderr,"Driver/Service loading failed\n");
	}else{
	  JDEDriver_init(d,cf);
	}
      }else if (strcmp(word,"driver")==0){
	if(driver_configuration_section==0) driver_configuration_section=1;
	else{
	  fprintf(stderr,"Error in configuration file %s. Driver's configuration section without 'end_driver' line\n", self->config_file);
	  return 0;
	}
      }else if (strcmp(word,"end_driver")==0){
	driver_configuration_section=0;
      }else if (strcmp(word,"service")==0){
	if(service_configuration_section==0) service_configuration_section=1;
	else{
	  fprintf(stderr,"Error in configuration file %s. Service's configuration section without 'end_service' line\n", self->config_file);
	  return 0;
	}
      }else if (strcmp(word,"end_service")==0){
	service_configuration_section=0;
      }else if(strcmp (word,"path")==0){
	/*Loads the path*/
	while((buffer_file[j]!='\n')&&
	      (buffer_file[j]!=' ')
              &&(buffer_file[j]!='\0')&&
	      (buffer_file[j]!='\t')) j++;
	sscanf(&buffer_file[j],"%s",word);
	free(self->path);
	self->path = strdup(word);
	/*backward compatibility*/
	strncpy(path, word, MAX_BUFFER);
      }else if (strcmp(word,"load_schema2")==0){
	while((buffer_file[j]!='\n')&&
	      (buffer_file[j]!=' ')&&
	      (buffer_file[j]!='\0')&&
	      (buffer_file[j]!='\t')) j++;
	words=sscanf(&buffer_file[j],"%s %s",word,word2);
	
	if (words==1){
	  cf = self->config_file;
	}else if (words==2){
	  cf = word2;
	}else{ 
	  fprintf(stderr,"Bad line in configuration file %s, ignoring it. Load_driver/service only accepts one or two parameters: driver_name [driverconfigfile]\n Offending line: '%s'\n", self->config_file, buffer_file);
	  continue;
	}
	s=load_schema(word,cf);
	if (s==0)
	  fprintf(stderr,"Module loading failed\n");
	else{
	  JDEHierarchy_add_schema(self,s);
	  JDESchema_init(s,cf);
	}
      }else{
	if ((driver_configuration_section==0) &&
	    (service_configuration_section==0) &&
	    (schema_configuration_section==0))
	  fprintf(stderr,"I don't know what to do with: %s\n",buffer_file);
      }
    }
  }
  return 1;
}

int JDEHierarchy_add_schema(JDEHierarchy * const self,
			     struct JDESchema* const s){
  int rc;

  assert(self!=0 && s != 0);
  pthread_mutex_lock(&(self->priv->mutex));
  rc = list_append(&(self->priv->schema_list),s);
  pthread_mutex_unlock(&(self->priv->mutex));
  s->hierarchy = self;
  return rc;
}

struct JDESchema* JDEHierarchy_find_schema (JDEHierarchy * const self, 
					    const char *name){
  JDESchema* s = 0;

  assert(self!=0 && name != 0);
  pthread_mutex_lock(&(self->priv->mutex));
  s = (JDESchema*)list_seek(&(self->priv->schema_list),name);
  pthread_mutex_unlock(&(self->priv->mutex));
  return s;
}


int JDEHierarchy_myexport (JDEHierarchy * const self,
			   const char *namespace, const char *symbol_name,
			   void *p){
  assert(self!=0);
  return myexport(namespace,symbol_name,p);/*FIXME: reimplement it with
					     simclist making the list
					     private to the hierarchy*/
}

void* JDEHierarchy_myimport (JDEHierarchy * const self,
			     const char *namespace, const char *symbol_name){
  assert(self!=0);
  return myimport(namespace,symbol_name);/*FIXME: reimplement it with
					   simclist making the list
					   private to the hierarchy*/
}


JDESchema * JDEHierarchy_root_schema_get (JDEHierarchy * const self){
  assert(self!=0);
  return self->priv->root_schema;
}

struct JDEInterfacePrx *
JDEHierarchy_interfaceprx_get(JDEHierarchy * const self,
			      const char* interface_name,
			      const char* instance_name,
			      JDESchema* const user){
  JDEInterface *refers_to = 0;
  void *i;

  assert(self!=0);
  assert(interface_name!=0);

  refers_to = (JDEInterface*)JDEHierarchy_myimport(user->hierarchy,
						   instance_name,"JDEInterface");
  if (refers_to!=0){
    assert(strcmp(refers_to->interface_name,interface_name)!=0);/*runtime
								  type check*/
    return new_JDEInterfacePrx(refers_to,user);
  }
  return 0;
}

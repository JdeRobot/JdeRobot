/*
 *
 *  Copyright (C) 1997-2008 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 *
 */

#define thisrelease "jderobot 4.3-svn"

#include "jde.h"
#include "loader.h"
#include "dlfcn.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

#include <sys/time.h>
#include <time.h>


/** Concurrency control when shutting down*/
pthread_mutex_t shuttingdown_mutex;

/* hierarchy */
/** Array with all the loaded schemas*/
JDESchema all[MAX_SCHEMAS];
/** Number of loaded schemas*/
int num_schemas=0;

/** Array with all the loaded drivers*/
JDEDriver mydrivers[MAX_SCHEMAS];
/** Number of loaded drivers*/
int num_drivers=0;

/** Shared variables' type definition*/
typedef struct sharedname{
   /** The exporter schema's name*/
   char schema[MAX_BUFFER];
   /** The name of the shared variable or funcion*/
   char name[MAX_BUFFER];
   /** Pointer to de shared variable of function casted to (void *)*/
   void *pointer;
  /**  Pointer to the next node*/
   struct sharedname *next;
}Tsharedname;

/** List with the shared variables and functions*/
static Tsharedname *sharedlist=NULL;

/** Name of the configuration file used by jde*/
char configfile[MAX_BUFFER];

/** The modules path*/
char path[MAX_BUFFER];


/**
 * Puts the state in newstate to the schema idexed by numschema
 * @param newstate Schema's new states
 * @param numschema Schema's index in the schema's array
 * @return void
 */
void put_state(const int numschema, const int newstate)
{
  all[numschema].state=newstate;
  /* only some changes are relevant. For instance change of one motor schema from active to ready is not, because it happens every iteration */
  if ((newstate==winner) || 
      (newstate==slept) || 
      (newstate==forced)|| 
      (newstate==notready)|| 
      (newstate==ready)|| 
      (newstate==active));
}

/**
 * This functions must be called at every schema's iteration, it increments the
 * schema's iteration counter to calculate schema's ips.
 * @param numschema Schema's identifier
 * @return void
 */
void speedcounter(const int numschema)
{
  if ((numschema>=0)&&(numschema<num_schemas))
    all[numschema].k++;
}

/**
 * It is used to share variables and functions between diferent schemas and
 * drivers (remember that they are in diferent names' spaces)
 * @param schema String containing the exporter schema's name
 * @param name String containing the exported variable's names
 * @param p Pointer to the variable or function casted to (void *)
 * @return 1 if the variables was correctly exported, othewise 0
 */
int myexport(const char *schema, const char *name, void *p)
     /* publishes the variable, to make it available to other schemas */
{
  Tsharedname * next;
  Tsharedname * this;
  if (p!=NULL) {
     if (sharedlist!=NULL){
        next=sharedlist->next;
        this=sharedlist;
        while (this->next!=NULL && (strcmp(schema, this->schema)!=0 ||
               strcmp(name, this->name)!=0) )
        {
           this=this->next;
        }
        if (this->next==NULL){
           /*Store this new symbol*/
           this->next=(Tsharedname *)malloc (sizeof(Tsharedname));
           this=this->next;
           strcpy(this->name,name);
           strcpy(this->schema,schema);
           this->pointer=p;
           this->next=NULL;
        }
        else{
           /*Symbol already stored*/
           fprintf(stderr,"Warning: Symbol \"%s\" at schema \"%s\" it's alredy stored, my export will be ignored now\n",
                   name, schema);
           return 0;
        }
     }
     else{
        /*Empty list*/
        sharedlist=(Tsharedname *)malloc (sizeof(Tsharedname));
        strcpy(sharedlist->name,name);
        strcpy(sharedlist->schema,schema);
        sharedlist->pointer=p;
        sharedlist->next=NULL;
     }
  }
  return 1;
}

/**
 * Get the pointer to a variable shared with myexport
 * @param schema String containing the exporter schema's name
 * @param name String containing the exported variable's names
 * @return The pointer to the variable casted to (void *)
 */
void *myimport(const char *schema, const char *name)
     /* returns NULL in case of not finding the requested variable */
{
   Tsharedname *this;

   this=sharedlist;
   while (this!=NULL){
      if ((strcmp(schema,this->schema)==0) &&
           (strcmp(name,this->name)==0))
      {
         return this->pointer;
      }
      this=this->next;
   }
   /*This statment will execute only if the symbos it's not at the list*/
   return NULL;
}

/**
 * Null arbitrarion function
 * @return void
 */
void null_arbitration()
{
  fprintf(stderr,"NULL arbitration\n");
}

/** Cronos thread identifier*/
static pthread_t cronos_th;
/** Cronos thread it is going to iterate each 2 seconds*/
#define cronos_cycle 2000 /* ms, to compute fps*/

/**
 * Cronos thread, to measure the real rythm of different schemas and drivers,
 * in iterations per second
 */
void *cronos_thread(void *not_used) 
{
  struct timeval tlast,tnow;
  long diff;
  int i;

  /*initialize variables to avoid compilation warning*/
  gettimeofday(&tnow,NULL);
  tlast = tnow;

 /* frame rate computing */   
  for(;;)
    {
      /* printf("cronos iteration\n"); */
      gettimeofday(&tnow,NULL);
      diff = (tnow.tv_sec-tlast.tv_sec)*1000000+tnow.tv_usec-tlast.tv_usec;
      
      for(i=0;i<num_schemas;i++)
	{
	  (all[i].fps)=(float)(all[i].k)*(float)1000000./(float)diff;
	  (all[i].k)=0;
	}
      tlast=tnow;
      /* discounts 10ms taken by calling usleep itself */
      usleep(cronos_cycle*1000-10000);
    }

}

int jdeinit(int argc, char** argv, const char* cf){
  char s[MAX_BUFFER];

  pthread_mutex_init(&shuttingdown_mutex,NULL);

  /* read the configuration file: load drivers and schemas */
  if (cf == 0 || *cf == 0) {/*no configfile give,try default ones*/ 
    if (parse_configfile("./jderobot.conf")){
      fprintf(stderr,"Configuration from ./jderobot.conf\n");
    }else{
      sprintf(s,"%s%s",CONFIGDIR,"/jderobot.conf");
      if (parse_configfile(s)){
	fprintf(stderr,"Configuration from %s\n",s);
      }else{
	fprintf(stderr,"Can not find any config file\n");
	return 0;
      }
    }
  }else{
    if (parse_configfile(cf)){
      fprintf(stdout,"Configuration from %s\n",cf);
    }else{
      fprintf(stderr,"Can not open config file: %s\n",cf);
      return 0;
    }
  }

  /* start the cronos thread */
  fprintf(stdout,"Starting cronos...\n");
  pthread_create(&cronos_th,NULL,cronos_thread,NULL);
  return 1;
}

/**
 * This function must be used instead of exit() to terminate de program
 * correctly
 * @param sig The same as the status argument at exit function
 * @return void
 */
void jdeshutdown(const int sig)
{
  static int shuttingdown=0;
  int shutdown=0;
  int i;

  pthread_mutex_lock(&shuttingdown_mutex);
  if (shuttingdown==0){
     shutdown=1;
     shuttingdown++;
  }
  else{
     shutdown=0;
     fprintf(stderr, "Jde is already shutting down\n");
  }
  pthread_mutex_unlock(&shuttingdown_mutex);

  if (shutdown==1){
     /* unload all the schemas loaded as plugins */
    for(i=num_schemas-1;i>=0;i--)
     {
        if (all[i].terminate!=NULL) all[i].terminate();
        //if (all[i].handle!=NULL) dlclose(all[i].handle);
     }

     /* unload all the drivers loaded as plugins */
     for(i=num_drivers-1;i>=0;i--)
     {
        if (mydrivers[i].terminate!=NULL) mydrivers[i].terminate();
        //if (mydrivers[i].handle!=NULL) dlclose(mydrivers[i].handle);
     }

     fprintf(stderr,"Bye\n");
     exit(sig);
  }
}


/**
 * @brief This function loads a module and returns a reference to its handler
 * 
 * @param module_name String containing the name of the module
 * @return A pointer to the handler
 */
void* load_module(const char* module_name){
   char *path2;
   char path_cp[MAX_BUFFER];
   char *directorio;
   void *handler=NULL;

   strncpy(path_cp, path, MAX_BUFFER);
   path2=path_cp;
   while ((directorio=strsep(&path2,":"))!=NULL && handler==NULL){
      char fichero[512];
      strncpy(fichero, directorio, 512);
      strncat(fichero,"/", 512-strlen(fichero));
      strncat(fichero, module_name, 512-strlen(fichero));
      /* printf ("trying >%s<\n",fichero);*/
      handler = dlopen(fichero, RTLD_LAZY);
      /* if (handler ==NULL)
	 fprintf(stderr,"I can't load: %s\n",dlerror());
      */
   }
   return handler;
}

/**
 * Loads a schema
 * @param name The schema's name
 * @return pointer to the loaded schema or null
 */
JDESchema* jde_loadschema(const char *name)
{
  char n[MAX_BUFFER];
  char *error;
  JDESchema* s = 0;
  
  if (num_schemas>=MAX_SCHEMAS) {
    fprintf(stderr, "WARNING: No entry available for %s schema\n",name);
    return 0;
  }

  strcpy(n,name); strcat(n,".so");
  s = &all[num_schemas];
  s->handle = load_module(n);

  if (!s->handle) {
    fprintf(stderr,"%s\n",dlerror());
    fprintf(stderr,"I can't load the %s schema or one dynamic library it depends on\n",name);
    return 0;
  }

  /* symbols from the plugin: */
  dlerror();
  strcpy(n,name); strcat(n,"_init");
  s->init = (void (*)(char *)) dlsym(s->handle,n);  
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
  dlerror();
  strcpy(n,name); strcat(n,"_id");
  s->id = (int *) dlsym(s->handle,n);
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
  dlerror();
  strcpy(n,name); strcat(n,"_run");
  s->run = (void (*)(int,int *,arbitration)) dlsym(s->handle,n);  
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
  dlerror();
  strcpy(n,name); strcat(n,"_stop");
  s->stop = (void (*)(void)) dlsym(s->handle,n);  
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
  dlerror();
  strcpy(n,name); strcat(n,"_terminate");
  s->terminate = (void (*)(void)) dlsym(s->handle,n);
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
  dlerror();
  strcpy(n,name); strcat(n,"_show");
  s->show = (void (*)(void)) dlsym(s->handle,n);  
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
  dlerror();
  strcpy(n,name); strcat(n,"_hide");
  s->hide = (void (*)(void)) dlsym(s->handle,n);  
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
  (*(s->id)) = num_schemas;
  strcpy(s->name,name);
  s->fps = 0.;
  s->k =0;
  s->state=slept;
  s->guistate=off;
  pthread_mutex_init(&s->mymutex,NULL);
  pthread_cond_init(&s->condition,NULL);
  /* the thread is created on schema's init execution. This only is the load of the schema */
  
  fprintf(stderr,"%s schema loaded (id %d)\n",name,(*(s->id)));
  num_schemas++;
  return s;
}

/**
 * Loads a driver
 * @param name The drivers's name
 * @return pointer to the loaded driver or null
 */
JDEDriver* jde_loaddriver(const char *name)
{
  char n[MAX_BUFFER];
  char *error;
  JDEDriver *d = 0;

  if (num_drivers>=MAX_SCHEMAS) {
    fprintf(stderr, "WARNING: No entry available for %s driver\n",name);
    return 0;
  }

  strcpy(n,name); strcat(n,".so");
  /* Drivers don't share their global variables, to avoid the symbol
     collisions */
  d = &mydrivers[num_drivers];
  d->handle = load_module(n);

  if (!(d->handle)){ fprintf(stderr,"%s\n",dlerror());
  fprintf(stderr,"I can't load the %s driver or one dynamic library it depends on\n",name);
  return 0;
  }

  /* symbols from the plugin: */
  dlerror();
  strcpy(n,name); strcat(n,"_init");
  d->init = (void (*)(char *)) dlsym(d->handle,n); 
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s driver\n",n,name);
  
  dlerror();
  strcpy(n,name); strcat(n,"_terminate");
  d->terminate = (void (*)()) dlsym(d->handle,n); 
  if ((error=dlerror()) != NULL)
    fprintf(stderr,"WARNING: Unresolved symbol %s in %s driver\n",n,name);
  
  d->id = num_drivers;
  strcpy(d->name,name);
  fprintf(stderr,"%s driver loaded (driver %d)\n",name,d->id);
  num_drivers++;
  return d;
}


/**
 * It reads a single line from config file, parses it and do the right thing.
 * @param myfile The config file descriptor.
 * @returns EOF in detects end of such file. Otherwise returns 0.
 */
int jde_readline(FILE *myfile)
     /* To init non-basic schemas, just raise the flag, putting the in active state. It will effectively start up in main, after the "init" of all basic schemas */

{
  /** When reading any driver section from the configuration file */
  static int driver_configuration_section=0;
  static int service_configuration_section=0;
  static int schema_configuration_section=0;
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  int i=0,j=0,k=0,words=0;
  char buffer_file[MAX_BUFFER]; 
  JDESchema *s;
  JDEDriver *d;
  char *cf = 0;
  runFn r;
  
  buffer_file[0]=fgetc(myfile);
  if (buffer_file[0]==EOF) return EOF;
  if (buffer_file[0]==(char)255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  /* Captura la linea y luego leeremos de la linea con sscanf, comprobando en la linea que el ultimo caracter es \n. No lo podemos hacer directamente con fscanf porque esta funcion no distingue espacio en blanco de \n */
  while((buffer_file[i]!='\n') && 
	  (buffer_file[i] != (char)255) &&  
	  (i<MAX_BUFFER-1) ) {
    buffer_file[++i]=fgetc(myfile);
  }

  if (i >= MAX_BUFFER-1) { 
    fprintf(stderr,"%s...\n", buffer_file);
    fprintf(stderr,"Line too long in config file!\n");
    exit(-1);
  }
  buffer_file[++i]='\0';
  
  if (sscanf(buffer_file,"%s",word)!=1) return 0; 
  /* return EOF; empty line*/
  else {
    if ((strcmp(word,"load")==0)||(strcmp(word,"load_schema")==0)){
      while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
      words=sscanf(&buffer_file[j],"%s %s",word,word2);
      
      if (words==1){
	cf = configfile;
      }
      else if (words==2){
	cf = word2;
      }
      else 
	fprintf(stderr,"Bad line in configuration file %s, ignoring it. Load_schema only accepts one or two parameters: schema_name [schemaconfigfile]\n Offending line: '%s'\n", configfile, buffer_file);
      
      s = jde_loadschema(word);
      if (s == 0){
	fprintf(stderr,"Schema loading failed\n");
	exit(1);
      }
      s ->init(cf);
    }
    
    else if (strcmp(word,"schema")==0)
      {
	if(schema_configuration_section==0) schema_configuration_section=1; 
	else{
	  fprintf(stderr,"Error in configuration file %s. Schema's configuration section without 'end_schema' line\n", configfile);
	  exit(-1);
	}
      }
    else if(strcmp(word,"end_schema")==0)
      schema_configuration_section=0;

    else if ((strcmp(word,"resume")==0) ||
	     (strcmp(word,"run")==0)||
	     (strcmp(word,"on")==0))
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	sscanf(&buffer_file[j],"%s",word);
	r=(runFn)myimport(word,"run");
	if (r!=NULL) r(SHELLHUMAN,NULL,NULL);
	}

    else if ((strcmp(word,"guiresume")==0)||
              (strcmp(word,"guion")==0)||
              (strcmp(word,"show")==0))
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	sscanf(&buffer_file[j],"%s",word);
	for(k=0;k<num_schemas;k++)
	  {
	    if (strcmp(all[k].name,word)==0) {
	      if (all[k].show!=NULL)
		all[k].show();
	      all[k].guistate=on;
	      break;
	    }
	  }
	if (k==num_schemas) 
	  fprintf(stderr,"Error in configuration file %s. Have you already loaded the schema before the offending line:' %s'\n", configfile, buffer_file);
      }


    else if ((strcmp(word,"load_driver")==0) ||
	     (strcmp(word,"load_service")==0))
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	words=sscanf(&buffer_file[j],"%s %s",word,word2);

	if (words==1){
	  cf = configfile;
	}
	else if (words==2){
	  cf = word2;
	}
	else 
	  fprintf(stderr,"Bad line in configuration file %s, ignoring it. Load_driver/service only accepts one or two parameters: driver_name [driverconfigfile]\n Offending line: '%s'\n", configfile, buffer_file);
	d = jde_loaddriver(word);
	if (d == 0){
	  fprintf(stderr,"Driver/Service loading failed\n");
	  exit(1);
	}
	d->init(cf);
      }

    else if (strcmp(word,"driver")==0)
      {
	if(driver_configuration_section==0) driver_configuration_section=1;
	else{
	  fprintf(stderr,"Error in configuration file %s. Driver's configuration section without 'end_driver' line\n", configfile);
	  exit(-1);
	}
      }
    else if (strcmp(word,"end_driver")==0)
      driver_configuration_section=0;
    

    else if (strcmp(word,"service")==0)
      {
	if(service_configuration_section==0) service_configuration_section=1;
	else{
	  fprintf(stderr,"Error in configuration file %s. Service's configuration section without 'end_service' line\n", configfile);
	  exit(-1);
	}
      }
    else if (strcmp(word,"end_service")==0)
      service_configuration_section=0;
    

    else if(strcmp (word,"path")==0){
       /*Loads the path*/
       while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')
              &&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t'))
          j++;
       sscanf(&buffer_file[j],"%s",word);
       strncpy(path, word, MAX_BUFFER);
    }else{
      if ((driver_configuration_section==0) &&
	  (service_configuration_section==0) &&
	  (schema_configuration_section==0))
	fprintf(stderr,"I don't know what to do with: %s\n",buffer_file);
    }
    return 0;
  }
}

int parse_configfile(const char* cf) {
  FILE *config;
  int i;

  if ((config=fopen(cf,"r"))==NULL)
    return 0;

  path[0]='\0';
  strncpy(configfile,cf,MAX_BUFFER);
  configfile[MAX_BUFFER-1] = '\0';

  fprintf(stdout,"Reading configuration...\n");
  do {
    i=jde_readline(config);
  }while(i!=EOF);
  return 1;
}

char * get_configfile(){
  return configfile;
}

JDESchema* get_schema(const int id){
  if (id < num_schemas)
    return &all[id];
  else
    return 0;
}

JDESchema *find_schema (const char *name){
  int i;

  for (i=0; i < num_schemas; i++)
    if (strcmp (name,all[i].name) == 0)
      return &all[i];

  return 0;
}

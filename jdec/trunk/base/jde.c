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

#define thisrelease "jdec 4.3-svn"

#include "jde.h"
#include "dlfcn.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>


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

/** Defines de path variable size*/
#define PATH_SIZE 512

/** The modules path*/
char path[PATH_SIZE];

/** The jdec prompt*/
#define PROMPT "jdec$ "


typedef struct {
  const char *name;		/* User printable name of the function. */
  rl_icpfunc_t *func;		/* Function to call to do the job. */
  const char *doc;		/* Documentation for this function.*/
} COMMAND;


enum STATE { BASE, SCHEMA };
typedef struct {
  enum STATE state;
  COMMAND *commands;
  char prompt[256];
  void *pdata;
} SHELLSTATE;

enum GENERATOR_ST { COMMANDS = 1, SCHEMAS = 2 };

/*commads functions declaration*/
int com_cd(char *);
int com_help(char *);
int com_list(char *);
int com_dir(char *);
int com_load_driver(char *);
int com_load_schema(char *);
int com_ps(char *);
int com_pwd(char *);
int com_exit(char *);
int com_view(char *);
int com_run(char *);
int com_stop(char *);
int com_show(char *);
int com_hide(char *);
int com_init(char *);
int com_terminate(char *);
int com_zoom(char *);

COMMAND bcommands[] = {
  { "help", com_help, "Display this help text" },
  { "cd", com_cd, "Change the working directory to DIR" },
  { "dir", com_dir, "List files at the working directory" },
  { "pwd", com_pwd, "Print the current working directory" },
  { "view", com_view, "View the contents of FILE" },
  { "ls", com_list, "List loaded schemas" },
  { "load_driver", com_load_driver, "Load driver" },
  { "load_service", com_load_driver, "Load service" },
  { "load_schema", com_load_schema, "Load schema" },
  { "ps", com_ps, "Print schemas states" },
  { "run", com_run, "Run schema" },
  { "stop", com_stop, "Stop schema" },
  { "show", com_show, "Show schema gui" },
  { "hide", com_hide, "Hide schema gui" },
  { "zoom", com_zoom, "Change to schema mode" },
  { "?", com_help, "Synonym for 'help'" },
  { "exit", com_exit, "Quit using jdeC" },
  { "quit", com_exit, "Synonym for 'exit'" },
  { (const char *)NULL, (rl_icpfunc_t *)NULL, (const char *)NULL }
};
const char *bprompt = "jdeC $> ";

COMMAND scommands[] = {
  { "help", com_help, "Display this text" },
  { "cd", com_cd, "Change to directory DIR" },
  { "dir", com_dir, "List files" },
  { "pwd", com_pwd, "Print the current working directory" },
  { "view", com_view, "View the contents of FILE" },
  { "run", com_run, "Run schema" },
  { "stop", com_stop, "Stop schema" },
  { "show", com_show, "Show schema gui" },
  { "hide", com_hide, "Hide schema gui" },
  { "init", com_init, "Init schema" },
  { "terminate", com_terminate, "Terminate schema" },
  { "resume", com_run, "Synonym for 'run'" },
  { "suspend", com_stop, "Synonym for 'stop'" },
  { "guiresume", com_show, "Synonym for 'show'" },
  { "guisuspend", com_hide, "Synonym for 'hide'" },
  { "?", com_help, "Synonym for 'help'" },
  { "exit", com_exit, "Exit schema mode" },
  { (const char *)NULL, (rl_icpfunc_t *)NULL, (const char *)NULL }
};

/*format strings used for schema cmds and prompts*/
const char *sprompt = "jdeC[%s] $> ";

/* Forward declarations. */
char *stripwhite (char *string);
COMMAND *find_command (const char *name);
JDESchema *find_schema (const char *name);
void initialize_readline ();
int execute_line (char *line);

/* When non-zero, this global means the user is done using this program. */
int done;
SHELLSTATE shstate;
enum GENERATOR_ST generator_state;

/**
 * Puts the state in newstate to the schema idexed by numschema
 * @param newstate Schema's new states
 * @param numschema Schema's index in the schema's array
 * @return void
 */
void put_state(int numschema, int newstate)
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
void speedcounter(int numschema)
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
int myexport(char *schema, char *name, void *p)
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
           printf ("Warning: Symbol \"%s\" at schema \"%s\" it's alredy stored, my export will be ignored now\n",
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
void *myimport(char *schema, char *name)
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
  printf("NULL arbitration\n");
}

/**
 * This function must be used instead of exit() to terminate de program
 * correctly
 * @param sig The same as the status argument at exit function
 * @return void
 */
void jdeshutdown(int sig)
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
        if (all[i].handle!=NULL) dlclose(all[i].handle);
     }

     /* unload all the drivers loaded as plugins */
     for(i=num_drivers-1;i>=0;i--)
     {
        if (mydrivers[i].terminate!=NULL) mydrivers[i].terminate();
        if (mydrivers[i].handle!=NULL) dlclose(mydrivers[i].handle);
     }

     printf("Bye\n");
     exit(sig);
  }
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


/* Reading the configuration file */
/** Name of the configuration file used by jde*/
static char configfile[MAX_BUFFER];
/** When reading any driver section from the configuration file */
static int driver_configuration_section=0;
static int service_configuration_section=0;
static int schema_configuration_section=0;
/**
 * @brief This function loads a module and returns a reference to its handler
 * 
 * @param module_name String containing the name of the module
 * @return A pointer to the handler
 */
void* load_module(char* module_name){
   char *path2;
   char path_cp[PATH_SIZE];
   char *directorio;
   void *handler=NULL;

   strncpy(path_cp, path, PATH_SIZE);
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
 * @return Always 1
 */
int jde_loadschema(char *name)
{
  char n[200];
  char *error;
  
  if (num_schemas>=MAX_SCHEMAS) 
    {
      printf("WARNING: No entry available for %s schema\n",name);
      exit(1);
    }

  strcpy(n,name); strcat(n,".so");
  /*  all[num_schemas].handle = dlopen(n,RTLD_LAZY|RTLD_GLOBAL);*/
  /* Schemas don't share their global variables, to avoid symbol collisions */
  all[num_schemas].handle = load_module(n);

  if (!(all[num_schemas].handle)) { 
    fprintf(stderr,"%s\n",dlerror());
    printf("I can't load the %s schema or one dynamic library it depends on\n",name);
    exit(1);
  }
  else {
      /* symbols from the plugin: */
      dlerror();
      strcpy(n,name); strcat(n,"_init");
      all[num_schemas].init = (void (*)(char *)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_id");
      all[num_schemas].id = (int *) dlsym(all[num_schemas].handle,n);
      if ((error=dlerror()) != NULL)
        printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_run");
      all[num_schemas].run = (void (*)(int,int *,arbitration)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_stop");
      all[num_schemas].stop = (void (*)(void)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_stop");
      all[num_schemas].terminate = (void (*)(void)) dlsym(all[num_schemas].handle,n);
      if ((error=dlerror()) != NULL)
         printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_show");
      all[num_schemas].show = (void (*)(void)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);
      
      dlerror();
      strcpy(n,name); strcat(n,"_hide");
      all[num_schemas].hide = (void (*)(void)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);
  
      (*(all[num_schemas].id)) = num_schemas;
      strcpy(all[num_schemas].name,name);
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].guistate=off;
      pthread_mutex_init(&all[num_schemas].mymutex,PTHREAD_MUTEX_TIMED_NP);
      pthread_cond_init(&all[num_schemas].condition,NULL);
      /* the thread is created on schema's init execution. This only is the load of the schema */

      printf("%s schema loaded (id %d)\n",name,(*(all[num_schemas].id)));
      num_schemas++;
      return 1;
    }
}

/**
 * Loads a driver
 * @param name The drivers's name
 * @return Always 1
 */
int jde_loaddriver(char *name)
{
  char n[200];
  char *error;

  strcpy(n,name); strcat(n,".so");
  /* Drivers don't share their global variables, to avoid the symbol collisions */
  mydrivers[num_drivers].handle = load_module(n);

  if (!(mydrivers[num_drivers].handle))
    { fprintf(stderr,"%s\n",dlerror());
    printf("I can't load the %s driver or one dynamic library it depends on\n",name);
    exit(1);
    }
  else 
    {
      /* symbols from the plugin: */
      dlerror();
      strcpy(n,name); strcat(n,"_init");
      mydrivers[num_drivers].init = (void (*)(char *)) dlsym(mydrivers[num_drivers].handle,n); 
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s driver\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_terminate");
      mydrivers[num_drivers].terminate = (void (*)()) dlsym(mydrivers[num_drivers].handle,n); 
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s driver\n",n,name);

      mydrivers[num_drivers].id = num_drivers;
      strcpy(mydrivers[num_drivers].name,name);
      printf("%s driver loaded (driver %d)\n",name,mydrivers[num_drivers].id);
      num_drivers++;
      return 1;
    }
}

/**
 * It reads a single line from config file, parses it and do the right thing.
 * @param myfile The config file descriptor.
 * @returns EOF in detects end of such file. Otherwise returns 0.
 */
int jde_readline(FILE *myfile)
     /* To init non-basic schemas, just raise the flag, putting the in active state. It will effectively start up in main, after the "init" of all basic schemas */

{
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  int i=0,j=0,k=0,words=0;
  char buffer_file[MAX_BUFFER]; 
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
    printf("%s...\n", buffer_file);
    printf ("Line too long in config file!\n");
    exit(-1);
  }
  buffer_file[++i]='\0';
  
  if (sscanf(buffer_file,"%s",word)!=1) return 0; 
  /* return EOF; empty line*/
  else {
    if ((strcmp(word,"load")==0)||(strcmp(word,"load_schema")==0))
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	words=sscanf(&buffer_file[j],"%s %s",word,word2);

	if (words==1){
	  jde_loadschema(word);
	  (*all[num_schemas-1].init)(configfile);
	}
	else if (words==2){
	  jde_loadschema(word);
	  (*all[num_schemas-1].init)(word2);
	}
	else 
	  printf("Bad line in configuration file %s, ignoring it. Load_schema only accepts one or two parameters: schema_name [schemaconfigfile]\n Offending line: '%s'\n", configfile, buffer_file);	
      }
    
    else if (strcmp(word,"schema")==0)
      {
	if(schema_configuration_section==0) schema_configuration_section=1; 
	else{
	  printf("Error in configuration file %s. Schema's configuration section without 'end_schema' line\n", configfile);
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
	  printf("Error in configuration file %s. Have you already loaded the schema before the offending line:' %s'\n", configfile, buffer_file);
      }


    else if ((strcmp(word,"load_driver")==0) ||
	     (strcmp(word,"load_service")==0))
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	words=sscanf(&buffer_file[j],"%s %s",word,word2);

	if (words==1){
	  jde_loaddriver(word);
	  (*mydrivers[num_drivers-1].init)(configfile);
	}
	else if (words==2){
	  jde_loaddriver(word);
	  (*mydrivers[num_drivers-1].init)(word2);
	}
	else 
	  printf("Bad line in configuration file %s, ignoring it. Load_driver only accepts one or two parameters: driver_name [driverconfigfile]\n Offending line: '%s'\n", configfile, buffer_file);
      }

    else if (strcmp(word,"driver")==0)
      {
	if(driver_configuration_section==0) driver_configuration_section=1;
	else{
	  printf("Error in configuration file %s. Driver's configuration section without 'end_driver' line\n", configfile);
	  exit(-1);
	}
      }
    else if (strcmp(word,"end_driver")==0)
      driver_configuration_section=0;
    

    else if (strcmp(word,"service")==0)
      {
	if(service_configuration_section==0) service_configuration_section=1;
	else{
	  printf("Error in configuration file %s. Service's configuration section without 'end_service' line\n", configfile);
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
       strncpy(path, word, PATH_SIZE);
    }    
    
    else{
      if ((driver_configuration_section==0) &&
	  (service_configuration_section==0) &&
	  (schema_configuration_section==0))
	printf("I don't know what to do with: %s\n",buffer_file);
    }
    return 0;
  }
}

/**
 * Jde main function
 * @param argc Number of arguments
 * @param argv Array with the params
 * @return The end status
 */
int main(int argc, char** argv)
{
  int i;
  char *line, *s;/* shell buffers*/
  FILE *config;
  int n=1; /* argument number in the console for the configuration file parameter */
  int filenameatconsole=FALSE;

 
  signal(SIGTERM, &jdeshutdown); /* kill interrupt handler */
  signal(SIGINT, &jdeshutdown); /* control-C interrupt handler */
  signal(SIGABRT, &jdeshutdown); /* failed assert handler */
  signal(SIGPIPE, SIG_IGN);
  
  /* Pablo Barrera: Por alguna raz�n libforms hace que fprintf("%f") ponga 
    una coma en vez de un punto como separador si las locales son espa�olas. 
    Con esto las ponemos a POSIX. Al salir el entorno es el normal */
  /*unsetenv("LANG");*/
  setenv("LC_ALL","POSIX",1);

  printf("%s\n",thisrelease);

  n=1;
  while(argv[n]!=NULL)
    {
      if ((strcmp(argv[n],"--help")==0) ||
	  (strcmp(argv[n],"-h")==0))
	{printf("Use: jde [config.file]\n\n     [config.file] Sets an specific config file. Don't use this option to read default configuration.\n\n");
	exit(0);}
      else if ((strcmp(argv[n],"--version")==0) ||
	       (strcmp(argv[n],"-v")==0))
	{printf("%s\n",thisrelease);
	exit(0);}
      else if ((strcmp(argv[n],"--gui")==0) ||
              (strcmp(argv[n],"-g")==0)){
          printf("Not valid command line argument \"--gui\". If you want to \nauto activate the gui use the configuration file.");
      }
      else 
	{filenameatconsole=TRUE;
	sprintf(configfile,"%s",argv[n]);
	}
      n++;
    }


  if (filenameatconsole==FALSE) 
    { 
      /* search in default locations */
      if ((config=fopen("./jde.conf","r"))==NULL){
	sprintf(configfile,"%s%s",CONFIGDIR,"/jde.conf");
	if ((config=fopen(configfile,"r"))==NULL){
	  printf("Can not find any config file\n");
	  exit(-1);
	}
	else printf("Configuration from %s\n",configfile);
      }
      else {
	sprintf(configfile,"%s","./jde.conf");
	printf("Configuration from ./jde.conf\n");
      }
    }
  else 
    {
      if ((config=fopen(configfile,"r"))==NULL){
	printf("Can not open config file: %s\n",configfile);
	exit(-1);
      }
      else {
	printf("Configuration from %s\n",configfile);
      }
    }
 
  /* read the configuration file: load drivers and schemas */
  path[0]='\0';
  pthread_mutex_init(&shuttingdown_mutex, PTHREAD_MUTEX_TIMED_NP);
  printf("Reading configuration...\n");
  do {
    i=jde_readline(config);
  }while(i!=EOF);

  /* start the cronos thread */
  printf("Starting cronos...\n");
  pthread_create(&cronos_th,NULL,cronos_thread,NULL); 
  
  /* read commands from keyboard */
  printf("Starting shell...\n");
  initialize_readline ();	/* Bind our completer. */

  /*initialize shstate*/
  shstate.state = BASE;
  shstate.commands = bcommands;
  strncpy(shstate.prompt,bprompt,256);
  shstate.prompt[255] = '\0';
  shstate.pdata = NULL;

  /* Loop reading and executing lines until the user quits. */
  while(done == 0){
    line = readline (shstate.prompt);
    
    if (!line)
      com_exit(NULL);
    else {
      /* Remove leading and trailing whitespace from the line.
	 Then, if there is anything left, add it to the history list
	 and execute it. */
      s = stripwhite (line);
    
      if (*s) {
	add_history (s);
	execute_line (s);
      }
      /*printf("%s\n",s);*/
      free (line);
    }
  }
  jdeshutdown(0);
  pthread_exit(0); 
  /* If we don't need this thread anymore, but want the others to keep running */

}




/* Execute a command line. */
int
execute_line (char *line){
  COMMAND *command;
  char *word;
  JDESchema *s;

  /* Isolate the command word.
     line is modified if token was found, see strsep(3) */
  word = strsep(&line," ");

  command = find_command (word);
  if (command)
    return ((*(command->func)) (line));

  /*search in loaded schemas*/
  if ((s=find_schema(word)) != 0){/*cmd is a schema name->run it*/
    if (s->run != NULL)
      s->run(SHELLHUMAN,NULL,NULL);
    return 0;
  }
    
  /*schema mode when schema name is the command*/
  /* for (schema_index = 0; schema_index < num_schemas; schema_index++) { */
/*     if (strcmp (all[schema_index].name, word) == 0) { */
/*       shstate.state = SCHEMA; */
/*       shstate.commands = scommands; */
/*       snprintf(shstate.prompt,256,sprompt,all[schema_index].name); */
/*       shstate.pdata = (void*)&all[schema_index]; */

/*       if (line && *line != '\0'){ /\*exec command on schema*\/ */
/* 	word = strsep(&line," "); */
/* 	command = find_command (word); */
/* 	if (command) */
/* 	  rc = ((*(command->func)) (line)); */
/* 	com_exit(0); */
/*       } */
/*       return rc; */
/*     } */
/*   } */

  /* no command or factory name found*/
  fprintf (stderr, "%s: No such command for jdeC.\n", word);
  return (-1);
}


/* Look up NAME as the name of a command, and return a pointer to that
   command.  Return a NULL pointer if NAME isn't a command name. */
COMMAND *
find_command (const char *name){
  register int i;
    
  for (i = 0; shstate.commands[i].name; i++)
    if (strcmp (name, shstate.commands[i].name) == 0)
      return (&shstate.commands[i]);

  return ((COMMAND *)NULL);
}

JDESchema *find_schema (const char *name){
  int i;

  for (i=0; i < num_schemas; i++)
    if (strcmp (name,all[i].name) == 0)
      return &all[i];

  return 0;
}

/* Strip whitespace from the start and end of STRING.  Return a pointer
   into STRING. */
char *
stripwhite (char *string){
  register char *s, *t;

  for (s = string; whitespace (*s); s++)
    ;
    
  if (*s == 0)
    return (s);

  t = s + strlen (s) - 1;
  while (t > s && whitespace (*t))
    t--;
  *++t = '\0';

  return s;
}

/* **************************************************************** */
/*                                                                  */
/*                  Interface to Readline Completion                */
/*                                                                  */
/* **************************************************************** */

char *command_generator(const char *, int);
char **command_completion(const char *, int, int);

/* Tell the GNU Readline library how to complete.  We want to try to complete
   on command names if this is the first word in the line, or on filenames
   if not. */
void
initialize_readline (){
  /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "jdeC";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = command_completion;
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
char **
command_completion (const char *text,int start, int end){
  char **matches;
  char cmd[256];

  matches = (char **)NULL;

  
  if (start == 0){/*line start*/
    switch(shstate.state){
    case BASE:
      generator_state = (COMMANDS|SCHEMAS);
      break;
    case SCHEMA:
      generator_state = COMMANDS;
      break;
    default:
      generator_state = 0;
    }
  }else {/*some commands have a schema name after*/
    if (sscanf(rl_line_buffer,"%s",cmd)){
      if ((strcmp (cmd,"run") == 0) || 
	  (strcmp (cmd,"stop") == 0) ||
	  (strcmp (cmd,"show") == 0) ||
	  (strcmp (cmd,"hide") == 0) ||
	  (strcmp (cmd,"zoom") == 0) )
	generator_state = SCHEMAS;
    }
  }
  matches = rl_completion_matches (text, command_generator);

  return (matches);
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
char *
command_generator (const char *text,int state){
  static int list_index, len;
  static int schema_index;
  const char *name;
  char str[256];

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if (!state) {
      list_index = 0;
      schema_index = 0;
      len = strlen (text);
  }

  /* Return the next name which partially matches from the command
     list. */
  if (generator_state & COMMANDS) {
    while ((name = shstate.commands[list_index].name)) {
      list_index++;
      if (strncmp (name, text, len) == 0)
	return (strdup(name));
    }
  }

  /*search on schema names*/
  if (generator_state & SCHEMAS) {
    while( schema_index < num_schemas) {
      sprintf(str,"%s",all[schema_index].name);
      schema_index++;
      if (strncmp (str, text, len) == 0)
	return (strdup(str));
    }
  }

  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

/* **************************************************************** */
/*                                                                  */
/*                       jdeC Commands                              */
/*                                                                  */
/* **************************************************************** */

/* String to pass to system ().  This is for the LIST, VIEW and RENAME
   commands. */
static char syscom[1024];

/* Return non-zero if ARG is a valid argument for CALLER, else print
   an error message and return zero. */
int
valid_argument (const char *caller, const char *arg){
  if (!arg || !*arg)
    {
      fprintf (stderr, "%s: Argument required.\n", caller);
      return (0);
    }

  return (1);
}

/*list files*/
int
com_dir (char *arg){
  const char *a = arg;

  if (!a)
    a = "";

  sprintf (syscom, "ls -FClg %s", a);
  return system (syscom);
}

/* List loaded schemas. */
int
com_list (char *arg){
  int i;

  for(i=0;i<num_schemas;i++)
    printf("%s\n",all[i].name);

  return 0;
}


int
com_view (char *arg){
  if (!valid_argument ("view", arg))
    return 1;

#if defined (__MSDOS__)
  /* more.com doesn't grok slashes in pathnames */
  sprintf (syscom, "less %s", arg);
#else
  sprintf (syscom, "more %s", arg);
#endif
  return (system (syscom));
}


/* Print out help for ARG, or for all of the commands if ARG is
   not present. */
int
com_help (char *arg){
  register int i;
  int printed = 0;

  for (i = 0; shstate.commands[i].name; i++){
    if (!arg || (strcmp (arg, shstate.commands[i].name) == 0)) {
      printf ("%s\t\t%s.\n", shstate.commands[i].name, shstate.commands[i].doc);
      printed++;
    }
  }

  if (!printed) {
    printf ("No commands match `%s'.  Possibilties are:\n", arg);

    for (i = 0; shstate.commands[i].name; i++) {
      /* Print in six columns. */
      if (printed == 6)	{
	printed = 0;
	printf ("\n");
      }

      printf ("%s\t", shstate.commands[i].name);
      printed++;
    }

    if (printed)
      printf ("\n");
  }
  return (0);
}

/* Change to the directory ARG. */
int
com_cd (char *arg){
  if (chdir (arg) == -1)
    {
      perror (arg);
      return 1;
    }

  com_pwd (0);
  return (0);
}

/* Print out the current working directory. */
int
com_pwd (char *ignore){
  char dir[1024], *s;

  s = getcwd (dir, sizeof(dir) - 1);
  if (s == 0)
    {
      fprintf (stderr, "Error getting pwd: %s\n", dir);
      return 1;
    }

  printf ("Current directory is %s\n", dir);
  return 0;
}

/* The user wishes to quit using this program.  Just set DONE
   non-zero. */
int
com_exit (char *arg){
  switch(shstate.state){
  case BASE:
    done = 1;
    break;
  case SCHEMA:
  default:
    break;
  }
  shstate.state = BASE;
  shstate.commands = bcommands;
  strncpy(shstate.prompt,bprompt,256);
  shstate.prompt[255] = '\0';
  shstate.pdata = NULL;
  printf ("\n");

  return (0);
}

/* Load ARG so. */
int
com_load_driver (char *arg){
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  int words;

  if (!valid_argument ("load_driver", arg))
    return 1;

  words=sscanf(arg,"%s %s",word,word2);
  if (words == 1){
    jde_loaddriver(word);
    (*mydrivers[num_drivers-1].init)(configfile);
  }else if (words==2){
    jde_loaddriver(word);
    (*mydrivers[num_drivers-1].init)(word2);
  }else{
    fprintf (stderr, "load_driver command accept 1 or 2 args only: load_driver <driver.so> [<config file>]");	
    return 1;
  }
  return 0;
}

int
com_load_schema (char *arg){
  char word[MAX_BUFFER], word2[MAX_BUFFER];
  int words;

  if (!valid_argument ("load_schema", arg))
    return 1;
  
  words=sscanf(arg,"%s %s",word,word2);
  if (words == 1){
    jde_loadschema(word);
    (*all[num_schemas-1].init)(configfile);
  }else if (words==2){
    jde_loadschema(word);
    (*all[num_schemas-1].init)(word2);
  }else{
    fprintf (stderr, "load_schema command accept 1 or 2 args only: load_schema <schema.so> [<config file>]");	
    return 1;
  }
  return 0;
}


int
com_ps (char *arg){
  int i,j,k;
  
  for(i=0;i<num_schemas;i++){
    if ((all[i].state==winner) ||
	(all[i].state==notready) ||
	(all[i].state==ready)) {
      printf("%s: %.0f ips, ",all[i].name,all[i].fps);
      if (all[i].state==winner) {
	printf("winner ( ");
	k=0;
	for (j=0;j<num_schemas;j++)
	  if (all[i].children[j]==TRUE) {
	    if (k==0) {
	      printf("\b");k++;
	    }
	    printf("%s ",all[j].name);
	  }
	printf("\b)");
      }
      else if (all[i].state==slept)
	printf("slept");
      else if (all[i].state==notready)
	printf("notready");
      else if (all[i].state==ready)
	printf("ready");
      else if (all[i].state==forced)
	printf("forced");
      printf("\n");
    }
  }
  return 0;
}

int
com_run (char *arg){
  JDESchema *s;

  if (shstate.state == BASE){
    if (!valid_argument ("run", arg))
      return 1;
    if ((s=find_schema(arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return 1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return 1;

  if (s->run != NULL)
    s->run(SHELLHUMAN,NULL,NULL);

  return 0;
}

int
com_stop (char *arg){
  JDESchema *s;
  
  if (shstate.state == BASE){
    if (!valid_argument ("stop", arg))
      return 1;
    if ((s=find_schema(arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return 1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return 1;

  if (s->stop != NULL)
    s->stop();

  return 0;
}

int
com_show (char *arg){
  JDESchema *s;

  if (shstate.state == BASE){
    if (!valid_argument ("show", arg))
      return 1;
    if ((s=find_schema(arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return 1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return 1;
  
  if (s->show != NULL) {
    s->show();
    s->guistate=on;
  }
  return 0;
}

int
com_hide (char *arg){
  JDESchema *s;

  if (shstate.state == BASE){
    if (!valid_argument ("hide", arg))
      return 1;
    if ((s=find_schema(arg)) == 0){
      fprintf (stderr,"%s: unknown schema\n",arg);
      return 1;
    }
  }else if (shstate.state == SCHEMA){
    s = (JDESchema*)shstate.pdata;
  }else
    return 1;

  if (s->hide!=NULL) {
    s->hide();
    s->guistate=on;
  }
  return 0;
}

int
com_init (char *arg){
  JDESchema *s = (JDESchema*)shstate.pdata;

  if (s->init != NULL) {
    if (!valid_argument ("init", arg))/*no args, use global configfile*/
      s->init(configfile);
    else
      s->init(arg);
  }

  return 0;
}

int
com_terminate (char *arg){
  JDESchema *s = (JDESchema*)shstate.pdata;
  
  if (s->terminate != NULL)
    s->terminate();

  return 0;
}

int com_zoom(char *arg){
  JDESchema *s;

  if (!valid_argument ("zoom", arg))
    return 1;
  if ((s=find_schema(arg)) == 0){
    fprintf (stderr,"%s: unknown schema\n",arg);
    return 1;
  }
  
  shstate.state = SCHEMA;
  shstate.commands = scommands;
  snprintf(shstate.prompt,256,sprompt,s->name);
  shstate.pdata = (void*)s;
  return 0;
}



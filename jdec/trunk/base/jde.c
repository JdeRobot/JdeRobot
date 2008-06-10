/*
 *  Copyright (C) 2006 José María Cañas Plaza
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  Authors : José María Cañas Plaza <jmplaza@gsyc.escet.urjc.es>
 */

#define thisrelease "jdec 4.3-svn"

#include "jde.h"
#include "dlfcn.h"

/** Maximum buffer size (for strings)*/
#define MAX_BUFFER 1024

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
        if (all[i].close!=NULL) all[i].close();
        if (all[i].handle!=NULL) dlclose(all[i].handle);
     }

     /* unload all the drivers loaded as plugins */
     for(i=num_drivers-1;i>=0;i--)
     {
        if (mydrivers[i].close!=NULL) mydrivers[i].close();
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

/**
 * Implements jde shell
 * @param mensaje The string typed by the user
 * @returns 0 if no erros ocurred, othewise -1
 */
int serve_keyboardmessage(char *mensaje)
{
 char word[256],word2[256];
 resumeFn r;
 suspendFn s;
 int i,j,k;

 /*printf("Command from keyboard: %s\n",mensaje); */
 if (sscanf(mensaje,"%s ",word)==1) {
    if ((strcmp(word,"resume")==0) ||
         (strcmp(word,"run")==0)||
         (strcmp(word,"on")==0))
    {
       if (sscanf(mensaje,"%s %s",word,word2)==2) {
          r=(resumeFn)myimport(word2,"resume");
          if (r!=NULL) r(SHELLHUMAN,NULL,NULL);
       }
       else printf("What schema do you want to resume?\n");
    }
    else if ((strcmp(word,"suspend")==0)||
              (strcmp(word,"kill")==0) ||
              (strcmp(word,"off")==0))
    {
       if (sscanf(mensaje,"%s %s",word,word2)==2) {
          s=(suspendFn)myimport(word2,"suspend");
          if (s!=NULL) s();
       }
       else printf("What schema do you want to suspend?\n");
    }
    else if ((strcmp(word,"guiresume")==0)||
              (strcmp(word,"guion")==0))
    {
       if (sscanf(mensaje,"%s %s",word,word2)==2) {
          for(i=0;i<num_schemas;i++){
             if (strcmp(all[i].name,word2)==0) {
                if (all[i].guiresume!=NULL)
                   all[i].guiresume();
                all[i].guistate=on;
                break;
             }
          }
       }
       else printf("What schema do you want to resume its GUI?\n");
    }
    else if ((strcmp(word,"guisuspend")==0)||
              (strcmp(word,"guioff")==0))
    {
       if (sscanf(mensaje,"%s %s",word,word2)==2)
       {
          for(i=0;i<num_schemas;i++){
             if (strcmp(all[i].name,word2)==0){
                if (all[i].guisuspend!=NULL)
                   all[i].guisuspend();
                all[i].guistate=on;
                break;
             }
          }
       }
       else printf("What schema do you want to suspend its GUI?\n");
    }
    else if ((strcmp(word,"quit")==0) ||
              (strcmp(word,"QUIT")==0) ||
              (strcmp(word,"exit")==0) ||
              (strcmp(word,"EXIT")==0))
       jdeshutdown(0);
    else if ((strcmp(word,"help")==0) ||
              (strcmp(word,"?")==0))
       printf("This is the shell of %s. Available commands:\n  * quit\n  * help\n  * ls\n      list the loaded schemas\n  * ps\n      print the non-slept schemas, their state and speed\n\n  * [schemaname]\n      run the schema\n  * kill [schemaname]\n      move the schema to slept state, stop the schema\n  * guion [schemaname]\n      show the GUI of the schema\n  * guioff [schemaname]\n      hide the GUI of the schema\n",thisrelease);
    else if ((strcmp(word,"ls")==0)||
              (strcmp(word,"list")==0) ||
              (strcmp(word,"LS")==0) ||
              (strcmp(word,"LIST")==0))
    {
       for(i=0;i<num_schemas;i++)
          printf("%s\n",all[i].name);
    }
    else if ((strcmp(word,"ps")==0)||
              (strcmp(word,"PS")==0))
    {
       for(i=0;i<num_schemas;i++){
          if ((all[i].state==winner) ||
               (all[i].state==notready) ||
               (all[i].state==ready))
          {
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
    }
    else
    {
       /* to activate an schema, just type its name on the jdec shell */
       for(i=0;i<num_schemas;i++){
          if (strcmp(all[i].name,word)==0){
             r=(resumeFn)myimport(word,"resume");
             if (r!=NULL) r(SHELLHUMAN,NULL,NULL);
             return 0;
          }
       }
       return -1;
    }
 }
 return 0;
   
}

/* Reading the configuration file */
/** Name of the configuration file used by jde*/
static char configfile[MAX_BUFFER];
/** When reading any driver section from the configuration file */
static int driver_configuration_section=0;
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
      strcpy(n,name); strcat(n,"_startup");
      all[num_schemas].startup = (void (*)(char *)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_id");
      all[num_schemas].id = (int *) dlsym(all[num_schemas].handle,n);
      if ((error=dlerror()) != NULL)
        printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_resume");
      all[num_schemas].resume = (void (*)(int,int *,arbitration)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_suspend");
      all[num_schemas].suspend = (void (*)(void)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_stop");
      all[num_schemas].close = (void (*)(void)) dlsym(all[num_schemas].handle,n);
      if ((error=dlerror()) != NULL)
         printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_guiresume");
      all[num_schemas].guiresume = (void (*)(void)) dlsym(all[num_schemas].handle,n);  
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);
      
      dlerror();
      strcpy(n,name); strcat(n,"_guisuspend");
      all[num_schemas].guisuspend = (void (*)(void)) dlsym(all[num_schemas].handle,n);  
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
      /* the thread is created on startup. This is the load */

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
      strcpy(n,name); strcat(n,"_startup");
      mydrivers[num_drivers].startup = (void (*)(char *)) dlsym(mydrivers[num_drivers].handle,n); 
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s driver\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_close");
      mydrivers[num_drivers].close = (void (*)()) dlsym(mydrivers[num_drivers].handle,n); 
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
     /* To startup non-basic schemas, just raise the flag, putting the in active state. It will effectively start up in main, after the "startup" of all basic schemas */

{
#define limit 256
  char word[limit], word2[limit];
  int i=0,j=0,k=0,words=0;
  char buffer_file[limit]; 
  resumeFn r;
  suspendFn s;
  
  buffer_file[0]=fgetc(myfile);
  if (buffer_file[0]==EOF) return EOF;
  if (buffer_file[0]==(char)255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  /* Captura la linea y luego leeremos de la linea con sscanf, comprobando en la linea que el ultimo caracter es \n. No lo podemos hacer directamente con fscanf porque esta funcion no distingue espacio en blanco de \n */
  while((buffer_file[i]!='\n') && 
	  (buffer_file[i] != (char)255) &&  
	  (i<limit-1) ) {
    buffer_file[++i]=fgetc(myfile);
  }

  if (i >= limit-1) { 
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
	  (*all[num_schemas-1].startup)(configfile);
	}
	else if (words==2){
	  jde_loadschema(word);
	  (*all[num_schemas-1].startup)(word2);
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
	r=(resumeFn)myimport(word,"resume");
	if (r!=NULL) r(SHELLHUMAN,NULL,NULL);
	}

    else if ((strcmp(word,"guiresume")==0)||
              (strcmp(word,"guion")==0))
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	sscanf(&buffer_file[j],"%s",word);
	for(k=0;k<num_schemas;k++)
	  {
	    if (strcmp(all[k].name,word)==0) {
	      if (all[k].guiresume!=NULL)
		all[k].guiresume();
	      all[k].guistate=on;
	      break;
	    }
	  }
	if (k==num_schemas) 
	  printf("Error in configuration file %s. Have you already loaded the schema before the offending line:' %s'\n", configfile, buffer_file);
      }


    else if (strcmp(word,"load_driver")==0)
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	words=sscanf(&buffer_file[j],"%s %s",word,word2);

	if (words==1){
	  jde_loaddriver(word);
	  (*mydrivers[num_drivers-1].startup)(configfile);
	}
	else if (words==2){
	  jde_loaddriver(word);
	  (*mydrivers[num_drivers-1].startup)(word2);
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
    else if(strcmp(word,"end_driver")==0)
      driver_configuration_section=0;
    
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
  int i,marca,leidos,last=0;
  char buff[MAX_BUFFER];
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
  printf(PROMPT);fflush(stdout); 
  for(;;)
    {
      /* leo hasta agotar todo lo que hay en ese descriptor */
      last=0;marca=0;
      do
	{
	  leidos=read(0,&(buff[marca]),1);
	  if (leidos>0) marca+=leidos;
	  if (marca>0) last=marca-1;
	} 
      while((buff[last]!='\n')||(leidos<=0));
      buff[last]='\0';
      serve_keyboardmessage(buff);
	printf(PROMPT);fflush(stdout); 
    }
  
  pthread_exit(0); 
  /* If we don't need this thread anymore, but want the others to keep running */

}

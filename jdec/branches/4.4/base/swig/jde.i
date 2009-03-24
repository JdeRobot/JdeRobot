%module jde

%{
#include <jde.h>
#include <jde_private.h>
%}

%{
  void JDESchema_init(JDESchema* const s, char *configfile){
    s->init(configfile);
  }

  void JDESchema_terminate(JDESchema* const s){
    s->terminate();
  }

  void JDESchema_stop(JDESchema* const s){
    s->stop();
  }

  void JDESchema_run(JDESchema* const s,
			int father, int *brothers, arbitration fn){
    s->run(father,brothers,fn);
  }

  void JDESchema_show(JDESchema* const s){
    s->show();
  }

  void JDESchema_hide(JDESchema* const s){
    s->hide();
  }
  
  void JDEDriver_init(JDEDriver* const d, char *configfile){
    d->init(configfile);
  }

  void JDEDriver_terminate(JDEDriver* const d){
    d->terminate();
  }
%}
/** Possible schema's states*/
enum states {slept,active,notready,ready,forced,winner};
/** Possible schema's gui states*/
enum guistates {off,on,pending_off,pending_on};

typedef struct {
  /** Dynamic library handler for the schema module*/
  void *handle;
  /** Schema's name*/
  char name[MAX_NAME];
  /** Schema's identifier*/
  int *id; /* schema identifier */
  /** Schema's state
   * @see states
   */
  int state;
  /** Schema's gui state
   * @see guistates
   */
  int guistate;
  /** Indicates the schema's identifier of the father*/
  int father;
  /** The children of this schema must be set at 1 in this array*/
  int children[MAX_SCHEMAS];
  /** Contains the ips of the schema*/
  float fps;
  /** It is used to calculate the fps value, it must be incremented at each
   * schema iteration
   * @see speedcounter*/
  long int k;


  /** A mutex to protect critical regions that affect de schema*/
  pthread_mutex_t mymutex;
  /** A condition to wait until de conditions are the correct to run*/
  pthread_cond_t condition;
  /** Schema's thread identifier*/
  pthread_t mythread;

  %extend{
    void init(char *configfile);
    void terminate();
    void stop();
    void run(int father, int *brothers, arbitration fn);
    void show();
    void hide();
  }
}JDESchema;

/** Jde driver type definition*/
typedef struct {
  /** Dynamic library handler for the driver module*/
  void *handle;
  /** Driver's name*/
  char name[MAX_NAME];
  /** Driver's identifier*/
  int id;
  
  %extend{
    void init(char *configfile);
    void terminate();
  }
}JDEDriver;



int jdeinit(const char* cf);
void jdeshutdown(const int sig);
JDESchema* jde_loadschema(const char *name);
JDEDriver* jde_loaddriver(const char *name);

char* get_configfile();
void null_arbitration();
JDESchema* find_schema (const char *name);
int get_state(const JDESchema* s);
void set_state(JDESchema* s,const int newstate);
void speedcounter2(JDESchema* s);
int myexport(const char *schema, const char *name, void *p);
void *myimport(const char *schema, const char *name);

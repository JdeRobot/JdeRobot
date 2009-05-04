%module schema

%{
#include <schema.h>
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

  %extend{
    void init(char *configfile);
    void terminate();
    void stop();
    void run(int father, int *brothers, arbitration fn);
    void show();
    void hide();
    int get_state();
    void set_state(int newstate);
    /*void wait_statechange();*/
    void speedcounter();
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


%module schema

%{
#include <schema.h>
%}

/** Possible schema's states*/
enum states {slept,active,notready,ready,forced,winner};
/** Possible schema's gui states*/
enum guistates {off,on,pending_off,pending_on};

typedef struct {
  /** Schema's name*/
  char name[MAX_NAME];
  
  /** Contains the ips of the schema*/
  float fps;
  /** It is used to calculate the fps value, it must be incremented at each
   * schema iteration
   * @see speedcounter*/
  long int k;

  %extend{
    int init(char *configfile);
    int terminate();
    int stop();
    int run(JDESchema* const father);
    int show();
    int hide();
    /*void wait_statechange();*/
    int speedcounter();
    int state;
  }
}JDESchema;


/** Jde driver type definition*/
typedef struct {
  /** Dynamic library handler for the driver module*/
  void *handle;
  /** Driver's name*/
  char name[MAX_NAME];
  /** Driver's identifier*/
  /*int id;*/
  
  %extend{
    void init(char *configfile);
    void terminate();
  }
}JDEDriver;


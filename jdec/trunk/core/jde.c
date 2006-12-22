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

#define thisrelease "JDE 4.2.0"

#include "jde.h"
#include "dlfcn.h"
#include "jdegui.h"
#define MAX_BUFFER 1024

/* sensor and motor variables */
char *greyA; 
char greyAA[SIFNTSC_COLUMNS*SIFNTSC_ROWS*1]; /**< sifntsc image itself */

char *colorA;
char colorAA[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageA_clock;
float fpsA=0;
int kA=0;
arbitration imageA_callbacks[MAX_SCHEMAS];
int imageA_users=0;
fn imageA_suspend;
fn imageA_resume;

char *colorB;
char colorBB[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageB_clock;
float fpsB=0;
int kB=0;
arbitration imageB_callbacks[MAX_SCHEMAS];
int imageB_users=0;
fn imageB_suspend;
fn imageB_resume;

char *colorC;
char colorCC[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageC_clock;
float fpsC=0;
int kC=0;
arbitration imageC_callbacks[MAX_SCHEMAS];
int imageC_users=0;
fn imageC_suspend;
fn imageC_resume;

char *colorD;
char colorDD[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3]; /**< sifntsc image itself */
unsigned long int imageD_clock;
float fpsD=0;
int kD=0;
arbitration imageD_callbacks[MAX_SCHEMAS];
int imageD_users=0;
fn imageD_suspend;
fn imageD_resume;

float pan_angle; /**< pan angle of the pantilt in degrees */
float tilt_angle;   /**< tilt angle of the pantilt in degrees */
unsigned long int pantiltencoders_clock;
float fpspantiltencoders=0;
int kpantiltencoders=0;
arbitration pantiltencoders_callbacks[MAX_SCHEMAS];
int pantiltencoders_users=0;
fn pantiltencoders_suspend;
fn pantiltencoders_resume;

int jde_laser[NUM_LASER];
unsigned long int laser_clock;
int klaser=0;
float fpslaser=0.;
arbitration laser_callbacks[MAX_SCHEMAS];
int laser_users=0;
fn laser_suspend;
fn laser_resume;

float us[NUM_SONARS];
unsigned long int us_clock[NUM_SONARS];
int ksonars=0;
float fpssonars=0.;
intcallback sonars_callbacks[MAX_SCHEMAS];
int sonars_users=0;
fn sonars_suspend;
fn sonars_resume;

float jde_robot[5]; /* mm, mm, rad */
unsigned long int encoders_clock;
float tspeed, rspeed; /* mm/s, deg/s */
unsigned long lasttime; /* microsecs */
int kencoders=0;
float fpsencoders=0.;
arbitration encoders_callbacks[MAX_SCHEMAS];
int encoders_users=0;
fn encoders_suspend;
fn encoders_resume;

float latitude,longitude;
float latitude_speed,longitude_speed;
int pantiltmotors_cycle=100; /* ms */
float fpspantiltmotors=0;
int kpantiltmotors=0;
fn pantiltmotors_suspend;
fn pantiltmotors_resume;

float v,w;
int motors_cycle=100; /* ms */
float ac=0.;
float fpsmotors=0;
int kmotors=0;
fn motors_suspend;
fn motors_resume;


/* sensor positions in the Robot FrameOfReference */
float laser_coord[5];
float us_coord[NUM_SONARS][5]; 
/**< Estructura para poder cambiar medidas de sensor a coordenadas locales al robot, y de estas al sist ref inicial: xsensor, ysensor, orientsensor,cossensor y sinsensor del sensor respecto del sistema solidario con el robot. Es fija. */
float camera_coord[5];


/** coordinate transformations from one FrameOfReference to another. */
void us2xy(int numsensor, float d,float phi, Tvoxel *point) 

/*  Calcula la posicion respecto de sistema de referencia inicial (sistema odometrico) del punto detectado en el sistema de coordenadas solidario al sensor. OJO depende de estructura posiciones y de por el sensor, sabiendo que:
   a) el robot se encuentra en robot[0], robot[1] con orientacion robot[2] respecto al sistema de referencia externo,
   b) que el sensor se encuentra en xsen, ysen con orientacion asen respecto del sistema centrado en el robot apuntando hacia su frente, 
   c) el punto esta a distancia d del sensor en el angulo phi 
*/ 
{
  float  Xp_sensor, Yp_sensor, Xp_robot, Yp_robot;

  Xp_sensor = d*cos(DEGTORAD*phi);
  Yp_sensor = d*sin(DEGTORAD*phi);
  /* Coordenadas del punto detectado por el US con respecto al sistema del sensor, eje x+ normal al sensor */
  Xp_robot = us_coord[numsensor][0] + Xp_sensor*us_coord[numsensor][3] - Yp_sensor*us_coord[numsensor][4];
  Yp_robot = us_coord[numsensor][1] + Yp_sensor*us_coord[numsensor][3] + Xp_sensor*us_coord[numsensor][4];
  /* Coordenadas del punto detectado por el US con respecto al robot */
  (*point).x = Xp_robot*jde_robot[3] - Yp_robot*jde_robot[4] + jde_robot[0];
  (*point).y = Yp_robot*jde_robot[3] + Xp_robot*jde_robot[4] + jde_robot[1];
  /* Coordenadas del punto con respecto al origen del SdeR */
}


void laser2xy(int reading, float d, Tvoxel *point)

/*  Calcula la posicion respecto de sistema de referencia inicial (sistema odometrico) del punto detectado en el sistema de coordenadas solidario al sensor. OJO depende de estructura posiciones y de por el sensor, sabiendo que:
   a) el robot se encuentra en robot[0], robot[1] con orientacion robot[2] respecto al sistema de referencia externo,
   b) que el sensor se encuentra en xsen, ysen con orientacion asen respecto del sistema centrado en el robot apuntando hacia su frente, 
*/ 
{
  float  Xp_sensor, Yp_sensor, Xp_robot, Yp_robot,phi;
  
  phi=-90.+180.*reading/NUM_LASER;
  Xp_sensor = d*cos(DEGTORAD*phi);
  Yp_sensor = d*sin(DEGTORAD*phi);
  Xp_robot = laser_coord[0] + Xp_sensor*laser_coord[3] - Yp_sensor*laser_coord[4];
  Yp_robot = laser_coord[1] + Yp_sensor*laser_coord[3] + Xp_sensor*laser_coord[4];
  /* Coordenadas del punto detectado por el laser con respecto al robot */
  (*point).x = Xp_robot*jde_robot[3] - Yp_robot*jde_robot[4] + jde_robot[0];
  (*point).y = Yp_robot*jde_robot[3] + Xp_robot*jde_robot[4] + jde_robot[1];
  /* Coordenadas del punto con respecto al origen del SdeR */
}


/* hierarchy */
JDESchema all[MAX_SCHEMAS];
int num_schemas=0;

JDEDriver mydrivers[MAX_SCHEMAS];
int num_drivers=0;

typedef struct sharedname{
  char schema[MAX_BUFFER];
  char name[MAX_BUFFER];
  void *pointer;
  /*  Tsharedname next;*/
}Tsharedname;

#define MAX_SHARED 600
static Tsharedname sharedlist[MAX_SHARED]; 
static int num_shared=0;

#define PROMPT "JDEC >> "

void put_state(int numschema, int newstate)
{
  all[numschema].state=newstate;
  /* only some changes are relevant. For instance change of one motor schema from active to ready is not, because it happens every iteration */
  if ((newstate==winner) || 
      (newstate==slept) || 
      (newstate==forced)|| 
      (newstate==active));
}

void speedcounter(int numschema)
{
  if ((numschema>=0)&&(numschema<num_schemas))
    all[numschema].k++;
}

int myexport(char *schema, char *name, void *p)
     /* publishes the variable, to make it available to other schemas */
{
  int i;
  int found=0;
  if (p!=NULL) 
    {
      for(i=0;i<num_shared;i++)
	if (sharedlist[i].pointer==NULL) 
	  {
	    sharedlist[i].pointer=p;
	    strcpy(sharedlist[i].name,name);
	    strcpy(sharedlist[i].schema,schema);
	    found=1;
	    break;
	  }
      if ((found==0)&&(num_shared<MAX_SHARED)) 
	{
	  sharedlist[num_shared].pointer=p;
	  strcpy(sharedlist[num_shared].name,name);
	  strcpy(sharedlist[num_shared].schema,schema);
	  num_shared++;
	}
      else if ((found==0)&&(num_shared>=MAX_SHARED))
	printf("Warning no space for sharing %s variable\n",name);
    }
  return 1;
}

void *myimport(char *schema, char *name)
     /* returns NULL in case of not finding the requested variable */
{
  void *value=NULL;
  int i=0;

  for(i=0;i<num_shared;i++)
    if ((strcmp(schema,sharedlist[i].schema)==0) &&
	(strcmp(name,sharedlist[i].name)==0))
	{ value=sharedlist[i].pointer;
	break;
	}
  return value;
}

void null_arbitration()
{
  printf("NULL arbitration\n");
}

int dummy(void)
{return 0;}

void jdeshutdown(int sig)
{
  int i;

  /* unload all the schemas loaded as plugins */
  for(i=0;i<num_schemas;i++)
    {
      if (all[i].close!=NULL) all[i].close();
      dlclose(all[i].handle);
    }
  
  /* unload all the drivers loaded as plugins */
  for(i=0;i<num_drivers;i++)
    {
      if (mydrivers[i].close!=NULL) mydrivers[i].close();
      dlclose(all[i].handle); 
    }

  jdegui_close();

  printf("Bye\n");
  exit(0);
}

void update_greyA(void)
{
  int i;
  for(i=0; i<(SIFNTSC_COLUMNS*SIFNTSC_ROWS);i++)
    {
      /* this pixel conversion is a little bit tricky: it MUST pass
	   through the (unsigned char) in order to get the right number
	   for values over 128 in colorA[3*i]. it MUST pass through the
	   (unsigned int) in order to allow the addition of three
	   values, which may overflow the 8 bits limit of chars and
	   unsigned chars */  
      greyA[i]=(char)(((unsigned int)((unsigned char)(colorA[3*i]))+(unsigned int)((unsigned char)(colorA[3*i+1]))+(unsigned int)((unsigned char)(colorA[3*i+2])))/3);
    }
}


/* Cronos thread, to measure the real rythm of different schemas and drivers, in iterations per second */
static pthread_t cronos_th;
#define cronos_cycle 2000 /* ms, to compute fps*/

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
      
      fpsA=kA*1000000/diff; 
      kA=0.; 
      fpsB=kB*1000000/diff;
      kB=0.;
      fpssonars=ksonars*1000000/(NUM_SONARS*diff);
      ksonars=0;
      fpslaser=klaser*1000000/diff;
      klaser=0;
      fpsencoders=kencoders*1000000/diff;
      kencoders=0;
      fpspantiltencoders=kpantiltencoders*1000000/diff;
      kpantiltencoders=0;

      fpsmotors=kmotors*1000000/diff;
      kmotors=0;
      fpspantiltmotors=kpantiltmotors*1000000/diff;
      kpantiltmotors=0;

      fpsgui=kgui*1000000/diff;
      kgui=0;
      for(i=0;i<num_schemas;i++)
	{
	  (all[i].fps)=(all[i].k) *1000000/diff;
	  (all[i].k)=0;
	}
      tlast=tnow;
      /* discounts 10ms taken by calling usleep itself */
      usleep(cronos_cycle*1000-10000);
    }

}


int serve_keyboardmessage(char *mensaje)
{
 char word[256],word2[256];

 /*printf("Command from keyboard: %s\n",mensaje); */
  if (sscanf(mensaje,"%s %s",word,word2)==2) 
    {
      if (strcmp(word,"resume")==0)
	{
	  if (strcmp(word2,"myschema")==0);
	}
      else if (strcmp(word,"suspend")==0)
	{
	  if (strcmp(word2,"myschema")==0);
	}
      else if ((strcmp(word,"mastergui")==0) ||
	       (strcmp(word,"gui")==0))
	{
	if (strcmp(word2,"on")==0)
	  mastergui_resume();
	else if (strcmp(word2,"off")==0)
	  mastergui_suspend();
	}
      else if ((strcmp(word,"sensorsmotorsgui")==0) ||
		(strcmp(word,"sensorsgui")==0) ||
		(strcmp(word,"motorsgui")==0))
		
	{
	  if (strcmp(word2,"on")==0) 
	    sensorsmotorsgui_resume();
	  else if (strcmp(word2,"off")==0)
	    sensorsmotorsgui_suspend();
	}
    }
  else if (sscanf(mensaje,"%s",word)==1) 
    {
      if ((strcmp(word,"quit")==0) ||
	  (strcmp(word,"QUIT")==0))
	jdeshutdown(0);
      else if (strcmp(word,"help")==0)
	{ printf("This is the shell of %s. Available commands are:\n  quit\n  mastergui [on|off]\n  sensorsmotorsgui [on|off]\n",thisrelease);
	}
    }
  else return -1;

  return 0;
}


/* Reading the configuration file */
static char configfile[MAX_BUFFER]; /* name of the configuration file used */
static int driver_section=0; /* when reading any driver section from the configuration file */
static int startwithgui=FALSE;
int jde_loadschema(char *name)
{
  char n[200];
  int i;
  char *error;

  strcpy(n,name); strcat(n,".so");
  /*  all[num_schemas].handle = dlopen(n,RTLD_LAZY|RTLD_GLOBAL);*/
  /* Schemas don't share their global variables, to avoid symbol collisions */
  all[num_schemas].handle = dlopen(n,RTLD_LAZY);

  if (  NULL == all[num_schemas].handle ) {
    strcpy(n, "./");
    strcat(n, name) ; 
    strcat(n, ".so"); 
    printf("name %s\n", n);
    all[num_schemas].handle = dlopen(n,RTLD_LAZY);
  }
  

  if (!(all[num_schemas].handle)) { 
    fprintf(stderr,"%s\n",dlerror());
    printf("I can't load the %s schema\n",name);
    exit(1);
  }
  else {
      /* symbols from the plugin: */
      dlerror();
      strcpy(n,name); strcat(n,"_startup");
      all[num_schemas].startup = (void (*)(void)) dlsym(all[num_schemas].handle,n);  
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
      pthread_mutex_init(&all[num_schemas].mymutex,PTHREAD_MUTEX_TIMED_NP);
      pthread_cond_init(&all[num_schemas].condition,NULL);
      /* the thread is created on startup. This is the load */

      for(i=0;i<MAX_LOADEDSCHEMAS;i++)
	{
	  if (associated_ID[i]==-1)
	    {
	    associated_ID[i]=num_schemas;
	    break;
	    }
	}    
      if (i==MAX_LOADEDSCHEMAS) printf("WARNING: No guientry available for %s schema\n",all[num_schemas].name);

      printf("%s schema loaded (id %d)\n",name,(*(all[num_schemas].id)));
      num_schemas++;
      return 1;
    }
}

int jde_loaddriver(char *name)
{
  char n[200];
  char *error;

  strcpy(n,name); strcat(n,".so");
  /* Drivers don't share their global variables, to avoid the symbol collisions */
  mydrivers[num_drivers].handle = dlopen(n,RTLD_LAZY);

  if (  NULL == mydrivers[num_drivers].handle ) {
    strcpy(n, "./");
    strcat(n, name) ; 
    strcat(n, ".so"); 
    mydrivers[num_drivers].handle = dlopen(n,RTLD_LAZY);
  }

  if (!(mydrivers[num_drivers].handle))
    { fprintf(stderr,"%s\n",dlerror());
    printf("I can't load the %s driver\n",name);
    exit(1);
    }
  else 
    {
      /* symbols from the plugin: */
      dlerror();
      strcpy(n,name); strcat(n,"_startup");
      mydrivers[num_drivers].startup = (void (*)(char *)) dlsym(mydrivers[num_drivers].handle,n); 
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      dlerror();
      strcpy(n,name); strcat(n,"_close");
      mydrivers[num_drivers].close = (void (*)()) dlsym(mydrivers[num_drivers].handle,n); 
      if ((error=dlerror()) != NULL)
	printf("WARNING: Unresolved symbol %s in %s schema\n",n,name);

      mydrivers[num_drivers].id = num_drivers;
      strcpy(mydrivers[num_drivers].name,name);
      printf("%s driver loaded (driver %d)\n",name,mydrivers[num_drivers].id);
      num_drivers++;
      return 1;
    }
}

int jde_readline(FILE *myfile)
     /* It reads a single line from config file, parses it and do the right thing. Returns EOF in detects end of such file. Otherwise returns 0.*/
     /* To startup non-basic schemas, just raise the flag, putting the in active state. It will effectively start up in main, after the "startup" of all basic schemas */

{
  const int limit = 256;
  char word[limit];
  int i=0,j=0;
  char buffer_file[limit]; 
 

  buffer_file[0]=fgetc(myfile);
  if (buffer_file[0]==EOF) return EOF;
  if (buffer_file[0]==255) return EOF; 
  if (buffer_file[0]=='#') {while(fgetc(myfile)!='\n'); return 0;}
  if (buffer_file[0]==' ') {while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);}
  if (buffer_file[0]=='\t') {while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);}

  /* Captura la linea y luego leeremos de la linea con sscanf, comprobando en la linea que el ultimo caracter es \n. No lo podemos hacer directamente con fscanf porque esta funcion no distingue espacio en blanco de \n */
  while((buffer_file[i]!='\n') && 
	  (buffer_file[i] != 255) &&  
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
    if (strcmp(word,"load")==0)
      {
	while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	sscanf(&buffer_file[j],"%s",word);
	jde_loadschema(word);
	(*all[num_schemas-1].startup)();
      }
    else if (strcmp(word,"driver")==0)
      {
	if(driver_section==0){
	  while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	  sscanf(&buffer_file[j],"%s",word);
	  driver_section=1;
	  jde_loaddriver(word);
	  (*mydrivers[num_drivers-1].startup)(configfile);
	  /*	  (*mydrivers[num_drivers-1].resume)();*/
	}else{
	  printf("error in config file. driver section without 'end_driver' keyword in file\n");
	  exit(-1);
	}
      }
    else if(strcmp(word,"end_driver")==0){
      while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
      driver_section=0;
    }else{
      if(driver_section==0) printf("i don't know what to do with: %s\n",buffer_file);
    }
    return 0;
  }
}


int main(int argc, char** argv)
{
  int i,j,marca,leidos,last=0;
  char buff[MAX_BUFFER];
  FILE *config;
  int n=1; /* argument number in the console for the configuration file parameter */
  int filenameatconsole=FALSE;

  /* I give them an initial dummy value to avoid the "segmentation fault" if no driver gives them a value. Good drivers will create proper resume and suspend functions for the sensors and actuators they support in the current configuration selected by the human user */
   encoders_suspend=dummy;
   encoders_resume=dummy;
   laser_suspend=dummy; 
   laser_resume=dummy;
   sonars_suspend=dummy;
   sonars_resume=dummy;
   imageA_resume=dummy;
   imageA_suspend=dummy;
   imageB_resume=dummy;
   imageB_suspend=dummy;
   imageC_resume=dummy;
   imageC_suspend=dummy;
   imageD_resume=dummy;
   imageD_suspend=dummy;
   pantiltencoders_resume=dummy;
   pantiltencoders_suspend=dummy;
   motors_suspend=dummy;
   motors_resume=dummy;
   pantiltmotors_suspend=dummy;
   pantiltmotors_resume=dummy;
  
  signal(SIGTERM, &jdeshutdown); /* kill interrupt handler */
  signal(SIGINT, &jdeshutdown); /* control-C interrupt handler */
  signal(SIGABRT, &jdeshutdown); /* failed assert handler */
  signal(SIGPIPE, SIG_IGN);
  
  /* Pablo Barrera: Por alguna razón libforms hace que fprintf("%f") ponga 
    una coma en vez de un punto como separador si las locales son españolas. 
    Con esto las ponemos a POSIX. Al salir el entorno es el normal */
  /*unsetenv("LANG");*/
  setenv("LC_ALL","POSIX",1);

  printf("%s\n",thisrelease);

  n=1;
  while(argv[n]!=NULL)
    {
      if ((strcmp(argv[n],"--help")==0) ||
	  (strcmp(argv[n],"-h")==0))
	{printf("Use: jde [config.file] [--gui]\n\n     [config.file] Sets an specific config file. Don't use this option to read default configuration.\n           [--gui] Starts JDE with the main gui activated.\n\n");
	exit(0);}
      else if ((strcmp(argv[n],"--version")==0) ||
	       (strcmp(argv[n],"-v")==0))
	{printf("%s\n",thisrelease);
	exit(0);}
      else if ((strcmp(argv[n],"--gui")==0) ||
	       (strcmp(argv[n],"-g")==0))
	startwithgui=TRUE;   
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
  
  
 
 
/*  Posicion y orientacion de los sensores con respecto al centro del eje trasero del robot. Dado un sistema de coordenadas con la X en la direccion de movimiento del robot, los angulos se miden considerando que el eje X toma valor 0 y siendo positivos cuando se gira en sentido contrario al de movimiento de las agujas del reloj. Se utiliza para cambiar las distancias sensoriales al sistema de referencia local, solidario con el robot-enclosure. la rellena con milimetros y grados.   */ 
  us_coord[0][0]=115.; us_coord[0][1]=130.; us_coord[0][2]=90.; 
  us_coord[1][0]=155.; us_coord[1][1]=115.; us_coord[1][2]=50.; 
  us_coord[2][0]=190.; us_coord[2][1]=80.; us_coord[2][2]=30.; 
  us_coord[3][0]=210.; us_coord[3][1]=25.; us_coord[3][2]=10.; 
  us_coord[4][0]=210.; us_coord[4][1]=-25.; us_coord[4][2]=350.; 
  us_coord[5][0]=190.; us_coord[5][1]=-80.; us_coord[5][2]=330.; 
  us_coord[6][0]=155.; us_coord[6][1]=-115.; us_coord[6][2]=310;
  us_coord[7][0]=115.; us_coord[7][1]=-130.; us_coord[7][2]=270.; 
  us_coord[8][0]=-115.; us_coord[8][1]=-130.; us_coord[8][2]=270.; 
  us_coord[9][0]=-155.; us_coord[9][1]=-115.; us_coord[9][2]=230.; 
  us_coord[10][0]=-190.; us_coord[10][1]=-80.; us_coord[10][2]=210.; 
  us_coord[11][0]=-210.; us_coord[11][1]=-25.; us_coord[11][2]=190.; 
  us_coord[12][0]=-210.; us_coord[12][1]=25.; us_coord[12][2]=170.; 
  us_coord[13][0]=-190.; us_coord[13][1]=80.; us_coord[13][2]=150.;  
  us_coord[14][0]=-155.; us_coord[14][1]=115.; us_coord[14][2]=130.; 
  us_coord[15][0]=-115.; us_coord[15][1]=130.; us_coord[15][2]=90.; 

  for(j=0;j<NUM_SONARS;j++)
    { 
      us_coord[j][3]= cos(us_coord[j][2]*DEGTORAD);
      us_coord[j][4]= sin(us_coord[j][2]*DEGTORAD);
    }

  laser_coord[0]=19.; laser_coord[1]=0.; laser_coord[2]=0.; 
  laser_coord[3]=cos(laser_coord[2]*DEGTORAD);
  laser_coord[4]=sin(laser_coord[2]*DEGTORAD);

  camera_coord[0]=190.; 
  camera_coord[1]=0.; 
  camera_coord[2]=0.; 
  camera_coord[3]= cos(camera_coord[2]*DEGTORAD);
  camera_coord[4]= sin(camera_coord[2]*DEGTORAD);


  /* initial values for sensor variables*/
  colorA=colorAA;
  colorB=colorBB;
  colorC=colorCC;
  colorD=colorDD;
  greyA=greyAA;
  for(i=0; i<(SIFNTSC_COLUMNS*SIFNTSC_ROWS);i++)
    {colorA[i*3]=(char) 0; /* blue */ 
    colorA[i*3+1]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30); /* green */
    colorA[i*3+2]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30); /* red */
    colorB[i*3]= 0;/* blue */ 
    colorB[i*3+1]=0; /* green */
    colorB[i*3+2]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30);/* red */
    colorC[i*3]= 0;/* blue */ 
    colorC[i*3+1]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30);/* green */
    colorC[i*3+2]=0; /* red */
    colorD[i*3]=(char)(((i%SIFNTSC_COLUMNS)%30)*255/30);/* blue */
    colorD[i*3+1]=0; /* green */
    colorD[i*3+2]= 0;/* red */ 
    }
  v=0.; w=0;


  /* reset the guientries-loadeschemas table before the 
     loading of the schemas (while parsing the conf file) 
     updates such table */
  for(i=0;i<MAX_LOADEDSCHEMAS;i++)
    associated_ID[i]=-1;
  
  /* read the configuration file: load drivers and schemas */
  printf("Reading configuration...\n");
  do {
    i=jde_readline(config);
  }while(i!=EOF);
  
  /* start the jdegui thread */
  printf("Starting gui stuff...\n");
  jdegui_startup(); 
  jdegui_resume();

  if (startwithgui==TRUE) mastergui_resume();

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

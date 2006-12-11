/*
 *  Copyright (C) 2006 Antonio Pineda Cabello
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
 *  Authors : Antonio Pineda Cabello <apineda@gsyc.escet.urjc.es>, Raul Isado <rauli@mi.madritel.es>
 */

/************************************************
 * jdec player driver                           *
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <libplayerc/playerc.h>
#include <jde.h>
#define MIN_DELAY_BETWEEN_COMMANDS 50 /* ms */

/* for player support through the player server */
playerc_client_t *player_client;
playerc_position2d_t *player_position;
playerc_sonar_t *player_sonar;
playerc_laser_t *player_laser;
playerc_power_t *player_power;
int DEBUG=0;
/*unsigned long v_rep=0, v_delay=0, v_total=0, w_rep=0, w_delay=0, w_total=0;*/

/* driver internal variables */
unsigned long tag=0;

/* player thread attributes */
pthread_t player_th;
int state;
pthread_mutex_t mymutex;
pthread_cond_t condition;

/* player driver API options */
char driver_name[256]="player";
int player_close_command=0;
int serve_laser=0; int serve_encoders=0; int serve_sonars=0; int serve_motors=0;
int laser_active=0; int encoders_active=0; int sonars_active=0; int motors_active=0;

/* player driver attributes */
int playerport;
char playerhost[256];
float correcting_x=0.;
float correcting_y=0.;
float correcting_theta=0.;


/* PLAYER DRIVER FUNCTIONS */
void player_close(){

  player_close_command=1;
  playerc_client_disconnect(player_client);
  playerc_client_destroy(player_client);
  printf("disconnected from Player\n");
}

int player_laser_resume(){

  if((serve_laser)&&(laser_active==0)){
    laser_active=1;
    printf("player: laser resume\n");

    if((encoders_active==0)&&(sonars_active==0)&&(motors_active==0)){

      /* player thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int player_laser_suspend(){

  if((serve_laser)&&(laser_active)){
    laser_active=0;
    printf("player: laser suspend\n");
    if((encoders_active==0)&&(sonars_active==0)&&(motors_active==0)){    

      /* player thread goes sleep */
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int player_encoders_resume(){

  if((serve_encoders)&&(encoders_active==0)){
    encoders_active=1;
    printf("player: encoders resume\n");

    if((laser_active==0)&&(sonars_active==0)&&(motors_active==0)){

      /* player thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;

}

int player_encoders_suspend(){

  if((serve_encoders)&&(encoders_active)){
    encoders_active=0;
    printf("player: encoders suspend\n");
    if((laser_active==0)&&(sonars_active==0)&&(motors_active==0)){    

      /* player thread goes sleep */
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int player_sonars_resume(){

  if((serve_sonars)&&(sonars_active==0)){
    sonars_active=1;
    printf("player: sonars resume\n");

    if((laser_active==0)&&(encoders_active==0)&&(motors_active==0)){

      /* player thread goes winner */
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int player_sonars_suspend(){

  if((serve_sonars)&&(sonars_active)){
    sonars_active=0;
    printf("player: sonars suspend\n");
    if((laser_active==0)&&(encoders_active==0)&&(motors_active==0)){

      /* player thread goes sleep */
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
    }
  }
  return 0;
}

int player_motors_resume(){

  if((serve_motors)&&(motors_active==0)){
    motors_active=1;
    printf("player: motors resume\n");

    /*if((laser_active==0)&&(encoders_active==0)&&(sonars_active==0)){
      pthread_mutex_lock(&mymutex);
      state=winner;
      pthread_cond_signal(&condition);
      pthread_mutex_unlock(&mymutex);
      }*/
  }
  return 0;

}

int player_motors_suspend(){

  if((serve_motors)&&(motors_active)){
    motors_active=0;
    printf("player: motors suspend\n");

    /*if((laser_active==0)&&(encoders_active==0)&&(sonars_active==0)){    
      pthread_mutex_lock(&mymutex);
      state=slept;
      pthread_mutex_unlock(&mymutex);
      }*/
  }
  return 0;
}

void player_motors_iteration(){

  float  w_new, v_new;
  static float w_sp=0., v_sp=0.;
  static unsigned long before;
  unsigned long now;
  int v_integer=0; int w_integer=0;
  struct timeval t;

  kmotors++;

  /* getting speed in integer value */
  v_integer=(int)v; w_integer=(int)w;

  if(v_integer>MAX_VEL) v_new=MAX_VEL/1000;
  else if (v_integer<-MAX_VEL) v_new=-MAX_VEL/1000;
  else v_new=v/1000;

  if(w_integer>MAX_RVEL) w_new=MAX_RVEL*DEGTORAD;
  else if(w_integer<-MAX_RVEL) w_new=-MAX_RVEL*DEGTORAD;
  else w_new=w*DEGTORAD;

  gettimeofday(&t,NULL);
  now=t.tv_sec*1000000+t.tv_usec;
  if (((v_sp!=v_new)||(w_sp!=w_new))&&((now-before)>(MIN_DELAY_BETWEEN_COMMANDS*1000))){
    v_sp=v_new;
    w_sp=w_new;
    before=now;
    
    /* sending speed to player server */
    playerc_position2d_set_cmd_vel(player_position,v_sp,0,w_sp,0);
  }
}

void *player_thread(void *not_used){
  struct timeval t;
  unsigned long now;
  static unsigned long last=0;
  static int howmany=0;

  printf("player: player thread started up\n");

  do{

    pthread_mutex_lock(&mymutex);

    if (state==slept){
      printf("player: player thread in sleep mode\n");
      pthread_cond_wait(&condition,&mymutex);
      printf("player: player thread woke up\n");
      pthread_mutex_unlock(&mymutex);
      
    }else{
      
      pthread_mutex_unlock(&mymutex);
      
      /* sending motors command */
      if((serve_motors)&&(motors_active)) player_motors_iteration();

      /* reading from player server */
      playerc_client_read(player_client);
      
      if (DEBUG==1)
	{
	  gettimeofday(&t,NULL);
	  now=t.tv_sec*1000000+t.tv_usec;      
	  howmany++;
	  if ((now-last)>10000000)
	    {fprintf(stderr,"player: client -> %d iterations in 10 secs\n",howmany);
	      last = now;
	      howmany=0;
	    }
	}
    }
  }while(player_close_command==0);
  pthread_exit(0);
}

void player_laser_callback(void *not_used)
{
  static unsigned long last=0;
  unsigned long now;
  struct timeval t;
  int j=0,cont=0;
  static int howmany=0;
  
  if (DEBUG==1)
    {
      gettimeofday(&t,NULL);
      now=t.tv_sec*1000000+t.tv_usec;
      howmany++;
      if ((now-last)>10000000) 
	{
	  printf ("player: laser -> %d readings in 10 secs\n",howmany);
	  last=now;
	  howmany=0;
	}
    }
  
  klaser++;
  laser_clock=tag;
  tag++;

  while((cont<NUM_LASER)&&(j<player_laser->scan_count)){
    if (j%2!=1){
      /*player ofrece 360 medidas cada 0.5 angulos, por lo k solo cojemos los angulos "enteros", 181 medidas*/
      /*NUM_LASER readings (181 in the robot, 179 in the simulator) */
      jde_laser[cont]=(int)(player_laser->scan[j][0]*1000);
      cont++;
    }
    j++;
  }
}


void player_encoders_callback(void *not_used)
{

 static unsigned long last=0;
  unsigned long now;
  struct timeval t;
  static int howmany=0;
  float robotx,roboty,robottheta;

  if (DEBUG==1)
    {
      gettimeofday(&t,NULL);
      now=t.tv_sec*1000000+t.tv_usec;
      howmany++;
      if ((now-last)>10000000) 
	{
	  printf ("player: encoders -> %d readings in 10 secs\n",howmany);
	  last=now;
	  howmany=0;
	}
    }
  
  kencoders++;
  encoders_clock=tag;
  tag++;

  robotx=(player_position->px)*1000*(float)cos(DEGTORAD*correcting_theta) - (player_position->py)*1000*(float)sin(DEGTORAD*correcting_theta) + correcting_x;
  roboty=(player_position->py)*1000*(float)cos(DEGTORAD*correcting_theta) + (player_position->px)*1000*(float)sin(DEGTORAD*correcting_theta) + correcting_y;
  robottheta=(player_position->pa*RADTODEG) + correcting_theta;
  if(robottheta<=0) robottheta=robottheta+360;

  jde_robot[0]=robotx;
  jde_robot[1]=roboty;
  jde_robot[2]=robottheta*DEGTORAD;
  jde_robot[3]=cos(jde_robot[2]);
  jde_robot[4]=sin(jde_robot[2]);
}

void player_sonar_callback(void *not_used)
{
  static unsigned long last=0;
  unsigned long now;
  struct timeval t;
  int j;
  static int howmany=0;
        
  if (DEBUG==1)
    {
      gettimeofday(&t,NULL);
      now=t.tv_sec*1000000+t.tv_usec;
      howmany++;
      if ((now-last)>10000000) 
	{
	  printf ("player: sonars -> %d readings in 10 secs\n",howmany);
	  last=now;
	  howmany=0;
	}
    }
  
  ksonars++;
  
  for (j=0;j<player_sonar->pose_count; j++){
    /* for pioneer 16 data per reading */
    us[j]=(float)player_sonar->scan[j]*1000;
    us_clock[j]=tag;
    tag++;
  }
}

void player_battery_callback(void *not_used)
{
 static unsigned long last=0;
  unsigned long now;
  struct timeval t;
  static int howmany=0;

  if (DEBUG==1)
    {
      gettimeofday(&t,NULL);
      now=t.tv_sec*1000000+t.tv_usec;
      howmany++;
      if ((now-last)>10000000) 
	{
	  printf ("player: battery -> %d readings in 10 secs\n",howmany);
	  last=now;
	  howmany=0;
	}
    }

  /* this is unused in JDE since we don't use the charge battery sensor.
     if you want to use this option you only have to use the 'player_power->charge' */
  tag++;
}

int player_init(){

  printf("connecting to Player Server at '%s:%d'\n",playerhost,playerport);
  player_client = playerc_client_create(NULL,playerhost,playerport);
  if (playerc_client_connect(player_client) != 0)
    {
      fprintf(stderr, "player: %s\n", playerc_error_str());
      exit(-1);
    }
 
  /* Create a position proxy (device id "position:0") and susbscribe in read/write mode */
  if(serve_encoders==1){
    player_position = playerc_position2d_create(player_client, 0);  
    if (playerc_position2d_subscribe(player_position, PLAYER_OPEN_MODE) != 0)
      {
	fprintf(stderr, "player: %s\n", playerc_error_str());
	printf("player: did you set 'position2d:0' in the robot provides line at Player config file?\n");
	printf("player: cannot initiate Player connection\n");
	exit(-1);
	/*return -1;*/
      }
    playerc_position2d_enable(player_position,1);

    /* registering the encoders callback to call when new encoders data arrived  */
    playerc_client_addcallback(player_client,&(player_position->info),&player_encoders_callback,&(player_position));
  } 

  /*Create a sonar proxy and subscribe in read/write mode*/
  if(serve_sonars){
    player_sonar=playerc_sonar_create(player_client, 0);
    if (playerc_sonar_subscribe(player_sonar, PLAYER_OPEN_MODE) != 0)
      {
	fprintf(stderr, "player: %s\n", playerc_error_str());
	printf("player: did you set 'sonar:0' in the robot provides line at Player config file?\n");
	printf("player: cannot initiate Player connection\n");
	exit(-1);
	/*return -1;*/
    }
    playerc_sonar_get_geom(player_sonar);

    /* registering the sonars callback to call when new sonars data arrived */
    playerc_client_addcallback(player_client,&(player_sonar->info),&player_sonar_callback,&(player_sonar->scan));
  }

  /*Create a laser proxy and subscribe in read/write mode*/

  if(serve_laser){
    player_laser=playerc_laser_create(player_client, 0);
    if (playerc_laser_subscribe(player_laser, PLAYER_OPEN_MODE) != 0)
      {
	fprintf(stderr, "player: %s\n", playerc_error_str());
	printf("player: did you set 'laser:0' in the robot provides line at Player config file?\n");
	printf("player: cannot initiate Player connection\n");
	exit(-1);
	/* return -1;*/
      }

    /* registering the laser callback to call when new laser data arrived */
    playerc_client_addcallback(player_client,&(player_laser->info),&player_laser_callback,&(player_laser->scan));
  }    

  /*if(serve_motors){
    player_power=playerc_power_create(player_client, 0);
    if (playerc_power_subscribe(player_power, PLAYER_OPEN_MODE) != 0)
    {
    fprintf(stderr, "Player: %s\n", playerc_error_str());
    return -1;
    }
    playerc_client_addcallback(player_client,&(player_power->info),&player_battery_callback,&(player_power->charge));
    }*/
  
  return 0;
}

int player_parseconf(char *configfile){

  int end_parse=0; int end_section=0; int driver_config_parsed=0;
  int hostname_hostport_ok=0;
  FILE *myfile;
  const int limit = 256;

  if ((myfile=fopen(configfile,"r"))==NULL){
    printf("player: cannot find config file\n");
    return -1;
  }

  do{
    
    char word[256],word2[256],buffer_file[256];
    int i=0; int j=0;

    buffer_file[0]=fgetc(myfile);
    
    /* end of file */
    if (feof(myfile)){
      end_section=1;
      end_parse=1;
      
      /* line comment */
    }else if (buffer_file[0]=='#') {
      while(fgetc(myfile)!='\n');
      
      /* white spaces */
    }else if (buffer_file[0]==' ') {
      while(buffer_file[0]==' ') buffer_file[0]=fgetc(myfile);

      /* tabs */
    }else if(buffer_file[0]=='\t') {
      while(buffer_file[0]=='\t') buffer_file[0]=fgetc(myfile);
      /* storing line in buffer */
    }else{
      
      while(buffer_file[i]!='\n') buffer_file[++i]=fgetc(myfile);
      buffer_file[++i]='\0';

      if (i >= limit-1) { 
	printf("%s...\n", buffer_file); 
	printf ("Line too long in config file!\n"); 
	exit(-1);
      }
      
      /* first word of the line */     
      if (sscanf(buffer_file,"%s",word)==1){
	if (strcmp(word,"driver")==0) {
	  while((buffer_file[j]!='\n')&&(buffer_file[j]!=' ')&&(buffer_file[j]!='\0')&&(buffer_file[j]!='\t')) j++;
	  sscanf(&buffer_file[j],"%s",word2);
	  
	  /* checking if this section matchs our driver name */
	  if (strcmp(word2,driver_name)==0){
	    /* the sections match */
	    do{
	      
	      char buffer_file2[256],word3[256],word4[256],word5[256],word6[256];
	      int k=0; int z=0;

	      buffer_file2[0]=fgetc(myfile);
	      
	      /* end of file */
	      if (feof(myfile)){
		end_section=1;
		end_parse=1;
		
		/* line comment */
	      }else if (buffer_file2[0]=='#') {
		while(fgetc(myfile)!='\n');
	      
		/* white spaces */
	      }else if (buffer_file2[0]==' ') {
		while(buffer_file2[0]==' ') buffer_file2[0]=fgetc(myfile);

		/* tabs */
	      }else if(buffer_file2[0]=='\t') {
		while(buffer_file2[0]=='\t') buffer_file2[0]=fgetc(myfile);

		/* storing line in buffer */
	      }else{
		
		while(buffer_file2[k]!='\n') buffer_file2[++k]=fgetc(myfile);
		buffer_file2[++k]='\0';
		
		/* first word of the line */
		if (sscanf(buffer_file2,"%s",word3)==1){
		  if (strcmp(word3,"end_driver")==0) {
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    driver_config_parsed=1;
		    end_section=1;
		    end_parse=1;
		    
		  }else if (strcmp(word3,"driver")==0) {
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    printf("player: error in config file.\n'end_section' keyword required before starting new driver section.\n");
		    end_section=1; end_parse=1;

		  }else if(strcmp(word3,"provides")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s",word3,word4)>1){

		      if(strcmp(word4,"laser")==0) serve_laser=1;
		      else if(strcmp(word4,"sonars")==0) serve_sonars=1;
		      else if(strcmp(word4,"encoders")==0) serve_encoders=1;
		      else if(strcmp(word4,"motors")==0) serve_motors=1;

		    }else{
		      printf("player: provides line incorrect\n");
		    }

		  }else if(strcmp(word3,"hostname")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)>3){
		      strcpy(playerhost,word4);
		      if(strcmp(word5,"port")!=0) printf("player: unrecognized word '%s'. assuming you mean 'port'.\n",word5);
		      playerport=atoi(word6);
		      hostname_hostport_ok=1;

		    }else{
		      printf("player: hostname line incorrect\n");
		    }

		  }else if(strcmp(word3,"initial_position")==0){
		    while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
		    if(sscanf(buffer_file2,"%s %s %s %s",word3,word4,word5,word6)>3){
		      correcting_x=(float)atof(word4);
		      correcting_y=(float)atof(word5);
		      correcting_theta=(float)atof(word6);
		      printf("player: correcting x=%.1f(mm)  y=%.1f(mm)  theta=%.1f(deg).\n",correcting_x,correcting_y,correcting_theta);
		      printf("player: make sure this is the initial position in world file.\n");

		    }else{
		      printf("player: initial_position line incorrect\n");
		    }

		  }else printf("player: I don't know what to do with '%s'\n",buffer_file2);
		}
	      }
	    }while(end_section==0);
	    end_section=0;
	  }
	}
      }
    }
  }while(end_parse==0);
  
  /* checking if a driver section was read correctly */
  if((driver_config_parsed==1)&&(hostname_hostport_ok==1)){
    if((serve_laser==0)&&(serve_motors==0)&&(serve_encoders==0)&&(serve_sonars==0)){
      printf("player: warning! no device provided.\n");
    }
    return 0;

  }else return -1;
}

int player_startup(char *configfile){

  /* we call the function to parse the config file */
  if(player_parseconf(configfile)==-1){
    printf("player: cannot initiate driver. configfile parsing error.\n");
    exit(-1);
  }

  /* player initialitation */
  player_init();

  /* player thread creation */
  pthread_mutex_lock(&mymutex);
  state=slept;
  pthread_create(&player_th,NULL,player_thread,NULL);
  pthread_mutex_unlock(&mymutex);

  /* resume and suspend asignments */
  if(serve_laser){laser_resume=player_laser_resume; laser_suspend=player_laser_suspend;}
  if(serve_encoders){encoders_resume=player_encoders_resume; encoders_suspend=player_encoders_suspend;}
  if(serve_sonars){sonars_resume=player_sonars_resume;sonars_suspend=player_sonars_suspend;}
  if(serve_motors){motors_resume=player_motors_resume;motors_suspend=player_motors_suspend;}

  return 0;
}

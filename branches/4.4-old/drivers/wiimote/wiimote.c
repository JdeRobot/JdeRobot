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
 *  Authors : Javier Martin Ramos <xaverbrennt@yahoo.es>
 *            Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 *
 */

/**
 *  jdec wiimote driver provides support for wiimote using cwiid library
 *
 *  @file wiimote.c
 *  @author Jose Antonio Santos Cadenas  <santoscadenas@gmail.com>
 *  @version 0.1
 *  @date 2008-02-21
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include <cwiid.h>
#include <jde.h>
/* #include <interfaces/wiimote.h>*/

/* wiimote driver API options */
/** wiimote driver name.*/
char *driver_name="wiimote";

/** Indicates if buttons are provided*/
int provide_buttons=0;
/** Indicates if accelerators are provided*/
int provide_accel=0;
/** Indicates if infra red sensor is provided*/
int provide_ir=0;
/** Indicates if nunchuk extension is provided*/
int provide_nunchuk=0;

/** Indicates the number of wiimotes served*/
int n_wiimotes=0;
/** Indicates the number of wiimotes started*/
int started_wiimotes=0;

/** Buttons schema identifier*/
int buttons_id;
/** Accel schema identifier*/
int accel_id;
/** Ir schema identifier*/
int ir_id;
/** Nunchuk schema identifier*/
int nunchuk_id;

/** Mutex to control concurrent acces to ref counters*/
pthread_mutex_t refmutex;
/** Indicates if buttons schema is active*/
int buttons_active=0;
/** Indicates if accel schema is active*/
int accel_active=0;
/** Indicates if ir schema is active*/
int ir_active=0;
/** Indicates if nunchuk schema is active*/
int nunchuk_active=0;

/** Struct containin the wiimote status*/
struct cwiid_state *wii_status;
/** Reception mode*/
unsigned char rpt_mode=0;

/** Buttons schema clock*/
unsigned long int buttons_clock=0;
/** Accel schema clock*/
unsigned long int accel_clock=0;
/** Ir schema clock*/
unsigned long int ir_clock=0;
/** Nunchuk schema clock*/
unsigned long int nunchuk_clock=0;

/** Data structure to handle wiimote*/
cwiid_wiimote_t ** wiimotes;

/*DRIVER FUNCTIONS*/
/** wii_buttons run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int buttons_run(int father, int *brothers, arbitration fn){
   if(provide_buttons==1){
      pthread_mutex_lock(&refmutex);
      buttons_active++;
      if ((all[buttons_id].father==GUIHUMAN) ||
           (all[buttons_id].father==SHELLHUMAN))
         all[buttons_id].father = father;
      if(buttons_active==1)
      {
         int i=0;
         pthread_mutex_unlock(&refmutex);
         /*printf("colorA schema run (mplayer driver)\n");*/
         all[buttons_id].father = father;
         all[buttons_id].fps = 0.;
         all[buttons_id].k =0;
         put_state(buttons_id,winner);
         for (i=0; i<n_wiimotes; i++){
            struct cwiid_state status;
            cwiid_get_state(wiimotes[i], &status);
            wii_status[i].buttons = status.buttons;
            rpt_mode=rpt_mode | CWIID_RPT_BTN;
            if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
               fprintf (stderr, "Error activating mode for wiimote %d\n", i);
            }
         }
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** wii_buttons stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int buttons_stop(){
   pthread_mutex_lock(&refmutex);
   buttons_active--;
   if((provide_buttons==1)&&(buttons_active==0)){
      int i;
      pthread_mutex_unlock(&refmutex);
      put_state(buttons_id,slept);
      for (i=0; i<n_wiimotes; i++){
         rpt_mode=rpt_mode & ~CWIID_RPT_BTN;
         if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
            fprintf (stderr, "Error activating mode for wiimote %d\n", i);
         }
      }
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** wii_accel run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int accel_run(int father, int *brothers, arbitration fn){
   if(provide_accel==1){
      pthread_mutex_lock(&refmutex);
      accel_active++;
      if ((all[accel_id].father==GUIHUMAN) ||
           (all[accel_id].father==SHELLHUMAN))
         all[accel_id].father = father;
      if(accel_active==1)
      {
         int i;
         pthread_mutex_unlock(&refmutex);
         /*printf("colorA schema run (mplayer driver)\n");*/
         all[accel_id].father = father;
         all[accel_id].fps = 0.;
         all[accel_id].k =0;
         put_state(accel_id,winner);
         for (i=0; i<n_wiimotes; i++){
            struct cwiid_state status;
            int j;
            cwiid_get_state(wiimotes[i], &status);
            for (j=0; j<3; j++){
               wii_status[i].acc[j] = status.acc[j];
            }
            rpt_mode=rpt_mode | CWIID_RPT_ACC;
            if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
               fprintf (stderr, "Error activating mode for wiimote %d\n", i);
            }
         }
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** wii_accel stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int accel_stop(){
   pthread_mutex_lock(&refmutex);
   accel_active--;
   if((provide_accel==1)&&(accel_active==0)){
      int i;
      pthread_mutex_unlock(&refmutex);
      put_state(accel_id,slept);
      for (i=0; i<n_wiimotes; i++){
         rpt_mode=rpt_mode & ~CWIID_RPT_ACC;
         if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
            fprintf (stderr, "Error activating mode for wiimote %d\n", i);
         }
      }
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** wii_ir run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int ir_run(int father, int *brothers, arbitration fn){
   if(provide_ir==1){
      pthread_mutex_lock(&refmutex);
      ir_active++;
      if ((all[ir_id].father==GUIHUMAN) ||
           (all[ir_id].father==SHELLHUMAN))
         all[ir_id].father = father;
      if(ir_active==1)
      {
         int i;
         pthread_mutex_unlock(&refmutex);
         /*printf("colorA schema run (mplayer driver)\n");*/
         all[ir_id].father = father;
         all[ir_id].fps = 0.;
         all[ir_id].k =0;
         put_state(ir_id,winner);
         for (i=0; i<n_wiimotes; i++){
            struct cwiid_state status;
            int j;
            cwiid_get_state(wiimotes[i], &status);
            for (j=0; j<CWIID_IR_SRC_COUNT; j++){
               wii_status[i].ir_src[j] = status.ir_src[j];
            }
            rpt_mode=rpt_mode | CWIID_RPT_IR;
            if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
               fprintf (stderr, "Error activating mode for wiimote %d\n", i);
            }
         }
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** wii_ir stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int ir_stop(){
   pthread_mutex_lock(&refmutex);
   ir_active--;
   if((provide_ir==1)&&(ir_active==0)){
      int i;
      pthread_mutex_unlock(&refmutex);
      put_state(ir_id,slept);
      for (i=0; i<n_wiimotes; i++){
         rpt_mode=rpt_mode & ~CWIID_RPT_IR;
         if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
            fprintf (stderr, "Error activating mode for wiimote %d\n", i);
         }
      }
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}

/** wii_nunchuk run function following jdec platform API schemas.
 *  @param father Father id for this schema.
 *  @param brothers Brothers for this schema.
 *  @param fn arbitration function for this schema.
 *  @return integer resuming result.*/
int nunchuk_run(int father, int *brothers, arbitration fn){
   if(provide_nunchuk==1){
      pthread_mutex_lock(&refmutex);
      nunchuk_active++;
      if ((all[nunchuk_id].father==GUIHUMAN) ||
           (all[nunchuk_id].father==SHELLHUMAN))
         all[nunchuk_id].father = father;
      if(nunchuk_active==1)
      {
         int i;
         pthread_mutex_unlock(&refmutex);
         /*printf("colorA schema run (mplayer driver)\n");*/
         all[nunchuk_id].father = father;
         all[nunchuk_id].fps = 0.;
         all[nunchuk_id].k =0;
         put_state(nunchuk_id,winner);
         for (i=0; i<n_wiimotes; i++){
            struct cwiid_state status;
            int j;
            cwiid_get_state(wiimotes[i], &status);
            for (j=0; j<2; j++){
               wii_status[i].ext.nunchuk.stick[j] = status.ext.nunchuk.stick[j];
            }
            for (j=0; j<3; j++){
               wii_status[i].ext.nunchuk.acc[j] = status.ext.nunchuk.acc[j];
            }
            rpt_mode=rpt_mode | CWIID_RPT_NUNCHUK;
            if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
               fprintf (stderr, "Error activating mode for wiimote %d\n", i);
            }
         }
      }
      else
         pthread_mutex_unlock(&refmutex);
   }
   return 0;
}

/** wii_nunchuk stop function following jdec platform API schemas.
 *  @return integer stopping result.*/
int nunchuk_stop(){
   pthread_mutex_lock(&refmutex);
   nunchuk_active--;
   if((provide_nunchuk==1)&&(nunchuk_active==0)){
      int i;
      pthread_mutex_unlock(&refmutex);
      put_state(nunchuk_id,slept);
      for (i=0; i<n_wiimotes; i++){
         rpt_mode=rpt_mode & ~CWIID_RPT_NUNCHUK;
         if(cwiid_set_rpt_mode(wiimotes[i], rpt_mode)){
            fprintf (stderr, "Error activating mode for wiimote %d\n", i);
         }
      }
   }
   else
      pthread_mutex_unlock(&refmutex);
   return 0;
}
/** wiimote callback funtion
 *  @param wiimote the wiimote producing the message
 *  @param mesg_count the number of messages
 *  @param msg the messages from the wiimote
 *  @param timestamp the message timestamp
 */
void cwiid_callback(cwiid_wiimote_t *wiimote, int mesg_count,
                    union cwiid_mesg mesg[], struct timespec *timestamp)
{
   int index, i, j;
   /*Find the index of the wiimote producing the callback*/
   for (i=0; i<n_wiimotes;i++){
      if (wiimote==wiimotes[i]){
         index=i;
      }
   }
   /*printf ("Callback del mando %d\n", index);*/
   for (i=0; i < mesg_count; i++){
      switch (mesg[i].type) {
         case CWIID_MESG_STATUS:
            printf("%s: battery=%d\n", driver_name, mesg[i].status_mesg.battery);
            switch (mesg[i].status_mesg.ext_type) {
               case CWIID_EXT_NONE:
                  printf("%s: Extenxion removed\n", driver_name);
                  break;
               case CWIID_EXT_NUNCHUK:
                  printf("%s: Nunchuk connected\n", driver_name);
                  break;
               default:
                  printf("%s: Unknow extension connected\n", driver_name);
                  break;
            }
            break;
         case CWIID_MESG_BTN:
            speedcounter(buttons_id);
            buttons_clock++;
            wii_status[index].buttons = mesg[i].btn_mesg.buttons;
            break;
         case CWIID_MESG_ACC:
            speedcounter(accel_id);
            for (j=0; j<3; j++){
               wii_status[index].acc[j] = mesg[i].acc_mesg.acc[j];
            }
            break;
         case CWIID_MESG_IR:
            speedcounter(ir_id);
            ir_clock++;
            for (j=0; j<CWIID_IR_SRC_COUNT; j++){
               wii_status[index].ir_src[j] = mesg[i].ir_mesg.src[j];
            }
            break;
         case CWIID_MESG_NUNCHUK:
            speedcounter(nunchuk_id);
            nunchuk_clock++;
            for (j=0; j<2; j++){
               wii_status[index].ext.nunchuk.stick[j] = mesg[i].nunchuk_mesg.stick[j];
            }
            for (j=0; j<3; j++){
               wii_status[index].ext.nunchuk.acc[j] = mesg[i].nunchuk_mesg.acc[j];
            }
            wii_status[index].ext.nunchuk.buttons = mesg[i].nunchuk_mesg.buttons;
            break;
         case CWIID_MESG_ERROR:
            fprintf(stderr, "%s: recieved error from the wiimote\n", driver_name);
            jdeshutdown(-1);
            break;
         default:
            break;
      }
   }
}

/** wiimote driver function to kill every wiimote and mencoder called.*/
void wiimote_terminate(){
   int i;
   for (i=0; i<started_wiimotes; i++){
      if (cwiid_close(wiimotes[i])) {
         fprintf(stderr, "Error disconnecting wiimote %d\n", i);
      }
   }
   printf("driver %s off\n", driver_name);
}

/** wiimote driver parse configuration file function.
 *  @param configfile path and name to the config file.
 *  @return 0 if parsing was successful or -1 if something went wrong.*/
int wiimote_parseconf(char *configfile){

   int end_parse=0; int end_section=0; int driver_config_parsed=0;
   FILE *myfile;
   const int limit = 256;

   if ((myfile=fopen(configfile,"r"))==NULL){
      printf("%s: cannot find config file\n", driver_name);
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
            printf ("%s: line too long in config file!\n", driver_name);
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
	      
                     char buffer_file2[256],word3[256],word4[256],word5[256];
                     int k=0;
                     int words=0;

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
                              driver_config_parsed=1;
                              end_section=1;
                              end_parse=1;
		    
                           }else if (strcmp(word3,"driver")==0) {
                              printf("%s: error in config file.\n'end_section' "
                                     "keyword required before starting new "
                                     "driver section.\n", driver_name);
                              end_section=1; end_parse=1;

                           }else if(strcmp(word3,"provides")==0){
                              words=sscanf(buffer_file2,"%s %s %s",
                                           word3,word4,word5);
                              if (words==2){
                                 if (strcmp(word4,"buttons")==0){
                                    provide_buttons=1;
                                 }
                                 else if (strcmp(word4,"accel")==0){
                                    provide_accel=1;
                                 }
                                 else if (strcmp(word4,"ir")==0){
                                    provide_ir=1;
                                 }
                                 else if (strcmp(word4,"nunchuk")==0){
                                    provide_nunchuk=1;
                                 }
                                 else{
                                    printf("%s: 'provides' line incorrect\n",
                                          driver_name);
                                 }
                              }
                              else{
                                 printf("%s: 'provides' line incorrect\n",
                                        driver_name);
                              }
                           }
                           else if(strcmp(word3,"n_wiimotes")==0){
                              /*Defines de number of wiimotes served*/
                              if (sscanf(buffer_file2,"%s %s %s",
                                  word3,word4,word5)==2)
                              {
                                 n_wiimotes=atoi(word4);
                              }
                           }
                           else printf("%s: i don't know what to do with"
                                        " '%s'\n", driver_name, buffer_file2);
                        }
                     }
                  }while(end_section==0);
                  end_section=0;
               }
            }
         }
      }
   }while(end_parse==0);
  
   /* checking if a driver section was read */
   if(driver_config_parsed==1){
      if(n_wiimotes==0 || (provide_buttons==0 && provide_accel==0 &&
                          provide_ir==0 && provide_nunchuk==0))
      {
         printf("%s: warning! wiimote sensor served, please check your "
                "configuration file.\n", driver_name);
      }
      return 0;
   }else return -1;
}

/** wiimote driver startup function following jdec platform API for drivers.
 *  @param configfile path and name to the config file of this driver.*/
void wiimote_init(char *configfile)
{
   int i;
   unsigned char leds[5]={CWIID_LED1_ON, CWIID_LED2_ON, CWIID_LED3_ON,
      CWIID_LED4_ON, CWIID_LED1_ON & CWIID_LED2_ON & CWIID_LED3_ON & CWIID_LED4_ON};

   /* we call the function to parse the config file */
   if(wiimote_parseconf(configfile)==-1){
      printf("%s: cannot initiate driver. configfile parsing error.\n",
             driver_name);
      jdeshutdown(-1);
   }
   
   /*Now we create wiimote structure for each wiimote served*/
   wiimotes=(cwiid_wiimote_t **)malloc(sizeof (cwiid_wiimote_t *)*n_wiimotes);
   if (wiimotes==NULL){
      printf ("%s: cannot create necessary data structures\n", driver_name);
   }

   wii_status=(struct cwiid_state *)malloc(sizeof(struct cwiid_state)*n_wiimotes);

   /* Connect to the wiimote */
   for (i=0; i<n_wiimotes; i++){
      printf("Put Wiimote %d in discoverable mode (press 1+2)...\n", i);
      if (!(wiimotes[i] = cwiid_open(BDADDR_ANY, 0))) {
         fprintf(stderr, "Unable to connect to wiimote %d\n", i);
         jdeshutdown(-1);
      }
      if (i<4){
         if (cwiid_set_led(wiimotes[i], leds[i])) {
            fprintf(stderr, "Error setting LEDs for wiimote %d \n", i);
         }
      }
      else{
         if (cwiid_set_led(wiimotes[i], leds[4])) {
            fprintf(stderr, "Error setting LEDs for wiimote %d \n", i);
         }
      }
      if (cwiid_set_mesg_callback(wiimotes[i], cwiid_callback)) {
         fprintf(stderr, "Unable to set message callback for wiimote %d\n", i);
      }
      if (cwiid_enable(wiimotes[i], CWIID_FLAG_MESG_IFC)) {
         fprintf(stderr, "Error enabling messages for wiimote %d\n", i);
      }
      cwiid_get_state(wiimotes[i], &wii_status[i]);
      started_wiimotes=i;
   }

   /*Activate the schemas*/
   if (provide_buttons){
      all[num_schemas].id = (int *) &buttons_id;
      strcpy(all[num_schemas].name,"wii_buttons");
      all[num_schemas].run = (runFn) buttons_run;
      all[num_schemas].stop = (stopFn) buttons_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport(all[buttons_id].name,"id",&buttons_id);
      myexport(all[buttons_id].name,"run",(void *)buttons_run);
      myexport(all[buttons_id].name,"stop",(void *)buttons_stop);
      for (i=0; i<n_wiimotes; i++){
         char sbuttons[30];
         char sclock[30];

         snprintf(sbuttons, 30, "%s%d", "buttons", i);
         snprintf(sclock, 30, "%s%d", "clock", i);
         myexport(all[buttons_id].name, sbuttons,&(wii_status[i].buttons));
         myexport(all[buttons_id].name, sclock, &buttons_clock);
      }
   }

   if (provide_accel){
      all[num_schemas].id = (int *) &accel_id;
      strcpy(all[num_schemas].name,"wii_accel");
      all[num_schemas].run = (runFn) accel_run;
      all[num_schemas].stop = (stopFn) accel_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport(all[accel_id].name,"id",&accel_id);
      myexport(all[accel_id].name,"run",(void *)accel_run);
      myexport(all[accel_id].name,"stop",(void *)accel_stop);
      for (i=0; i<n_wiimotes; i++){
         char saccel[30];
         char sclock[30];

         snprintf(saccel, 30, "%s%d", "accel", i);
         snprintf(sclock, 30, "%s%d", "clock", i);
         myexport(all[accel_id].name, saccel,&(wii_status[i].acc));
         myexport(all[accel_id].name, sclock, &accel_clock);
      }
   }

   if (provide_ir){
      all[num_schemas].id = (int *) &ir_id;
      strcpy(all[num_schemas].name,"wii_ir");
      all[num_schemas].run = (runFn) ir_run;
      all[num_schemas].stop = (stopFn) ir_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport(all[ir_id].name,"id",&ir_id);
      myexport(all[ir_id].name,"run",(void *)ir_run);
      myexport(all[ir_id].name,"stop",(void *)ir_stop);
      for (i=0; i<n_wiimotes; i++){
         char sir[30];
         char sclock[30];

         snprintf(sir, 30, "%s%d", "ir", i);
         snprintf(sclock, 30, "%s%d", "clock", i);
         myexport(all[ir_id].name, sir,&(wii_status[i].ir_src));
         myexport(all[ir_id].name, sclock, &ir_clock);
      }
   }

   if (provide_nunchuk){
      all[num_schemas].id = (int *) &nunchuk_id;
      strcpy(all[num_schemas].name,"wii_nunchuk");
      all[num_schemas].run = (runFn) nunchuk_run;
      all[num_schemas].stop = (stopFn) nunchuk_stop;
      printf("%s schema loaded (id %d)\n",all[num_schemas].name,num_schemas);
      (*(all[num_schemas].id)) = num_schemas;
      all[num_schemas].fps = 0.;
      all[num_schemas].k =0;
      all[num_schemas].state=slept;
      all[num_schemas].terminate = NULL;
      all[num_schemas].handle = NULL;
      num_schemas++;
      myexport(all[nunchuk_id].name,"id",&nunchuk_id);
      myexport(all[nunchuk_id].name,"run",(void *)nunchuk_run);
      myexport(all[nunchuk_id].name,"stop",(void *)nunchuk_stop);
      for (i=0; i<n_wiimotes; i++){
         char snunchuk[30];
         char sclock[30];

         snprintf(snunchuk, 30, "%s%d", "nunchuk", i);
         snprintf(sclock, 30, "%s%d", "clock", i);
         myexport(all[nunchuk_id].name, snunchuk,&(wii_status[i].ext.nunchuk));
         myexport(all[nunchuk_id].name, sclock, &nunchuk_clock);
      }
   }
}

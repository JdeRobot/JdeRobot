
/************************************************
 * jdec networkserver driver                    *
 ************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include "jde.h"
#include "jdemessages.h"

      /* constants for devices */
#define COLORA_DEVICE 0
#define COLORB_DEVICE 1
#define COLORC_DEVICE 2
#define COLORD_DEVICE 3
#define PANTILT_ENCODERS_DEVICE 4
#define PANTILT_MOTORS_DEVICE 5
#define LASER_DEVICE 6
#define SONARS_DEVICE 7
#define ENCODERS_DEVICE 8
#define MOTORS_DEVICE 9
      /* number of devices */
#define MAXDEVICE 10

pthread_t network_thread;
int state;
pthread_mutex_t mymutex;
pthread_cond_t condition;

/* networkserver driver API options */
char driver_name[256]="networkserver";

#define MAX_MESSAGE 2048
#define MAX_QUEUE 5
   // Maximum number of clients waiting for server's accept

struct client{
   int cs;
   char subscriptions[MAXDEVICE];
   char name[256];
   unsigned long clk;
};
#define CLIENT_TH_CYCLE 33 /*para servir las imágenes en tiempo real*/

/* port to bind the server */
int networkserver_port=0;

/* devices detected and their network id */
int serve_device[MAXDEVICE];
int device_network_id[MAXDEVICE];

/*Imported variables*/
void *variables[MAXDEVICE];
resumeFn resume[MAXDEVICE];
suspendFn suspend[MAXDEVICE];

pthread_mutex_t socketmutex;

/*PID's*/
pthread_t listen_pid;

/* other vars */
int display_fps=0;
int networkserver_close_command=0;

/* if we need to show fps */
void networkserver_display_fps(){
   display_fps=1;
}

void networkserver_close(){

   printf("driver networkserver off\n");
}

int networkserver_parseconf(char *configfile){

   int end_parse=0; int end_section=0; int driver_config_parsed=0;
   FILE *myfile;
   const int limit = 256;

   if ((myfile=fopen(configfile,"r"))==NULL){
      printf("networkserver: cannot find config file\n");
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
            printf ("networkserver: line too long in config file!\n");
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
                              printf("networkserver: error in config file.\n'end_section' keyword required before starting new driver section.\n");
                              end_section=1; end_parse=1;

                           }else if(strcmp(word3,"serves")==0){
                              while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
                              if(sscanf(buffer_file2,"%s %s %s",word3,word4,word5)>2){
		      
                                 if((strcmp(word4,"colorA")==0)&&(serve_device[COLORA_DEVICE]==0)){
                                    serve_device[COLORA_DEVICE]=1;
                                    device_network_id[COLORA_DEVICE]=atoi(word5);
                                 }
                                 else if((strcmp(word4,"colorB")==0)&&(serve_device[COLORB_DEVICE]==0)){
                                    serve_device[COLORB_DEVICE]=1;
                                    device_network_id[COLORB_DEVICE]=atoi(word5);
                                 }
                                 else if((strcmp(word4,"colorC")==0)&&(serve_device[COLORC_DEVICE]==0)){
                                    serve_device[COLORC_DEVICE]=1;
                                    device_network_id[COLORC_DEVICE]=atoi(word5);
                                 }
                                 else if((strcmp(word4,"colorD")==0)&&(serve_device[COLORD_DEVICE]==0)){
                                    serve_device[COLORD_DEVICE]=1;
                                    device_network_id[COLORD_DEVICE]=atoi(word5);
                                 }
                                 else{
                                    printf("networkserver: serves line incorrect\n");
                                 }
                              }
                              else if(sscanf(buffer_file2,"%s %s",word3,word4)>1){
		      
                                 if((strcmp(word4,"laser")==0)&&(serve_device[LASER_DEVICE]==0)){serve_device[LASER_DEVICE]=1;}
                                 else if((strcmp(word4,"sonars")==0)&&(serve_device[SONARS_DEVICE]==0)){serve_device[SONARS_DEVICE]=1;}
                                 else if((strcmp(word4,"encoders")==0)&&(serve_device[ENCODERS_DEVICE]==0)){serve_device[ENCODERS_DEVICE]=1;}
                                 else if((strcmp(word4,"motors")==0)&&(serve_device[MOTORS_DEVICE]==0)){serve_device[MOTORS_DEVICE]=1;}
                                 else if((strcmp(word4,"pantiltencoders")==0)&&(serve_device[PANTILT_ENCODERS_DEVICE]==0)){serve_device[PANTILT_ENCODERS_DEVICE]=1;}
                                 else if((strcmp(word4,"pantiltmotors")==0)&&(serve_device[PANTILT_MOTORS_DEVICE]==0)){serve_device[PANTILT_MOTORS_DEVICE]=1;}
                                 else{printf("networkserver: serves line incorrect\n");}
                              }

                           }else if(strcmp(word3,"socket")==0){
                              while((buffer_file2[z]!='\n')&&(buffer_file2[z]!=' ')&&(buffer_file2[z]!='\0')&&(buffer_file2[z]!='\t')) z++;
                              if(sscanf(buffer_file2,"%s %s",word3,word4)>1){
                                 networkserver_port=atoi(word4);
                                 printf("networkserver: server will be started using port %d\n",networkserver_port);
                              }else{
                                 printf("networkserver: socket line incorrect\n");
                              }
                           }else printf("networkserver: i don't know what to do with '%s'\n",buffer_file2);
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
      if((serve_device[COLORA_DEVICE]==0)&&(serve_device[COLORB_DEVICE]==0)&&(serve_device[COLORC_DEVICE]==0)&&(serve_device[COLORD_DEVICE]==0)){
         if((serve_device[PANTILT_MOTORS_DEVICE]==0)&&(serve_device[PANTILT_ENCODERS_DEVICE]==0)){
            if((serve_device[MOTORS_DEVICE]==0)&&(serve_device[LASER_DEVICE]==0)&&(serve_device[ENCODERS_DEVICE]==0)&&(serve_device[SONARS_DEVICE]==0)){
               printf("networkserver: warning! no device served.\n");
            }
         }
      }
      return 0;
   }else return -1;
}

void init(){
   int i;
   for (i=0;i<MAXDEVICE; i++){
      /*Do myimport's*/
      if (serve_device[i]){
         switch (i){

            case COLORA_DEVICE:
               variables[i]=myimport ("colorA", "colorA");
               resume[i]=(resumeFn) myimport ("colorA", "resume");
               suspend[i]=(suspendFn) myimport ("colorA", "suspend");
               if (!variables[i]){
                  serve_device[i]=0;
                  fprintf (stderr, "I can't fetch 'colorA', it'll not be served\n");
               }
               else{
                  resume[i](-1, NULL, NULL);
               }
               break;
            case COLORB_DEVICE:
               variables[i]=myimport ("colorB", "colorB");
               resume[i]=(resumeFn) myimport ("colorB", "resume");
               suspend[i]=(suspendFn) myimport ("colorB", "suspend");
               if (!variables[i]){
                  serve_device[i]=0;
                  fprintf (stderr, "I can't fetch 'colorB', it'll not be served\n");
               }
               else{
                  resume[i](-1, NULL, NULL);
               }
               break;
            case COLORC_DEVICE:
               variables[i]=myimport ("colorC", "colorC");
               resume[i]=(resumeFn) myimport ("colorC", "resume");
               suspend[i]=(suspendFn) myimport ("colorC", "suspend");
               if (!variables[i]){
                  serve_device[i]=0;
                  fprintf (stderr, "I can't fetch 'colorC', it'll not be served\n");
               }
               else{
                  resume[i](-1, NULL, NULL);
               }
               break;
            case COLORD_DEVICE:
               variables[i]=myimport ("colorD", "colorD");
               resume[i]=(resumeFn) myimport ("colorD", "resume");
               suspend[i]=(suspendFn) myimport ("colorD", "suspend");
               if (!variables[i]){
                  serve_device[i]=0;
                  fprintf (stderr, "I can't fetch 'colorD', it'll not be served\n");
               }
               else{
                  resume[i](-1, NULL, NULL);
               }
               break;
            case LASER_DEVICE:
               variables[i]=myimport ("laser", "laser");
               resume[i]=(resumeFn) myimport("laser", "resume");
               suspend[i]=(suspendFn) myimport("laser", "suspend");
               if (!variables[i]){
                  serve_device[i]=0;
                  fprintf (stderr, "I can't fetch 'laser', it'll not be served\n");
               }
               else{
                  resume[i](-1, NULL, NULL);
               }
         }
      }
   }
}

void bootstrap(int *s, struct sockaddr_in *addr_servicio, unsigned short port) {
   *s = socket(AF_INET, SOCK_STREAM, 0);
   if (*s < 0) {
      perror("");
      jdeshutdown(0);
   }

   addr_servicio->sin_family = AF_INET;
   addr_servicio->sin_port = htons(port);
   addr_servicio->sin_addr.s_addr = INADDR_ANY;

   if (bind(*s, (struct sockaddr *) addr_servicio, sizeof(*addr_servicio)) < 0){
      perror("");
      jdeshutdown(0);
   }

   if (listen(*s, MAX_QUEUE) < 0) {
      perror("");
      jdeshutdown(0);
   }
}

void my_write(int fd, const void *buf, size_t count) { 
   int escritos = 0;
   int total_escritos = 0;
   do {
      escritos = write(fd, (char *) buf + total_escritos, count-total_escritos);
      if (escritos < 0) {
         if (errno != EAGAIN){
            perror("I can't write in client socket");
            pthread_exit(0);
         }
      }
      else{
         total_escritos += escritos;
      }
   } while (total_escritos < count);
}

void dispatch_petition(struct client *info, char *petition) {
   long int codigo_mensaje;
   char output_buffer[MAX_MESSAGE];

   if (info->name[0]=='\0'){
      strncpy(info->name, petition, 256);
   }
//    printf("dispatching %s\n", petition);
   if (sscanf(petition,"%d",(int *)&codigo_mensaje)==EOF){
      printf("No entiendo el mensaje (%s) del cliente (%s)\n",
             petition,info->name);
      return;
   }
   if (codigo_mensaje==NETWORKSERVER_rgb24bpp_sifntsc_image_query){
      /*El cliente pide una imagen*/
      int cam;
      char *image=NULL;

      if (sscanf(petition,"%ld %d",&codigo_mensaje,&cam)!=2){
         return;
      }
      /*Comprobar si el código de cámara está servido*/
      if (serve_device[COLORA_DEVICE]==1 && device_network_id[COLORA_DEVICE]==cam){
         image=*(char **)variables[COLORA_DEVICE];
      }
      else if (serve_device[COLORB_DEVICE]==1 && device_network_id[COLORB_DEVICE]==cam){
         image=*(char **)variables[COLORB_DEVICE];
      }
      else if (serve_device[COLORC_DEVICE]==1 && device_network_id[COLORC_DEVICE]==cam){
         image=*(char **)variables[COLORC_DEVICE];
      }
      else if (serve_device[COLORD_DEVICE]==1 && device_network_id[COLORD_DEVICE]==cam){
         image=*(char **)variables[COLORD_DEVICE];
      }

      if (image!=NULL)
      {
         char myimage[SIFNTSC_COLUMNS*SIFNTSC_ROWS*3];
         struct timeval t;
         unsigned long int now;
         
         gettimeofday(&t,NULL);
         now=t.tv_sec*1000000+t.tv_usec;
         /*Envía la hora de captura, como no lo sabemos se envía la hora actual*/

         sprintf(output_buffer,"%d %lu %d %d %d %d\n",
                 NETWORKSERVER_rgb24bpp_sifntsc_image, now, cam, SIFNTSC_COLUMNS,
                 SIFNTSC_ROWS,3);

         my_write(info->cs,output_buffer,strlen(output_buffer));
         /*Hago una copia local de la imagen para evitar posibles cambios en
         ella durante la escritura*/
         memcpy(myimage, image, SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
         my_write(info->cs,myimage, SIFNTSC_COLUMNS*SIFNTSC_ROWS*3);
      }
   }
   else if (codigo_mensaje==NETWORKSERVER_subscribe_laser){
      /*Subscripción al laser*/
      info->subscriptions[LASER_DEVICE]=1;
   }
   else if (codigo_mensaje==NETWORKSERVER_unsubscribe_laser){
      /*Subscripción al laser*/
      info->subscriptions[LASER_DEVICE]=0;
   }
   else if (codigo_mensaje==NETWORKSERVER_goodbye ||
            codigo_mensaje==NETWORKSERVER_goodbye_images)
   {
      close(info->cs);
      printf("Cierro cliente %s, por su mensaje de goodbye\n",info->name);
      pthread_exit(0);
   }
   else
      printf("Mensaje NO reconocido del cliente %s: (%s)\n", info->name,petition);
}

void dispatch_subscriptions(struct client * info) {
   int i, j;
   char buff[MAX_MESSAGE];

   info->clk++; /*Se incrementa el reloj que indica el momento del mensaje*/
   
   for (i = 0; i < MAXDEVICE; i++) {
      if (info->subscriptions[i]) {
         switch(i) {
            case LASER_DEVICE:
               /*Componer el mensaje*/
               sprintf(buff,"%d %lu",NETWORKSERVER_laser,info->clk);
               for (j=0;j<180; j++){
                  sprintf(buff+strlen(buff)," %d",(int)(((int *)variables[i])[j]));
               }
               sprintf(buff+strlen(buff),"\n");
               /*Envío del mensaje*/
               my_write(info->cs,buff,strlen(buff));
               break;
         }
      }
   }
}

void *client_thread(void *pcs) {
   char buf1[MAX_MESSAGE];
   char buf2[MAX_MESSAGE];
   char *buffer = buf1;
   char *buffer_tmp = buf2;
   char *buffer_aux;
   char *msg, *msg_end;
   int bytes_read;
   int nbytes = 0; // number of valid bytes in buffer
   int continuar;

   struct timeval a, b;
   long n=0; /* iteration */
   long next,bb,aa;

   struct client info;
   info.cs = *(int *)pcs;
   info.clk = 0;
   pthread_mutex_unlock(&socketmutex);
   memset(info.subscriptions, 0, MAXDEVICE);

   if ( fcntl(info.cs, F_SETFL, O_NONBLOCK) ) {
         pthread_exit(0);
   }

   /*Leer el nombre*/
   do{
      bytes_read = read(info.cs, buffer + nbytes, MAX_MESSAGE - nbytes);
      if (bytes_read < 0) {
         if (errno == EAGAIN) {
            continuar=1;
         }
         else{
            pthread_exit(0);
         }
      }
      else{
         continuar=0;
      }
   }while (continuar);
   
   strncpy (info.name, buffer, 256);
   printf ("Nuevo cliente: %s.\n", info.name);
   
   gettimeofday(&a, NULL);
   aa=a.tv_sec*1000000+a.tv_usec;
   n=0;

   for (;;) {
      n++;
      bytes_read = read(info.cs, buffer + nbytes, MAX_MESSAGE - nbytes);
      if (bytes_read < 0) {
         if (errno != EAGAIN) {
            pthread_exit(0);
         }
      }
      else if (bytes_read == 0) {
         pthread_exit(0);
      }
      else {
         nbytes += bytes_read;
      }

      if (nbytes > 0) {
         // Possible message/s to dispatch
         buffer[nbytes] = '\0';
         msg = buffer;
         msg_end = strstr(buffer, "\n");
         while (msg_end != NULL) { // For each message in buffer
            *msg_end = '\0';
            msg_end++; // msg_end now points to the folling messages
            nbytes -= msg_end - msg;/* the spare bytes in buffer for the
               following messages */
            dispatch_petition(&info, msg);
            /*printf (msg);*/
            msg = msg_end;
            msg_end = strstr(msg, "\n");
         }
         memcpy(buffer_tmp, msg, nbytes); /* save those bytes of the last
            message, which is incomplete */
         buffer_aux = buffer;
         buffer = buffer_tmp;
         buffer_tmp = buffer_aux;
      }

      dispatch_subscriptions(&info);

      gettimeofday(&b,NULL);
      bb=b.tv_sec*1000000+b.tv_usec;
      next=aa+(n+1)*(long)CLIENT_TH_CYCLE*1000-bb;
      if (next>5000) {
         usleep(next-5000);
         /* discounts 5ms taken by calling usleep itself, on average */
      }
   }
   pthread_exit(0);
}

void *listen_thread() {
   int s; /* FD socket escucha */
   int cs; /* FD socket despacho cliente */
   struct sockaddr_in addr_control, addr_remoto;
   socklen_t length_addr_remoto;
   pthread_t pid;

   bootstrap(&s, &addr_control, networkserver_port);
   for (;;) {
      memset(&addr_remoto, 0, sizeof(addr_remoto));
      length_addr_remoto = sizeof(addr_remoto);
      pthread_mutex_lock(&socketmutex);
      cs = accept(s, (struct sockaddr *) &addr_remoto, &length_addr_remoto);
      if (cs < 0) {
         perror("");
         jdeshutdown(0);
      }
      pthread_create(&pid,NULL,client_thread,(void*)&cs);
   }
}

void networkserver_startup(char *configfile)
{
   int i;

   /* reseting serve color array and setting default options */
   for(i=0;i<MAXDEVICE;i++){
      serve_device[i]=0;
      device_network_id[i]=0;
      variables[i]=NULL;
   }

   /* we call the function to parse the config file */
   if(networkserver_parseconf(configfile)==-1){
      printf("networkserver: cannot initiate driver. configfile parsing error.\n");
      exit(-1);
   }

   /*To import variables and resume schemas that export them*/
   init();
   pthread_create(&listen_pid,NULL,listen_thread,(void*)NULL);
   printf("networkserver driver started up\n");
}

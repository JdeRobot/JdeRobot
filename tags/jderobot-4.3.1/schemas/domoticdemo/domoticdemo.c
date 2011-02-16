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
 *  Authors : Sara Marugán Alonso <smarugan@gsyc.es>
 * 
 */

#include <jde.h>
#include <domoticdemo.h>
#include <graphics_gtk.h>
#include <interfaces/x10.h>
#include <time.h>
#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <interfaces/varcolor.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <cv.h>
#include <highgui.h>

#define DELETE_STACK_ITERS 60 /*ms*/

#define MSG_LEN 70

int domoticdemo_id=0;
int domoticdemo_brothers[MAX_SCHEMAS];
arbitration domoticdemo_callforarbitration;

/*Global variables*/

/* exported variables */
int domoticdemo_cycle=30; /* ms */

/* Imported symbols*/
X10Iface *x10=NULL;
X10Events *x10_events=NULL;
runFn x10_run=NULL;
stopFn x10_stop=NULL;

Varcolor *myvarA;
int mysoncolorA;
unsigned char **mycolorA=NULL;
int mycolorAheight;
int mycolorAwidth;
runFn mycolorArun;
stopFn mycolorAstop;

int init=0;
CvMat imgMyColorA;
IplImage *imgWorkingA = NULL;

int terminate_command=0;

/*GUI Variables*/
GladeXML *xml; /*Fichero xml*/
GtkWidget *win; /*Ventana*/
int context;
int n_msg=0;

registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Callbacks*/
gboolean on_delete_window (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gdk_threads_leave();
   domoticdemo_hide();
   gdk_threads_enter();
   return TRUE;
}

/*void captura_imagen(){
      	int j;
        time_t t;
	char name[MAX_BUFFER];

      	CvArr *p_img = &imgMyColorA;
	for (j = 0; j < mycolorAheight*mycolorAwidth*3; j++) {
		imgWorkingA->imageData[j] = imgMyColorA.data.ptr[j];
	}
	memset(name,0,sizeof(name));
	time(&t);
	strncpy(name,ctime(&t),strlen(ctime(&t))-1);
	strncat(name,".png",strlen(".png"));
	cvSaveImage(name,imgWorkingA);
}*/

void graba15segs_video(){
	CvVideoWriter *videofile;
        time_t t;
	char fifo[MAX_BUFFER];
	int iterations= cvRound(15./(domoticdemo_cycle/1000.));
	int i,j;
        CvArr *p_img;

	memset(fifo,0,sizeof(fifo));
	time(&t);
	strncpy(fifo,ctime(&t),strlen(ctime(&t))-1);
	strncat(fifo,".avi",strlen(".avi"));
	videofile=cvCreateVideoWriter("/home/salons/jderobot-4.3.0-estable/base/prueba.avi",CV_FOURCC('P', 'I', 'M', '1'),30,cvSize(mycolorAwidth,mycolorAheight),1);
	printf("#domoticdemo: creado avi\n");

	for(i=0; i<iterations; i++){
		p_img = &imgMyColorA;
		for (j = 0; j < mycolorAheight*mycolorAwidth*3; j++) {
			imgWorkingA->imageData[j] = imgMyColorA.data.ptr[j];
		}
		cvWriteFrame(videofile,imgWorkingA);
		cvWaitKey(20);
	}
	cvReleaseVideoWriter(&videofile);
}

void recorder(){
   static int prev_state=0;
   static int fifo_fd;
   char name[MAX_BUFFER];
   int i,iterations= cvRound(5./(domoticdemo_cycle/1000.));
   time_t t; 
   unsigned long int last;

	memset(name,0,sizeof(name));
	time(&t);
	strncpy(name,ctime(&t),strlen(ctime(&t))-1);
	strncat(name,".avi",strlen(".avi"));
  
      /*Comienza una nueva grabación*/
      /*Se crea un fifo y se lanza mplayer*/
      unlink ("fifovid");
      if ( (mkfifo ("fifovid", 0600) != 0) ){
         fprintf (stderr, "imposible crear el fifo\n");
         exit (1);
      }
      if (fork() == 0) {/* We create a new process...
         // ... close its stdin, stdout & stderr ...*/
         char str[50];
         int file;
         file = open("/dev/null",O_RDWR);
         close(0); dup(file);
         close(1); dup(file);
         close(2); dup(file);

         sprintf(str,"fps=%.1f:w=%d:h=%d:format=%s",7.5,
                 mycolorAwidth,mycolorAheight, "bgr24");
         execlp("mencoder","mencoder","fifovid","-demuxer","rawvideo", "-rawvideo",
                str, "-o", name, "-ovc", "copy" ,NULL);
         printf("Error executing mencoder\n");
         exit(1);
      }
      else{
	      if ((fifo_fd=open("fifovid", O_WRONLY))<0){
		 fprintf (stderr, "Error al abrir el fifo\n");
		 exit(1);
	      }

              last=-1;
    	      i=0;
	      while(i<iterations){
		      /*Está grabando, se escribe en el fifo la imagen actual colorA*/
		      if((unsigned long int)(*myvarA).clock!=last){
			      if (write (fifo_fd, (*mycolorA), mycolorAwidth*mycolorAheight*3)>mycolorAwidth*mycolorAheight*3){
				 fprintf (stderr, "Error al Escribir en el fifo\n");
				 exit(1);
			      }
			      last=(unsigned long int)(*myvarA).clock;
			      //printf("# clock %d\n",(unsigned long int)(*myvarA).clock);
			      i++;
		      }
	      }

	      /*Termina la grabación se cierra el fifo y morirá con ello mencoder*/
	      close (fifo_fd);
	      unlink("fifovid");
	      wait (NULL); /*Esperar la muerte de mencoder*/
      }
}

void domoticdemo_iteration(){
   static int k=0;
   static unsigned int clock;
   int s;

   speedcounter(domoticdemo_id);

   if(k==0){
	sleep(1);
	if ((s=x10->start_monitor())==-1){
		perror("Domoticdemo: Error launching monitor");
		jdeshutdown(-1);
        }
	else
		printf("Domoticdemo: monitor started\n");

	CvSize frameSizeA = cvSize(mycolorAwidth,mycolorAheight);
        imgMyColorA = cvMat(mycolorAheight, mycolorAwidth, CV_8UC3, *mycolorA);
	imgWorkingA = cvCreateImage(frameSizeA, IPL_DEPTH_8U, 3);
	clock=0;
	k++;
   }
   else{
	if(terminate_command==0){
		if(clock!=*(x10_events->clock)){
			//printf("#domoticdemo: nuevo evento -> %s está %d\n",x10_events->unit,*(x10_events->status));
			if(*(x10_events->status)==1){
				system("mplayer warning.wav > /dev/null 2>&1");
				system("mplayer warning.wav > /dev/null 2>&1");
				//captura_imagen();
				recorder();
	 		}
			clock=*(x10_events->clock);
		}
	}
   }
}

/*Importar símbolos*/
void domoticdemo_imports(){ 
   int *aux = NULL; 
   int id;

   x10=(X10Iface *)myimport ("x10","x10Iface");
   x10_events=(X10Events *)myimport("x10","x10Events");
   if(x10_events==NULL){
	fprintf(stderr, "domoticdemo ERROR: I can't import symbol x10Events\n");
   }  
   x10_run=(runFn)myimport ("x10","run");
   x10_stop=(stopFn)myimport ("x10","stop");

   if (!x10 || !x10_run || !x10_stop){
      fprintf(stderr, "domoticdemo ERROR: I can't import symbols from x10 driver\n");
      jdeshutdown(-1);
   }

   aux = (int *)myimport("varcolorA","id");
   if (aux) {
      mysoncolorA=*aux;
      mycolorArun=(runFn)myimport("varcolorA", "run");
      mycolorAstop=(stopFn *)myimport("varcolorA","stop");
      myvarA=(Varcolor *)myimport("varcolorA","varcolorA");
      mycolorA=(char*)&((*myvarA).img);
      mycolorAheight = (int)((*myvarA).height);
      mycolorAwidth = (int)((*myvarA).width);
   }
   if ( !(aux && mycolorA && mycolorAheight
       && mycolorAwidth) )
   {
      fprintf(stderr, "tracker2D: not enough variables from varcolorA");
      jdeshutdown(1);
   }
}

/*Exportar símbolos*/
void domoticdemo_exports(){

   myexport("domoticdemo", "id", &domoticdemo_id);
   myexport("domoticdemo","cycle",&domoticdemo_cycle);
   myexport("domoticdemo","run",(void *)domoticdemo_run);
   myexport("domoticdemo","stop",(void *)domoticdemo_stop);
}

/*Las inicializaciones van en esta parte*/
void domoticdemo_guiinit(){
   if (myregister_displaycallback==NULL){
      if ((myregister_displaycallback=
           (registerdisplay)myimport ("graphics_gtk",
            "register_displaycallback"))==NULL)
      {
         printf ("I can't fetch register_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=
           (deletedisplay)myimport ("graphics_gtk", "delete_displaycallback"))
           ==NULL)
      {
         printf ("I can't fetch delete_displaycallback from graphics_gtk\n");
         jdeshutdown(1);
      }
   }
}

void domoticdemo_terminate(){
	terminate_command=1;
	x10->stop_monitor();
}

void domoticdemo_stop()
{
  pthread_mutex_lock(&(all[domoticdemo_id].mymutex));
  put_state(domoticdemo_id,slept);
  printf("domoticdemo: off\n");
  pthread_mutex_unlock(&(all[domoticdemo_id].mymutex));
  x10_stop();
  init=0;
}


void domoticdemo_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[domoticdemo_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[domoticdemo_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[domoticdemo_id].children[i]=FALSE;
  all[domoticdemo_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) domoticdemo_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {domoticdemo_brothers[i]=brothers[i];i++;}
    }
  domoticdemo_callforarbitration=fn;
  put_state(domoticdemo_id,notready);
  printf("domoticdemo: on\n");
  pthread_cond_signal(&(all[domoticdemo_id].condition));
  pthread_mutex_unlock(&(all[domoticdemo_id].mymutex));
  domoticdemo_imports();
  x10_run(domoticdemo_id, NULL, NULL);
  mycolorArun(domoticdemo_id, NULL, NULL);
}

void *domoticdemo_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[domoticdemo_id].mymutex));

      if (all[domoticdemo_id].state==slept)
      {
	 pthread_cond_wait(&(all[domoticdemo_id].condition),&(all[domoticdemo_id].mymutex));
	 pthread_mutex_unlock(&(all[domoticdemo_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[domoticdemo_id].state==notready)
	    put_state(domoticdemo_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[domoticdemo_id].state==ready)
	 {put_state(domoticdemo_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[domoticdemo_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[domoticdemo_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    domoticdemo_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)domoticdemo_cycle*1000-bb;

	    if (next>5000)
	    {
	       usleep(next-5000);
	       /* discounts 5ms taken by calling usleep itself, on average */
	    }
	    else  ;
	 }
	 else
	    /* just let this iteration go away. overhead time negligible */
	 {
	    pthread_mutex_unlock(&(all[domoticdemo_id].mymutex));
	    usleep(domoticdemo_cycle*1000);
	 }
      }
   }
}

void domoticdemo_init(char *configfile)
{
  pthread_mutex_lock(&(all[domoticdemo_id].mymutex));
  printf("domoticdemo schema started up\n");
  domoticdemo_exports();
  put_state(domoticdemo_id,slept);
  pthread_create(&(all[domoticdemo_id].mythread),NULL,domoticdemo_thread,NULL);
  pthread_mutex_unlock(&(all[domoticdemo_id].mymutex));
  domoticdemo_guiinit();
}

void domoticdemo_guidisplay(){
   static int old_n_msg=0;
   static int iters=0;
   int i;
   gdk_threads_enter();
   if (n_msg!=0){
      if (old_n_msg==n_msg){
         iters++;
         if (iters>DELETE_STACK_ITERS){
            for (i=0; i<n_msg; i++){
               gtk_statusbar_pop((GtkStatusbar *)glade_xml_get_widget(xml, "status"),
                                  context);
            }
            iters=0;
            n_msg=0;
         }
      }
      else{
         iters=0;
      }
   }
   else{
      iters=0;
   }
   old_n_msg=n_msg;
   gdk_threads_leave();
}

void domoticdemo_hide(void){
   mydelete_displaycallback(domoticdemo_guidisplay);
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   all[domoticdemo_id].guistate=pending_off;
}

void domoticdemo_show(void){
   static int cargado=0;
   static pthread_mutex_t domoticdemo_gui_mutex;

   pthread_mutex_lock(&domoticdemo_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&domoticdemo_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("domotic_demo.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window1");
      /*Conectar los callbacks*/
      {
         /*g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "property")), "changed",
                          G_CALLBACK(on_property_changed), NULL);
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "apply")), "clicked",
                          G_CALLBACK(on_apply_clicked), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "monitor")), "clicked",
                          G_CALLBACK(on_monitor_clicked), NULL);*/
      }
      
      gtk_widget_hide((GtkWidget *)(glade_xml_get_widget(xml, "level_label")));
      gtk_widget_hide((GtkWidget *)(glade_xml_get_widget(xml, "level")));

      context=gtk_statusbar_get_context_id ((GtkStatusbar *)
            glade_xml_get_widget(xml, "status"),
                                 "main context");
      gtk_statusbar_push((GtkStatusbar *)glade_xml_get_widget(xml, "status"),
                          context,
                          "Please select an action");
      
      if (win==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      else{
         gtk_widget_show(win);
         gtk_widget_queue_draw(GTK_WIDGET(win));
      }
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&domoticdemo_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(domoticdemo_guidisplay);
   all[domoticdemo_id].guistate=pending_on;
}


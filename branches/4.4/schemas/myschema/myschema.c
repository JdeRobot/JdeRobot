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

#include "jde.h"
#include "forms.h"
#include "graphics_xforms.h"
#include "myschemagui.h"
#include "myschema.h"

/*Gui callbacks*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;

int myschema_id=0; 
int myschema_brothers[MAX_SCHEMAS];
arbitration myschema_callforarbitration;

enum myschema_states {init,t1,r1,t2,r2,t3,r3,t4,end};
static int myschema_state;
FD_myschemagui *fd_myschemagui=NULL;

/* exported variables */
int myschema_cycle=100; /* ms */
int valor=0;

void myschema_iteration(){  
  static int d=0;
  speedcounter(myschema_id);
  if (d==15)
  {
     valor=15;
  }
  else if (d==25)
  {
     valor=25;
  }
  else if (d==80)
  {
     valor=100;
  }
  d++;
}


/*Importar símbolos*/
void myschema_imports(){

}

/*Exportar símbolos*/
void myschema_exports(){

   myexport("myschema","cycle",&myschema_cycle);
   myexport("myschema","run",(void *)myschema_run);
   myexport("myschema","stop",(void *)myschema_stop);
   myexport("myschema", "valor", &valor);
}

/*Las inicializaciones van en esta parte*/
void myschema_guiinit(){
   if (myregister_buttonscallback==NULL){
      if ((myregister_buttonscallback=(registerbuttons)myimport ("graphics_xforms", "register_buttonscallback"))==NULL){
         printf ("I can't fetch register_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_buttonscallback=(deletebuttons)myimport ("graphics_xforms", "delete_buttonscallback"))==NULL){
         printf ("I can't fetch delete_buttonscallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((myregister_displaycallback=(registerdisplay)myimport ("graphics_xforms", "register_displaycallback"))==NULL){
         printf ("I can't fetch register_displaycallback from graphics_xforms\n");
         jdeshutdown(1);
      }
      if ((mydelete_displaycallback=(deletedisplay)myimport ("graphics_xforms", "delete_displaycallback"))==NULL){
         jdeshutdown(1);
         printf ("I can't fetch delete_displaycallback from graphics_xforms\n");
      }
   }
}

void myschema_terminate(){
 if (fd_myschemagui!=NULL)
    {
      if (all[myschema_id].guistate==on) 
	fl_hide_form(fd_myschemagui->myschemagui);
      fl_free_form(fd_myschemagui->myschemagui);
    }
  printf ("myschema terminated\n");
}


void myschema_stop()
{
  /* printf("myschema: cojo-stop\n");*/
  pthread_mutex_lock(&(all[myschema_id].mymutex));
  put_state(myschema_id,slept);
  printf("myschema: off\n");
  pthread_mutex_unlock(&(all[myschema_id].mymutex));
  /*  printf("myschema: suelto-stop\n");*/
}


void myschema_run(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[myschema_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[myschema_id].mymutex));
  /* this schema runs its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[myschema_id].children[i]=FALSE;
  all[myschema_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) myschema_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {myschema_brothers[i]=brothers[i];i++;}
    }
  myschema_callforarbitration=fn;
  put_state(myschema_id,notready);
  printf("myschema: on\n");
  pthread_cond_signal(&(all[myschema_id].condition));
  pthread_mutex_unlock(&(all[myschema_id].mymutex));
  myschema_imports();
}

void *myschema_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[myschema_id].mymutex));

      if (all[myschema_id].state==slept)
      {
	 myschema_state=init;
	 pthread_cond_wait(&(all[myschema_id].condition),&(all[myschema_id].mymutex));
	 pthread_mutex_unlock(&(all[myschema_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[myschema_id].state==notready)
	    put_state(myschema_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[myschema_id].state==ready)
	 {put_state(myschema_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[myschema_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[myschema_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    myschema_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)myschema_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[myschema_id].mymutex));
	    usleep(myschema_cycle*1000);
	 }
      }
   }
}

void myschema_init(char *configfile)
{
  pthread_mutex_lock(&(all[myschema_id].mymutex));
  printf("myschema schema started up\n");
  myschema_exports();
  put_state(myschema_id,slept);
  pthread_create(&(all[myschema_id].mythread),NULL,myschema_thread,NULL);
  pthread_mutex_unlock(&(all[myschema_id].mymutex));
  myschema_guiinit();
}

void myschema_guibuttons(void *obj){
}

void myschema_guidisplay(){
  char text[80]="";
  static float k=0;
  k=k+1.;
  sprintf(text,"it %.1f",k);
  fl_set_object_label(fd_myschemagui->fps,text);
}


void myschema_hide_aux(void){
  mydelete_buttonscallback(myschema_guibuttons);
  mydelete_displaycallback(myschema_guidisplay);
  fl_hide_form(fd_myschemagui->myschemagui);
}

void myschema_hide(){
   callback fn;
   if ((fn=(callback)myimport ("graphics_xforms", "gui_callback"))!=NULL){
      fn ((gui_function)myschema_hide_aux);
   }
}

void myschema_show_aux(void){
  static int k=0;

  if (k==0) /* not initialized */
    {
      k++;
      fd_myschemagui = create_form_myschemagui();
      fl_set_form_position(fd_myschemagui->myschemagui,400,50);
    }
  myregister_buttonscallback(myschema_guibuttons);
  myregister_displaycallback(myschema_guidisplay);
  fl_show_form(fd_myschemagui->myschemagui,FL_PLACE_POSITION,FL_FULLBORDER,"myschema");
}

void myschema_show(){
   callback fn;
   if ((fn=(callback)myimport ("graphics_xforms", "gui_callback"))!=NULL){
      fn ((gui_function)myschema_show_aux);
   }
}

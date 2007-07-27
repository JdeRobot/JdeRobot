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
#include "jdegui.h"
#include "papitogui.h"
#include "papito.h"

int papito_id=0; 
int papito_brothers[MAX_SCHEMAS];
arbitration papito_callforarbitration;
int papito_cycle=100; /* ms */

enum papito_states {init,hijos,end};
enum hijo_states {ON, OFF};
static int papito_state;
FD_papitogui *fd_papitogui;
static int hijo_state=OFF;
int d=0;

/*Imported variables*/
resumeFn hijoresume;
suspendFn hijosuspend;
int* hijo_variable=NULL;

void papito_iteration()
{  
  d++;
  speedcounter(papito_id);
  /* printf("papito iteration %d\n",d); */
  
  if (d==100)
    {
      papito_state=hijos;
      if (hijoresume!=NULL){
	hijoresume(papito_id,NULL,null_arbitration);
	hijo_state=ON;
      }
    }
  else if (d==200)
    {
      papito_state=init;
      if (hijosuspend!=NULL){
	hijosuspend();
	hijo_state=OFF;
      }
    }
  else if (d>100 && d<200 && hijo_variable!=NULL)
    {
      /*En el intervalo de tiempo que el hijo está lanzado se utilizan
       sus variables*/
      printf ("Variable del hijo: %d\n", *hijo_variable);
    }
}

/*Exporta variables en esta función*/
void papito_exports(){

   /*Exporta*/
   myexport("papito","cycle",&papito_cycle);
   myexport("papito","resume",(void *)papito_resume);
   myexport("papito","papito_cycle",&papito_cycle);

}

/*Importar símbolos*/
void papito_imports(){
   hijoresume=(resumeFn)myimport("myschema","resume");
   if (hijoresume==NULL) printf("papito: Warning myschema_resume symbol not resolved\n");
   hijosuspend=(suspendFn)myimport("myschema","suspend");
   if (hijosuspend==NULL) printf("papito: Warning myschema_suspend symbol not resolved\n");
   hijo_variable=(int *)myimport("myschema","valor");
   if (hijo_variable==NULL) printf("papito: Warning myschema_valor symbol not resolved\n");
}

/*Las inicializaciones van en esta parte*/
void papito_init(){
}

/*Al suspender el esquema*/
void papito_fin(){
   if (hijo_state==ON)
      hijosuspend();
}

void papito_suspend()
{
  /* printf("papito: cojo-suspend\n");*/
  pthread_mutex_lock(&(all[papito_id].mymutex));
  put_state(papito_id,slept);
  printf("papito: off\n");
  pthread_mutex_unlock(&(all[papito_id].mymutex));
  /*  printf("papito: suelto-suspend\n");*/
  papito_fin();
}


void papito_resume(int father, int *brothers, arbitration fn)
{
  int i;
  
  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[papito_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[papito_id].mymutex));
   /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[papito_id].children[i]=FALSE;
  all[papito_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) papito_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {papito_brothers[i]=brothers[i];i++;}
    }
  papito_callforarbitration=fn;
  put_state(papito_id,notready);
  printf("papito: on\n");
  pthread_cond_signal(&(all[papito_id].condition));
  pthread_mutex_unlock(&(all[papito_id].mymutex));
  papito_imports();
}

void *papito_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[papito_id].mymutex));

      if (all[papito_id].state==slept)
      {
	 papito_state=init;
	 pthread_cond_wait(&(all[papito_id].condition),&(all[papito_id].mymutex));
	 pthread_mutex_unlock(&(all[papito_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[papito_id].state==notready)
	    put_state(papito_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[papito_id].state==ready)
	 {put_state(papito_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[papito_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[papito_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    papito_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)papito_cycle*1000-bb;

	    /* diff = (b.tv_sec*1000000+b.tv_usec)-(a.tv_sec*1000000+a.tv_usec);*/
	    /* next = (long)papito_cycle*1000-diff-3; */
	      
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
	    pthread_mutex_unlock(&(all[papito_id].mymutex));
	    usleep(papito_cycle*1000);
	 }
      }
   }
}

void papito_startup()
{
  pthread_mutex_lock(&(all[papito_id].mymutex));
  printf("papito schema started up\n");
  papito_exports();
  put_state(papito_id,slept);
  pthread_create(&(all[papito_id].mythread),NULL,papito_thread,NULL);
  pthread_mutex_unlock(&(all[papito_id].mymutex));
  papito_init();
}

void papito_guibuttons(FL_OBJECT *obj)
{
}

void papito_guidisplay()
{
  char text[80]="";
  static float k=0;
  k=k+1.;
  sprintf(text,"%d it",d);
  fl_set_object_label(fd_papitogui->fps,text);
}


void papito_guisuspend(void)
{
  delete_buttonscallback(papito_guibuttons);
  delete_displaycallback(papito_guidisplay);
  fl_hide_form(fd_papitogui->papitogui);
}

void papito_guiresume(void)
{
  static int k=0;

  if (k==0) /* not initialized */
    {
      k++;
      fd_papitogui = create_form_papitogui();
      fl_set_form_position(fd_papitogui->papitogui,400,50);
    }
  register_buttonscallback(papito_guibuttons);
  register_displaycallback(papito_guidisplay);
  fl_show_form(fd_papitogui->papitogui,FL_PLACE_POSITION,FL_FULLBORDER,"papito");
}

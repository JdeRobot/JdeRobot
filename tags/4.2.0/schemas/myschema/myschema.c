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
#include "myschemagui.h"
/*#include "perceptive1.h"*/

int myschema_id=0; 
int myschema_brothers[MAX_SCHEMAS];
arbitration myschema_callforarbitration;

enum myschema_states {init,t1,r1,t2,r2,t3,r3,t4,end};
static int myschema_state;
FD_myschemagui *fd_myschemagui;

/* exported variables */
int myschema_cycle=100; /* ms */

/*imported variables */
int *myc=NULL;

void myschema_iteration()
{  
  static int d;
  speedcounter(myschema_id);
  /*  printf("myschema iteration %d: a=%d c=%d\n",d++,a,c);*/
  /*
  if (myc==NULL) printf("myschema: c=NULL\n");
  else printf("myschema: c=%d\n",*myc);
  */
}


void myschema_suspend()
{
  /* printf("myschema: cojo-suspend\n");*/
  pthread_mutex_lock(&(all[myschema_id].mymutex));
  put_state(myschema_id,slept);
  printf("myschema: off\n");
  pthread_mutex_unlock(&(all[myschema_id].mymutex));
  /*  printf("myschema: suelto-suspend\n");*/
}


void myschema_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[myschema_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[myschema_id].mymutex));
  /* this schema resumes its execution with no children at all */
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
}

void *myschema_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      /* printf("myschema: iteration-cojo\n");*/
      pthread_mutex_lock(&(all[myschema_id].mymutex));

      if (all[myschema_id].state==slept) 
	{
	  /*printf("myschema: off\n");*/
	  v=0; w=0;
	  /*  printf("myschema: suelto para dormirme en condicional\n");*/
	  pthread_cond_wait(&(all[myschema_id].condition),&(all[myschema_id].mymutex));
	  /*  printf("myschema: cojo tras dormirme en condicional\n");*/
	  /*printf("myschema: on\n");*/
	  myschema_state=init; 
	  /* esto es para la aplicación, no tiene que ver con infraestrucura */
	  pthread_mutex_unlock(&(all[myschema_id].mymutex));
	  /* printf("myschema: iteration-suelto1\n");*/
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[myschema_id].state==notready) put_state(myschema_id,ready);
	  else all[myschema_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[myschema_id].state==ready) put_state(myschema_id,winner);


	  if (all[myschema_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[myschema_id].mymutex));
	      /*printf("myschema: iteration-suelto2\n");*/

	      gettimeofday(&a,NULL);
	      myschema_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = myschema_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(myschema_cycle*1000-diff);
	      else 
		{printf("time interval violated: myschema\n"); usleep(myschema_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[myschema_id].mymutex));
	      /*printf("myschema: iteration-suelto3\n");*/
	      usleep(myschema_cycle*1000);
	    }
	}
    }
}

void myschema_startup()
{
  pthread_mutex_lock(&(all[myschema_id].mymutex));
  printf("myschema schema started up\n");
  myexport("myschema","myschema_cycle",&myschema_cycle);
  myexport("myschema","myschema_resume",(void *)myschema_resume);
  myexport("myschema","myschema_suspend",(void *)myschema_suspend);
  myc=(int *)myimport("myperceptive","c");
  if (myc==NULL) printf("myschema: Warning c symbol not resolved\n");
  put_state(myschema_id,slept);
  pthread_create(&(all[myschema_id].mythread),NULL,myschema_thread,NULL);
  pthread_mutex_unlock(&(all[myschema_id].mymutex));
}

void myschema_guibuttons(FL_OBJECT *obj)
{
  /*  printf("schema: guibuttons\n");*/
}

void myschema_guidisplay()
{
  char text[80]="";
  static float k=0;
  k=k+1.;
  sprintf(text,"it %.1f",k);
  fl_set_object_label(fd_myschemagui->fps,text);
  /* printf("schema: guidisplay\n");*/
}


void myschema_guisuspend(void)
{
  delete_buttonscallback(myschema_guibuttons);
  delete_displaycallback(myschema_guidisplay);
  fl_hide_form(fd_myschemagui->myschemagui);
}

void myschema_guiresume(void)
{
  static int k=0;

  if (k==0) /* not initialized */
    {
      k++;
      fd_myschemagui = create_form_myschemagui();
      fl_set_form_position(fd_myschemagui->myschemagui,400,50);
    }
  register_buttonscallback(myschema_guibuttons);
  register_displaycallback(myschema_guidisplay);
  fl_show_form(fd_myschemagui->myschemagui,FL_PLACE_POSITION,FL_FULLBORDER,"myschema");
}

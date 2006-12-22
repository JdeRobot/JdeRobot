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

int papito_id=0; 
int papito_brothers[MAX_SCHEMAS];
arbitration papito_callforarbitration;
int papito_cycle=100; /* ms */

enum papito_states {init,hijos,end};
static int papito_state;
FD_papitogui *fd_papitogui;
int d;

resumeFn hijoresume;
suspendFn hijosuspend;

void papito_iteration()
{  
  all[papito_id].k++;
  d++;
  /* printf("papito iteration %d\n",d); */
  
  if (d==100)
    {
      papito_state=hijos;
      if (hijoresume!=NULL)
	hijoresume(papito_id,NULL,null_arbitration);
    }
  else if (d==200)
    {
      papito_state=init;
      if (hijosuspend!=NULL)
	hijosuspend();
    }
}


void papito_suspend()
{
  /* printf("papito: cojo-suspend\n");*/
  pthread_mutex_lock(&(all[papito_id].mymutex));
  put_state(papito_id,slept);
  printf("papito: off\n");
  pthread_mutex_unlock(&(all[papito_id].mymutex));
  /*  printf("papito: suelto-suspend\n");*/
}


void papito_resume(int father, int *brothers, arbitration fn)
{
  int i;
  
  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
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
}

void *papito_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      /* printf("papito: iteration-cojo\n");*/
      pthread_mutex_lock(&(all[papito_id].mymutex));

      if (all[papito_id].state==slept) 
	{
	  /*printf("papito: off\n");*/
	  v=0; w=0;
	  /*  printf("papito: suelto para dormirme en condicional\n");*/
	  pthread_cond_wait(&(all[papito_id].condition),&(all[papito_id].mymutex));
	  /*  printf("papito: cojo tras dormirme en condicional\n");*/
	  /*printf("papito: on\n");*/
	  papito_state=init; 
	  /* esto es para la aplicación, no tiene que ver con infraestrucura */
	  pthread_mutex_unlock(&(all[papito_id].mymutex));
	  /* printf("papito: iteration-suelto1\n");*/
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[papito_id].state==notready) put_state(papito_id,ready);
	  else all[papito_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[papito_id].state==ready) put_state(papito_id,winner);


	  if (all[papito_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[papito_id].mymutex));
	      /*printf("papito: iteration-suelto2\n");*/

	      gettimeofday(&a,NULL);
	      papito_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = papito_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(papito_cycle*1000-diff);
	      else 
		{printf("time interval violated: papito\n"); usleep(papito_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[papito_id].mymutex));
	      /*printf("papito: iteration-suelto3\n");*/
	      usleep(papito_cycle*1000);
	    }
	}
    }
}

void papito_startup()
{
  pthread_mutex_lock(&(all[papito_id].mymutex));
  printf("papito schema started up\n");
  myexport("papito","papito_cycle",&papito_cycle);
  hijoresume=(resumeFn)myimport("myschema","myschema_resume");
  if (hijoresume==NULL) printf("papito: Warning myschema_resume symbol not resolved\n");
  hijosuspend=(suspendFn)myimport("myschema","myschema_suspend");
  if (hijosuspend==NULL) printf("papito: Warning myschema_suspend symbol not resolved\n");
  put_state(papito_id,slept);
  pthread_create(&(all[papito_id].mythread),NULL,papito_thread,NULL);
  pthread_mutex_unlock(&(all[papito_id].mymutex));
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

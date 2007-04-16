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

int speedy_id=0; 
int speedy_brothers[MAX_SCHEMAS];
arbitration speedy_callforarbitration;

/* exported variables */
int speedy_cycle=1; /* ms */


void speedy_iteration()
{  
  /*
  static int d;
  printf("speedy iteration %d\n",d++);
  */
  speedcounter(speedy_id);  
  /* empty iteration, the fastest one */
  /*usleep(50000);*/
}


void speedy_suspend()
{
  pthread_mutex_lock(&(all[speedy_id].mymutex));
  put_state(speedy_id,slept);
  /*  printf("speedy: off\n");*/
  pthread_mutex_unlock(&(all[speedy_id].mymutex));
}


void speedy_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[speedy_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[speedy_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[speedy_id].children[i]=FALSE;
  all[speedy_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) speedy_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {speedy_brothers[i]=brothers[i];i++;}
    }
  speedy_callforarbitration=fn;
  put_state(speedy_id,notready);
  /* printf("speedy: on\n");*/
  pthread_cond_signal(&(all[speedy_id].condition));
  pthread_mutex_unlock(&(all[speedy_id].mymutex));
}

void *speedy_thread(void *not_used) 
{
  struct timeval a,b,c;
  long n=0; /* iteration */
  struct timespec d;
  long diff,next,bb,aa;

  for(;;)
    {
      pthread_mutex_lock(&(all[speedy_id].mymutex));

      if (all[speedy_id].state==slept) 
	{
	  pthread_cond_wait(&(all[speedy_id].condition),&(all[speedy_id].mymutex));
	  pthread_mutex_unlock(&(all[speedy_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[speedy_id].state==notready) 
	    put_state(speedy_id,ready);
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[speedy_id].state==ready) 
	    {put_state(speedy_id,winner);
	    gettimeofday(&a,NULL);
	    aa=a.tv_sec*1000000+a.tv_usec;
	    n=0; 
	    }

	  if (all[speedy_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[speedy_id].mymutex));
	      /*      gettimeofday(&a,NULL);*/
	      n++;
	      speedy_iteration();
	      gettimeofday(&b,NULL);
	      bb=b.tv_sec*1000000+b.tv_usec;
	      next=aa+(n+1)*(long)speedy_cycle*1000-bb;

	      /* diff = (b.tv_sec*1000000+b.tv_usec)-(a.tv_sec*1000000+a.tv_usec);*/
	      /* next = (long)speedy_cycle*1000-diff-3; */
	      
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
	      pthread_mutex_unlock(&(all[speedy_id].mymutex));
	      usleep(speedy_cycle*1000);
	    }
	}
    }
}

void speedy_startup()
{
  pthread_mutex_lock(&(all[speedy_id].mymutex));
  printf("speedy schema started up\n");
  myexport("speedy","id",&speedy_id);
  myexport("speedy","cycle",&speedy_cycle);
  myexport("speedy","resume",(void *)speedy_resume);
  myexport("speedy","suspend",(void *)speedy_suspend);
  put_state(speedy_id,slept);
  pthread_create(&(all[speedy_id].mythread),NULL,speedy_thread,NULL);
  pthread_mutex_unlock(&(all[speedy_id].mymutex));
}



void speedy_guisuspend(void)
{
}

void speedy_guiresume(void)
{
 
}

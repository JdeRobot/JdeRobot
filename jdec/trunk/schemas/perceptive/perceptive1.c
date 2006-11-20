#include <jde.h>
#include <jdegui.h>
#include "perceptive1.h"

int perceptive1_id=0;
int perceptive1_brothers[MAX_SCHEMAS];
arbitration perceptive1_callforarbitration;

int a=1;

/* exported variables */
int perceptive1_cycle=100; /* ms */
int c=1;

/* imported variables */


void perceptive1_iteration()
{  
  static int d;
  speedcounter(perceptive1_id);
  printf("perceptive1 iteration %d a=%d b=%d c=%d\n",d++,a,a,c);
}


void perceptive1_suspend()
{
  pthread_mutex_lock(&(all[perceptive1_id].mymutex));
  put_state(perceptive1_id,slept);
  printf("perceptive1: off\n");
  pthread_mutex_unlock(&(all[perceptive1_id].mymutex));
}


void perceptive1_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[perceptive1_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[perceptive1_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[perceptive1_id].children[i]=FALSE;
  all[perceptive1_id].father=father;
  perceptive1_callforarbitration=fn;
  put_state(perceptive1_id,winner);
  printf("perceptive1: on\n");
  pthread_cond_signal(&(all[perceptive1_id].condition));
  pthread_mutex_unlock(&(all[perceptive1_id].mymutex));
}

void *perceptive1_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      pthread_mutex_lock(&(all[perceptive1_id].mymutex));

      if (all[perceptive1_id].state==slept) 
	{
	  v=0; w=0;
	  pthread_cond_wait(&(all[perceptive1_id].condition),&(all[perceptive1_id].mymutex));
	  pthread_mutex_unlock(&(all[perceptive1_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[perceptive1_id].state==notready) put_state(perceptive1_id,ready);
	  else all[perceptive1_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[perceptive1_id].state==ready) put_state(perceptive1_id,winner);


	  if (all[perceptive1_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[perceptive1_id].mymutex));
	      gettimeofday(&a,NULL);
	      perceptive1_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = perceptive1_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(perceptive1_cycle*1000-diff);
	      else 
		{printf("time interval violated: perceptive1\n"); usleep(perceptive1_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[perceptive1_id].mymutex));
	      usleep(perceptive1_cycle*1000);
	    }
	}
    }
}

void perceptive1_startup()
{
  pthread_mutex_lock(&(all[perceptive1_id].mymutex));
  printf("perceptive1 schema started up\n");
  printf("perceptive1 a=%d\n",a);
  myexport("c",&c);
  myexport("perceptive1_cycle",&perceptive1_cycle);
  put_state(perceptive1_id,slept);
  pthread_create(&(all[perceptive1_id].mythread),NULL,perceptive1_thread,NULL);
  pthread_mutex_unlock(&(all[perceptive1_id].mymutex));
}

void perceptive1_guibuttons(FL_OBJECT *obj)
{
}

void perceptive1_guidisplay()
{
}

void perceptive1_guiresume()
{
  register_buttonscallback(perceptive1_guibuttons);
  register_displaycallback(perceptive1_guidisplay);
}

void perceptive1_guisuspend()
{
 delete_buttonscallback(perceptive1_guibuttons);
 delete_displaycallback(perceptive1_guidisplay);
}

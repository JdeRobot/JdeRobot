#include <jde.h>
#include <jdegui.h>
#include "myperceptive.h"

int myperceptive_id=0;
int myperceptive_brothers[MAX_SCHEMAS];
arbitration myperceptive_callforarbitration;

int a=1;

/* exported variables */
int myperceptive_cycle=100; /* ms */
int c=1;

/* imported variables */


void myperceptive_iteration()
{  
  static int d;
  speedcounter(myperceptive_id);
  printf("myperceptive iteration %d a=%d b=%d c=%d\n",d++,a,a,c);
}


void myperceptive_suspend()
{
  pthread_mutex_lock(&(all[myperceptive_id].mymutex));
  put_state(myperceptive_id,slept);
  printf("myperceptive: off\n");
  pthread_mutex_unlock(&(all[myperceptive_id].mymutex));
}


void myperceptive_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN) 
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[myperceptive_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[myperceptive_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[myperceptive_id].children[i]=FALSE;
  all[myperceptive_id].father=father;
  myperceptive_callforarbitration=fn;
  put_state(myperceptive_id,winner);
  printf("myperceptive: on\n");
  pthread_cond_signal(&(all[myperceptive_id].condition));
  pthread_mutex_unlock(&(all[myperceptive_id].mymutex));
}

void *myperceptive_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      pthread_mutex_lock(&(all[myperceptive_id].mymutex));

      if (all[myperceptive_id].state==slept) 
	{
	  v=0; w=0;
	  pthread_cond_wait(&(all[myperceptive_id].condition),&(all[myperceptive_id].mymutex));
	  pthread_mutex_unlock(&(all[myperceptive_id].mymutex));
	}
      else 
	{
	  /* check preconditions. For now, preconditions are always satisfied*/
	  if (all[myperceptive_id].state==notready) put_state(myperceptive_id,ready);
	  else all[myperceptive_id].state=ready;
	  /* check brothers and arbitrate. For now this is the only winner */
	  if (all[myperceptive_id].state==ready) put_state(myperceptive_id,winner);


	  if (all[myperceptive_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	    {
	      pthread_mutex_unlock(&(all[myperceptive_id].mymutex));
	      gettimeofday(&a,NULL);
	      myperceptive_iteration();
	      gettimeofday(&b,NULL);  

	      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
	      next = myperceptive_cycle*1000-diff-10000; 
	      /* discounts 10ms taken by calling usleep itself */
	      if (next>0) usleep(myperceptive_cycle*1000-diff);
	      else 
		{printf("time interval violated: myperceptive\n"); usleep(myperceptive_cycle*1000);
		}
	    }
	  else 
	    /* just let this iteration go away. overhead time negligible */
	    {
	      pthread_mutex_unlock(&(all[myperceptive_id].mymutex));
	      usleep(myperceptive_cycle*1000);
	    }
	}
    }
}

void myperceptive_startup()
{
  pthread_mutex_lock(&(all[myperceptive_id].mymutex));
  printf("myperceptive schema started up\n");
  printf("myperceptive a=%d\n",a);
  myexport("myperceptive","c",&c);
  myexport("myperceptive","myperceptive_cycle",&myperceptive_cycle);
  put_state(myperceptive_id,slept);
  pthread_create(&(all[myperceptive_id].mythread),NULL,myperceptive_thread,NULL);
  pthread_mutex_unlock(&(all[myperceptive_id].mymutex));
}

void myperceptive_guibuttons(FL_OBJECT *obj)
{
}

void myperceptive_guidisplay()
{
}

void myperceptive_guiresume()
{
  register_buttonscallback(myperceptive_guibuttons);
  register_displaycallback(myperceptive_guidisplay);
}

void myperceptive_guisuspend()
{
 delete_buttonscallback(myperceptive_guibuttons);
 delete_displaycallback(myperceptive_guidisplay);
}

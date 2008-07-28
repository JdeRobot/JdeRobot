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
 *  Authors : José Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

#include "jde.h"
#include "wiioperator.h"
#include "wiimote.h"

int wiioperator_id=0;
int wiioperator_brothers[MAX_SCHEMAS];
arbitration wiioperator_callforarbitration;

/*Global variables*/

/* exported variables */
int wiioperator_cycle=30; /* ms */

/* Imported symbols*/
unsigned short *buttons=NULL;
resumeFn buttons_resume=NULL;
suspendFn buttons_suspend=NULL;

struct nunchuk_state *nunchuk=NULL;
resumeFn nunchuk_resume=NULL;
suspendFn nunchuk_suspend=NULL;

int use_nunchuck=0;

float *v=NULL;
float *w=NULL;

resumeFn motors_resume=NULL;
suspendFn motors_suspend=NULL;

int init=0;

/*GUI Variables*/

/*Callbacks*/


void wiioperator_iteration(){
   int ch_v=0, ch_w=0;
   static float my_v=300, my_w=20;
   float v_step=10;
   float w_step=5;
   static int x_center=0, y_center=0;

   speedcounter(wiioperator_id);

   /*Increment or decrement speeds*/
   if (*buttons & CWIID_BTN_PLUS){
      my_v+=v_step;
   }
   if (*buttons & CWIID_BTN_MINUS){
      my_v-=v_step;
      if (my_v < 0)
         my_v=0;
   }
   if (*buttons & CWIID_BTN_1){
      my_w+=w_step;
   }
   if (*buttons & CWIID_BTN_2){
      my_w-=w_step;
      if (my_w < 0)
         my_w=0;
   }

   /*Drive*/
   if (!use_nunchuck){
      if (*buttons & CWIID_BTN_B){
         *v=my_v;
         ch_v=1;
      }
      else if (*buttons & CWIID_BTN_A){
         *v=-my_v;
         ch_v=1;
      }

      if (*buttons & CWIID_BTN_LEFT){
         *w=my_w;
         ch_w=1;
      }
      if (*buttons & CWIID_BTN_RIGHT){
         *w=-my_w;
         ch_w=1;
      }
      if (!ch_v)
         *v=0;
      if (!ch_w)
         *w=0;
   }
   else{
      if (!init){
         if ((*nunchuk).buttons & CWIID_NUNCHUK_BTN_C){
            x_center=(*nunchuk).stick[CWIID_X];
            y_center=(*nunchuk).stick[CWIID_Y];
            init=1;
         }
      }
      else{
         *v=my_v*(((*nunchuk).stick[CWIID_Y]-y_center)/20);
         *w=my_w*(-((*nunchuk).stick[CWIID_X]-x_center)/20);
      }
   }
}

/*Importar símbolos*/
void wiioperator_imports(){
   buttons=(unsigned short *)myimport ("wii_buttons","buttons0");
   buttons_resume=(resumeFn)myimport ("wii_buttons","resume");
   buttons_suspend=(suspendFn)myimport ("wii_buttons","suspend");

   nunchuk=(struct nunchuk_state*)myimport("wii_nunchuk", "nunchuk0");
   nunchuk_resume=(resumeFn )myimport("wii_nunchuk", "resume");
   nunchuk_suspend=(suspendFn )myimport("wii_nunchuk", "suspend");

   v=myimport("motors","v");
   w=myimport("motors","w");
   motors_resume=(resumeFn)myimport("motors","resume");
   motors_suspend=(suspendFn)myimport("motors","suspend");

   if (!nunchuk || !nunchuk_resume || !nunchuk_suspend){
      fprintf (stderr, "WARNING: I can't import symbols from "
            "wii_nunchuk, trying whith buttons\n");
      use_nunchuck=0;
      if (!buttons || !buttons_resume || !buttons_suspend)
      {
         fprintf (stderr, "ERROR: I can't import necessary symbols from "
               "wii_buttons\n");
         jdeshutdown(-1);
      }
   }
   else{
      use_nunchuck=1;
   }

   if ( !v || !w || !motors_resume || !motors_suspend){
      fprintf (stderr, "ERROR: I can't import necessary symbols from "
            "motors\n");
      jdeshutdown(-1);
   }
}

/*Exportar símbolos*/
void wiioperator_exports(){

   myexport("wiioperator", "id", &wiioperator_id);
   myexport("wiioperator","cycle",&wiioperator_cycle);
   myexport("wiioperator","resume",(void *)wiioperator_resume);
   myexport("wiioperator","suspend",(void *)wiioperator_suspend);
}

/*Las inicializaciones van en esta parte*/
void wiioperator_init(){
}

/*Al suspender el esquema*/
void wiioperator_end(){
}

void wiioperator_stop(){
}

void wiioperator_suspend()
{
  pthread_mutex_lock(&(all[wiioperator_id].mymutex));
  put_state(wiioperator_id,slept);
  printf("wiioperator: off\n");
  pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
  wiioperator_end();
  buttons_suspend();
  if (use_nunchuck){
     nunchuk_suspend();
  }
  motors_suspend();
  init=0;
}


void wiioperator_resume(int father, int *brothers, arbitration fn)
{
  int i;

  /* update the father incorporating this schema as one of its children */
  if (father!=GUIHUMAN && father!=SHELLHUMAN)
    {
      pthread_mutex_lock(&(all[father].mymutex));
      all[father].children[wiioperator_id]=TRUE;
      pthread_mutex_unlock(&(all[father].mymutex));
    }

  pthread_mutex_lock(&(all[wiioperator_id].mymutex));
  /* this schema resumes its execution with no children at all */
  for(i=0;i<MAX_SCHEMAS;i++) all[wiioperator_id].children[i]=FALSE;
  all[wiioperator_id].father=father;
  if (brothers!=NULL)
    {
      for(i=0;i<MAX_SCHEMAS;i++) wiioperator_brothers[i]=-1;
      i=0;
      while(brothers[i]!=-1) {wiioperator_brothers[i]=brothers[i];i++;}
    }
  wiioperator_callforarbitration=fn;
  put_state(wiioperator_id,notready);
  printf("wiioperator: on\n");
  pthread_cond_signal(&(all[wiioperator_id].condition));
  pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
  wiioperator_imports();
  buttons_resume(wiioperator_id, NULL, NULL);
  if (use_nunchuck){
     nunchuk_resume(wiioperator_id, NULL, NULL);
  }
  motors_resume(wiioperator_id, NULL, NULL);
  if (use_nunchuck){
     printf ("wiioperator: Please put the nunchuck joistick in a centered "
           "position and push 'C' button to start driving\n");
  }
}

void *wiioperator_thread(void *not_used)
{
   struct timeval a,b;
   long n=0; /* iteration */
   long next,bb,aa;

   for(;;)
   {
      pthread_mutex_lock(&(all[wiioperator_id].mymutex));

      if (all[wiioperator_id].state==slept)
      {
	 pthread_cond_wait(&(all[wiioperator_id].condition),&(all[wiioperator_id].mymutex));
	 pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
      }
      else
      {
	 /* check preconditions. For now, preconditions are always satisfied*/
	 if (all[wiioperator_id].state==notready)
	    put_state(wiioperator_id,ready);
	 /* check brothers and arbitrate. For now this is the only winner */
	 if (all[wiioperator_id].state==ready)
	 {put_state(wiioperator_id,winner);
	 gettimeofday(&a,NULL);
	 aa=a.tv_sec*1000000+a.tv_usec;
	 n=0;
	 }

	 if (all[wiioperator_id].state==winner)
	    /* I'm the winner and must execute my iteration */
	 {
	    pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
	    /*      gettimeofday(&a,NULL);*/
	    n++;
	    wiioperator_iteration();
	    gettimeofday(&b,NULL);
	    bb=b.tv_sec*1000000+b.tv_usec;
	    next=aa+(n+1)*(long)wiioperator_cycle*1000-bb;

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
	    pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
	    usleep(wiioperator_cycle*1000);
	 }
      }
   }
}

void wiioperator_startup(char *configfile)
{
  pthread_mutex_lock(&(all[wiioperator_id].mymutex));
  printf("wiioperator schema started up\n");
  wiioperator_exports();
  put_state(wiioperator_id,slept);
  pthread_create(&(all[wiioperator_id].mythread),NULL,wiioperator_thread,NULL);
  pthread_mutex_unlock(&(all[wiioperator_id].mymutex));
  wiioperator_init();
}

void wiioperator_guidisplay(){

}



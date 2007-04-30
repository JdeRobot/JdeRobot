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
#include "mastergui.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <forms.h>

#define PI 3.141592654

pthread_t jdegui_mythread;

guibuttons buttonscallbacks[MAX_SCHEMAS];
int num_buttonscallbacks=0;
int register_buttonscallback(guibuttons f)
{
  int i;
  int found=0;
  if (f!=NULL) 
    {
      for(i=0;i<num_buttonscallbacks;i++)
	if (buttonscallbacks[i]==NULL) 
	  {
	    buttonscallbacks[i]=f;
	    found=1;
	    break;
	  }
      if ((found==0)&&(num_buttonscallbacks<MAX_SCHEMAS)) 
	{
	  buttonscallbacks[num_buttonscallbacks]=f;
	  num_buttonscallbacks++;
	}
      else if ((found==0)&&(num_buttonscallbacks>=MAX_SCHEMAS))
	printf("Warning no space for registering a buttonscallback\n");
    }
  return 1;
}

int delete_buttonscallback(guibuttons f)
{
  int i;
  if (f!=NULL) 
    {
      for(i=0;i<num_buttonscallbacks;i++)
	if (buttonscallbacks[i]==f) 
	  {
	    buttonscallbacks[i]=NULL;
	    break;
	  }
    }
  return 1;
}


guidisplay displaycallbacks[MAX_SCHEMAS];
int num_displaycallbacks=0;
int register_displaycallback(guidisplay f)
{
  int i;
  int found=0;
  if (f!=NULL) 
    {
      for(i=0;i<num_displaycallbacks;i++)
	if (displaycallbacks[i]==NULL) 
	  {
	    displaycallbacks[i]=f;
	    found=1;
	    break;
	  }
      if ((found==0)&&(num_displaycallbacks<MAX_SCHEMAS)) 
	{
	  displaycallbacks[num_displaycallbacks]=f;
	  num_displaycallbacks++;
	}
      else if ((found==0)&&(num_displaycallbacks>=MAX_SCHEMAS))
	printf("Warning no space for registering a displaycallback\n");
    }
  return 1;
}

int delete_displaycallback(guidisplay f)
{
  int i;
  if (f!=NULL) 
    {
      for(i=0;i<num_displaycallbacks;i++)
	if (displaycallbacks[i]==f) 
	  {
	    displaycallbacks[i]=NULL;
	    break;
	  }
    }
  return 1;
}

/* GUI entries for dynamically loaded schemas */ 
FL_OBJECT *vis[MAX_SCHEMAS];
FL_OBJECT *act[MAX_SCHEMAS];
FL_OBJECT *fps[MAX_SCHEMAS];
FL_OBJECT *stat[MAX_SCHEMAS];

float fpsgui=0;
int kgui=0;
int jdegui_cycle=70; /* ms */

#define PUSHED 1
#define RELEASED 0

/* Necesarias para las Xlib, para todos los guis */
Display* display;
int screen;

static int mastergui_on=FALSE;
static int mastergui_request=FALSE;
static int kmastergui=0;

static FD_mastergui *fd_mastergui;
static Window  hierarchy_win;

/* to display the hierarchy */
static int state_dpy[MAX_SCHEMAS];
static int FontSize=18; /* font size in hierarchy canvas */
static int iteracion_display=0;
#define FORCED_REFRESH 5000 /* ms */ 
/* every "forced_refresh" the hierarchy is drawn from scratch.*/


void mastergui_suspend(void)
{
  if (mastergui_on==TRUE)
      mastergui_request=FALSE;
}

void mastergui_resume(void)
{
  if (mastergui_on==FALSE)
      mastergui_request=TRUE;
}

static void mastergui_buttons(FL_OBJECT *obj)
{
  int i;
 
  if (obj == fd_mastergui->exit) jdeshutdown(0);
  else if (obj == fd_mastergui->hide) mastergui_suspend(); 
 
  /* GUI entries for loaded schemas */
  for(i=0;i<MAX_SCHEMAS;i++)
    {
      if (i<num_schemas)
	{
	  if (obj == act[i]) 
	    { 
	      if (all[i].state==slept)
		(*all[i].resume)(GUIHUMAN,NULL,null_arbitration);
	      else
		{
		  (*all[i].suspend)();
		  /* if visualization is active, turn it off */
		  if ((all[i].guistate==on)||(all[i].guistate==pending_on))
		    all[i].guistate=pending_off;
		}
	    }
	  else if (obj == vis[i])
	    {
	      if ((all[i].guistate==on)||(all[i].guistate==pending_on))
		all[i].guistate=pending_off;
	      else 
		all[i].guistate=pending_on;
	    }
	}
      else break;
    }
}


static void draw_children(int schema,int *x,int *y)
{
  int i;
  int somechild=FALSE;
  int sizeX,boxY;

  boxY=FontSize+2;
  sizeX=(int)((float)FontSize*0.7);

  /* print the names of all the children of "schema" */
  for (i=0;i<MAX_SCHEMAS;i++)
    {
      if (all[schema].children[i]==TRUE)
	{  
	  somechild=TRUE;
 	  if (all[i].state==notready) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),sizeX*strlen(all[i].name),boxY,FL_RED,9,FontSize,all[i].name);
	  else if (all[i].state==ready) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),sizeX*strlen(all[i].name),boxY,FL_DARKGOLD,9,FontSize,all[i].name);
	  else if (all[i].state==winner) fl_drw_text(FL_ALIGN_LEFT,(*x),(*y),sizeX*strlen(all[i].name),boxY,FL_GREEN,9,FontSize,all[i].name);
	  
	  if ((*x+sizeX*(strlen(all[i].name)+2)) < (fd_mastergui->hierarchy->x + fd_mastergui->hierarchy->w))
	    (*x)+=sizeX*(strlen(all[i].name)+2);
	}
    }

  /* update the cursor position. Only if some child were displayed */
  if (somechild==TRUE)
    if (((*y)+boxY) < (fd_mastergui->hierarchy->y+fd_mastergui->hierarchy->h)) 
      (*y)+=boxY;

  /* expand the winner children of "schema", hopefully there will be only one */
  for (i=0;i<MAX_SCHEMAS;i++)
    {
      if ((all[schema].children[i]==TRUE) &&
	  (all[i].state==winner))
	{
	  draw_children(i,x,y);    
	}
    }
}

static void mastergui_display()
{
  int i,haschanged,j;
  int cursorX,cursorY;
  char fpstext[80]="";
  int sizeX,boxY;

  boxY=FontSize+2;
  sizeX=(int)((float)FontSize*0.7);  

  sprintf(fpstext,"%.1f ips",fpsgui);
  fl_set_object_label(fd_mastergui->guifps,fpstext);  

  /* GUI entries for loaded schemas: their guistate, measured frequency 
     and state (the state is also displayed in the hierarchy) */
  for(i=0;i<MAX_SCHEMAS; i++)
    {
      if (i<num_schemas)
	{ 
	  fl_set_object_label(act[i],all[i].name);
	  if (all[i].state==winner)
	      sprintf(fpstext,"%.1f",all[i].fps);
	  else 
	      sprintf(fpstext," ");

	  fl_set_object_label(fps[i],fpstext);

	  if (all[i].state==slept)
	    {
	      fl_set_object_label(stat[i],"slept");
	      fl_set_object_color(stat[i],FL_COL1,FL_GREEN);
	      fl_set_object_lcol(stat[i],FL_MCOL);
	    }
	  else if (all[i].state==notready)
	    { 
	      fl_set_object_label(stat[i],"notready");
	      fl_set_object_color(stat[i],FL_RED,FL_GREEN);
	      fl_set_object_lcol(stat[i],FL_BLUE);
	    }
	  else if (all[i].state==ready)
	    { 
	      fl_set_object_label(stat[i],"ready");
	      fl_set_object_color(stat[i],FL_DARKGOLD,FL_GREEN);
	      fl_set_object_lcol(stat[i],FL_BLUE);
	    }
	  else if (all[i].state==winner)
	    { 
	      fl_set_object_label(stat[i],"winner");
	      fl_set_object_color(stat[i],FL_GREEN,FL_GREEN);
	      fl_set_object_lcol(stat[i],FL_BLUE);
	    }

	  if (all[i].guistate==on) ;
	  else if (all[i].guistate==off) ;
	  /* In case of fl_set_button(vis[i],RELEASED) (or PUSHED) issued
	     at each iteration here, the graphical button does not change 
	     its state properly. It is only issued once, at jdegui_iteration,
	     when guistate upgraded or downgraded */

	  fl_show_object(act[i]);
	  fl_show_object(vis[i]);
	  fl_show_object(fps[i]);
	  fl_show_object(stat[i]);
	}
      else 
	{
	  fl_hide_object(act[i]);
	  fl_hide_object(vis[i]);
	  fl_hide_object(fps[i]);
	  fl_hide_object(stat[i]);
	}
    }

  /* hierarchy oscilloscope */
  haschanged=FALSE;
  for(i=0;i<num_schemas;i++) 
    {
      if (all[i].state!=state_dpy[i]) 
	{haschanged=TRUE;
	break;
	}
    }
  if (iteracion_display*jdegui_cycle>FORCED_REFRESH) 
    iteracion_display=0;
  else iteracion_display++;

  if ((haschanged==TRUE) ||     /* the hierarchy has changed */
      (iteracion_display==0))   /* slow refresh of the complete master gui, needed because incremental refresh of the hierarchy misses window occlusions */
    {
      /* clear of the hierarchy "window" */
      fl_winset(hierarchy_win); 
      fl_rectbound(fd_mastergui->hierarchy->x-1,fd_mastergui->hierarchy->y-1,fd_mastergui->hierarchy->w,fd_mastergui->hierarchy->h,FL_COL1);         
  
      j=0; cursorX=fd_mastergui->hierarchy->x+5; cursorY=fd_mastergui->hierarchy->y+5;
      for(i=0;i<num_schemas;i++)
	{
	  state_dpy[i]=all[i].state;
	  if ((all[i].state!=slept) && 
	      ((all[i].father==(*all[i].id)) 
	       || (all[i].father==GUIHUMAN) 
	       || (all[i].father==SHELLHUMAN)))
	    {
	      /* the root of one hierarchy */
	      j++;
	      cursorX=fd_mastergui->hierarchy->x+5; /* "carriage return" */		
	      if (j!=1)
		{ /* horizontal line */
		  if ((cursorY+5) < (fd_mastergui->hierarchy->y+fd_mastergui->hierarchy->h)) cursorY+=5;
		  fl_line(cursorX,cursorY,cursorX+fd_mastergui->hierarchy->w-15,cursorY,FL_BLACK);
		  if ((cursorY+5) < (fd_mastergui->hierarchy->y+fd_mastergui->hierarchy->h)) cursorY+=5;
		}
	      if (all[i].state==notready) 
		fl_drw_text(FL_ALIGN_LEFT,cursorX,cursorY,sizeX*strlen(all[i].name),boxY,FL_RED,9,FontSize,all[i].name);
	      else if (all[i].state==ready) 
		fl_drw_text(FL_ALIGN_LEFT,cursorX,cursorY,sizeX*strlen(all[i].name),boxY,FL_DARKGOLD,9,FontSize,all[i].name);
	      else if (all[i].state==winner) 
		fl_drw_text(FL_ALIGN_LEFT,cursorX,cursorY,sizeX*strlen(all[i].name),boxY,FL_GREEN,9,FontSize,all[i].name);	      

	      if ((cursorY+boxY) < (fd_mastergui->hierarchy->y+fd_mastergui->hierarchy->h)) cursorY+=boxY;
	      draw_children(i,&cursorX,&cursorY); 
	    }     
	}
    }
}
 

static void jdegui_iteration()
{ 
  FL_OBJECT *obj; 
  int i;

  kgui++;
 
  /* asynchronous requests for change of schema visualization state,
     both from the jdec-shell or clicking into the masterGUI */
   for(i=0;i<num_schemas;i++)
     {
       if (all[i].guistate==pending_on)
	 {
	   all[i].guistate=on;
	   if (kmastergui!=0)   /* mastergui already initialized */
	     fl_set_button(vis[i],PUSHED); 
	   if (all[i].guiresume!=NULL)
	     (*all[i].guiresume)();
	 }
       if (all[i].guistate==pending_off)
	 {
	   all[i].guistate=off;
	   if (kmastergui!=0) /* mastergui already initialized */
	     fl_set_button(vis[i],RELEASED);
	   if (all[i].guisuspend!=NULL)
	     (*all[i].guisuspend)();
	 } 
     }

  /* asynchronous requests for change of visualization state of mastergui from the shell*/
  if (mastergui_request!=mastergui_on)
    {
      if ((mastergui_request==FALSE) && (mastergui_on==TRUE))
	/* mastergui_suspend request */
	{
	  fl_hide_form(fd_mastergui->mastergui);
	  mastergui_on=FALSE;
	}
      else if ((mastergui_request==TRUE) && (mastergui_on==FALSE))
	/* mastergui_resume request */
	{
	  mastergui_on=TRUE;
	  if (kmastergui==0) /* not initialized */
	    {
	      kmastergui++;
	      fd_mastergui = create_form_mastergui();
	      /* tabla de asociacion guientry-esquema */
	      vis[0]=fd_mastergui->vis0;
	      vis[1]=fd_mastergui->vis1;
	      vis[2]=fd_mastergui->vis2;
	      vis[3]=fd_mastergui->vis3;
	      vis[4]=fd_mastergui->vis4;
	      vis[5]=fd_mastergui->vis5;
	      vis[6]=fd_mastergui->vis6;
	      vis[7]=fd_mastergui->vis7;
	      vis[8]=fd_mastergui->vis8;
	      vis[9]=fd_mastergui->vis9;
	      vis[10]=fd_mastergui->vis10;
	      vis[11]=fd_mastergui->vis11;
	      vis[12]=fd_mastergui->vis12;
	      vis[13]=fd_mastergui->vis13;
	      vis[14]=fd_mastergui->vis14;
	      vis[15]=fd_mastergui->vis15;
	      vis[16]=fd_mastergui->vis16;
	      vis[17]=fd_mastergui->vis17;
	      vis[18]=fd_mastergui->vis18;
	      vis[19]=fd_mastergui->vis19;
	      act[0]=fd_mastergui->act0;
	      act[1]=fd_mastergui->act1;
	      act[2]=fd_mastergui->act2;
	      act[3]=fd_mastergui->act3;
	      act[4]=fd_mastergui->act4;
	      act[5]=fd_mastergui->act5;
	      act[6]=fd_mastergui->act6;
	      act[7]=fd_mastergui->act7;
	      act[8]=fd_mastergui->act8;
	      act[9]=fd_mastergui->act9;
	      act[10]=fd_mastergui->act10;
	      act[11]=fd_mastergui->act11;
	      act[12]=fd_mastergui->act12;
	      act[13]=fd_mastergui->act13;
	      act[14]=fd_mastergui->act14;
	      act[15]=fd_mastergui->act15;
	      act[16]=fd_mastergui->act16;
	      act[17]=fd_mastergui->act17;
	      act[18]=fd_mastergui->act18;
	      act[19]=fd_mastergui->act19;
	      fps[0]=fd_mastergui->fps0;
	      fps[1]=fd_mastergui->fps1;
	      fps[2]=fd_mastergui->fps2;
	      fps[3]=fd_mastergui->fps3;
	      fps[4]=fd_mastergui->fps4;
	      fps[5]=fd_mastergui->fps5;
	      fps[6]=fd_mastergui->fps6;
	      fps[7]=fd_mastergui->fps7;
	      fps[8]=fd_mastergui->fps8;
	      fps[9]=fd_mastergui->fps9;
	      fps[10]=fd_mastergui->fps10;
	      fps[11]=fd_mastergui->fps11;
	      fps[12]=fd_mastergui->fps12;
	      fps[13]=fd_mastergui->fps13;
	      fps[14]=fd_mastergui->fps14;
	      fps[15]=fd_mastergui->fps15;
	      fps[16]=fd_mastergui->fps16;
	      fps[17]=fd_mastergui->fps17;
	      fps[18]=fd_mastergui->fps18;
	      fps[19]=fd_mastergui->fps19;
	      stat[0]=fd_mastergui->stat0;
	      stat[1]=fd_mastergui->stat1;
	      stat[2]=fd_mastergui->stat2;
	      stat[3]=fd_mastergui->stat3;
	      stat[4]=fd_mastergui->stat4;
	      stat[5]=fd_mastergui->stat5;
	      stat[6]=fd_mastergui->stat6;
	      stat[7]=fd_mastergui->stat7;
	      stat[8]=fd_mastergui->stat8;
	      stat[9]=fd_mastergui->stat9;
	      stat[10]=fd_mastergui->stat10;
	      stat[11]=fd_mastergui->stat11;
	      stat[12]=fd_mastergui->stat12;
	      stat[13]=fd_mastergui->stat13;
	      stat[14]=fd_mastergui->stat14;
	      stat[15]=fd_mastergui->stat15;
	      stat[16]=fd_mastergui->stat16;
	      stat[17]=fd_mastergui->stat17;
	      stat[18]=fd_mastergui->stat18;
	      stat[19]=fd_mastergui->stat19;
	      fl_set_form_position(fd_mastergui->mastergui,200,50);
	    }
	  fl_show_form(fd_mastergui->mastergui,FL_PLACE_POSITION,FL_FULLBORDER,"jdec master gui");
	  hierarchy_win = FL_ObjWin(fd_mastergui->hierarchy);
	}
    }

  /* buttons check (polling) */
  /* Puesto que el control no se cede al form, sino que se hace polling de sus botones pulsados, debemos proveer el enlace para los botones que no tengan callback asociada, en esta rutina de polling. OJO aquellos que tengan callback asociada jamas se veran con fl_check_forms, la libreria llamara automaticamente a su callback sin que fl_check_forms o fl_do_forms se enteren en absoluto.*/
  obj = fl_check_forms();
  if (mastergui_on==TRUE) mastergui_buttons(obj);
  for(i=0;i<num_buttonscallbacks;i++)
    {
      if (buttonscallbacks[i]!=NULL)
	(buttonscallbacks[i])(obj);
    }

  /* display iteration */
  if (mastergui_on==TRUE) mastergui_display();
  for(i=0;i<num_displaycallbacks;i++)
    {
      if (displaycallbacks[i]!=NULL)
	(displaycallbacks[i])();
    }
}

static void *jdegui_thread(void *not_used) 
{
  struct timeval a,b;
  long diff, next;

  for(;;)
    {
      gettimeofday(&a,NULL);
      jdegui_iteration();
      gettimeofday(&b,NULL);  
      diff = (b.tv_sec-a.tv_sec)*1000000+b.tv_usec-a.tv_usec;
      next = jdegui_cycle*1000-diff-10000; 
      /* discounts 10ms taken by calling usleep itself */
      if (next>0) usleep(jdegui_cycle*1000-diff);
      else; 
      /* Time interval violated but don't bother with 
	 any warning message, just display as fast as it can */
    }
}

void jdegui_startup()
{
  int myargc=1,i=0;
  char **myargv;
  char *aa;
  char a[]="myjde";
 
  aa=a;
  myargv=&aa;
  display= fl_initialize(&myargc,myargv,"jdec",0,0);
  screen = DefaultScreen(display);

  for(i=0;i<MAX_SCHEMAS;i++) state_dpy[i]=slept;
 
  pthread_create(&(jdegui_mythread),NULL,jdegui_thread,NULL);
}

void jdegui_resume()
{}
void jdegui_suspend()
{}

void jdegui_close()
{
  pthread_kill(jdegui_mythread,SIGKILL);
}

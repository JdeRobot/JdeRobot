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
 *            José Antonio Santos Cadenas <santoscadenas@gmail.com>
 */

#include "jde.h"
#include "forms.h"
#include "graphics_xforms.h"
#include "masterguigui.h"
#include "mastergui.h"

int mastergui_brothers[MAX_SCHEMAS];
arbitration mastergui_callforarbitration;

enum mastergui_states {init,t1,r1,t2,r2,t3,r3,t4,end};
FD_masterguigui *fd_masterguigui=NULL;

#define PUSHED 1
#define RELEASED 0

static Window  hierarchy_win;

/* to display the hierarchy */
static int state_dpy[MAX_SCHEMAS];
static int FontSize=18; /* font size in hierarchy canvas */
static int iteracion_display=0;
#define FORCED_REFRESH 5000 /* ms */
/* every "forced_refresh" the hierarchy is drawn from scratch.*/

/* GUI entries for dynamically loaded schemas */ 
FL_OBJECT *vis[MAX_SCHEMAS];
FL_OBJECT *act[MAX_SCHEMAS];
FL_OBJECT *fps[MAX_SCHEMAS];
FL_OBJECT *stat[MAX_SCHEMAS];

/* exported variables */
int mastergui_cycle=100; /* ms */
int mastergui_id=0;

/*Imported variables*/
registerbuttons myregister_buttonscallback;
registerdisplay myregister_displaycallback;
deletebuttons mydelete_buttonscallback;
deletedisplay mydelete_displaycallback;


int gui_activated=0;

/*Importar símbolos*/
void mastergui_imports(){
}

/*Exportar símbolos*/
void mastergui_exports(){
   myexport("mastergui","id",&mastergui_id);
   myexport("mastergui","cycle",&mastergui_cycle);
   myexport("mastergui","resume",(void *)mastergui_resume);
   myexport("mastergui","suspend",(void *)mastergui_suspend);
}

/*Las inicializaciones van en esta parte*/
void mastergui_init(){
   int i;
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

   for(i=0;i<MAX_SCHEMAS;i++) state_dpy[i]=slept;
}

void gui_init(){
   fd_masterguigui = create_form_masterguigui();
   
   /* tabla de asociacion guientry-esquema */
   vis[0]=fd_masterguigui->vis0;
   vis[1]=fd_masterguigui->vis1;
   vis[2]=fd_masterguigui->vis2;
   vis[3]=fd_masterguigui->vis3;
   vis[4]=fd_masterguigui->vis4;
   vis[5]=fd_masterguigui->vis5;
   vis[6]=fd_masterguigui->vis6;
   vis[7]=fd_masterguigui->vis7;
   vis[8]=fd_masterguigui->vis8;
   vis[9]=fd_masterguigui->vis9;
   vis[10]=fd_masterguigui->vis10;
   vis[11]=fd_masterguigui->vis11;
   vis[12]=fd_masterguigui->vis12;
   vis[13]=fd_masterguigui->vis13;
   vis[14]=fd_masterguigui->vis14;
   vis[15]=fd_masterguigui->vis15;
   vis[16]=fd_masterguigui->vis16;
   vis[17]=fd_masterguigui->vis17;
   vis[18]=fd_masterguigui->vis18;
   vis[19]=fd_masterguigui->vis19;
   act[0]=fd_masterguigui->act0;
   act[1]=fd_masterguigui->act1;
   act[2]=fd_masterguigui->act2;
   act[3]=fd_masterguigui->act3;
   act[4]=fd_masterguigui->act4;
   act[5]=fd_masterguigui->act5;
   act[6]=fd_masterguigui->act6;
   act[7]=fd_masterguigui->act7;
   act[8]=fd_masterguigui->act8;
   act[9]=fd_masterguigui->act9;
   act[10]=fd_masterguigui->act10;
   act[11]=fd_masterguigui->act11;
   act[12]=fd_masterguigui->act12;
   act[13]=fd_masterguigui->act13;
   act[14]=fd_masterguigui->act14;
   act[15]=fd_masterguigui->act15;
   act[16]=fd_masterguigui->act16;
   act[17]=fd_masterguigui->act17;
   act[18]=fd_masterguigui->act18;
   act[19]=fd_masterguigui->act19;
   fps[0]=fd_masterguigui->fps0;
   fps[1]=fd_masterguigui->fps1;
   fps[2]=fd_masterguigui->fps2;
   fps[3]=fd_masterguigui->fps3;
   fps[4]=fd_masterguigui->fps4;
   fps[5]=fd_masterguigui->fps5;
   fps[6]=fd_masterguigui->fps6;
   fps[7]=fd_masterguigui->fps7;
   fps[8]=fd_masterguigui->fps8;
   fps[9]=fd_masterguigui->fps9;
   fps[10]=fd_masterguigui->fps10;
   fps[11]=fd_masterguigui->fps11;
   fps[12]=fd_masterguigui->fps12;
   fps[13]=fd_masterguigui->fps13;
   fps[14]=fd_masterguigui->fps14;
   fps[15]=fd_masterguigui->fps15;
   fps[16]=fd_masterguigui->fps16;
   fps[17]=fd_masterguigui->fps17;
   fps[18]=fd_masterguigui->fps18;
   fps[19]=fd_masterguigui->fps19;
   stat[0]=fd_masterguigui->stat0;
   stat[1]=fd_masterguigui->stat1;
   stat[2]=fd_masterguigui->stat2;
   stat[3]=fd_masterguigui->stat3;
   stat[4]=fd_masterguigui->stat4;
   stat[5]=fd_masterguigui->stat5;
   stat[6]=fd_masterguigui->stat6;
   stat[7]=fd_masterguigui->stat7;
   stat[8]=fd_masterguigui->stat8;
   stat[9]=fd_masterguigui->stat9;
   stat[10]=fd_masterguigui->stat10;
   stat[11]=fd_masterguigui->stat11;
   stat[12]=fd_masterguigui->stat12;
   stat[13]=fd_masterguigui->stat13;
   stat[14]=fd_masterguigui->stat14;
   stat[15]=fd_masterguigui->stat15;
   stat[16]=fd_masterguigui->stat16;
   stat[17]=fd_masterguigui->stat17;
   stat[18]=fd_masterguigui->stat18;
   stat[19]=fd_masterguigui->stat19;
   fl_set_form_position(fd_masterguigui->masterguigui,200,50);
}

/*Al suspender el esquema*/
void mastergui_stop(){
  if (fd_masterguigui!=NULL)
    {
      if (all[mastergui_id].guistate==on){
         mastergui_guisuspend();
         all[mastergui_id].guistate=off;
      }
// 	fl_hide_form(fd_masterguigui->masterguigui);
//       fl_free_form(fd_masterguigui->masterguigui);
    }
  printf ("mastergui close\n");
}


void mastergui_suspend()
{
   mastergui_guisuspend();
   put_state(mastergui_id, slept);
}


void mastergui_resume(int father, int *brothers, arbitration fn)
{
   mastergui_guiresume();
   put_state(mastergui_id, winner);
}

void mastergui_startup()
{
   pthread_mutex_lock(&(all[mastergui_id].mymutex));
   printf("mastergui schema started up\n");
   mastergui_exports();
   put_state(mastergui_id,slept);
   pthread_mutex_unlock(&(all[mastergui_id].mymutex));
   mastergui_init();
}

void mastergui_guibuttons(void *obj1){
   int i;
   FL_OBJECT *obj=(FL_OBJECT *)obj1;
 
   if (obj == fd_masterguigui->exit)
      jdeshutdown(0);
   else if (obj == fd_masterguigui->hide)
      all[mastergui_id].guistate=pending_off;
 
   /* GUI entries for loaded schemas */
   for(i=0;i<MAX_SCHEMAS;i++)
   {
      if (i<num_schemas)
      {
         if (obj == act[i])
         {
            /*if (all[i].state==slept) */
            if (fl_get_button(act[i])==PUSHED)
               (*all[i].resume)(GUIHUMAN,NULL,null_arbitration);
            else
            {
               (*all[i].suspend)();
               /* if visualization is active, turn it off */
               if (all[i].guistate==on){
                  if (all[i].guisuspend!=NULL)
                     all[i].guisuspend();
                  all[i].guistate=pending_off;
               }
            }
         }
         else if (obj == vis[i])
         {
            if ((fl_get_button(vis[i])==RELEASED) && (all[i].guistate==on)){
               all[i].guistate=pending_off;
               if (all[i].guisuspend)
                  all[i].guisuspend();
            }
            else if ((fl_get_button(vis[i])==PUSHED) && (all[i].guistate==off)){
               all[i].guistate=pending_on;
               if (all[i].guiresume!=NULL)
                  all[i].guiresume();
            }
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
	  
         if ((*x+sizeX*(strlen(all[i].name)+2)) < (fd_masterguigui->hierarchy->x + fd_masterguigui->hierarchy->w))
            (*x)+=sizeX*(strlen(all[i].name)+2);
      }
   }

   /* update the cursor position. Only if some child were displayed */
   if (somechild==TRUE)
      if (((*y)+boxY) < (fd_masterguigui->hierarchy->y+fd_masterguigui->hierarchy->h))
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

void mastergui_guidisplay(){
   int i,haschanged,j;
   int cursorX,cursorY;
   char fpstext[80]="";
   int sizeX,boxY;

   speedcounter(mastergui_id);

   boxY=FontSize+2;
   sizeX=(int)((float)FontSize*0.7);

   sprintf(fpstext,"%.1f ips",all[mastergui_id].fps);
   fl_set_object_label(fd_masterguigui->guifps,fpstext);


  /* GUI entries for loaded schemas: their guistate, measured frequency 
   and state (the state is also displayed in the hierarchy) */
   for(i=0;i<MAX_SCHEMAS; i++){
      if (i<num_schemas){
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

      /* asynchronous requests for change of schema visualization state,
         both from the jdec-shell or clicking into the masterGUI */
         if (all[i].guistate==pending_on){
            all[i].guistate=on;
            fl_set_button(vis[i],PUSHED);
         }
         else if (all[i].guistate==pending_off){
            all[i].guistate=off;
            fl_set_button(vis[i],RELEASED);
         }
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
   if (iteracion_display*70>FORCED_REFRESH)
      iteracion_display=0;
   else iteracion_display++;

   if ((haschanged==TRUE) ||     /* the hierarchy has changed */
        (iteracion_display==0))   /* slow refresh of the complete master gui, needed because incremental refresh of the hierarchy misses window occlusions */
   {
      /* clear of the hierarchy "window" */
      fl_winset(hierarchy_win);
      fl_rectbound(fd_masterguigui->hierarchy->x-1,fd_masterguigui->hierarchy->y-1,fd_masterguigui->hierarchy->w,fd_masterguigui->hierarchy->h,FL_COL1);
  
      j=0; cursorX=fd_masterguigui->hierarchy->x+5; cursorY=fd_masterguigui->hierarchy->y+5;
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
            cursorX=fd_masterguigui->hierarchy->x+5; /* "carriage return" */
            if (j!=1)
            { /* horizontal line */
               if ((cursorY+5) < (fd_masterguigui->hierarchy->y+fd_masterguigui->hierarchy->h)) cursorY+=5;
               fl_line(cursorX,cursorY,cursorX+fd_masterguigui->hierarchy->w-15,cursorY,FL_BLACK);
               if ((cursorY+5) < (fd_masterguigui->hierarchy->y+fd_masterguigui->hierarchy->h)) cursorY+=5;
            }
            if (all[i].state==notready)
               fl_drw_text(FL_ALIGN_LEFT,cursorX,cursorY,sizeX*strlen(all[i].name),boxY,FL_RED,9,FontSize,all[i].name);
            else if (all[i].state==ready)
               fl_drw_text(FL_ALIGN_LEFT,cursorX,cursorY,sizeX*strlen(all[i].name),boxY,FL_DARKGOLD,9,FontSize,all[i].name);
            else if (all[i].state==winner)
               fl_drw_text(FL_ALIGN_LEFT,cursorX,cursorY,sizeX*strlen(all[i].name),boxY,FL_GREEN,9,FontSize,all[i].name);

            if ((cursorY+boxY) < (fd_masterguigui->hierarchy->y+fd_masterguigui->hierarchy->h)) cursorY+=boxY;
            draw_children(i,&cursorX,&cursorY);
         }
      }
   }
}



void mastergui_guisuspend_aux(void){
   gui_activated--;
   if (gui_activated==0){
      fl_hide_form(fd_masterguigui->masterguigui);
   
      mydelete_buttonscallback(mastergui_guibuttons);
      mydelete_displaycallback(mastergui_guidisplay);
   }
}

void mastergui_guisuspend(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "suspend_callback"))!=NULL){
         fn ((gui_function)mastergui_guisuspend_aux);
      }
   }
   else{
      fn ((gui_function)mastergui_guisuspend_aux);
   }
}

void mastergui_guiresume_aux(void){
   static int init=0;
   if (init==0){
      gui_init();
      init=1;
   }
   gui_activated++;
   if (gui_activated==1){
      all[mastergui_id].guistate=pending_on;
      fl_show_form(fd_masterguigui->masterguigui,FL_PLACE_POSITION,
                   FL_FULLBORDER,"jdec master gui");
      hierarchy_win = FL_ObjWin(fd_masterguigui->hierarchy);
      all[mastergui_id].guistate=off;

      if (myregister_buttonscallback!=NULL){
         myregister_buttonscallback(mastergui_guibuttons);
         myregister_displaycallback(mastergui_guidisplay);
      }
      else{
         fprintf (stderr, "mastergui: myregister_buttonscallback not fetched\n");
         jdeshutdown(1);
      }
   }
}

void mastergui_guiresume(){
   static callback fn=NULL;
   if (fn==NULL){
      if ((fn=(callback)myimport ("graphics_xforms", "resume_callback"))!=NULL){
         fn ((gui_function)mastergui_guiresume_aux);
      }
   }
   else{
      fn ((gui_function)mastergui_guiresume_aux);
   }
}

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
#include "graphics_gtk.h"
#include "mastergui.h"

#include <glade/glade.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

int mastergui_brothers[MAX_SCHEMAS];
arbitration mastergui_callforarbitration;


#define FORCED_REFRESH 5000 /* ms */
#define FORCED_REFRESH_GTK 2000 /* ms */
/* every "forced_refresh" the hierarchy is drawn from scratch.*/


/* exported variables */
int mastergui_cycle=100; /* ms */
int mastergui_id=0;

/*Imported variables*/
registerdisplay myregister_displaycallback;
deletedisplay mydelete_displaycallback;

/*Gtk variables*/

enum
{
   COL_ID = 0,
   COL_NAME,
   H_NUM_COLS,
   COL_SHOW = H_NUM_COLS,
   COL_PLAY,
   COL_IPS,
   COL_STATE,
   NUM_COLS
} ;

GtkTreeStore         *treestore=NULL;
GtkTreeView          *view=NULL;
GtkTreeStore         *treestore2=NULL;
GtkTreeView          *view2=NULL;
GladeXML *xml=NULL; /*Fichero xml*/
GtkWidget *win=NULL;


int gui_activated=0;
int use_gtk=0;
int refresh_display=FORCED_REFRESH_GTK;
int show_hierarchy=0;

/*Callbacks*/
void on_menu_about_activated(GtkMenuItem *item, gpointer user_data){
   gtk_widget_show(GTK_WIDGET(glade_xml_get_widget(xml, "aboutdialog")));
}

void on_about_close (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gtk_widget_hide(GTK_WIDGET(glade_xml_get_widget(xml, "aboutdialog")));
   gtk_widget_queue_draw(GTK_WIDGET(glade_xml_get_widget(xml, "aboutdialog")));
}

gboolean on_about_destroy (GtkWidget *widget, GdkEvent *event, gpointer user_data){
   gtk_widget_hide(widget);
   gtk_widget_queue_draw(widget);
   return TRUE;
}

void on_menu_hierarchy_toggled (GtkCheckMenuItem *menu_item, gpointer user_data){
   if (gtk_check_menu_item_get_active(menu_item)){
      show_hierarchy=1;
      gtk_widget_show(glade_xml_get_widget(xml, "hierarchy"));
      gtk_widget_show(glade_xml_get_widget(xml, "hierarchy_alig"));
   }
   else{
      show_hierarchy=0;
      gtk_widget_hide(glade_xml_get_widget(xml, "hierarchy"));
      gtk_widget_hide(glade_xml_get_widget(xml, "hierarchy_alig"));
      gtk_window_resize (GTK_WINDOW(win),1,1);
      gtk_widget_queue_draw(win);
   }
}


void on_exit_menu_activate(GtkMenuItem *item, gpointer user_data){
   gdk_threads_leave();
   jdeshutdown(0);
   gdk_threads_enter();
}

gboolean on_delete_window (GtkWidget *widget,
                           GdkEvent *event,
                           gpointer user_data)
{
   gdk_threads_leave();
   mastergui_guisuspend();
   gdk_threads_enter();
   return TRUE;
}

void on_start_gui_toggled (GtkCellRendererToggle   *cell_renderer,
                           gchar                   *path,
                           gpointer                data)
{
   int            id;
   GtkTreeModel   *model = GTK_TREE_MODEL(data);
   gchar          *s_id = NULL;
   GtkTreeIter    iter;


   gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(model), &iter, path);
   gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, COL_ID, &s_id, -1);
   id=atoi(s_id);

   if (all[id].guistate==on || all[id].guistate==pending_on){
      /*suspend gui*/
      if (all[id].guisuspend!=NULL){
         gdk_threads_leave();
         all[id].guisuspend();
         gdk_threads_enter();
      }
      all[id].guistate=off;
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_SHOW, FALSE, -1);
   }
   else{
      /*resume gui*/
      if (all[id].guiresume!=NULL){
         if (all[id].state!=winner && all[id].state!=ready &&
             all[id].resume!=NULL)
         {
            /*Start schema*/
            gdk_threads_leave();
            all[id].resume(GUIHUMAN,NULL,null_arbitration);
            gdk_threads_enter();
            gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_PLAY, TRUE, -1);
         }
         gdk_threads_leave();
         all[id].guiresume();
         gdk_threads_enter();
         all[id].guistate=on;
         gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_SHOW, TRUE, -1);
      }
      else{
         all[id].guistate=off;
         gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_SHOW, FALSE, -1);
      }
   }
}

void on_start_schema_toggled (GtkCellRendererToggle   *cell_renderer,
                              gchar                   *path,
                              gpointer                data)
{
   int            id;
   GtkTreeModel   *model = GTK_TREE_MODEL(data);
   gchar          *s_id = NULL;
   GtkTreeIter    iter;


   gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(model), &iter, path);
   gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, COL_ID, &s_id, -1);
   id=atoi(s_id);

   if (all[id].state==winner || all[id].state==ready){
      /*suspend gui*/
      if (all[id].suspend!=NULL){
         gdk_threads_leave();
         all[id].suspend();
         gdk_threads_enter();
         if (all[id].guistate==on || all[id].guistate==pending_on){
            /*suspend gui*/
            if (all[id].guisuspend!=NULL){
               gdk_threads_leave();
               all[id].guisuspend();
               gdk_threads_enter();
            }
            all[id].guistate=off;
            gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_SHOW, FALSE, -1);
         }
      }
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_PLAY, FALSE, -1);
   }
   else{
      /*resume gui*/
      if (all[id].resume!=NULL){
         gdk_threads_leave();
         all[id].resume(GUIHUMAN,NULL,null_arbitration);
         gdk_threads_enter();

         gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_PLAY, TRUE, -1);
      }
      else{
         gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_PLAY, FALSE, -1);
      }
   }
}

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
   if (myregister_displaycallback==NULL){
      myregister_displaycallback=(registerdisplay)myimport ("graphics_gtk", "register_displaycallback");
      mydelete_displaycallback=(deletedisplay)myimport ("graphics_gtk", "delete_displaycallback");
      if (myregister_displaycallback==NULL || mydelete_displaycallback==NULL){
         printf ("I can't fetch functions from graphics_xforms\n");
         jdeshutdown(1);
      }
   }
}

/*Al suspender el esquema*/
void mastergui_stop(){
   if (win){
      mastergui_guisuspend();
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

void mastergui_startup(char *configfile)
{
   pthread_mutex_lock(&(all[mastergui_id].mymutex));
   printf("mastergui schema started up\n");
   mastergui_exports();
   put_state(mastergui_id,slept);
   pthread_mutex_unlock(&(all[mastergui_id].mymutex));
   mastergui_init();
}

int is_father(int father, int child){
   if (all[child].father==father)
      return 1;
   else if (all[father].children[child]==1)
      return 1;
   else
      return 0;
}

int has_children(int i){
   int j;
   for (j=0; j<num_schemas; j++){
      if (is_father(i, j))
         return 1;
   }
   return 0;
}

/**
 * Displays childrens of a schema in a GtkTreeView
 * \param tree_store The GtkTreeStore where the hierarchy is displayed
 * \param parent The parent GtkTreeStore identifer
 */
void display_children(GtkTreeStore *tree_store, GtkTreeIter *parent){
   int i;
   GtkTreeIter    iter;
   int displayed[MAX_SCHEMAS];
   
   gboolean cont=FALSE, deleted=FALSE;
   
   int pid;
   char *s_pid;
   if (parent!=NULL){
      gtk_tree_model_get(GTK_TREE_MODEL(tree_store), parent, COL_ID, &s_pid, -1);
      pid=atoi(s_pid);
   }
   
   /*Review if the hierarchy is displayed. Includes children not displayed and
   removes displayed schemas that are not childres of "GtkTreeIter parent"*/

   for (i=0; i<num_schemas; i++){
      displayed[i]=0;
   }
   
   /*First of all review the displayed hierarchy*/
   if (gtk_tree_model_iter_children(GTK_TREE_MODEL(tree_store), &iter, parent)){
      do{
         int id;
         char *s_id;
         gtk_tree_model_get(GTK_TREE_MODEL(tree_store), &iter, COL_ID, &s_id, -1);
         id=atoi(s_id);
         cont=FALSE;
         deleted=FALSE;

         if (parent==NULL){
            /*This is the first level, here we must display the GUIHUMAN or 
            SHELLHUMAN children*/
            if (all[id].father==GUIHUMAN || all[id].father==SHELLHUMAN){
               displayed[id]=1;
               if (has_children(id)){
                  display_children(tree_store, &iter);
               }
            }
            else{
               /*Delete the iter*/
               cont=gtk_tree_store_remove(tree_store, &iter);
               deleted=TRUE;
            }
         }
         else{
            if (gtk_tree_store_is_ancestor (tree_store,  parent, &iter)){
               /*Is it really a child?*/
               if (is_father(pid, id)){
                  displayed[id]=1;
                  if (has_children(id)){
                     display_children(tree_store, &iter);
                  }
               }
               else{
                  /*Delete the iter*/
                  cont=gtk_tree_store_remove(tree_store, &iter);
                  deleted=TRUE;
               }
            }
         }

         if (!deleted){
            cont=gtk_tree_model_iter_next (GTK_TREE_MODEL(tree_store), &iter);
         }
      }while(cont);
   }
   
   /*Next we include not displayed childs*/
   for (i=0; i<num_schemas; i++){
      int insert=0;
      GtkTreeIter iter;
      char s_i[10];
      snprintf (s_i, 10, "%d", i);
      if (displayed[i]==0){
         if (parent==NULL){
            if (all[i].father==SHELLHUMAN || all[i].father==GUIHUMAN){
               insert=1;
            }
         }
         else{
            /*Check if it's a child of parent*/
            if (is_father(pid, i)){
               insert=1;
            }
         }
         if (insert){
            gtk_tree_store_insert_with_values   (tree_store,
                  &iter,
                  parent,
                  MAX_SCHEMAS,
                  COL_ID, s_i,
                  COL_NAME, all[i].name,
                  -1);
            if (has_children(i)){
               display_children(tree_store, &iter);
            }
         }
      }
   }
}

void mastergui_guidisplay_gtk(){
   if (refresh_display<0){
      /*Here we refresh the hierarchy*/
      gdk_threads_enter();

      if (show_hierarchy)
         display_children(treestore2, NULL);

      if (win!=NULL){
         gtk_window_resize (GTK_WINDOW(win),1,1);
         gtk_widget_queue_draw(win);
      }

      gdk_threads_leave();

      refresh_display=FORCED_REFRESH_GTK;
   }
   else{
      if (all[mastergui_id].fps>1){
         refresh_display-=1000/all[mastergui_id].fps;
      }
      else{
         refresh_display-=1000;
      }
   }
   speedcounter(mastergui_id);
}

void mastergui_guisuspend(){
   mydelete_displaycallback(mastergui_guidisplay_gtk);
   if (win!=NULL){
      gdk_threads_enter();
      gtk_widget_hide(win);
      gdk_threads_leave();
   }
   all[mastergui_id].guistate=off;
}

void appendSchemaTreeView (int id) {
   GtkTreeIter          toplevel;
   GtkTreeModel         *model;
   gdouble              ips;
   gchar                s_id[10];

   model = gtk_tree_view_get_model(GTK_TREE_VIEW(view));
   ips = all[id].fps;
   snprintf(s_id, 10, "%d", id);

   gtk_tree_store_append(GTK_TREE_STORE(model), &toplevel, NULL);
   gtk_tree_store_set(GTK_TREE_STORE(model), &toplevel,
                      COL_ID, s_id,
                      COL_NAME, all[id].name,
                      COL_SHOW, FALSE,
                      COL_PLAY, FALSE,
                      COL_IPS, ips,
                      -1);
   gtk_tree_view_set_model(view, GTK_TREE_MODEL(treestore));
}

static void cell_show_func (GtkTreeViewColumn *col,
                            GtkCellRenderer   *cell,
                            GtkTreeModel      *model,
                            GtkTreeIter       *iter,
                            gpointer           data)
{
   gchar    buf[32];
   GValue   val = {0, };
   int      id;

   gtk_tree_model_get_value(model, iter, COL_ID, &val);
   g_snprintf(buf, sizeof(buf), "%s", g_value_get_string(&val));

   id=atoi(buf);
   if (id<0 || id>=num_schemas){
      fprintf (stderr, "mastergui: WARNING not valid id '%s'\n", buf);
      return;
   }

   if (all[id].guistate==on || all[id].guistate==pending_on){
      g_object_set(cell, "active", TRUE, NULL);
   }
   else{
      g_object_set(cell, "active", FALSE, NULL);
   }
}

static void cell_play_func (GtkTreeViewColumn *col,
                            GtkCellRenderer   *cell,
                            GtkTreeModel      *model,
                            GtkTreeIter       *iter,
                            gpointer           data)
{
   gchar    buf[32];
   GValue   val = {0, };
   int      id;

   gtk_tree_model_get_value(model, iter, COL_ID, &val);
   g_snprintf(buf, sizeof(buf), "%s", g_value_get_string(&val));
   id=atoi(buf);

   if (id<0 || id>=num_schemas){
      fprintf (stderr, "mastergui: WARNING not valid id '%s'\n", buf);
      return;
   }

   if (all[id].state==winner || all[id].state==ready){
      g_object_set(cell, "active", TRUE, NULL);
   }
   else{
      g_object_set(cell, "active", FALSE, NULL);
   }
}

static void cell_data_func_gpa (GtkTreeViewColumn *col,
                                GtkCellRenderer   *cell,
                                GtkTreeModel      *model,
                                GtkTreeIter       *iter,
                                gpointer           data)
{
   gchar    buf[32];
   GValue   val = {0, };
   int      id;

   gtk_tree_model_get_value(model, iter, COL_ID, &val);
   snprintf(buf, sizeof(buf), "%s", g_value_get_string(&val));
   id = atoi(buf);
   
   if (id<0 || id>=num_schemas){
      fprintf (stderr, "mastergui: WARNING not valid id '%s'\n", buf);
      return;
   }
   g_snprintf(buf, sizeof(buf), "%.1f", all[id].fps);
   g_object_set(cell, "text", buf, NULL);
}

void cell_state_func (GtkTreeViewColumn   *col,
                      GtkCellRenderer     *cell,
                      GtkTreeModel        *model,
                      GtkTreeIter         *iter,
                      gpointer            data)
{
   gchar    buf[32];
   GValue   val = {0, };
   int      id;
   guint    state;

   gtk_tree_model_get_value(model, iter, COL_ID, &val);
   snprintf(buf, sizeof(buf), "%s", g_value_get_string(&val));
   id = atoi(buf);
   
   if (id<0 || id>=num_schemas){
      fprintf (stderr, "mastergui: WARNING not valid id '%s'\n", buf);
      return;
   }
   state = all[id].state;


   switch (state) {
      case slept:
         g_object_set(cell,
                      "background", "Grey",
                      "foreground", "Black",
                      "text", "slept",
                      NULL);
         break;
      case ready:
         g_object_set(cell,
                      "background", "Orange",
                      "foreground", "Black",
                      "text", "ready",
                      NULL);
         break;
      case winner:
         g_object_set(cell,
                      "background", "Green",
                      "foreground", "Black",
                      "text", "winner",
                      NULL);
         break;
      case notready:
         g_object_set(cell,
                      "background", "Red",
                      "foreground", "Black",
                      "text", "checking",
                      NULL);
         break;
      default:
         g_object_set(cell,
                      "background", "Grey",
                      "foreground", "Black",
                      "text", "winner",
                      NULL);
         break;
   }
}

/**
 * Used to initializate TreeViews, creating it's columns and setting it's data types
 */
static void startTreeView() {
   GtkCellRenderer      *renderer;
   GtkTreeViewColumn    *col;


   view = (GtkTreeView *)glade_xml_get_widget (xml, "treeview");
   view2 = (GtkTreeView *)glade_xml_get_widget (xml, "treeview2");

   treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_BOOLEAN, G_TYPE_BOOLEAN, G_TYPE_FLOAT, G_TYPE_INT);
   treestore2 = gtk_tree_store_new(H_NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

   gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(treestore));
   gtk_tree_view_set_model(GTK_TREE_VIEW(view2), GTK_TREE_MODEL(treestore2));

   g_object_unref(treestore); /* destroy model automatically with view */
   g_object_unref(treestore2); /* destroy model automatically with view */
   gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_NONE);
   gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view2)), GTK_SELECTION_NONE);

   /* --- Column #0 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "Id");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_set_visible(col, FALSE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "text", COL_ID);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);
   /* --- Column #0, Hierarchy --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "Id");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_set_visible(col, FALSE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "text", COL_ID);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view2), col);
   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);

   /* --- Column #1 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "Schema");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "text", COL_NAME);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);
   /* --- Column #1, Hierarchy --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "Schema");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "text", COL_NAME);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view2), col);
   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);

   /* --- Column #2 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "State");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   gtk_tree_view_column_set_cell_data_func(col, renderer, cell_state_func, NULL, NULL);
   g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);
   g_object_set(renderer, "foreground-set", TRUE, NULL);
   g_object_set(renderer, "background-set", TRUE, NULL);

   /* --- Column #3 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_toggle_new();
   gtk_tree_view_column_set_title(col, "Show");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "active", COL_SHOW);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   gtk_tree_view_column_set_cell_data_func(col, renderer, cell_show_func, NULL, NULL);
   g_signal_connect(renderer, "toggled", G_CALLBACK(on_start_gui_toggled),
                    treestore);

   /* --- Column #4 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_toggle_new();
   gtk_cell_renderer_set_fixed_size(renderer, 100, -1);
   gtk_tree_view_column_set_title(col, "Play");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_add_attribute(col, renderer, "active", COL_PLAY);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
   gtk_tree_view_column_set_cell_data_func(col, renderer, cell_play_func, NULL, NULL);
   g_signal_connect(renderer, "toggled", G_CALLBACK(on_start_schema_toggled),
                    treestore);

   /* --- Column #5 --- */
   col = gtk_tree_view_column_new();
   renderer = gtk_cell_renderer_text_new();
   gtk_tree_view_column_set_title(col, "Cycle time (milliseconds)");
   gtk_tree_view_column_set_resizable(col, TRUE);
   gtk_tree_view_column_set_max_width(col, 5);
   gtk_tree_view_column_pack_start(col, renderer, TRUE);
   gtk_tree_view_column_set_cell_data_func(col, renderer, cell_data_func_gpa, NULL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

}

void mastergui_guiresume(){
   static int cargado=0;
   static pthread_mutex_t imgrectifier_gui_mutex;

   pthread_mutex_lock(&imgrectifier_gui_mutex);
   if (!cargado){
      loadglade ld_fn;
      cargado=1;
      pthread_mutex_unlock(&imgrectifier_gui_mutex);
      /*Cargar la ventana desde el archivo xml .glade*/
      gdk_threads_enter();
      if ((ld_fn=(loadglade)myimport("graphics_gtk","load_glade"))==NULL){
         fprintf (stderr,"I can't fetch 'load_glade' from 'graphics_gtk'.\n");
         jdeshutdown(1);
      }
      xml = ld_fn ("mastergui.glade");
      if (xml==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }
      win = glade_xml_get_widget(xml, "window1");
      if (win==NULL){
         fprintf(stderr, "Error al cargar la interfaz gráfica\n");
         jdeshutdown(1);
      }

      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
         
      /*Generar la visualización en árbol*/
      startTreeView();
      /*Añadir todos los esquemas al árbol*/
      {
         int i;
         for (i=0; i<num_schemas; i++){
            appendSchemaTreeView(i);
         }
      }

      /*Hide frames until they are displayed*/
      gtk_widget_hide(glade_xml_get_widget(xml, "hierarchy"));
      gtk_widget_hide(glade_xml_get_widget(xml, "hierarchy_alig"));
      gtk_window_resize (GTK_WINDOW(win),1,1);
      gtk_widget_queue_draw(win);

      /*Conectar los callbacks*/
      {
         g_signal_connect(G_OBJECT(win), "delete-event",
                          G_CALLBACK(on_delete_window), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "menu_exit")),
                          "activate", G_CALLBACK(on_exit_menu_activate), NULL);
         g_signal_connect(GTK_WIDGET(glade_xml_get_widget(xml, "menu_hierarchy")),
                          "toggled", G_CALLBACK(on_menu_hierarchy_toggled), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "aboutdialog")),
                          "response", G_CALLBACK(on_about_close), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "aboutdialog")),
                          "delete-event", G_CALLBACK(on_about_destroy), NULL);
         g_signal_connect(G_OBJECT(glade_xml_get_widget(xml, "menu_about")),
                          "activate", G_CALLBACK(on_menu_about_activated), NULL);
      }
      
      gdk_threads_leave();
   }
   else{
      pthread_mutex_unlock(&imgrectifier_gui_mutex);
      gdk_threads_enter();
      gtk_widget_show(win);
      gtk_widget_queue_draw(GTK_WIDGET(win));
      gdk_threads_leave();
   }

   myregister_displaycallback(mastergui_guidisplay_gtk);
   all[mastergui_id].guistate=on;
}

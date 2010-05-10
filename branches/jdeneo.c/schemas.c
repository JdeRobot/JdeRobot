/*
 *  jdeneo.c
 *
 *  Copyright (C) 2006-2007 Jesús Ruiz-Ayúcar and José María Cañas
 *  for the Universidad Rey Juan Carlos robotic group.
 *  http://www.robotica-urjc.es
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "schemas.h"

GSList *schemas;
GSList *vars;

gboolean schemaDefaultArbitration(gchar *id) {
   return TRUE;
}

schema_data *schemaGetSchemaData(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d;

      mylist = g_slist_next (mylist);
   }
   return NULL;
}

gboolean schemaExists(gchar *id) {
   gint i = 0;
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id, id))
         return TRUE;

      mylist = g_slist_next (mylist);
      i++;
   }
   return FALSE;
}

gboolean schemaCount() {
   gint i = 0;
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      mylist = g_slist_next (mylist);
      i++;
   }
   return i;
}

gboolean schemaDelete(gchar *id) {
   function suspend;
   schema_data *s_d = schemaGetSchemaData(id);

   /* Si existe el esquema lo borramos */
   if (schemaExists(id)) {
      /* NOTE: ¿Y si no estaba suspendido qué? */
      suspend = schemaGetSuspend(id);
      suspend();

      /* Lo cerramos */
      g_module_close(s_d->module);

      /* Y actualizamos la lista de esquemas */
      schemas = g_slist_remove(schemas, s_d);
      return TRUE;
   }
   else
      return FALSE;
}

void showSchemaVars() {
   GSList *mylist = vars;

   schema_vars *s_v;

   while (mylist != NULL) {
      s_v = mylist->data;
      g_print("id = %s. name = %s. value = %d\n", s_v->id, s_v->varname, *(int *)s_v->var);
      mylist = g_slist_next (mylist);
   }
}

void *getSchemaVar(gchar *id, gchar *varname) {
   GSList *mylist = vars;

   schema_vars *s_v;

   while (mylist != NULL) {
      s_v = mylist->data;
      if (!strcmp(s_v->id,id) && !strcmp(s_v->varname,varname))
         return s_v->var;
      mylist = g_slist_next (mylist);
   }
   return NULL;
}

gchar *loadingSchema(gchar *plugin) {
   gchar *p, **schema_id, **schema_path;
   schema_data *s_d;
   function startup, suspend, loadgui, stopgui; /*  Variable que apuntará a las funciones */
   GModule *modulo;
   guint *schema_interval;

   /* Reservamos espacio para el registro */
   s_d = g_malloc(sizeof(schema_data));

   /* Abrimos el módulo */
   modulo = g_module_open (plugin, G_MODULE_BIND_LOCAL);

   /* Almacenamos info en la estructura */
   s_d->filename = strdup(plugin);
   if (!g_module_symbol(modulo, "schema_id", (gpointer *) &schema_id))
       g_error ("error: %s", g_module_error ());
   /* Si no hemos cargado el esquema antes (no está duplucado), procedemos */
   if (!schemaExists(*schema_id)) {
      s_d->id = strdup(*schema_id);

      /* Ahora nos quedamos solo con el directorio */
      p = strrchr(plugin, '/');

      if(p) {
         *p = '\0';
      }

      if (!modulo)
         g_error ("error: %s", g_module_error ());

      /* Obtenemos la ruta del módulo */
      if (!g_module_symbol(modulo, "schema_path", (gpointer *) &schema_path))
         g_error ("error: %s", g_module_error ());
      *schema_path = strdup(plugin);

      /* Cargamos la gui. No es obligatoria */
      if (g_module_symbol(modulo, "schema_gui_startup", (gpointer *) &loadgui)) {
         if (!g_module_symbol(modulo, "schema_gui_suspend", (gpointer *) &stopgui))
               g_error ("error: %s", g_module_error ());
         loadgui();

         s_d->suspend_gui = stopgui;
         s_d->startup_gui = loadgui;
      }
      else {
         s_d->suspend_gui = NULL;
         s_d->startup_gui = NULL;
      }

      /* Se obtiene el resto de simbolos obligatorios */
      if (!g_module_symbol(modulo, "schema_startup", (gpointer *) &startup))
            g_error ("error: %s", g_module_error ());
      if (!g_module_symbol(modulo, "schema_suspend", (gpointer *) &suspend))
            g_error ("error: %s", g_module_error ());
      if (!g_module_symbol(modulo, "schema_interval", (gpointer *) &schema_interval))
            g_error ("error: %s", g_module_error ());

      s_d->startup = startup;
      s_d->suspend = suspend;
      s_d->module = modulo;
      s_d->interval = schema_interval;
      s_d->state = slept;
      s_d->guistate = SUSPEND;
      s_d->father = "GUIHUMAN";
      s_d->group = 0;
      s_d->arbitration = schemaDefaultArbitration;
      schemas = g_slist_append(schemas, s_d);

      /* Sacamos el puntero a la lista de variables exportadas */
      startup();

      return s_d->id;
   }
   else {
      g_warning("Duplicated schema \"%s\" on %s",(*schema_id),plugin);
      g_free(s_d);
      g_module_close(modulo);
      return NULL;
   }
}

function schemaGetStartup(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->startup;

      mylist = g_slist_next (mylist);
   }
   return NULL;
}

function schemaGetSuspend(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->suspend;

      mylist = g_slist_next (mylist);
   }
   return NULL;
}

function schemaGetStartupGui(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->startup_gui;

      mylist = g_slist_next (mylist);
   }
   return NULL;
}

function schemaGetSuspendGui(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->suspend_gui;

      mylist = g_slist_next (mylist);
   }
   return NULL;
}

function_arb schemaGetArbitration(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->arbitration;

      mylist = g_slist_next (mylist);
   }
   return (function_arb)NULL;
}

void putSchemaVar(gpointer var, gchar *varname, gchar *schema_id) {
   schema_vars *misvars;

   misvars = g_malloc(sizeof(schema_vars));  /* Reservamos espacio para el registro */
   misvars->id = strdup(schema_id);          /* Asignamos el id del esquema */
   misvars->varname = strdup(varname);       /* Asignamos el nombre de nuestra variable */
   misvars->var = var;                       /* Asignamos el puntero a nuestra variable */
   vars = g_slist_append(vars, misvars);     /* Añadimos el registro a la lista */
}

GModule *schemaGetModule(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->module;

      mylist = g_slist_next (mylist);
   }
   return NULL;
}

guint schemaGetState(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->state;

      mylist = g_slist_next (mylist);
   }
   return (guint)NULL;
}

guint schemaGetGuiState(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->guistate;

      mylist = g_slist_next (mylist);
   }
   return (guint)NULL;
}

void schemaSetState(gchar *id, guint state) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         s_d->state = state;

      mylist = g_slist_next (mylist);
   }
}

void schemaSetGroup(gchar *id, guint group) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         s_d->group = group;

      mylist = g_slist_next (mylist);
   }
}

void schemaSetGuiState(gchar *id, guint state) {
   GSList         *mylist = schemas;
   schema_data    *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         s_d->guistate = state;

      mylist = g_slist_next (mylist);
   }
}

void schemaStart (GtkCellRendererToggle   *cell_renderer,
            gchar                   *path,
            gpointer                data) {
   function startup, suspend;
   GtkTreeModel   *model = GTK_TREE_MODEL(data);
   gchar          *name = NULL;
   GtkTreeIter    iter;

   gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(model), &iter, path);
   gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, COL_NAME, &name, -1);

   if (schemaGetState(name) == slept) {
      schemaSetState(name, checking);
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_PLAY, TRUE, -1);
      startup = schemaGetStartup(name);
      startup();
   }
   else {
      schemaSetState(name, slept);
      gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_PLAY, FALSE, -1);
      suspend = schemaGetSuspend(name);
      suspend();
   }
}

void schemaStartGui (GtkCellRendererToggle   *cell_renderer,
            gchar                   *path,
            gpointer                data) {
   function       startupgui, suspendgui;
   GtkTreeModel   *model = GTK_TREE_MODEL(data);
   gchar          *name = NULL;
   GtkTreeIter    iter;

   gtk_tree_model_get_iter_from_string(GTK_TREE_MODEL(model), &iter, path);
   gtk_tree_model_get(GTK_TREE_MODEL(model), &iter, COL_NAME, &name, -1);

   if (schemaGetStartupGui(name) != NULL) {
      if (schemaGetGuiState(name) == SUSPEND) {
         schemaSetGuiState(name, STARTED);
         gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_SHOW, TRUE, -1);
         startupgui = schemaGetStartupGui(name);
         startupgui();
      }
      else {
         schemaSetGuiState(name, SUSPEND);
         gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_SHOW, FALSE, -1);
         suspendgui = schemaGetSuspendGui(name);
         suspendgui();
      }
   }
}

guint schemaGetInterval(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return *(s_d->interval);

      mylist = g_slist_next (mylist);
   }
   return 0;
}

guint schemaGetGroup(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         return s_d->group;

      mylist = g_slist_next (mylist);
   }
   return 0;
}

/* Hay que cambiar esto. */
void schemaSetInterval(GtkCellRendererText *celltext,
               const gchar         *string_path,
               const gchar         *new_text,
               gpointer             data) {
   schema_data    *s_d;
   function       startup, suspend;
   GtkTreeModel   *model = GTK_TREE_MODEL(data);
   GtkTreeIter    iter;
   gchar          *name = NULL;
   gfloat         oldval = 0.0;
   guint          newval = 0;

   gtk_tree_model_get_iter_from_string(model, &iter, string_path);
   gtk_tree_model_get(model, &iter, COL_NAME, &name, COL_IPS, &oldval, -1);

   if (sscanf(new_text, "%ud", &newval) != 1)
      g_warning("in %s: problem converting string '%s' into integer.\n", __FUNCTION__, new_text);


   /* Asignamos el nuevo tiempo */
   s_d = schemaGetSchemaData(name);
   *(s_d->interval) = newval;

   gtk_tree_store_set(GTK_TREE_STORE(model), &iter, COL_IPS, (gfloat)newval, -1);

   if (schemaGetState(name) == STARTED) {
      suspend = schemaGetSuspend(name);
      suspend();

      /* Y llamamos a startup*/
      startup = schemaGetStartup(name);
      startup();
   }
}

gchar *schemaGetFather(gchar *id) {
   GSList *mylist = schemas;

   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id)) {
         return s_d->father;
      }

      mylist = g_slist_next (mylist);
   }
   return (guint)NULL;
}

gboolean schemaIsAnyWinner(gchar *id) {
   gchar *father;
   guint group;
   GSList *mylist = schemas;
   schema_data *s_d;

   /* Obtenemos el id del padre */
   father = schemaGetFather(id);
   group = schemaGetGroup(id);

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->father,father) &&
         (s_d->state == winner) && 
         (s_d->group == group))
            return TRUE;

      mylist = g_slist_next (mylist);
   }
   return FALSE;
}

gboolean schemaIsAnyReady(gchar *id) {
   gchar *father;
   guint group;
   GSList *mylist = schemas;
   schema_data *s_d;

   /* Obtenemos el id del padre */
   father = schemaGetFather(id);
   group = schemaGetGroup(id);

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->father,father) &&
         (s_d->state == winner || s_d->state == ready) &&
         (s_d->group == group))
            return TRUE;

      mylist = g_slist_next (mylist);
   }
   return FALSE;
}

void schemaSetArbitration(gchar *id, function_arb func) {
   GSList *mylist = schemas;
   schema_data *s_d;

   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id))
         s_d->arbitration = func;
      mylist = g_slist_next (mylist);
   }
}

void schemaSetFather(gchar *id, gchar *father) {
   GSList               *mylist = schemas;
   schema_data          *s_d;
   GtkTreeIter          iter, *padre, *hijo, toplevel;
   gboolean             valid;
   gboolean             newFather = TRUE;
   gchar                *name;


   while (mylist != NULL) {
      s_d = mylist->data;
      if (!strcmp(s_d->id,id)) {
         if (!strcmp(s_d->father,father))
            newFather = FALSE;
         else
            s_d->father = strdup(father);
      }
      mylist = g_slist_next (mylist);
   }

   if (newFather) {
      /* Getting father and children id */
      valid = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(treestore), &iter);
      while (valid) {
         gtk_tree_model_get(GTK_TREE_MODEL(treestore), &iter, COL_NAME, &name, -1);
         if (!g_ascii_strcasecmp(name, id)) {
            hijo = gtk_tree_iter_copy(&iter);
         }
         if (!g_ascii_strcasecmp(name, father)) {
            padre = gtk_tree_iter_copy(&iter);
         }
         valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(treestore), &iter);
      }

      /* Deleting previous children id */
      gtk_tree_store_remove(GTK_TREE_STORE(treestore), hijo);

      /* Updating tree view */
      gtk_tree_store_append(GTK_TREE_STORE(treestore), &toplevel, padre);
      gtk_tree_store_set(GTK_TREE_STORE(treestore), &toplevel,
                        COL_NAME, id,
                        COL_SHOW, (schemaGetGuiState(id) != SUSPEND),
                        COL_PLAY, (schemaGetState(id) != slept),
                        COL_IPS, (gdouble)schemaGetInterval(id),
                        -1);
      gtk_tree_view_set_model(view, GTK_TREE_MODEL(treestore));
   }
}

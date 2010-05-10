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

#ifndef SCHEMAS
#define SCHEMAS
#include "gui.h"
#include "string.h"
#include "jde.h"

#define STARTED 1
#define SUSPEND 2

/* Tipo para la estructura que almacena info de cada esquema */
typedef struct _schema_data schema_data;
typedef void *(*function) (void);
typedef void *(*function_param) (void *);
typedef gboolean (*function_arb) (gchar *);

struct _schema_data {
   gchar *id;
   gchar *filename;
   guint *interval;
   function startup;
   function suspend;
   function startup_gui;
   function suspend_gui;
   function_arb arbitration;
   gchar *father;
   guint group;
   guint state;
   guint guistate;
   GModule *module;
};

extern GSList *schemas;

void schemaStart (GtkCellRendererToggle   *cell_renderer,
            gchar                   *path,
            gpointer                data);

void schemaStartGui (GtkCellRendererToggle   *cell_renderer,
            gchar                   *path,
            gpointer                data);

void schemaSetInterval(GtkCellRendererText *celltext,
               const gchar         *string_path,
               const gchar         *new_text,
               gpointer             data);

gboolean schemaExists(gchar *id);

gboolean schemaDelete(gchar *id);

void *getSchemaVar(gchar *id, gchar *varname);

void putSchemaVar(gpointer var, gchar *varname, gchar *schema);

gchar *loadingSchema(gchar *);

function schemaGetStartup(gchar *id);

function schemaGetSuspend(gchar *id);

function schemaGetStartupGui(gchar *id);

guint schemaGetInterval(gchar *id);

guint schemaGetState(gchar *id);

void schemaSetState(gchar *id, guint state);

void schemaSetGroup(gchar *id, guint group);

gboolean schemaIsAnyWinner(gchar *id);

gboolean schemaIsAnyReady(gchar *id);

function_arb schemaGetArbitration(gchar *id);

void schemaSetArbitration(gchar *id, function_arb func);

void schemaSetFather(gchar *id, gchar *father);
#endif

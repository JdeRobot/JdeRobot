#ifndef IMPRIMIR_H
#define IMPRIMIR_H

#include <gtk/gtk.h>

#include <libgnomecanvas/libgnomecanvas.h>

//#include <gdk_imlib_types.h>
#include <gtk/gtkprintunixdialog.h>
#include <gtk/gtkpagesetupunixdialog.h>
#include <string.h>
#include <png.h>
#include <gdk/gdk.h>
//#include <gdk_imlib.h>
//#include <gdk_imlib_types.h>
#include <sys/stat.h>
#ifndef __G_LIB_H__
#  include <glib.h>
#endif

#ifndef __GDK_H__
#  include <gdk/gdk.h>
#endif

#ifndef __GTK_H__
#  define GTK_ENABLE_BROKEN 1
#  include <gtk/gtk.h>
#endif

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
//void
//dialogoImpresion(gchar *file);

/* **** *** ** * GUARDAR PIXMAP EN IMAGEN * ** *** **** */
void
guardarPixmapImagenPng(GtkWidget *canvas, gchar*ruta);


int
sp_png_write_rgba (const unsigned char *filename, const unsigned char *px, int width, int height, int rowstride);
void exportar (GnomeCanvas *canvas);
void exportar2 (GnomeCanvas *canvas);
#endif


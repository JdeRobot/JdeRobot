#include <gtk/gtk.h>

//void
//dialogoImpresion(gchar *file);

/* **** *** ** * GUARDAR PIXMAP EN IMAGEN * ** *** **** */
void
guardarPixmapImagenPng(GtkWidget *canvas, gchar*ruta);


int
sp_png_write_rgba (const unsigned char *filename, const unsigned char *px, int width, int height, int rowstride);
void exportar (GnomeCanvas *canvas);
void exportar2 (GnomeCanvas *canvas);


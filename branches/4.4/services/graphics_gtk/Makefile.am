if ENABLE_GRAPHICS_GTK
service_LTLIBRARIES = graphics_gtk.la
endif

graphics_gtk_la_SOURCES = graphics_gtk.c
graphics_gtk_la_LDFLAGS = -module -avoid-version $(GRAPHICS_GTK_LIBS) $(SERVICE_LDFLAGS) $(SERVICE_LIBS)
graphics_gtk_la_CFLAGS = $(GRAPHICS_GTK_CFLAGS) $(SERVICE_CFLAGS) -I$(top_srcdir)/base
dist_conf_DATA = graphics_gtk.conf
include_HEADERS = graphics_gtk.h

MAINTAINERCLEANFILES = Makefile.in Makefile

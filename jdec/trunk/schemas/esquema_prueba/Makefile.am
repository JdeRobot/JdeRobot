if ENABLE_ESQUEMA_PRUEBA
schema_LTLIBRARIES = esquema_prueba.la
endif

esquema_prueba_la_SOURCES = esquema_prueba.c esquema_prueba.h
esquema_prueba_la_LDFLAGS = -module -avoid-version $(ESQUEMA_PRUEBA_LIBS)
esquema_prueba_la_LIBADD =
esquema_prueba_la_CFLAGS = $(ESQUEMA_PRUEBA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = esquema_prueba.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

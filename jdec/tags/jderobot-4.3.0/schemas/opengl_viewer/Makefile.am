if ENABLE_OPENGL_VIEWER
schema_LTLIBRARIES = opengl_viewer.la
endif

opengl_viewer_la_SOURCES = opengl_viewer.c opengl_viewer.h obj_loader.c obj_loader.h
opengl_viewer_la_LDFLAGS = -module -avoid-version $(OPENGL_VIEWER_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
opengl_viewer_la_CFLAGS = $(OPENGL_VIEWER_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = opengl_viewer.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

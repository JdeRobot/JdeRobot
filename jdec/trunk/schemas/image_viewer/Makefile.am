if ENABLE_IMAGE_VIEWER
schema_LTLIBRARIES = image_viewer.la
endif

image_viewer_la_SOURCES = image_viewer.c image_viewer.h
image_viewer_la_LDFLAGS = -module -avoid-version $(IMAGE_VIEWER_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
image_viewer_la_LIBADD =
image_viewer_la_CFLAGS = $(IMAGE_VIEWER_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = image_viewer.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

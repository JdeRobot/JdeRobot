if ENABLE_OPENCVDEMO
schema_LTLIBRARIES = opencvdemo.la
endif

opencvdemo_la_SOURCES = opencvdemo.c opencvdemo.h
opencvdemo_la_LDFLAGS = -module -avoid-version $(OPENCVDEMO_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
opencvdemo_la_LIBADD = $(top_srcdir)/libs/colorspaces/libcolorspaces.la
opencvdemo_la_CFLAGS = $(OPENCVDEMO_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/libs/colorspaces -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = opencvdemo.glade
dist_conf_DATA = opencvdemo.conf


MAINTAINERCLEANFILES = Makefile.in Makefile

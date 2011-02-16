if ENABLE_DOMOTICDEMO
schema_LTLIBRARIES = domoticdemo.la
endif

domoticdemo_la_SOURCES = domoticdemo.c domoticdemo.h
domoticdemo_la_LDFLAGS = -module -avoid-version $(DOMOTICDEMO_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
domoticdemo_la_CFLAGS = $(DOMOTICDEMO_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = domoticdemo.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

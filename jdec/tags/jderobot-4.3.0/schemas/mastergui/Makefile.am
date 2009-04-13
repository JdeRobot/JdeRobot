if ENABLE_MASTERGUI
schema_LTLIBRARIES = mastergui.la
endif

mastergui_la_SOURCES = mastergui.c mastergui.h masterguigui.fd
mastergui_la_LDFLAGS = -module -avoid-version $(MASTERGUI_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
mastergui_la_CFLAGS = $(MASTERGUI_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk

dist_glade_DATA = mastergui.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

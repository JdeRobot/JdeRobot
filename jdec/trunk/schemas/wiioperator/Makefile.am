if ENABLE_WIIOPERATOR
schema_LTLIBRARIES = wiioperator.la
endif

wiioperator_la_SOURCES = wiioperator.c wiioperator.h
wiioperator_la_LDFLAGS = -module -avoid-version $(WIIOPERATOR_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
wiioperator_la_LIBADD =
wiioperator_la_CFLAGS = $(WIIOPERATOR_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir) -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = wiioperator.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

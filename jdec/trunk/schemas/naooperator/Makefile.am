if ENABLE_NAOOPERATOR
schema_LTLIBRARIES = naooperator.la
endif

naooperator_la_SOURCES = naooperator.c naooperator.h
naooperator_la_LDFLAGS = -module -avoid-version $(NAOOPERATOR_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
naooperator_la_CFLAGS = $(NAOOPERATOR_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = naooperator.glade
dist_conf_DATA = naooperator.conf


MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_COLORTUNER
schema_LTLIBRARIES = colortuner.la
endif

colortuner_la_SOURCES = colortuner.c colortuner.h 
colortuner_la_LDFLAGS = -module -avoid-version $(COLORTUNER_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
colortuner_la_LIBADD = $(top_srcdir)/libs/colorspaces/libcolorspaces.la
colortuner_la_CFLAGS = $(COLORTUNER_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/libs/colorspaces -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = colortuner.glade 


MAINTAINERCLEANFILES = Makefile.in Makefile


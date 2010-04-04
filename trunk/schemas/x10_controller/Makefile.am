if ENABLE_X10_CONTROLLER
schema_LTLIBRARIES = x10_controller.la
endif

x10_controller_la_SOURCES = x10_controller.c x10_controller.h
x10_controller_la_LDFLAGS = -module -avoid-version $(X10_CONTROLLER_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
x10_controller_la_CFLAGS = $(X10_CONTROLLER_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = x10_controller.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

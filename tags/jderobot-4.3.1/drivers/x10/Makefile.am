if ENABLE_X10
driver_LTLIBRARIES = x10.la
endif

x10_la_SOURCES = x10.c x10.h
x10_la_LDFLAGS = -module -avoid-version $(X10_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
x10_la_CFLAGS = $(X10_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base
dist_conf_DATA = x10.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

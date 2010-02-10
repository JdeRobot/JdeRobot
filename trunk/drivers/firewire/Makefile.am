if ENABLE_FIREWIRE
driver_LTLIBRARIES = firewire.la
endif

firewire_la_SOURCES = firewire.c
firewire_la_LDFLAGS = -module -avoid-version $(DRIVER_LDFLAGS) $(DRIVER_LIBS) $(FIREWIRE_LIBS)
firewire_la_CFLAGS = $(FIREWIRE_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base
dist_conf_DATA = firewire.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

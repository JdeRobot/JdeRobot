if ENABLE_FIREWIRE
driver_LTLIBRARIES = firewire.la
endif

firewire_la_SOURCES = firewire.c
firewire_la_LDFLAGS = -module -avoid-version @FIREWIRE_LIBS@
firewire_la_CFLAGS = @FIREWIRE_CFLAGS@ -I$(top_srcdir)/base
firewire_ladir = @CFGDIR@
dist_firewire_la_DATA = firewire.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

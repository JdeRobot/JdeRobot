if ENABLE_NETWORKSERVER
driver_LTLIBRARIES = networkserver.la
endif

networkserver_la_SOURCES = networkserver.c jdemessages.h
networkserver_la_LDFLAGS = -module -avoid-version @NETWORKSERVER_LIBS@
networkserver_la_CFLAGS = @NETWORKSERVER_CFLAGS@ -I$(top_srcdir)/base
networkserver_ladir = @CFGDIR@
dist_networkserver_la_DATA = networkserver.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

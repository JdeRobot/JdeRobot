if ENABLE_NETWORKSERVER
service_LTLIBRARIES = networkserver.la
endif

networkserver_la_SOURCES = networkserver.c
networkserver_la_LDFLAGS = -module -avoid-version $(NETWORKSERVER_LIBS) $(SERVICE_LDFLAGS) $(SERVICE_LIBS)
networkserver_la_CFLAGS = $(NETWORKSERVER_CFLAGS) $(SERVICE_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base
dist_conf_DATA = networkserver.conf
include_HEADERS = jdemessages.h

MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_NETWORKCLIENT
driver_LTLIBRARIES = networkclient.la
endif

networkclient_la_SOURCES = networkclient.c jdemessages.h
networkclient_la_LDFLAGS = -module -avoid-version $(NETWORKCLIENT_LIBS)
networkclient_la_CFLAGS = $(NETWORKCLIENT_CFLAGS) -I$(top_srcdir)/base
dist_conf_DATA = networkclient.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

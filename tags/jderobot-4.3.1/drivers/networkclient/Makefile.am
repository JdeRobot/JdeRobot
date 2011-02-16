if ENABLE_NETWORKCLIENT
driver_LTLIBRARIES = networkclient.la
endif

networkclient_la_SOURCES = networkclient.c jdemessages.h
networkclient_la_LDFLAGS = -module -avoid-version $(NETWORKCLIENT_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
networkclient_la_CFLAGS = $(NETWORKCLIENT_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/services/networkserver
dist_conf_DATA = networkclient.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_MPLAYER
driver_LTLIBRARIES = mplayer.la
endif

mplayer_la_SOURCES = mplayer.c
mplayer_la_LDFLAGS = -module -avoid-version $(MPLAYER_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
mplayer_la_CFLAGS = $(MPLAYER_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base
dist_conf_DATA = mplayer.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

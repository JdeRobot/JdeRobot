if ENABLE_MPLAYER
driver_LTLIBRARIES = mplayer.la
endif

mplayer_la_SOURCES = mplayer.c
mplayer_la_LDFLAGS = -module -avoid-version @MPLAYER_LIBS@
mplayer_la_CFLAGS = @MPLAYER_CFLAGS@ -I$(top_srcdir)/base
mplayer_ladir = @CFGDIR@
dist_mplayer_la_DATA = mplayer.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

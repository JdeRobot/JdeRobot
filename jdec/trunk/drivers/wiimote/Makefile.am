if ENABLE_WIIMOTE
driver_LTLIBRARIES = wiimote.la
endif

wiimote_la_SOURCES = wiimote.c
wiimote_la_LDFLAGS = -module -avoid-version @WIIMOTE_LIBS@
wiimote_la_CFLAGS = @WIIMOTE_CFLAGS@ -I$(top_srcdir)/base

EXTRA_DIST = wiimote.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_WIIMOTE
driver_LTLIBRARIES = wiimote.la
endif

wiimote_la_SOURCES = wiimote.c
wiimote_la_LDFLAGS = -module -avoid-version $(WIIMOTE_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
wiimote_la_CFLAGS = $(WIIMOTE_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base
dist_conf_DATA = wiimote.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

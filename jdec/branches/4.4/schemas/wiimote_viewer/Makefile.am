if ENABLE_WIIMOTE_VIEWER
schema_LTLIBRARIES = wiimote_viewer.la
endif

wiimote_viewer_la_SOURCES = wiimote_viewer.c wiimote_viewer.h
wiimote_viewer_la_LDFLAGS = -module -avoid-version $(WIIMOTE_VIEWER_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
wiimote_viewer_la_CFLAGS = $(WIIMOTE_VIEWER_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir) -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = wiimote_viewer.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_GRAPHICS_XFORMS
service_LTLIBRARIES = graphics_xforms.la
endif

graphics_xforms_la_SOURCES = graphics_xforms.c
graphics_xforms_la_LDFLAGS = -module -avoid-version $(GRAPHICS_XFORMS_LIBS) $(GRAPHICS_GTK_LIBS) $(SERVICE_LDFLAGS) $(SERVICE_LIBS) 
graphics_xforms_la_CFLAGS = $(GRAPHICS_XFORMS_CFLAGS) $(GRAPHICS_GTK_CFLAGS) $(SERVICE_CFLAGS) -I$(top_srcdir)/base
dist_conf_DATA = graphics_xforms.conf
include_HEADERS = graphics_xforms.h

MAINTAINERCLEANFILES = Makefile.in Makefile

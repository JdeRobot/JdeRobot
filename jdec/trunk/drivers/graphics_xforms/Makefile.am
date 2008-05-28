if ENABLE_GRAPHICS_XFORMS
driver_LTLIBRARIES = graphics_xforms.la
endif

graphics_xforms_la_SOURCES = graphics_xforms.c graphics_xforms.h
graphics_xforms_la_LDFLAGS = -module -avoid-version @GRAPHICS_XFORMS_LIBS@
graphics_xforms_la_CFLAGS = @GRAPHICS_XFORMS_CFLAGS@ -I$(top_srcdir)/base

EXTRA_DIST = graphics_xforms.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

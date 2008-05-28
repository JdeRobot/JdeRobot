if ENABLE_GRAPHICS_GTK
driver_LTLIBRARIES = graphics_gtk.la
endif

graphics_gtk_la_SOURCES = graphics_gtk.c graphics_gtk.h
graphics_gtk_la_LDFLAGS = -module -avoid-version @GRAPHICS_GTK_LIBS@
graphics_gtk_la_CFLAGS = @GRAPHICS_GTK_CFLAGS@ -I$(top_srcdir)/base

EXTRA_DIST = graphics_gtk.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

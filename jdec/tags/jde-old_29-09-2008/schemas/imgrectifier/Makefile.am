if ENABLE_IMGRECTIFIER
schema_LTLIBRARIES = imgrectifier.la
endif

imgrectifier_la_SOURCES = imgrectifier.c imgrectifier.h
imgrectifier_la_LDFLAGS = -module -avoid-version $(IMGRECTIFIER_LIBS)
imgrectifier_la_LIBADD =
imgrectifier_la_CFLAGS = $(IMGRECTIFIER_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
dist_glade_DATA = imgrectifier.glade

MAINTAINERCLEANFILES = Makefile.in Makefile

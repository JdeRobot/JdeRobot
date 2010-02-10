if ENABLE_CALIBRADOR
schema_LTLIBRARIES = calibrador.la
endif

calibrador_la_SOURCES = calibrador.c calibrador.h clasificator.c clasificator.h segment.c segment.h calibradorgui.fd
nodist_calibrador_la_SOURCES = calibradorgui.c calibradorgui.h
calibrador_la_LDFLAGS = -module -avoid-version $(CALIBRADOR_LIBS)
calibrador_la_LIBADD = $(top_srcdir)/libs/colorspaces/libcolorspaces.la $(top_srcdir)/libs/progeo/libprogeo.la
calibrador_la_CFLAGS = $(CALIBRADOR_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/colorspaces -I$(top_srcdir)/libs/progeo -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = calibradorgui.h
CLEANFILES = calibradorgui.c calibradorgui.h
calibradorgui.h: calibradorgui.fd
	$(FDESIGN) -convert calibradorgui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile
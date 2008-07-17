if ENABLE_INTROROB
schema_LTLIBRARIES = introrob.la
endif

introrob_la_SOURCES = introrob.c introrob.h navegacion.c navegacion.h introrobgui.fd
nodist_introrob_la_SOURCES = introrobgui.c introrobgui.h
introrob_la_LDFLAGS = -module -avoid-version $(INTROROB_LIBS)
introrob_la_LIBADD = $(top_srcdir)/libs/pioneer/libpioneer.la
introrob_la_CFLAGS = $(INTROROB_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/pioneer -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = introrobgui.h
CLEANFILES = introrobgui.c introrobgui.h
introrobgui.h: introrobgui.fd
	$(FDESIGN) -convert introrobgui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

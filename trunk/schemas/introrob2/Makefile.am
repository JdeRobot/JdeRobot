if ENABLE_INTROROB
schema_LTLIBRARIES = introrob2.la
endif

introrob2_la_SOURCES = introrob2.c introrob2.h navegacion.c navegacion.h introrob2gui.fd
nodist_introrob2_la_SOURCES = introrob2gui.c introrob2gui.h
introrob2_la_LDFLAGS = -module -avoid-version $(INTROROB_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
introrob2_la_LIBADD = $(top_srcdir)/libs/pioneer/libpioneer.la $(top_srcdir)/interfaces/libinterfaces.la
introrob2_la_CFLAGS = $(INTROROB_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/pioneer -I$(top_srcdir)/services/graphics_xforms -I$(top_srcdir)/interfaces
BUILT_SOURCES = introrob2gui.h
CLEANFILES = introrob2gui.c introrob2gui.h
introrob2gui.h: introrob2gui.fd
	$(FDESIGN) -convert introrob2gui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_EXTRINSICS
schema_LTLIBRARIES = extrinsics.la
endif

extrinsics_la_SOURCES = extrinsics.c extrinsics.h extrinsicsgui.fd
nodist_extrinsics_la_SOURCES = extrinsicsgui.c extrinsicsgui.h
extrinsics_la_LDFLAGS = -module -avoid-version $(EXTRINSICS_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
extrinsics_la_LIBADD = $(top_srcdir)/libs/progeo/libprogeo.la
extrinsics_la_CFLAGS = $(EXTRINSICS_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/progeo -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = extrinsicsgui.h
CLEANFILES = extrinsicsgui.c extrinsicsgui.h
extrinsicsgui.h: extrinsicsgui.fd
	$(FDESIGN) -convert extrinsicsgui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

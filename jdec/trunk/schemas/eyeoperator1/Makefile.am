if ENABLE_EYEOPERATOR
schema_LTLIBRARIES = eyeoperator1.la
endif

eyeoperator1_la_SOURCES = eyeoperator1.c eyeoperator1.h eyeoperator1gui.fd
nodist_eyeoperator1_la_SOURCES = eyeoperator1gui.c eyeoperator1gui.h
eyeoperator1_la_LDFLAGS = -module -avoid-version $(EYEOPERATOR_LIBS) $(IPP_LIBS)
eyeoperator1_la_LIBADD =
eyeoperator1_la_CFLAGS = $(EYEOPERATOR_CFLAGS) $(IPP_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/drivers/graphics_xforms -I../opflow
BUILT_SOURCES = eyeoperator1gui.h
CLEANFILES = eyeoperator1gui.c eyeoperator1gui.h
eyeoperator1gui.h: eyeoperator1gui.fd
	$(FDESIGN) -convert eyeoperator1gui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

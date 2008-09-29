if ENABLE_EYEOPERATOR
schema_LTLIBRARIES = eyeoperator4.la
endif

eyeoperator4_la_SOURCES = eyeoperator4.c eyeoperator4.h eyeoperator4gui.fd
nodist_eyeoperator4_la_SOURCES = eyeoperator4gui.c eyeoperator4gui.h
eyeoperator4_la_LDFLAGS = -module -avoid-version $(EYEOPERATOR_LIBS) $(IPP_LIBS)
eyeoperator4_la_LIBADD =
eyeoperator4_la_CFLAGS = $(EYEOPERATOR_CFLAGS) $(IPP_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/drivers/graphics_xforms -I../opflow
BUILT_SOURCES = eyeoperator4gui.h
CLEANFILES = eyeoperator4gui.c eyeoperator4gui.h
eyeoperator4gui.h: eyeoperator4gui.fd
	$(FDESIGN) -convert eyeoperator4gui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

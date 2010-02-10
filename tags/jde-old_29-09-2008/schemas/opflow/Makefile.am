if ENABLE_OPFLOW
schema_LTLIBRARIES = opflow.la
endif

opflow_la_SOURCES = opflow.c opflow.h opflowgui.fd
nodist_opflow_la_SOURCES = opflowgui.c opflowgui.h
opflow_la_LDFLAGS = -module -avoid-version $(OPFLOW_LIBS) $(IPP_LIBS)
opflow_la_LIBADD =
opflow_la_CFLAGS = $(OPFLOW_CFLAGS) $(IPP_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/drivers/graphics_xforms
BUILT_SOURCES = opflowgui.h
CLEANFILES = opflowgui.c opflowgui.h
opflowgui.h: opflowgui.fd
	$(FDESIGN) -convert opflowgui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

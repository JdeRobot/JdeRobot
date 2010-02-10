if ENABLE_OPENGLDEMO
schema_LTLIBRARIES = opengldemo.la
endif

opengldemo_la_SOURCES = opengldemo.c opengldemo.h opengldemogui.fd
nodist_opengldemo_la_SOURCES = opengldemogui.c opengldemogui.h
opengldemo_la_LDFLAGS = -module -avoid-version $(OPENGLDEMO_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
opengldemo_la_CFLAGS = $(OPENGLDEMO_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = opengldemogui.h
CLEANFILES = opengldemogui.c opengldemogui.h
opengldemogui.h: opengldemogui.fd
	$(FDESIGN) -convert opengldemogui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

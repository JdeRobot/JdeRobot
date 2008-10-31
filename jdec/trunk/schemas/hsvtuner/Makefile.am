if ENABLE_HSVTUNER
schema_LTLIBRARIES = hsvtuner.la
endif

hsvtuner_la_SOURCES = hsvtuner.c hsvtuner.h hsvtunergui.fd
nodist_hsvtuner_la_SOURCES = hsvtunergui.c hsvtunergui.h
hsvtuner_la_LDFLAGS = -module -avoid-version $(HSVTUNER_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
hsvtuner_la_LIBADD = $(top_srcdir)/libs/colorspaces/libcolorspaces.la
hsvtuner_la_CFLAGS = $(HSVTUNER_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/colorspaces -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = hsvtunergui.h
CLEANFILES = hsvtunergui.c hsvtunergui.h
hsvtunergui.h: hsvtunergui.fd
	$(FDESIGN) -convert hsvtunergui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

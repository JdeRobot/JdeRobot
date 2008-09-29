if ENABLE_TELEOPERATOR
schema_LTLIBRARIES = teleoperator.la
endif

teleoperator_la_SOURCES = teleoperator.c teleoperator.h pioneeropengl.c pioneeropengl.h teleoperatorgui.fd
nodist_teleoperator_la_SOURCES = teleoperatorgui.c teleoperatorgui.h
teleoperator_la_LDFLAGS = -module -avoid-version $(TELEOPERATOR_LIBS)
teleoperator_la_LIBADD = $(top_srcdir)/libs/pioneer/libpioneer.la
teleoperator_la_CFLAGS = $(TELEOPERATOR_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/pioneer -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = teleoperatorgui.h
CLEANFILES = teleoperatorgui.c teleoperatorgui.h
teleoperatorgui.h: teleoperatorgui.fd
	$(FDESIGN) -convert teleoperatorgui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

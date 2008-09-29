if ENABLE_MASTERGUI
schema_LTLIBRARIES = mastergui.la
endif

mastergui_la_SOURCES = mastergui.c mastergui.h masterguigui.fd
nodist_mastergui_la_SOURCES = masterguigui.c masterguigui.h
mastergui_la_LDFLAGS = -module -avoid-version $(MASTERGUI_LIBS)
mastergui_la_LIBADD =
mastergui_la_CFLAGS = $(MASTERGUI_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_gtk
BUILT_SOURCES = masterguigui.h
CLEANFILES = masterguigui.c masterguigui.h
masterguigui.h: masterguigui.fd
	$(FDESIGN) -convert masterguigui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

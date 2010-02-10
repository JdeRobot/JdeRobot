if ENABLE_RECORDER
schema_LTLIBRARIES = recorder.la
endif

recorder_la_SOURCES = recorder.c recorder.h recordergui.fd
nodist_recorder_la_SOURCES = recordergui.c recordergui.h
recorder_la_LDFLAGS = -module -avoid-version $(RECORDER_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
recorder_la_CFLAGS = $(RECORDER_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = recordergui.h
CLEANFILES = recordergui.c recordergui.h
recordergui.h: recordergui.fd
	$(FDESIGN) -convert recordergui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

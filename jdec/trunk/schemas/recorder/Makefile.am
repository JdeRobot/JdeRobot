if ENABLE_RECORDER
schema_LTLIBRARIES = recorder.la
endif

recorder_la_SOURCES = recorder.c recorder.h recordergui.fd
nodist_recorder_la_SOURCES = recordergui.c recordergui.h
recorder_la_LDFLAGS = -module -avoid-version $(RECORDER_LIBS)
recorder_la_LIBADD =
recorder_la_CFLAGS = $(RECORDER_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/drivers/graphics_xforms
BUILT_SOURCES = recordergui.h
CLEANFILES = recordergui.c recordergui.h
recordergui.h: recordergui.fd
	$(FDESIGN) -I "X11/forms.h" -convert recordergui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

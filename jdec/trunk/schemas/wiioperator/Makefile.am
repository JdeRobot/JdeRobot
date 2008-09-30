if ENABLE_WIIOPERATOR
schema_LTLIBRARIES = wiioperator.la
endif

wiioperator_la_SOURCES = wiioperator.c wiioperator.h
wiioperator_la_LDFLAGS = -module -avoid-version $(WIIOPERATOR_LIBS)
wiioperator_la_LIBADD =
wiioperator_la_CFLAGS = $(WIIOPERATOR_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/drivers/wiimote

MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_PANTILT
driver_LTLIBRARIES = pantilt.la
endif

pantilt_la_SOURCES = pantilt.c
pantilt_la_LDFLAGS = -module -avoid-version @PANTILT_LIBS@
pantilt_la_CFLAGS = @PANTILT_CFLAGS@ -I$(top_srcdir)/base
pantilt_ladir = @CFGDIR@
dist_pantilt_la_DATA = pantilt.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

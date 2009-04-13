if ENABLE_PANTILT
driver_LTLIBRARIES = pantilt.la
endif

pantilt_la_SOURCES = pantilt.c
pantilt_la_LDFLAGS = -module -avoid-version $(PANTILT_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
pantilt_la_CFLAGS = $(PANTILT_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir)/base
dist_conf_DATA = pantilt.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

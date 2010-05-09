if ENABLE_VIDEO4LINUX
driver_LTLIBRARIES = video4linux2.la
endif

video4linux2_la_SOURCES = video4linux2.c
video4linux2_la_LDFLAGS = -module -avoid-version $(VIDEO4LINUX_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
video4linux2_la_CFLAGS = $(VIDEO4LINUX_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base
dist_conf_DATA = video4linux2.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

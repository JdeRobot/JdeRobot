if ENABLE_VIDEO4LINUX
driver_LTLIBRARIES = video4linux.la
endif

video4linux_la_SOURCES = video4linux.c
video4linux_la_LDFLAGS = -module -avoid-version $(VIDEO4LINUX_LIBS)
video4linux_la_CFLAGS = $(VIDEO4LINUX_CFLAGS) -I$(top_srcdir)/base
dist_conf_DATA = video4linux.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

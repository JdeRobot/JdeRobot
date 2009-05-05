if ENABLE_OPENCV
driver_LTLIBRARIES = opencv.la
endif

firewire_la_SOURCES = opencv.c
firewire_la_LDFLAGS = -module -avoid-version $(DRIVER_LDFLAGS) $(DRIVER_LIBS) $(OPENCV_LIBS)
firewire_la_CFLAGS = $(OPENCV_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base 
dist_conf_DATA = opencv.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

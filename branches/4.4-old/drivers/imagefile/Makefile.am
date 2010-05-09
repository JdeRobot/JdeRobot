if ENABLE_IMAGEFILE
driver_LTLIBRARIES = imagefile.la
endif

imagefile_la_SOURCES = imagefile.c
imagefile_la_LDFLAGS = -module -avoid-version $(IMAGEFILE_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
imagefile_la_CFLAGS = $(IMAGEFILE_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base 
dist_conf_DATA = imagefile.conf *.ppm *.pnm

MAINTAINERCLEANFILES = Makefile.in Makefile

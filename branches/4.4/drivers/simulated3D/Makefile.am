if ENABLE_SIMULATED3D
driver_LTLIBRARIES = simulated3D.la
endif

simulated3D_la_SOURCES = simulated3D.c
simulated3D_la_LDFLAGS = -module -avoid-version $(SIMULATED3D_LIBS) $(DRIVER_LDFLAGS) $(DRIVER_LIBS)
simulated3D_la_LIBADD = $(top_srcdir)/libs/progeo/libprogeo.la
simulated3D_la_CFLAGS = $(SIMULATED3D_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir) -I$(top_srcdir)/base -I$(top_srcdir)/libs/progeo 
dist_conf_DATA = simulated3D.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

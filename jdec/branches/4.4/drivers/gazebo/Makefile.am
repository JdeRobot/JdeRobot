if ENABLE_GAZEBO
driver_LTLIBRARIES = gazebo.la
endif

gazebo_la_SOURCES = gazebo.c
gazebo_la_LDFLAGS = -module -avoid-version $(DRIVER_LDFLAGS) $(DRIVER_LIBS) $(GAZEBO_LIBS)
gazebo_la_CFLAGS = $(GAZEBO_CFLAGS) $(DRIVER_CFLAGS) -I$(top_srcdir)/base
dist_conf_DATA = gazebo.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

if ENABLE_GAZEBO
driver_LTLIBRARIES = gazebo.la
endif

gazebo_la_SOURCES = gazebo.c
gazebo_la_LDFLAGS = -module -avoid-version @GAZEBO_LIBS@
gazebo_la_CFLAGS = @GAZEBO_CFLAGS@ -I$(top_srcdir)/base
gazebo_ladir = @CFGDIR@
dist_gazebo_la_DATA = gazebo.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

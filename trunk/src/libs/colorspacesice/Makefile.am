lib_LTLIBRARIES = libcolorspacesice.la

libcolorspacesice_ladir = $(includedir)/colorspacesice
libcolorspacesice_la_SOURCES = image.cpp
libcolorspacesice_la_LIBADD = $(top_srcdir)/src/libs/colorspaces/libcolorspaces.la \
				$(top_srcdir)/src/libs/colorspaces/libcolorspaces++.la \
				$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la
libcolorspacesice_la_CPPFLAGS = -I.. -I$(top_srcdir)/src/interfaces/cpp

libcolorspacesice_la_HEADERS = image.h

EXTRA_DIST = Changelog
MAINTAINERCLEANFILES = Makefile.in Makefile

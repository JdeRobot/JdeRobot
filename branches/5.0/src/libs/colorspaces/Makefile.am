lib_LTLIBRARIES = libcolorspaces.la libcolorspaces++.la

libcolorspaces_ladir = $(includedir)/colorspaces
libcolorspaces_la_SOURCES = rgb2hsv.c rgb2yuv.c formats.c color.c image.c imagetest.c
libcolorspaces_la_LDFLAGS = -lm
libcolorspaces_la_CPPFLAGS = -I..

libcolorspaces___la_SOURCES = formats++.cpp image++.cpp
libcolorspaces___la_LIBADD = libcolorspaces.la
libcolorspaces___la_CPPFLAGS = -I..

libcolorspaces_la_HEADERS = formats.h color.h image.h imagetest.h formats++.h image++.h

include_HEADERS = colorspaces.h

EXTRA_DIST = Changelog
MAINTAINERCLEANFILES = Makefile.in Makefile

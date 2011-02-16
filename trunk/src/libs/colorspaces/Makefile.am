lib_LTLIBRARIES = libcolorspaces.la 
if ENABLE_LIBCOLORSPACESMM
lib_LTLIBRARIES += libcolorspacesmm.la
endif

libcolorspaces_ladir = $(includedir)/colorspaces
libcolorspaces_la_SOURCES = rgb2hsv.c rgb2yuv.c
libcolorspaces_la_LDFLAGS = -lm
libcolorspaces_la_CFLAGS =
libcolorspaces_la_HEADERS = colorspaces.h colorspacesmm.h

libcolorspacesmm_ladir = $(includedir)/colorspaces
libcolorspacesmm_la_SOURCES = imagecv.cpp
libcolorspacesmm_la_LDFLAGS = $(COLORSPACESMM_LDFLAGS)
libcolorspacesmm_la_CPPFLAGS = $(COLORSPACESMM_CPPFLAGS)
libcolorspacesmm_la_HEADERS = imagecv.h uncopyable.h

EXTRA_DIST = Changelog
MAINTAINERCLEANFILES = Makefile.in Makefile

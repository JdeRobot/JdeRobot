if ENABLE_LIBJDEROBOTICE
lib_LTLIBRARIES = libJderobotIce.la
endif

libJderobotIce_ladir = $(includedir)/jderobotice
libJderobotIce_la_SOURCES = component.cpp context.cpp application.cpp \
				componentthread.cpp componentthread.h \
				statusImpl.h tracerImpl.h catchutils.cpp
libJderobotIce_la_CPPFLAGS = $(GEARBOX_CPPFLAGS) -I$(top_srcdir)/src/libs \
				-I$(top_srcdir)/src/interfaces/cpp
libJderobotIce_la_LDFLAGS = $(LIBICE) $(LIBICEUTIL) $(LIBICEGRID) $(GEARBOX_LDFLAGS) \
				$(LIBGBXUTILACFR) $(LIBGBXICEUTILACFR)
libJderobotIce_la_HEADERS = jderobotice.h component.h context.h application.h \
				exceptions.h catchutils.h

EXTRA_DIST = Changelog
MAINTAINERCLEANFILES = Makefile.in Makefile
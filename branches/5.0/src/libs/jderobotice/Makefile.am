if ENABLE_LIBJDEROBOTICE
lib_LTLIBRARIES = libJderobotIce.la
endif

libJderobotIce_ladir = $(includedir)/jderobotice
libJderobotIce_la_SOURCES = component.cpp context.cpp application.cpp \
				componentthread.cpp componentthread.h \
				statusImpl.h tracerImpl.h catchutils.cpp \
				subsystemthread.cpp interfaceconnect.cpp
libJderobotIce_la_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS)
libJderobotIce_la_LDFLAGS =  $(JDEROBOTICE_LDFLAGS)
libJderobotIce_la_HEADERS = component.h context.h application.h \
				exceptions.h catchutils.h subsystemthread.h \
				interfaceconnect.h jderobotice.h

EXTRA_DIST = Changelog
MAINTAINERCLEANFILES = Makefile.in Makefile

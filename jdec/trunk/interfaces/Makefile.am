lib_LTLIBRARIES = libinterfaces.la

# See http://www.gnu.org/software/libtool/manual/libtool.html#Versioning
# for versioning policy.
libinterfaces_la_SOURCES = interface.c encoders.c motors.c laser.c
libinterfaces_la_LDFLAGS = $(INTERFACES_LIBS) $(PYTHON_LDFLAGS) $(PYTHON_EXTRA_LDFLAGS) \
    $(PYTHON_EXTRA_LIBS) -rdynamic -version-info 0:0:0
libinterfaces_la_LIBADD = $(top_srcdir)/base/libjde.la
libinterfaces_la_CFLAGS = $(PYTHON_CPPFLAGS) -I$(top_srcdir)/base

include_HEADERS = interface.h encoders.h motors.h laser.h varcolor.h wiimote.h

SUBDIRS = . swig
MAINTAINERCLEANFILES = Makefile.in Makefile

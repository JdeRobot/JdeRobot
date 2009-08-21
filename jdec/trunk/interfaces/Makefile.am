lib_LTLIBRARIES = libinterfaces.la
bin_PROGRAMS = test1 test2

# See http://www.gnu.org/software/libtool/manual/libtool.html#Versioning
# for versioning policy.
libinterfaces_la_SOURCES = interface.c encoders.c motors.c laser.c varcolor.c
libinterfaces_la_LDFLAGS = $(INTERFACES_LIBS) $(PYTHON_LDFLAGS) $(PYTHON_EXTRA_LDFLAGS) \
    $(PYTHON_EXTRA_LIBS) -rdynamic -version-info 0:0:0
libinterfaces_la_LIBADD = $(top_srcdir)/base/libjde.la $(top_srcdir)/libs/simclist/libsimclist.la
libinterfaces_la_CFLAGS = $(PYTHON_CPPFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/simclist

test1_SOURCES = test1.c
test1_CFLAGS = -I$(top_srcdir)/base
test1_LDADD = libinterfaces.la

test2_SOURCES = test2.c
test2_CFLAGS = -I$(top_srcdir)/base
test2_LDADD = libinterfaces.la

include_HEADERS = interface.h encoders.h motors.h laser.h varcolor.h wiimote.h

SUBDIRS = . swig
MAINTAINERCLEANFILES = Makefile.in Makefile

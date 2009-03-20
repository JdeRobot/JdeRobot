swig_LTLIBRARIES = _motors.la _encoders.la

_motors_la_SOURCES = motors.i
nodist__motors_la_SOURCES = motors_wrap.c
_motors_la_LDFLAGS = -module -avoid-version
_motors_la_LIBADD = $(top_srcdir)/interfaces/libinterfaces.la
_motors_la_CFLAGS = -I$(top_srcdir)/base -I$(top_srcdir)/interfaces $(SWIG_PYTHON_CPPFLAGS)

_encoders_la_SOURCES = encoders.i
nodist__encoders_la_SOURCES = encoders_wrap.c
_encoders_la_LDFLAGS = -module -avoid-version
_encoders_la_LIBADD = $(top_srcdir)/interfaces/libinterfaces.la
_encoders_la_CFLAGS = -I$(top_srcdir)/base -I$(top_srcdir)/interfaces $(SWIG_PYTHON_CPPFLAGS)

dist_swig_DATA = motors.py encoders.py
BUILT_SOURCES = motors_wrap.c encoders_wrap.c
CLEANFILES = $(BUILT_SOURCES) motors.py encoders.py
%_wrap.c: %.i
	$(SWIG) $(SWIG_PYTHON_OPT) $<

MAINTAINERCLEANFILES = Makefile.in Makefile

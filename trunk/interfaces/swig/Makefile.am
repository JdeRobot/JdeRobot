swig_LTLIBRARIES = _motors.la _encoders.la _laser.la

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

_laser_la_SOURCES = laser.i
nodist__laser_la_SOURCES = laser_wrap.c
_laser_la_LDFLAGS = -module -avoid-version
_laser_la_LIBADD = $(top_srcdir)/interfaces/libinterfaces.la
_laser_la_CFLAGS = -I$(top_srcdir)/base -I$(top_srcdir)/interfaces $(SWIG_PYTHON_CPPFLAGS)

#_varcolor_la_SOURCES = varcolor.i
#nodist__varcolor_la_SOURCES = varcolor_wrap.c
#_varcolor_la_LDFLAGS = -module -avoid-version
#_varcolor_la_LIBADD = $(top_srcdir)/interfaces/libinterfaces.la
#_varcolor_la_CFLAGS = -I$(top_srcdir)/base -I$(top_srcdir)/interfaces $(SWIG_PYTHON_CPPFLAGS)


#dist_swig_DATA = interface.py motors.py encoders.py laser.py varcolor.py
dist_swig_DATA =  motors.py encoders.py laser.py 
#BUILT_SOURCES = interface_wrap.c motors_wrap.c encoders_wrap.c laser_wrap.c varcolor_wrap.c
BUILT_SOURCES = motors_wrap.c encoders_wrap.c laser_wrap.c
CLEANFILES = $(dist_swig_DATA) $(BUILT_SOURCES)
%_wrap.c: %.i
	$(SWIG) $(SWIG_PYTHON_OPT) -I$(top_srcdir)/base/swig $<

MAINTAINERCLEANFILES = Makefile.in Makefile

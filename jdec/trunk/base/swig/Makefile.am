swig_LTLIBRARIES = _jde.la _schema.la

_jde_la_SOURCES = jde.i
nodist__jde_la_SOURCES = jde_wrap.c
_jde_la_LDFLAGS = -module -avoid-version
_jde_la_LIBADD = $(top_srcdir)/base/libjde.la
_jde_la_CFLAGS = -I$(top_srcdir)/base $(SWIG_PYTHON_CPPFLAGS)

_schema_la_SOURCES = schema.i
nodist__schema_la_SOURCES = schema_wrap.c
_schema_la_LDFLAGS = -module -avoid-version
_schema_la_LIBADD = $(top_srcdir)/base/libjde.la
_schema_la_CFLAGS = -I$(top_srcdir)/base $(SWIG_PYTHON_CPPFLAGS)


dist_swig_DATA = jde.py schema.py pyschema.py
BUILT_SOURCES = jde_wrap.c schema_wrap.c
CLEANFILES = $(BUILT_SOURCES) jde.py schema.py
%_wrap.c: %.i
	$(SWIG) $(SWIG_PYTHON_OPT) $<

MAINTAINERCLEANFILES = Makefile.in Makefile

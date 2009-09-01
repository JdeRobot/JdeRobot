swig_LTLIBRARIES = _jde.la _schema.la _interface.la _hierarchy.la

#initialization
dist_swig_DATA =
BUILT_SOURCES =
CLEANFILES =

_jde_la_SOURCES = jde.i
nodist__jde_la_SOURCES = jde_wrap.c
_jde_la_LDFLAGS = -module -avoid-version
_jde_la_LIBADD = $(top_srcdir)/base/libjde.la
_jde_la_CFLAGS = -I$(top_srcdir)/base $(SWIG_PYTHON_CPPFLAGS)
dist_swig_DATA += jde.py
BUILT_SOURCES += jde_wrap.c
CLEANFILES +=  jde.py jde_wrap.c

_schema_la_SOURCES = schema.i
nodist__schema_la_SOURCES = schema_wrap.c
_schema_la_LDFLAGS = -module -avoid-version
_schema_la_LIBADD = $(top_srcdir)/base/libjde.la
_schema_la_CFLAGS = -I$(top_srcdir)/base $(SWIG_PYTHON_CPPFLAGS)
dist_swig_DATA += schema.py pyschema.py
BUILT_SOURCES += schema_wrap.c
CLEANFILES +=  schema.py schema_wrap.c

_interface_la_SOURCES = interface.i
nodist__interface_la_SOURCES = interface_wrap.c
_interface_la_LDFLAGS = -module -avoid-version
_interface_la_LIBADD = $(top_srcdir)/base/libjde.la
_interface_la_CFLAGS = -I$(top_srcdir)/base $(SWIG_PYTHON_CPPFLAGS)
dist_swig_DATA += interface.py
BUILT_SOURCES += interface_wrap.c
CLEANFILES +=  interface.py interface_wrap.c

_hierarchy_la_SOURCES = hierarchy.i
nodist__hierarchy_la_SOURCES = hierarchy_wrap.c
_hierarchy_la_LDFLAGS = -module -avoid-version
_hierarchy_la_LIBADD = $(top_srcdir)/base/libjde.la
_hierarchy_la_CFLAGS = -I$(top_srcdir)/base $(SWIG_PYTHON_CPPFLAGS)
dist_swig_DATA += hierarchy.py
BUILT_SOURCES += hierarchy_wrap.c
CLEANFILES +=  hierarchy.py hierarchy_wrap.c


%_wrap.c: %.i
	$(SWIG) $(SWIG_PYTHON_OPT) $<

MAINTAINERCLEANFILES = Makefile.in Makefile

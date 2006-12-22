#libconfig
if USE_INSTALLED_LIBCONFIG
	INCCONFIG =
	LIBCONFIG = @CONFIG_LINK@
else
	INCCONFIG = -I$(top_srcdir)/@CONFIG_LIB_DIR@
	LIBCONFIG = $(top_builddir)/@CONFIG_LIB_DIR@/libconfig++.la
endif


AM_CXXFLAGS = @INCLTDL@ $(INCCONFIG) -I$(top_srcdir)/include  @LIBJDEDEPS_CFLAGS@


lib_LTLIBRARIES = libjde+.la
bin_PROGRAMS = jdescope


#jde+ go to jde_. See doc automake->Canonicalization
libjde__la_SOURCES = hierarchy.cpp data.cpp schematypes.cpp \
			schemainstance.cpp schemastate.cpp \
			schemaimplementation.cpp isc.cpp util.cpp \
			message.cpp
libjde__la_LIBADD = @LIBLTDL@ 
libjde__la_LDFLAGS = @LIBJDEDEPS_LIBS@

jdescope_SOURCES = jdescope.cpp
jdescope_LDADD = libjde+.la
jdescope_LDFLAGS = @JDESCOPEDEPS_LIBS@ --export-dynamic
jdescope_CXXFLAGS = $(AM_CXXFLAGS) @JDESCOPEDEPS_CFLAGS@
jdescopedir = $(bindir)
dist_jdescope_DATA = jdescope.glade

pixmapdir = $(pkgdatadir)/pixmap
dist_pixmap_DATA = pixmap/*.svg

EXTRA_DIST = tests

#libconfig
#if USE_INSTALLED_LIBCONFIG
#	INCCONFIG =
#	LIBCONFIG = @CONFIG_LINK@
#else
#	INCCONFIG = -I$(top_srcdir)/@CONFIG_LIB_DIR@
#	LIBCONFIG = $(top_builddir)/@CONFIG_LIB_DIR@/libconfig++.la
#endif


AM_CXXFLAGS = @INCLTDL@ -I$(top_srcdir)/corejde+/include  -I$(top_srcdir)/tinyxml @LIBJDEDEPS_CFLAGS@ 

#$(INCCONFIG) 


lib_LTLIBRARIES = libjde+.la
bin_PROGRAMS = jde+shell


#jde+ go to jde_. See doc automake->Canonicalization
libjde__la_SOURCES = data.cpp \
			hierarchy.cpp \
			isc.cpp \
			message.cpp \
			schemaimplementation.cpp \
			schemainstance.cpp \
			schemainterface.cpp \
			schemastate.cpp \
			schematypes.cpp \
			util.cpp \
			tinystr.cpp tinyxml.cpp \
			tinyxmlerror.cpp tinyxmlparser.cpp

libjde__la_LIBADD = @LIBLTDL@
#$(top_srcdir)/tinyxml/libtinyxml.la
libjde__la_LDFLAGS = @LIBJDEDEPS_LIBS@

jde_shell_SOURCES = jde+shell.cpp
jde_shell_LDADD = libjde+.la
jde_shell_LDFLAGS = @READLINE_LIB@ --export-dynamic

#jdescope_SOURCES = jdescope.cpp
#jdescope_LDADD = libjde+.la
#jdescope_LDFLAGS = @JDESCOPEDEPS_LIBS@ --export-dynamic
#jdescope_CXXFLAGS = $(AM_CXXFLAGS) @JDESCOPEDEPS_CFLAGS@
#jdescopedir = $(bindir)
#dist_jdescope_DATA = jdescope.glade

#pixmapdir = $(pkgdatadir)/pixmap
#dist_pixmap_DATA = pixmap/*.svg

EXTRA_DIST = tests

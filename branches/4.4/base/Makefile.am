lib_LTLIBRARIES = libjde.la
bin_PROGRAMS = jderobot

# See http://www.gnu.org/software/libtool/manual/libtool.html#Versioning
# for versioning policy.
libjde_la_SOURCES = jderobot.c hierarchy.c loader.c schema.c interface.c
#libjde_la_LDFLAGS = $(JDE_LIBS) -rdynamic -version-info 0:0:0
libjde_la_LDFLAGS = $(JDE_LIBS) $(PYTHON_LDFLAGS) -version-info 0:0:0
libjde_la_CPPFLAGS = $(PYTHON_CPPFLAGS) -D CONFIGDIR=\"$(confdir)\" @CPPFLAGS@
libjde_la_CFLAGS = -I$(top_srcdir)/libs/simclist
libjde_la_LIBADD = $(top_srcdir)/libs/simclist/libsimclist.la
jderobot_SOURCES = jdeshell.c
nodist_jderobot_SOURCES = jderobot.conf
jderobot_LDFLAGS = $(JDESHELL_LIBS)
jderobot_LDADD = libjde.la
dist_conf_DATA = jderobot.conf

include_HEADERS = jde.h hierarchy.h loader.h schema.h interface.h

edit = sed \
	-e 's|@pkgdatadir[@]|$(pkgdatadir)|g' \
	-e 's|@driverdir[@]|$(driverdir)|g' \
	-e 's|@schemadir[@]|$(schemadir)|g' \
	-e 's|@servicedir[@]|$(servicedir)|g' \
	-e 's|@gladedir[@]|$(gladedir)|g'

jderobot.conf: Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	mv $@.tmp $@

jderobot.conf: jderobot.conf.in

BUILT_SOURCES = jderobot.conf
CLEANFILES = $(BUILT_SOURCES)
MAINTAINERCLEANFILES = Makefile.in Makefile
SUBDIRS= . swig

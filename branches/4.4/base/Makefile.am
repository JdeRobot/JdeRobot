lib_LTLIBRARIES = libjde.la
bin_PROGRAMS = jderobot

AM_CPPFLAGS = -D CONFIGDIR=\"$(confdir)\" @CPPFLAGS@

# See http://www.gnu.org/software/libtool/manual/libtool.html#Versioning
# for versioning policy.
libjde_la_SOURCES = jderobot.c schema.c loader.c
libjde_la_LDFLAGS = $(JDE_LIBS) $(PYTHON_LDFLAGS) $(PYTHON_EXTRA_LDFLAGS) \
			 $(PYTHON_EXTRA_LIBS) -rdynamic -version-info 0:0:0
libjde_la_CFLAGS = $(PYTHON_CPPFLAGS)
#libjde_la_LDFLAGS = $(JDE_LIBS) -version-info 0:0:0
jderobot_SOURCES = jdeshell.c
nodist_jderobot_SOURCES = jderobot.conf
jderobot_LDFLAGS = $(JDESHELL_LIBS)
jderobot_LDADD = libjde.la
dist_conf_DATA = jderobot.conf

include_HEADERS = jde.h jde_private.h loader.h schema.h

edit = sed \
	-e 's|@driverdir[@]|$(driverdir)|g' \
	-e 's|@schemadir[@]|$(schemadir)|g' \
	-e 's|@servicedir[@]|$(servicedir)|g' \
	-e 's|@gladedir[@]|$(gladedir)|g'

jderobot.conf: Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	mv $@.tmp $@

jderobot.conf: jderobot.conf.in

SUBDIRS = . swig
BUILTSOURCES = jderobot.conf
CLEANFILES = jderobot.conf
MAINTAINERCLEANFILES = Makefile.in Makefile

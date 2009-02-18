lib_LTLIBRARIES = libjde.la
bin_PROGRAMS = jde

AM_CPPFLAGS = -D CONFIGDIR=\"$(confdir)\" @CPPFLAGS@

# See http://www.gnu.org/software/libtool/manual/libtool.html#Versioning
# for versioning policy.
libjde_la_SOURCES = jde.c
#libjde_la_LDFLAGS = $(JDE_LIBS) -rdynamic -version-info 0:0:0
libjde_la_LDFLAGS = $(JDE_LIBS) -version-info 0:0:0
jde_SOURCES = jdeshell.c
nodist_jde_SOURCES = jde.conf
jde_LDFLAGS = $(JDESHELL_LIBS)
jde_LDADD = libjde.la
dist_conf_DATA = jde.conf

include_HEADERS = jde.h jde_private.h

edit = sed \
	-e 's|@driverdir[@]|$(driverdir)|g' \
	-e 's|@schemadir[@]|$(schemadir)|g' \
	-e 's|@servicedir[@]|$(servicedir)|g' \
	-e 's|@gladedir[@]|$(gladedir)|g'

jde.conf: Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	mv $@.tmp $@

jde.conf: jde.conf.in

BUILTSOURCES = jde.conf
CLEANFILES = jde.conf
MAINTAINERCLEANFILES = Makefile.in Makefile

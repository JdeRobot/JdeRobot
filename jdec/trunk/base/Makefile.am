bin_PROGRAMS = jde

AM_CPPFLAGS = -D CONFIGDIR=\"$(confdir)\" @CPPFLAGS@

jde_SOURCES = jde.c
nodist_jde_SOURCES = jde.conf
jde_LDFLAGS = -rdynamic -lreadline
dist_conf_DATA = jde.conf

include_HEADERS = jde.h

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

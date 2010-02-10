bin_PROGRAMS = randomwalk

dist_conf_DATA = randomwalk.conf

randomwalk_SOURCES = randomwalk.c
randomwalk_LDFLAGS = $(JDESHELL_LIBS)
randomwalk_CFLAGS = -I$(top_srcdir)/base -I$(top_srcdir)/interfaces
randomwalk_LDADD = $(top_srcdir)/base/libjde.la $(top_srcdir)/interfaces/libinterfaces.la

edit = sed \
	-e 's|@pkgdatadir[@]|$(pkgdatadir)|g' \
	-e 's|@driverdir[@]|$(driverdir)|g' \
	-e 's|@schemadir[@]|$(schemadir)|g' \
	-e 's|@servicedir[@]|$(servicedir)|g' \
	-e 's|@gladedir[@]|$(gladedir)|g'

randomwalk.conf: Makefile
	rm -f $@ $@.tmp
	$(edit) '$@.in' >$@.tmp
	mv $@.tmp $@

randomwalk.conf: randomwalk.conf.in

BUILT_SOURCES = randomwalk.conf
CLEANFILES = $(BUILT_SOURCES)
MAINTAINERCLEANFILES = Makefile.in Makefile

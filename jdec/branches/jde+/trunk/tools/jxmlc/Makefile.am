AUTOMAKE_OPTIONS = no-dependencies
MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = jxmlc
EXTRA_DIST = jxmlc.in validation

nodist_bin_SCRIPTS = jxmlc

jxmlcdatadir = @pkgdatadir@/jxmlc
dist_jxmlcdata_DATA = iface_tpl.cpp iface_tpl.h schema_tpl.cpp schema_tpl.h

jxmlc: jxmlc.in
	sed -e 's,[@]jxmlcdatadir[@],$(jxmlcdatadir),g' \
	< $^ > $@

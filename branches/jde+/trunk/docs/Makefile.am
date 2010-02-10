MAINTAINERCLEANFILES = Makefile.in Makefile
EXTRA_DIST = doxygen.conf doxygen.conf.in jde+.xmi

if DOXY
noinst_DATA = doxy

doxy: doxygen.conf
	$(DOXYGEN) doxygen.conf
	touch doxy

endif

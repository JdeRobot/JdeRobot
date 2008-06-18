DOCDIRS = progeo-manual manual howto-ipp howto-gazebo
DOCS = howto-gazebo/howto-gazebo.pdf howto-ipp/howto-ipp.pdf manual/jdecmanual.pdf progeo-manual/progeo-manual.pdf 

all-local: $(DOCS) html/index.html

# Assuming the package knows how to install itself
install-data-local: $(DOCS) html/index.html
	mkdir -p $(DESTDIR)$(docdir); \
	cp -r html $(DESTDIR)$(docdir)/; \
	for i in $(DOCS); do \
		cp $$i $(DESTDIR)$(docdir)/; \
	done

html/index.html: jdec.doxygen
	$(DOXYGEN) jdec.doxygen

howto-gazebo/howto-gazebo.pdf: howto-gazebo/howto-gazebo.tex
	cd howto-gazebo && $(MAKE) $(AM_MAKEFLAGS) all

howto-ipp/howto-ipp.pdf: howto-ipp/howto-ipp.tex
	cd howto-ipp && $(MAKE) $(AM_MAKEFLAGS) all

manual/jdecmanual.pdf: manual/jdecmanual.tex
	cd manual && $(MAKE) $(AM_MAKEFLAGS) all

progeo-manual/progeo-manual.pdf: progeo-manual/progeo-manual.tex
	cd progeo-manual && $(MAKE) $(AM_MAKEFLAGS) all

clean-local:
	rm -rf html; \
	for i in $(DOCDIRS); do \
		(cd $$i && $(MAKE) $(AM_MAKEFLAGS) clean); \
	done

uninstall-local:
	for i in $(DOCDIRS); do \
		(cd $$i && $(MAKE) $(AM_MAKEFLAGS) uninstall); \
	done

EXTRA_DIST = $(DX_CONFIG) $(DOCDIRS)
MAINTAINERCLEANFILES = Makefile.in Makefile

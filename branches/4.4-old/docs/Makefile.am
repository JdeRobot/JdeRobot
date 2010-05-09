if ENABLE_DOCS
all-local: html/index.html

# Assuming the package knows how to install itself
install-data-local: html/index.html
	mkdir -p $(DESTDIR)$(docdir); \
	cp -r html $(DESTDIR)$(docdir)/;
endif

html/index.html: jdec.doxygen
	$(DOXYGEN) jdec.doxygen

clean-local:
	rm -rf html;

uninstall-local:

EXTRA_DIST = jdec.doxygen
MAINTAINERCLEANFILES = Makefile.in Makefile

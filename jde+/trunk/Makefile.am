#if USE_INSTALLED_LIBCONFIG
#SUBDIRS_1 =
#else
#SUBDIRS_1 = @CONFIG_LIB_DIR@
#endif

SUBDIRS = $(SUBDIRS_1) libltdl corejde+ docs tools

EXTRA_DIST = AUTHORS ChangeLog NEWS README INSTALL \
	COPYING TODO examples
MAINTAINERCLEANFILES = configure aclocal.m4 Makefile.in Makefile \
			config/* INSTALL

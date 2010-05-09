if ENABLE_FOLLOWBALL
schema_LTLIBRARIES = followball.la
endif

followball_la_SOURCES = followball.c followball.h followballgui.fd
nodist_followball_la_SOURCES = followballgui.c followballgui.h
followball_la_LDFLAGS = -module -avoid-version $(FOLLOWBALL_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
followball_la_LIBADD =  $(top_srcdir)/libs/colorspaces/libcolorspaces.la
followball_la_CFLAGS = $(FOLLOWBALL_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/libs/colorspaces -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = followballgui.h
CLEANFILES = followballgui.c followballgui.h
followballgui.h: followballgui.fd
	$(FDESIGN) -convert followballgui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

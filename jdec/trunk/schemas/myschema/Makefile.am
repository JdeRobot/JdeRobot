if ENABLE_MYSCHEMA
schema_LTLIBRARIES = myschema.la
endif

myschema_la_SOURCES = myschema.c myschema.h myschemagui.fd
nodist_myschema_la_SOURCES = myschemagui.c myschemagui.h
myschema_la_LDFLAGS = -module -avoid-version $(MYSCHEMA_LIBS) $(SCHEMA_LDFLAGS) $(SCHEMA_LIBS)
myschema_la_CFLAGS = $(MYSCHEMA_CFLAGS) $(SCHEMA_CFLAGS) -I$(top_srcdir)/base -I$(top_srcdir)/services/graphics_xforms
BUILT_SOURCES = myschemagui.h
CLEANFILES = myschemagui.c myschemagui.h
myschemagui.h: myschemagui.fd
	$(FDESIGN) -convert myschemagui.fd

MAINTAINERCLEANFILES = Makefile.in Makefile

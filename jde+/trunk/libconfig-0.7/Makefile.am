
lib_LTLIBRARIES = libconfig.la libconfig++.la

# Versioning rules ( C:R:A )
# 1. Start with version 0:0:0.
# 2. If the library source code has changed at all, incremement R.
# 3. If any interfaces have been added, removed, or changed, increment C and
#    set R to 0.
# 4. If any interfaces have been added, increment A.
# 5. If any interfaces have been removed, set A to 0.
# For more info see page 27 of the GNU Libtool Manual.

VERINFO = -version-info 1:0:0

PARSER_PREFIX = libconfig_yy

libconfig_la_LDFLAGS = $(VERINFO)

libconfig___la_LDFLAGS = $(VERINFO)

libsrc = libconfig.c scanner.l grammar.y private.h config.tab.h scanner.h
libinc = libconfig.h

libsrc_cpp =  $(libsrc) libconfigcpp.c++
libinc_cpp =  $(libinc) libconfig.h++

libconfig_la_SOURCES = $(libsrc)

libconfig___la_SOURCES = $(libsrc_cpp)

libcflags = -Wall -D_REENTRANT

libconfig_la_CFLAGS = $(libcflags)
libconfig_la_CPPFLAGS = -DPARSER_PREFIX=\"$(PARSER_PREFIX)\"

libconfig___la_CFLAGS = $(libcflags)
libconfig___la_CPPFLAGS = -DPARSER_PREFIX=\"$(PARSER_PREFIX)\"

include_HEADERS = $(libinc) $(libinc_cpp)

AM_YFLAGS = -p $(PARSER_PREFIX)

AM_LFLAGS = --prefix=$(PARSER_PREFIX)

EXTRA_DIST = samples
SUBDIRS = doc

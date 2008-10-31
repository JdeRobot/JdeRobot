bin_PROGRAMS = jde

AM_CPPFLAGS = -D CONFIGDIR=\"$(confdir)\" @CPPFLAGS@

jde_SOURCES = jde.c
jde_LDFLAGS = -rdynamic
dist_conf_DATA = jde.conf

include_HEADERS = jde.h
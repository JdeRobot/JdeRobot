bin_PROGRAMS = jde

jde_SOURCES = jde.c
jde_LDFLAGS = --export-dynamic
jde_CFLAGS = -D CONFIGDIR=\"@CFGDIR@\"
jdedir = @CFGDIR@
dist_jde_DATA = jde.conf

include_HEADERS = jde.h
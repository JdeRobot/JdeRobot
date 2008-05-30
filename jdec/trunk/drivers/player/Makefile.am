if ENABLE_PLAYER
driver_LTLIBRARIES = player.la
endif

player_la_SOURCES = player.c
player_la_LDFLAGS = -module -avoid-version @PLAYER_LIBS@
player_la_CFLAGS = @PLAYER_CFLAGS@ -I$(top_srcdir)/base
player_ladir = @CFGDIR@
dist_player_la_DATA = player.conf

MAINTAINERCLEANFILES = Makefile.in Makefile

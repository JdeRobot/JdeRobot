if ENABLE_COMPONENT_PLAYERSERVER
bin_PROGRAMS = playerserver
endif

playerserver_SOURCES = playerserver.cpp
playerserver_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(PLAYER_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
playerserver_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(PLAYER_LDFLAGS)
playerserver_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = playerserver.cfg

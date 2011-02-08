if ENABLE_COMPONENT_GAZEBOSERVER
bin_PROGRAMS = gazeboserver
endif

gazeboserver_SOURCES = gazeboserver.cpp
gazeboserver_CPPFLAGS = $(JDEROBOTICE_CPPFLAGS) $(GAZEBO_CPPFLAGS) -DGLADE_DIR=\""$(gladedir)"\"
gazeboserver_LDFLAGS = $(JDEROBOTICE_LDFLAGS) $(GAZEBO_LDFLAGS)
gazeboserver_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = gazeboserver.cfg

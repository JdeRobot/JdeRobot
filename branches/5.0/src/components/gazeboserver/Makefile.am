if ENABLE_COMPONENT_GAZEBOSERVER
bin_PROGRAMS = gazeboserver
endif


gazeboserver_SOURCES = gazeboserver.cpp
gazeboserver_CPPFLAGS = $(LIBJDEROBOTICE_CPPFLAGS) \
				-I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp
gazeboserver_LDFLAGS = $(GAZEBO_LIBS)
gazeboserver_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = gazeboserver.cfg

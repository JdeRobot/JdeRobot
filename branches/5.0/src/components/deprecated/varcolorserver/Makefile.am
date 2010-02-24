if ENABLE_COMPONENT_VARCOLORSERVER
bin_PROGRAMS = varcolorserver
endif


varcolorserver_SOURCES = varcolorserver.cpp gstpipeline.cpp gstpipeline.h
varcolorserver_CPPFLAGS = $(GSTREAMER_CPPFLAGS) $(LIBJDEROBOTICE_CPPFLAGS) \
				-I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp
varcolorserver_LDFLAGS = $(GSTREAMER_LIBS)
varcolorserver_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspaces.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspaces++.la \
			$(top_srcdir)/src/libs/colorspacesice/libcolorspacesice.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = varcolorserver.cfg
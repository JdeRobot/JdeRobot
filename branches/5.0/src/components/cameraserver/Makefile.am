if ENABLE_COMPONENT_CAMERASERVER
bin_PROGRAMS = cameraserver
endif


cameraserver_SOURCES = cameraserver.cpp gstpipeline.cpp gstpipeline.h
cameraserver_CPPFLAGS = $(GSTREAMER_CPPFLAGS) $(LIBJDEROBOTICE_CPPFLAGS) \
				-I$(top_srcdir)/src/libs -I$(top_srcdir)/src/interfaces/cpp
cameraserver_LDFLAGS = $(GSTREAMER_LIBS)
cameraserver_LDADD = $(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = cameraserver.cfg
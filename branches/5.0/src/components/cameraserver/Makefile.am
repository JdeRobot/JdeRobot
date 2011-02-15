if ENABLE_COMPONENT_CAMERASERVER
bin_PROGRAMS = cameraserver
endif

cameraserver_SOURCES = cameraserver.cpp gstpipeline.cpp gstpipeline.h
cameraserver_CPPFLAGS = $(GSTREAMER_CPPFLAGS) $(JDEROBOTICE_CPPFLAGS) $(FIREWIRE_CPPFLAGS)
cameraserver_LDFLAGS =  $(JDEROBOTICE_LDFLAGS) $(JDEROBOTUTIL_LDFLAGS) $(GSTREAMER_LDFLAGS) $(FIREWIRE_LDFLAGS)
cameraserver_LDADD = $(GSTREAMER_LIBS) \
			$(top_srcdir)/src/libs/jderobotice/libJderobotIce.la \
			$(top_srcdir)/src/libs/jderobotutil/libJderobotUtil.la \
			$(top_srcdir)/src/libs/colorspaces/libcolorspacesmm.la \
			$(top_srcdir)/src/interfaces/cpp/jderobot/libJderobotInterfaces.la

dist_conf_DATA = cameraserver.cfg
